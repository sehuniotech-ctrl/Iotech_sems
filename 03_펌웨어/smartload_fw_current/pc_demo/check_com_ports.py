import serial.tools.list_ports


def main() -> None:
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        print("No COM ports found.")
        return

    print("Available COM ports:")
    for port in ports:
        print(f"- {port.device}: {port.description}")


if __name__ == "__main__":
    main()
