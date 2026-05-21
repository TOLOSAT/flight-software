# Qebab: a simple TCP client for sending TC and receiving TM from the QEMU debug console.
# QEmu Bridge Access Bus

import socket

host = "127.0.0.1"
port = 4444

print(
    """
   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
   ⠀⠀⠀⠀⠀⠀⢀⣠⣤⣶⣶⣶⣿⣿⣿⣿⣿⣿⣶⣶⣶⣤⣄⠀⠀⠀⠀⠀⠀⠀
   ⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠿⠇⠀⠀⠀⠀⠀⠀
   ⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣦⣤⣤⣤⣤⡄⠀⠀⠀⠀⠀⠀
   ⠀⠀⠀⠀⠀⠀⠀⣀⣉⣭⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁⠀⠀⠀⠀⠀⠀
   ⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠟⠀⠀⠀⠀⠀⠀⠀
   ⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⣴⠀⠀⠀⠀
   ⠀⠀⢀⡄⠀⠀⠀⠀⠙⠛⠿⠿⣿⣿⣿⣿⣿⣿⣟⣛⣛⡋⠀⠀⣾⣿⣀⠀⠀⠀
   ⠀⠀⣼⣇⣀⠀⠀⠀⠀⢶⣶⣶⣶⣾⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠉⢻⣿⠇⠀⠀
   ⠀⠀⠻⢿⣿⠀⠀⠀⠀⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⠀⠀⣴⣾⠟⠁⠀⠀⠀
   ⠀⠀⠀⢀⣯⢀⡀⠀⠀⠀⠀⣤⣭⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀⠈⢻⣦⠀⠀⠀⠀
   ⠀⠀⠀⠈⠛⠻⢿⡄⠀⠀⠀⠘⢿⣿⣿⣿⣿⠿⠃⠀⠀⠀⠀⢀⡼⠋⠀⠀⠀⠀
   ⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⢠⡄⠀⠀⠀⠀⠀⠀⠈⠁⠀⠀⠀⠀⠀⠀
   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
                 888              888
 e88 888  ,e e,  888 88e   ,"Y88b 888 88e
d888 888 d88 88b 888 888b "8" 888 888 888b
Y888 888 888   , 888 888P ,ee 888 888 888P
 "88 888  "YeeP" 888 88"  "88 888 888 88"
     888
     888
      """
)

print("Connecting to QEMU debug console...")

s = socket.create_connection((host, port))
s.settimeout(0.2)

while True:
    cmd = input("Enter command (hex): ")

    s.sendall(bytes.fromhex(cmd))

    data = b""

    while True:
        try:
            chunk = s.recv(4096)

            if not chunk:
                break

            data += chunk

        except socket.timeout:
            # no more bytes incoming
            break

    print(data.hex())
