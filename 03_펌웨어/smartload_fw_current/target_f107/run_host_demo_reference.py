from pathlib import Path


APP_MEASUREMENT_UPDATE_MS = 1000
APP_OVERCURRENT_LIMIT_A = 16.0
APP_OVERVOLTAGE_LIMIT_V = 250.0
APP_STANDBY_POWER_LIMIT_W = 3.0


class MeterState:
    def __init__(self):
        self.soft_reset_done = False
        self.demo_mode_started = False
        self.read_cycle = 0


def read_reg(state: MeterState, reg: str):
    if not state.demo_mode_started and reg != "SYS_STATUS":
        raise RuntimeError(f"read before startup: {reg}")

    if reg == "SYS_STATUS":
        return 0x0001 if state.demo_mode_started else 0x0000
    if reg == "URMS":
        return 22340
    if reg == "IRMS":
        if state.read_cycle < 2:
            return 850
        if state.read_cycle < 4:
            return 12
        return 18250
    if reg == "IRMS2":
        return 5
    if reg == "PMEAN":
        if state.read_cycle < 2:
            return 1800
        if state.read_cycle < 4:
            return 2
        return 5500
    if reg == "QMEAN":
        return 320
    if reg == "SMEAN":
        return 2100
    if reg == "POWERF":
        return 860
    if reg == "FREQ":
        return 6000
    if reg == "PANGLE":
        return 35
    if reg == "AP_ENERGY":
        return 1
    if reg == "RP_ENERGY":
        state.read_cycle += 1
        return 1
    return 0


def write_reg(state: MeterState, reg: str, value: int):
    print(f"[write] reg={reg} value=0x{value:04X}")
    if reg == "SOFT_RESET" and value == 0x789A:
        state.soft_reset_done = True
        state.demo_mode_started = False
        state.read_cycle = 0
    elif reg == "CAL_START" and value == 0x5678:
        if not state.soft_reset_done:
            raise RuntimeError("CAL_START before reset")
    elif reg == "ADJ_START" and value == 0x5678:
        if not state.soft_reset_done:
            raise RuntimeError("ADJ_START before reset")
        state.demo_mode_started = True


def delay_ms(ms: int):
    print(f"[delay] {ms} ms")


def main():
    state = MeterState()
    print("=== Smart Load host demo start ===")

    write_reg(state, "SOFT_RESET", 0x789A)
    delay_ms(10)
    write_reg(state, "CAL_START", 0x5678)
    write_reg(state, "ADJ_START", 0x5678)
    delay_ms(20)
    print("ATM90E26 startup success")

    last_update_ms = 0
    relay_on = True
    last_trip_reason = 0
    active_energy_wh = 0.0
    reactive_energy_varh = 0.0

    for step in range(6):
        tick_ms = (step + 1) * 1000
        if (tick_ms - last_update_ms) < APP_MEASUREMENT_UPDATE_MS:
            continue
        last_update_ms = tick_ms

        _ = read_reg(state, "SYS_STATUS")
        voltage_v = read_reg(state, "URMS") / 100.0
        line_current_a = read_reg(state, "IRMS") / 1000.0
        _neutral_current_a = read_reg(state, "IRMS2") / 1000.0
        active_power_w = read_reg(state, "PMEAN") / 1000.0
        reactive_power_var = read_reg(state, "QMEAN") / 1000.0
        apparent_power_va = read_reg(state, "SMEAN") / 1000.0
        power_factor = read_reg(state, "POWERF") / 1000.0
        frequency_hz = read_reg(state, "FREQ") / 100.0
        _phase_angle_deg = read_reg(state, "PANGLE") / 10.0
        active_energy_wh += float(read_reg(state, "AP_ENERGY"))
        reactive_energy_varh += float(read_reg(state, "RP_ENERGY"))

        if line_current_a >= APP_OVERCURRENT_LIMIT_A:
            relay_on = False
            last_trip_reason = 2
        elif voltage_v >= APP_OVERVOLTAGE_LIMIT_V:
            relay_on = False
            last_trip_reason = 3
        elif active_power_w <= APP_STANDBY_POWER_LIMIT_W:
            last_trip_reason = 1

        print(
            f"[tick={tick_ms} ms] V={voltage_v:.2f}V I={line_current_a:.3f}A "
            f"P={active_power_w:.3f}W Q={reactive_power_var:.3f}var "
            f"S={apparent_power_va:.3f}VA PF={power_factor:.3f} "
            f"F={frequency_hz:.2f}Hz Relay={'ON' if relay_on else 'OFF'} "
            f"Trip={last_trip_reason}"
        )

    print("=== Smart Load host demo end ===")


if __name__ == "__main__":
    main()
