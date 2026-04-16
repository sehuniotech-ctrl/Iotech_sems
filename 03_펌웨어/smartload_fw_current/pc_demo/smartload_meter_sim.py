import math
import socketserver
import threading
from dataclasses import dataclass


HOST = "127.0.0.1"
PORT = 9101

ATM90E26_REG_SYS_STATUS = 0x01
ATM90E26_REG_IRMS = 0x48
ATM90E26_REG_URMS = 0x49
ATM90E26_REG_PMEAN = 0x4A
ATM90E26_REG_QMEAN = 0x4B
ATM90E26_REG_FREQ = 0x4C
ATM90E26_REG_POWERF = 0x4D
ATM90E26_REG_PANGLE = 0x4E
ATM90E26_REG_SMEAN = 0x4F
ATM90E26_REG_IRMS2 = 0x68
ATM90E26_REG_AP_ENERGY = 0x40
ATM90E26_REG_RP_ENERGY = 0x43


@dataclass
class MeterState:
    voltage_v: float = 220.0
    current_a: float = 1.0
    frequency_hz: float = 60.0
    power_factor: float = 0.90
    phase_angle_deg: float = 25.8
    active_energy_wh: float = 0.0
    reactive_energy_varh: float = 0.0

    def set_values(self, voltage_v: float, current_a: float, frequency_hz: float, power_factor: float) -> None:
        self.voltage_v = voltage_v
        self.current_a = current_a
        self.frequency_hz = frequency_hz
        self.power_factor = max(-1.0, min(1.0, power_factor))
        angle_rad = math.acos(max(-1.0, min(1.0, abs(self.power_factor)))) if self.power_factor != 0 else math.pi / 2
        self.phase_angle_deg = math.degrees(angle_rad)
        if self.power_factor < 0:
            self.phase_angle_deg *= -1.0

    @property
    def apparent_power_va(self) -> float:
        return self.voltage_v * self.current_a

    @property
    def active_power_w(self) -> float:
        return self.apparent_power_va * self.power_factor

    @property
    def reactive_power_var(self) -> float:
        s = self.apparent_power_va
        p = self.active_power_w
        q_abs = math.sqrt(max((s * s) - (p * p), 0.0))
        return -q_abs if self.power_factor < 0 else q_abs

    def tick_energy(self, interval_s: float = 1.0) -> None:
        self.active_energy_wh += self.active_power_w * interval_s / 3600.0
        self.reactive_energy_varh += self.reactive_power_var * interval_s / 3600.0

    def read_reg(self, reg: int) -> int:
        if reg == ATM90E26_REG_SYS_STATUS:
            return 0x0001
        if reg == ATM90E26_REG_URMS:
            return int(round(self.voltage_v * 100.0)) & 0xFFFF
        if reg == ATM90E26_REG_IRMS:
            return int(round(self.current_a * 1000.0)) & 0xFFFF
        if reg == ATM90E26_REG_IRMS2:
            return 0
        if reg == ATM90E26_REG_PMEAN:
            return int(round(self.active_power_w * 1000.0)) & 0xFFFF
        if reg == ATM90E26_REG_QMEAN:
            return int(round(self.reactive_power_var * 1000.0)) & 0xFFFF
        if reg == ATM90E26_REG_SMEAN:
            return int(round(abs(self.apparent_power_va) * 1000.0)) & 0xFFFF
        if reg == ATM90E26_REG_POWERF:
            return int(round(self.power_factor * 1000.0)) & 0xFFFF
        if reg == ATM90E26_REG_FREQ:
            return int(round(self.frequency_hz * 100.0)) & 0xFFFF
        if reg == ATM90E26_REG_PANGLE:
            return int(round(self.phase_angle_deg * 10.0)) & 0xFFFF
        if reg == ATM90E26_REG_AP_ENERGY:
            self.tick_energy()
            return int(round(self.active_energy_wh)) & 0xFFFF
        if reg == ATM90E26_REG_RP_ENERGY:
            return int(round(self.reactive_energy_varh)) & 0xFFFF
        return 0


STATE = MeterState()
LOCK = threading.Lock()


class MeterRequestHandler(socketserver.StreamRequestHandler):
    def handle(self) -> None:
        while True:
            raw = self.rfile.readline()
            if not raw:
                return

            line = raw.decode("utf-8", errors="ignore").strip()
            if not line:
                continue

            response = self.process_command(line)
            self.wfile.write((response + "\n").encode("utf-8"))

    def process_command(self, line: str) -> str:
        parts = line.split()
        cmd = parts[0].upper()

        if cmd == "PING":
            return "OK PONG"

        if cmd == "SET":
            params = {}
            for token in parts[1:]:
                if "=" not in token:
                    continue
                key, value = token.split("=", 1)
                params[key.upper()] = float(value)
            with LOCK:
                STATE.set_values(
                    params.get("V", STATE.voltage_v),
                    params.get("I", STATE.current_a),
                    params.get("F", STATE.frequency_hz),
                    params.get("PF", STATE.power_factor),
                )
                return (
                    f"OK V={STATE.voltage_v:.2f} I={STATE.current_a:.3f} "
                    f"F={STATE.frequency_hz:.2f} PF={STATE.power_factor:.3f}"
                )

        if cmd == "READ" and len(parts) >= 2:
            reg = int(parts[1], 16)
            with LOCK:
                value = STATE.read_reg(reg)
            return f"OK 0x{value:04X}"

        if cmd == "STATE":
            with LOCK:
                return (
                    f"OK V={STATE.voltage_v:.2f} I={STATE.current_a:.3f} "
                    f"P={STATE.active_power_w:.3f} Q={STATE.reactive_power_var:.3f} "
                    f"S={STATE.apparent_power_va:.3f} PF={STATE.power_factor:.3f} "
                    f"F={STATE.frequency_hz:.2f}"
                )

        return "ERR UNKNOWN_COMMAND"


class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    allow_reuse_address = True


def main() -> None:
    print(f"Smart Load meter simulator listening on {HOST}:{PORT}")
    with ThreadedTCPServer((HOST, PORT), MeterRequestHandler) as server:
        server.serve_forever()


if __name__ == "__main__":
    main()
