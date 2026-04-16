from __future__ import annotations

from dataclasses import dataclass
from typing import Iterable


STX = 0x7E
ETX = 0x7E
VER_V1 = 0x01

MSG_READ_REQ = 0x01
MSG_READ_RES = 0x02
MSG_CTRL_REQ = 0x03
MSG_CTRL_RES = 0x04
MSG_EVENT = 0x05
MSG_ACK = 0x06
MSG_NACK = 0x07

TYPE_UINT8 = 0x01
TYPE_UINT16 = 0x02
TYPE_UINT32 = 0x03
TYPE_INT32 = 0x04
TYPE_BIT_FIELD = 0x05
TYPE_LIST = 0x06

ITEM_DATA_EMPTY = 0x0000
ITEM_SL_ID = 0x0001
ITEM_VOLTAGE = 0x0002
ITEM_CURRENT = 0x0003
ITEM_ENERGY_ACC = 0x0004
ITEM_RELAY_STATE = 0x0005
ITEM_ALARM_CODE = 0x0006

ITEM_READ_LIST = 0x1001
ITEM_RELAY_COMMAND = 0x1002
ITEM_CONTROL_REASON = 0x1003
ITEM_CONTROL_TOKEN = 0x1004

RELAY_OFF = 0x00
RELAY_ON = 0x01
RELAY_SWITCHING = 0x02
RELAY_UNKNOWN = 0x03


def crc16_modbus(data: bytes) -> int:
    crc = 0xFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 0x0001:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc & 0xFFFF


def u16be(value: int) -> bytes:
    return int(value).to_bytes(2, "big", signed=False)


def u32be(value: int) -> bytes:
    return int(value).to_bytes(4, "big", signed=False)


def encode_value(item_type: int, value: int | bytes | Iterable[int]) -> bytes:
    if isinstance(value, bytes):
        return value
    if item_type == TYPE_UINT8:
        return int(value).to_bytes(1, "big", signed=False)
    if item_type in (TYPE_UINT16, TYPE_BIT_FIELD):
        return int(value).to_bytes(2, "big", signed=False)
    if item_type == TYPE_UINT32:
        return int(value).to_bytes(4, "big", signed=False)
    if item_type == TYPE_INT32:
        return int(value).to_bytes(4, "big", signed=True)
    if item_type == TYPE_LIST:
        return b"".join(u16be(v) for v in value)
    raise ValueError(f"unsupported item_type: 0x{item_type:02X}")


@dataclass
class TLV:
    item_id: int
    item_type: int
    value: bytes

    def encode(self) -> bytes:
        return u16be(self.item_id) + bytes([self.item_type, len(self.value)]) + self.value

    @classmethod
    def from_value(cls, item_id: int, item_type: int, value: int | bytes | Iterable[int]) -> "TLV":
        return cls(item_id=item_id, item_type=item_type, value=encode_value(item_type, value))


@dataclass
class Frame:
    version: int
    msg_type: int
    seq: int
    sl_id: int
    payload: bytes

    def encode(self) -> bytes:
        body = bytes(
            [
                self.version & 0xFF,
                self.msg_type & 0xFF,
                self.seq & 0xFF,
                self.sl_id & 0xFF,
            ]
        ) + u16be(len(self.payload)) + self.payload
        crc = crc16_modbus(body)
        return bytes([STX]) + body + crc.to_bytes(2, "little") + bytes([ETX])


def build_read_request(sl_id: int, seq: int, item_ids: Iterable[int]) -> bytes:
    payload = b"".join(u16be(item_id) for item_id in item_ids)
    return Frame(VER_V1, MSG_READ_REQ, seq, sl_id, payload).encode()


def build_control_request(
    sl_id: int,
    seq: int,
    relay_command: int,
    control_reason: int | None = None,
    control_token: int | None = None,
) -> bytes:
    tlvs = [TLV.from_value(ITEM_RELAY_COMMAND, TYPE_UINT8, relay_command)]
    if control_reason is not None:
        tlvs.append(TLV.from_value(ITEM_CONTROL_REASON, TYPE_UINT8, control_reason))
    if control_token is not None:
        tlvs.append(TLV.from_value(ITEM_CONTROL_TOKEN, TYPE_UINT32, control_token))
    payload = b"".join(tlv.encode() for tlv in tlvs)
    return Frame(VER_V1, MSG_CTRL_REQ, seq, sl_id, payload).encode()


def parse_frame(frame: bytes) -> dict:
    if len(frame) < 10:
        raise ValueError("frame too short")
    if frame[0] != STX or frame[-1] != ETX:
        raise ValueError("invalid STX/ETX")

    body = frame[1:-3]
    recv_crc = int.from_bytes(frame[-3:-1], "little")
    calc_crc = crc16_modbus(body)
    if recv_crc != calc_crc:
        raise ValueError(f"CRC mismatch recv=0x{recv_crc:04X} calc=0x{calc_crc:04X}")

    version = body[0]
    msg_type = body[1]
    seq = body[2]
    sl_id = body[3]
    payload_len = int.from_bytes(body[4:6], "big")
    payload = body[6:]
    if len(payload) != payload_len:
        raise ValueError(f"LEN mismatch expected={payload_len} actual={len(payload)}")

    return {
        "version": version,
        "msg_type": msg_type,
        "seq": seq,
        "sl_id": sl_id,
        "payload_len": payload_len,
        "payload": payload,
        "crc": recv_crc,
    }


def parse_tlvs(payload: bytes) -> list[dict]:
    items: list[dict] = []
    idx = 0
    while idx < len(payload):
        if idx + 4 > len(payload):
            raise ValueError("truncated TLV header")
        item_id = int.from_bytes(payload[idx : idx + 2], "big")
        item_type = payload[idx + 2]
        item_len = payload[idx + 3]
        start = idx + 4
        end = start + item_len
        if end > len(payload):
            raise ValueError("truncated TLV value")
        value = payload[start:end]
        items.append(
            {
                "item_id": item_id,
                "item_type": item_type,
                "item_len": item_len,
                "value_raw": value,
                "value_text": decode_value(item_id, item_type, value),
            }
        )
        idx = end
    return items


def decode_value(item_id: int, item_type: int, value: bytes) -> str:
    if item_type == TYPE_UINT8:
        num = int.from_bytes(value, "big")
        if item_id == ITEM_RELAY_STATE:
            states = {
                RELAY_OFF: "OFF",
                RELAY_ON: "ON",
                RELAY_SWITCHING: "SWITCHING",
                RELAY_UNKNOWN: "UNKNOWN",
            }
            return f"{num} ({states.get(num, 'RESERVED')})"
        return str(num)
    if item_type in (TYPE_UINT16, TYPE_BIT_FIELD):
        num = int.from_bytes(value, "big")
        if item_id == ITEM_ALARM_CODE:
            bits = []
            names = {
                0: "과전류",
                1: "과전압",
                2: "저전압",
                3: "릴레이 구동 실패",
                4: "측정칩 이상",
                5: "내부 메모리 이상",
                6: "센서 또는 측정값 이상",
            }
            for bit, name in names.items():
                if num & (1 << bit):
                    bits.append(name)
            return f"0x{num:04X}" + (f" ({', '.join(bits)})" if bits else "")
        return str(num)
    if item_type == TYPE_UINT32:
        num = int.from_bytes(value, "big")
        if item_id == ITEM_VOLTAGE:
            return f"{num / 10.0:.1f} V"
        if item_id == ITEM_CURRENT:
            return f"{num / 100.0:.2f} A"
        if item_id == ITEM_ENERGY_ACC:
            return f"{num} Wh"
        return str(num)
    if item_type == TYPE_INT32:
        return str(int.from_bytes(value, "big", signed=True))
    if item_type == TYPE_LIST:
        ids = [int.from_bytes(value[i : i + 2], "big") for i in range(0, len(value), 2)]
        return "[" + ", ".join(f"0x{x:04X}" for x in ids) + "]"
    return value.hex(" ").upper()


def hexdump(data: bytes) -> str:
    return data.hex(" ").upper()


def build_sample_read_response(
    sl_id: int,
    seq: int,
    voltage_raw: int = 2200,
    current_raw: int = 5000,
    relay_state: int = RELAY_ON,
) -> bytes:
    payload = b"".join(
        [
            TLV.from_value(ITEM_VOLTAGE, TYPE_UINT32, voltage_raw).encode(),
            TLV.from_value(ITEM_CURRENT, TYPE_UINT32, current_raw).encode(),
            TLV.from_value(ITEM_RELAY_STATE, TYPE_UINT8, relay_state).encode(),
        ]
    )
    return Frame(VER_V1, MSG_READ_RES, seq, sl_id, payload).encode()
