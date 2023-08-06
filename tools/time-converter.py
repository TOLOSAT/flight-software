#!/usr/bin/env python3
import argparse
import time

TAI_UNIX_OFFSET = 378691200
TC_HEADER_S9SS128 = "0x1855c000000c1909800000"

def get_tai_time():
    """Get the current time in TAI format."""
    return int(time.time() + TAI_UNIX_OFFSET)

def get_cuc_time():
    """Get the current time in CUC format (hexadecimal representation)."""
    tai_time = get_tai_time()
    # Convert the TAI time to CUC format with 0x1d prefix and 0x00 suffix
    cuc_time = [0x1d] + list(tai_time.to_bytes(4, 'big')) + [0x00]
    return "0x" + "".join(f"{byte:02x}" for byte in cuc_time)

def convert_cuc_to_time(cuc_time_str):
    """Convert the CUC time to readable GMT time."""
    cuc_time_str = cuc_time_str.lower()
    if not cuc_time_str.startswith("0x1d") :
        print("Invalid CUC time format.")
        return

    cuc_time_bytes = bytes.fromhex(cuc_time_str[4:-2])
    tai_time = int.from_bytes(cuc_time_bytes, 'big')
    unix_time = tai_time - TAI_UNIX_OFFSET
    return time.strftime('%Y-%m-%d %H:%M:%S GMT', time.gmtime(unix_time))

def get_tc_s9ss128():
    """Get the TC S9SS128"""
    cuc_time = get_cuc_time()
    cuc_time_str = cuc_time.lower()
    if not cuc_time_str.startswith("0x1d") :
        print("Invalid CUC time format.")
        return

    cuc_time_bytes = bytes.fromhex(cuc_time_str[2:])
    tc_bytes = bytes.fromhex(TC_HEADER_S9SS128[2:]) + cuc_time_bytes

    # Calculate CRC-16/CCITT-FALSE
    crc = 0xFFFF
    for byte in tc_bytes:
        crc ^= byte << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ 0x1021
            else:
                crc <<= 1
    crc &= 0xFFFF
    tc_s9ss128 = tc_bytes + crc.to_bytes(2, 'big')

    return "0x" + "".join(f"{byte:02x}" for byte in tc_s9ss128)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert the current time to TAI format.")
    parser.add_argument("-t", "--time", action="store_true", help="Display the current time in TAI format.")
    parser.add_argument("-i", "--input", type=str, help="Convert the specified CUC time to readable GMT time.")
    
    args = parser.parse_args()

    if args.time:
        print("TAI format time:", get_tai_time())
        print("CUC format time:", get_cuc_time())
        print("TC S9SS128 :", get_tc_s9ss128())
    elif args.input:
        print("GMT time for CUC time:", convert_cuc_to_time(args.input))
    else:
        parser.print_help()
