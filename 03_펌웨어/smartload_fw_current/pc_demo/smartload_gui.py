import socket
import tkinter as tk
from tkinter import ttk


HOST = "127.0.0.1"
PORT = 9101


def send_command(command: str) -> str:
    with socket.create_connection((HOST, PORT), timeout=3.0) as sock:
        sock.sendall((command + "\n").encode("utf-8"))
        return sock.recv(4096).decode("utf-8", errors="ignore").strip()


def on_send() -> None:
    command = (
        f"SET V={voltage_var.get()} "
        f"I={current_var.get()} "
        f"F={freq_var.get()} "
        f"PF={pf_var.get()}"
    )
    try:
        response = send_command(command)
        status_var.set(response)
    except Exception as exc:
        status_var.set(f"ERR {exc}")


app = tk.Tk()
app.title("Smart Load GUI Sender")
app.geometry("420x240")

frame = ttk.Frame(app, padding=16)
frame.pack(fill="both", expand=True)

voltage_var = tk.StringVar(value="220")
current_var = tk.StringVar(value="50")
freq_var = tk.StringVar(value="60")
pf_var = tk.StringVar(value="0.90")
status_var = tk.StringVar(value="대기 중")

for row, (label, var) in enumerate(
    [
        ("Voltage (V)", voltage_var),
        ("Current (A)", current_var),
        ("Frequency (Hz)", freq_var),
        ("Power Factor", pf_var),
    ]
):
    ttk.Label(frame, text=label).grid(row=row, column=0, sticky="w", pady=4)
    ttk.Entry(frame, textvariable=var, width=16).grid(row=row, column=1, sticky="ew", pady=4)

ttk.Button(frame, text="계측값 보내기", command=on_send).grid(row=4, column=0, columnspan=2, pady=12)
ttk.Label(frame, text="상태").grid(row=5, column=0, sticky="w", pady=4)
ttk.Label(frame, textvariable=status_var, wraplength=320).grid(row=5, column=1, sticky="w", pady=4)

frame.columnconfigure(1, weight=1)
app.mainloop()
