import zlib
import argparse

def calculate_crc32(input_file, output_file=None):
    try:
        # Read the contents of the input file
        with open(input_file, 'rb') as file:
            data = file.read()
        
        # Compute CRC32
        crc32 = zlib.crc32(data) & 0xffffffff
        crc32_hex = format(crc32, '08x')  # Convert CRC32 to hexadecimal
        
        # If an output file is specified, duplicate the contents and add the CRC32
        if output_file:
            with open(output_file, 'wb') as file:
                file.write(data)  # Write original content
                file.write(crc32.to_bytes(4, 'big'))  # Add CRC32 in binary
            print(f"CRC32 added to output file {output_file} in binary.")
        else:
            # Otherwise, display the CRC32 in hexadecimal in the console
            print(f"CRC32: 0x{crc32_hex}")
    except FileNotFoundError:
        print(f"The {input_file} file was not found.")
    except Exception as e:
        print(f"Une erreur est survenue : {e}")

if __name__ == "__main__":
    # Configuring the argument analyser
    parser = argparse.ArgumentParser(description="Calculates and displays or adds the CRC32 of a file.")
    parser.add_argument("input_file", help="The input file for calculating the CRC32.")
    parser.add_argument("-o", "--output_file", help="The output file where containing the contents + CRC32 at the end.", default=None)
    
    # Analysis of arguments
    args = parser.parse_args()
    
    # Call the main function with the arguments
    calculate_crc32(args.input_file, args.output_file)
