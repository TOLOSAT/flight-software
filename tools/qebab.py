# Qebab: a simple TCP client for sending TC and receiving TM from the QEMU debug console.
# QEmu Bridge Access Bus

import socket
import threading

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

shutdown = False


def receive_data():
    """Continuously receive data from the socket."""
    global shutdown
    while not shutdown:
        try:
            chunk = s.recv(4096)
            if not chunk:
                break
            print(f"\n[RX] {chunk.hex()}")
        except socket.timeout:
            continue
        except Exception as e:
            if not shutdown:
                print(f"[ERROR] {e}")
            break


def send_data():
    """Send user-provided commands."""
    global shutdown
    try:
        while not shutdown:
            cmd = input("Enter command (hex, or 'quit' to exit): ").strip()
            if cmd.lower() == "quit":
                shutdown = True
                break
            if cmd:
                try:
                    s.sendall(bytes.fromhex(cmd))
                    print(f"[TX] {cmd}")
                except ValueError:
                    print("[ERROR] Invalid hex format")
    except EOFError:
        shutdown = True
    except Exception as e:
        print(f"[ERROR] {e}")
        shutdown = True


rx_thread = threading.Thread(target=receive_data, daemon=True)
tx_thread = threading.Thread(target=send_data)

rx_thread.start()
tx_thread.start()

tx_thread.join()
shutdown = True
s.close()
print("Disconnected.")
