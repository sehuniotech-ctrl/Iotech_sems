from __future__ import annotations

import argparse
import sys
import time

import serial
import serial.tools.list_ports

from smartload_protocol import (
    RELAY_OFF,
    RELAY_ON,
    build_control_request,
    build_read_request,
    hexdump,
    parse_frame,
    parse_tlvs,
)


def list_ports() -> int:
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        print("No COM ports found.")
        return 0

    print("Available COM ports:")
    for port in ports:
        print(f"- {port.device}: {port.description}")
    return 0


def parse_item_ids(text: str) -> list[int]:
    values = []
    for token in text.split(","):
        token = token.strip()
        if not token:
            continue
        values.append(int(token, 0))
    return values


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

    raise TimeoutError("response timeout")


def send_and_print(ser: serial.Serial, frame: bytes, timeout_s: float) -> int:
    print("TX:", hexdump(frame))
    ser.reset_input_buffer()
    ser.write(frame)
    ser.flush()

    try:
        response = read_one_frame(ser, timeout_s)
    except TimeoutError as exc:
        print(f"RX: timeout ({exc})")
        return 1

    print("RX:", hexdump(response))
    try:
        parsed = parse_frame(response)
    except Exception as exc:
        print(f"Parse error: {exc}")
        return 2

    print(
        "Frame:",
        f"VER=0x{parsed['version']:02X}",
        f"MSG=0x{parsed['msg_type']:02X}",
        f"SEQ=0x{parsed['seq']:02X}",
        f"SL_ID=0x{parsed['sl_id']:02X}",
        f"LEN={parsed['payload_len']}",
    )

    payload = parsed["payload"]
    if payload:
        try:
            for item in parse_tlvs(payload):
                print(
                    "  TLV:",
                    f"ITEM_ID=0x{item['item_id']:04X}",
                    f"TYPE=0x{item['item_type']:02X}",
                    f"LEN={item['item_len']}",
                    f"RAW={item['value_raw'].hex(' ').upper()}",
                    f"VALUE={item['value_text']}",
                )
        except Exception:
            print("  PAYLOAD(raw):", payload.hex(" ").upper())
    return 0


def open_serial(args: argparse.Namespace) -> serial.Serial:
    return serial.Serial(
        port=args.port,
        baudrate=args.baud,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=0.1,
    )


def cmd_read(args: argparse.Namespace) -> int:
    item_ids = parse_item_ids(args.items)
    frame = build_read_request(sl_id=args.sl_id, seq=args.seq, item_ids=item_ids)
    with open_serial(args) as ser:
        return send_and_print(ser, frame, args.timeout)


def cmd_control(args: argparse.Namespace) -> int:
    command = RELAY_ON if args.state.upper() == "ON" else RELAY_OFF
    frame = build_control_request(
        sl_id=args.sl_id,
        seq=args.seq,
        relay_command=command,
        control_reason=args.reason,
        control_token=args.token,
    )
    with open_serial(args) as ser:
        return send_and_print(ser, frame, args.timeout)


def cmd_raw(args: argparse.Namespace) -> int:
    frame = bytes.fromhex(args.hex)
    with open_serial(args) as ser:
        return send_and_print(ser, frame, args.timeout)


def cmd_monitor(args: argparse.Namespace) -> int:
    with open_serial(args) as ser:
        print(f"Monitoring {args.port} @ {args.baud} (Ctrl+C to stop)")
        while True:
            try:
                response = read_one_frame(ser, args.timeout)
            except TimeoutError:
                continue
            print("RX:", hexdump(response))
            try:
                parsed = parse_frame(response)
                print(
                    f"  VER=0x{parsed['version']:02X} MSG=0x{parsed['msg_type']:02X} "
                    f"SEQ=0x{parsed['seq']:02X} SL_ID=0x{parsed['sl_id']:02X} LEN={parsed['payload_len']}"
                )
                try:
                    for item in parse_tlvs(parsed["payload"]):
                        print(
                            f"    ITEM=0x{item['item_id']:04X} TYPE=0x{item['item_type']:02X} "
                            f"VALUE={item['value_text']}"
                        )
                except Exception:
                    if parsed["payload"]:
                        print("    PAYLOAD(raw):", parsed["payload"].hex(" ").upper())
            except Exception as exc:
                print(f"  Parse error: {exc}")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Smart Load protocol serial test tool")
    sub = parser.add_subparsers(dest="command", required=True)

    p = sub.add_parser("ports", help="list COM ports")
    p.set_defaults(func=lambda args: list_ports())

    common = argparse.ArgumentParser(add_help=False)
    common.add_argument("--port", required=True, help="COM port, example: COM6")
    common.add_argument("--baud", type=int, default=9600, help="baudrate")
    common.add_argument("--timeout", type=float, default=1.0, help="response timeout seconds")

    p = sub.add_parser("read", parents=[common], help="send a read request")
    p.add_argument("--sl-id", type=lambda x: int(x, 0), default=0x11)
    p.add_argument("--seq", type=lambda x: int(x, 0), default=0x21)
    p.add_argument(
        "--items",
        default="0x0002,0x0003,0x0005",
        help="comma-separated item IDs, example: 0x0002,0x0003,0x0005",
    )
    p.set_defaults(func=cmd_read)

    p = sub.add_parser("control", parents=[common], help="send a relay control request")
    p.add_argument("--sl-id", type=lambda x: int(x, 0), default=0x11)
    p.add_argument("--seq", type=lambda x: int(x, 0), default=0x22)
    p.add_argument("--state", choices=["ON", "OFF"], required=True)
    p.add_argument("--reason", type=lambda x: int(x, 0))
    p.add_argument("--token", type=lambda x: int(x, 0))
    p.set_defaults(func=cmd_control)

    p = sub.add_parser("raw", parents=[common], help="send a raw hex frame")
    p.add_argument("--hex", required=True, help="hex string, example: 7E 01 01 ... 7E")
    p.set_defaults(func=cmd_raw)

    p = sub.add_parser("monitor", parents=[common], help="monitor serial frames")
    p.set_defaults(func=cmd_monitor)
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()
    try:
        return args.func(args)
    except serial.SerialException as exc:
        print(f"Serial error: {exc}")
        return 10
    except KeyboardInterrupt:
        print("Stopped.")
        return 130


if __name__ == "__main__":
    sys.exit(main())
