# Copyright (c) TOLOSAT 2026
# SPDX-License-Identifier: Apache-2.0

import socket
import threading
import curses

host = "127.0.0.1"
port = 4444

shutdown = False
rx_column = 0

LOGO = r"""
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
‎ 
‎
"""

s = socket.create_connection((host, port))
s.settimeout(0.2)


def draw_logo(win):
    win.clear()

    _, width = win.getmaxyx()

    lines = [line.rstrip() for line in LOGO.splitlines() if line.strip()]

    logo_width = max(len(line) for line in lines)

    start_y = 0
    start_x = max(0, (width - logo_width) // 2)

    for i, line in enumerate(lines):

        try:
            win.addstr(start_y + i, start_x, line[: width - start_x - 1])

        except:
            pass

    win.refresh()


def receive_data(rx_win):
    global shutdown
    global rx_column

    while not shutdown:
        try:
            chunk = s.recv(4096)

            if not chunk:
                break

            height, width = rx_win.getmaxyx()

            # "FF " = 3 chars
            bytes_per_line = max(1, (width - 1) // 3)

            for b in chunk:

                if rx_column == 0:
                    rx_win.addstr("\n")

                rx_win.addstr(f"{b:02X} ")

                rx_column += 1

                if rx_column >= bytes_per_line:
                    rx_column = 0

            rx_win.refresh()

        except socket.timeout:
            continue

        except Exception as e:
            if not shutdown:

                if rx_column != 0:
                    rx_win.addstr("\n")
                    rx_column = 0

                rx_win.addstr(f"[ERROR] {e}\n")
                rx_win.refresh()

            break


def print_rx_message(rx_win, message):
    global rx_column

    if rx_column != 0:
        rx_win.addstr("\n")
        rx_column = 0

    rx_win.addstr("\n" + message + "\n")
    rx_win.refresh()


def main(stdscr):
    global shutdown

    curses.curs_set(1)

    curses.start_color()
    curses.use_default_colors()

    stdscr.bkgd(" ", curses.color_pair(0))

    height, width = stdscr.getmaxyx()

    input_height = 3
    top_height = height - input_height

    # RX window
    rx_win = curses.newwin(top_height, width, 0, 0)

    # Full width input window
    tx_win = curses.newwin(input_height, width, top_height, 0)

    rx_win.scrollok(True)

    rx_win.bkgd(" ", curses.color_pair(0))
    tx_win.bkgd(" ", curses.color_pair(0))

    draw_logo(rx_win)

    rx_thread = threading.Thread(target=receive_data, args=(rx_win,), daemon=True)

    rx_thread.start()

    while not shutdown:

        tx_win.clear()

        # separator line
        tx_win.hline(0, 0, curses.ACS_HLINE, width)

        tx_win.addstr(1, 2, "QEBAB (HEX) > ")

        tx_win.refresh()

        curses.echo()

        try:
            cmd = tx_win.getstr(1, 9).decode().strip()
        except KeyboardInterrupt:
            shutdown = True
            break

        curses.noecho()

        if cmd.lower() == "quit":
            shutdown = True
            break

        if cmd:
            try:
                s.sendall(bytes.fromhex(cmd))

                print_rx_message(rx_win, f"[TX] {cmd}")

            except ValueError:
                print_rx_message(rx_win, "[ERROR] Invalid hex format")

            except Exception as e:
                print_rx_message(rx_win, f"[ERROR] {e}")

                shutdown = True

    s.close()


if __name__ == "__main__":
    curses.wrapper(main)
