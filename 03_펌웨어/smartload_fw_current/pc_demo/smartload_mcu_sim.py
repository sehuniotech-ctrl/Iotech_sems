import argparse
import socket
import time

import serial


HOST = "127.0.0.1"
PORT = 9101


def send_command(command: str) -> str:
    with socket.create_connection((HOST, PORT), timeout=3.0) as sock:
        sock.sendall((command + "\n").encode("utf-8"))
        return sock.recv(4096).decode("utf-8", errors="ignore").strip()


def read_state() -> dict[str, float]:
    response = send_command("STATE")
    if not response.startswith("OK "):
        raise RuntimeError(response)

    data: dict[str, float] = {}
    for token in response.split()[1:]:
        if "=" not in token:
            continue
        key, value = token.split("=", 1)
        data[key.upper()] = float(value)
    return data


def build_frame(sl_id: int) -> str:
    state = read_state()
    voltage_v = state.get("V", 0.0)
    current_a = state.get("I", 0.0)
    neutral_a = 0.0
    active_power_w = state.get("P", 0.0)
    reactive_power_var = state.get("Q", 0.0)
    apparent_power_va = state.get("S", 0.0)
    power_factor = state.get("PF", 0.0)
    frequency_hz = state.get("F", 0.0)
    phase_angle_deg = 0.0
    active_energy_wh = 0.0
    reactive_energy_varh = 0.0

    return (
        f"SL_ID={sl_id},V={voltage_v:.2f},I={current_a:.3f},IN={neutral_a:.3f},"
        f"P={active_power_w:.3f},Q={reactive_power_var:.3f},S={apparent_power_va:.3f},"
        f"PF={power_factor:.3f},F={frequency_hz:.2f},ANGLE={phase_angle_deg:.1f},"
        f"E_WH={active_energy_wh},E_VARH={reactive_energy_varh}"
    )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", required=True, help="virtual COM port for Hercules, example: COM6")
    parser.add_argument("--baud", type=int, default=9600)
    parser.add_argument("--sl-id", type=int, default=1)
    parser.add_argument("--interval", type=float, default=1.0)
    args = parser.parse_args()

    with serial.Serial(args.port, args.baud, timeout=1) as ser:
        print(f"MCU simulator started -> {args.port} @ {args.baud}")
        while True:
            frame = build_frame(args.sl_id)
            print(frame)
            ser.write((frame + "\r\n").encode("ascii", errors="ignore"))
            time.sleep(args.interval)


if __name__ == "__main__":
    main()
