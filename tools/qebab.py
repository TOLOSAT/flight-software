# Qebab: a simple TCP client for sending TC and receiving TM from the QEMU debug console.
# QEmu Bridge Access Bus

import socket

host = "127.0.0.1"
port = 4444

print("Connecting to QEMU debug console on")
# TODO: figure out why the first response is truncated
s = socket.create_connection((host, port))
while True:
    s.send(bytes.fromhex(input("Enter command (hex): ")))
    print(s.recv(4096).hex())
