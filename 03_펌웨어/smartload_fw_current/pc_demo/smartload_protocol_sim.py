from __future__ import annotations

import argparse
import sys
import time

import serial

from smartload_protocol import (
    Frame,
    ITEM_ALARM_CODE,
    ITEM_CURRENT,
    ITEM_DATA_EMPTY,
    ITEM_ENERGY_ACC,
    ITEM_RELAY_COMMAND,
    ITEM_RELAY_STATE,
    ITEM_VOLTAGE,
    MSG_ACK,
    MSG_CTRL_REQ,
    MSG_CTRL_RES,
    MSG_NACK,
    MSG_READ_REQ,
    MSG_READ_RES,
    RELAY_OFF,
    RELAY_ON,
    TYPE_BIT_FIELD,
    TYPE_UINT8,
    TYPE_UINT32,
    TLV,
    hexdump,
    parse_frame,
)


def read_one_frame(ser: serial.Serial, timeout_s: float) -> bytes:
    deadline = time.time() + timeout_s
    buffer = bytearray()
    started = False

    while time.time() < deadline:
        chunk = ser.read(1)
        if not chunk:
            continue
        byte = chunk[0]
        if not started:
            if byte == 0x7E:
                buffer.append(byte)
                started = True
            continue
        buffer.append(byte)
        if len(buffer) >= 2 and byte == 0x7E:
            return bytes(buffer)

    raise TimeoutError("request timeout")


class SimulatorState:
    def __init__(self, sl_id: int, voltage_raw: int, current_raw: int, energy_wh: int, relay_state: int, alarm_code: int):
        self.sl_id = sl_id
        self.voltage_raw = voltage_raw
        self.current_raw = current_raw
        self.energy_wh = energy_wh
        self.relay_state = relay_state
        self.alarm_code = alarm_code

    def build_read_response(self, seq: int, requested_payload: bytes) -> bytes:
        item_ids = [
            int.from_bytes(requested_payload[i : i + 2], "big")
            for i in range(0, len(requested_payload), 2)
            if i + 2 <= len(requested_payload)
        ]
        tlvs = []
        for item_id in item_ids:
            if item_id == ITEM_VOLTAGE:
                tlvs.append(TLV.from_value(item_id, TYPE_UINT32, self.voltage_raw))
            elif item_id == ITEM_CURRENT:
                tlvs.append(TLV.from_value(item_id, TYPE_UINT32, self.current_raw))
            elif item_id == ITEM_ENERGY_ACC:
                tlvs.append(TLV.from_value(item_id, TYPE_UINT32, self.energy_wh))
            elif item_id == ITEM_RELAY_STATE:
                tlvs.append(TLV.from_value(item_id, TYPE_UINT8, self.relay_state))
            elif item_id == ITEM_ALARM_CODE:
                tlvs.append(TLV.from_value(item_id, TYPE_BIT_FIELD, self.alarm_code))
            elif item_id == ITEM_DATA_EMPTY:
                tlvs.append(TLV.from_value(item_id, TYPE_UINT32, 0))

        payload = b"".join(tlv.encode() for tlv in tlvs[:3])
        return Frame(version=0x01, msg_type=MSG_READ_RES, seq=seq, sl_id=self.sl_id, payload=payload).encode()

    def build_control_response(self, seq: int, request_payload: bytes) -> bytes:
        idx = request_payload.find(ITEM_RELAY_COMMAND.to_bytes(2, "big"))
        if idx >= 0 and idx + 5 <= len(request_payload):
            value = request_payload[idx + 4]
            self.relay_state = RELAY_ON if value == RELAY_ON else RELAY_OFF

        payload = b"".join(
            [
                bytes([0x00]),
                TLV.from_value(ITEM_RELAY_STATE, TYPE_UINT8, self.relay_state).encode(),
            ]
        )
        return Frame(version=0x01, msg_type=MSG_CTRL_RES, seq=seq, sl_id=self.sl_id, payload=payload).encode()


def main() -> int:
    parser = argparse.ArgumentParser(description="Smart Load protocol serial simulator")
    parser.add_argument("--port", required=True, help="COM port connected to host tool")
    parser.add_argument("--baud", type=int, default=9600)
    parser.add_argument("--sl-id", type=lambda x: int(x, 0), default=0x11)
    parser.add_argument("--voltage", type=float, default=220.0, help="voltage in V")
    parser.add_argument("--current", type=float, default=50.0, help="current in A")
    parser.add_argument("--energy", type=int, default=100000, help="energy in Wh")
    parser.add_argument("--relay", choices=["ON", "OFF"], default="ON")
    parser.add_argument("--alarm", type=lambda x: int(x, 0), default=0x0000)
    parser.add_argument("--timeout", type=float, default=1.0)
    args = parser.parse_args()

    state = SimulatorState(
        sl_id=args.sl_id,
        voltage_raw=int(round(args.voltage * 10.0)),
        current_raw=int(round(args.current * 100.0)),
        energy_wh=args.energy,
        relay_state=RELAY_ON if args.relay == "ON" else RELAY_OFF,
        alarm_code=args.alarm,
    )

    with serial.Serial(args.port, args.baud, timeout=0.1) as ser:
        print(f"Simulator listening on {args.port} @ {args.baud}, SL_ID=0x{args.sl_id:02X}")
        while True:
            try:
                frame = read_one_frame(ser, args.timeout)
            except TimeoutError:
                continue

            print("REQ:", hexdump(frame))
            try:
                parsed = parse_frame(frame)
            except Exception as exc:
                print(f"Parse error: {exc}")
                nack = Frame(0x01, MSG_NACK, 0x00, state.sl_id, bytes([0x04])).encode()
                ser.write(nack)
                ser.flush()
                continue

            if parsed["msg_type"] == MSG_READ_REQ:
                response = state.build_read_response(parsed["seq"], parsed["payload"])
            elif parsed["msg_type"] == MSG_CTRL_REQ:
                response = state.build_control_response(parsed["seq"], parsed["payload"])
            else:
                response = Frame(0x01, MSG_ACK, parsed["seq"], state.sl_id, b"").encode()

            print("RES:", hexdump(response))
            ser.write(response)
            ser.flush()


if __name__ == "__main__":
    sys.exit(main())
