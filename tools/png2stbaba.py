#! /usr/bin/env python3

# load image with pip

from PIL import Image
import argparse

palette = [
    (32, 32, 32),    # 0 - Transparent skip
    (0, 0, 0),       # 1 - Black
    (36, 0, 255),    # 2 - Blue
    (36, 0, 182),    # 3 - Dark Blue
    (255, 36, 0),    # 4 - Red
    (182, 36, 0),    # 5 - Dark Red
    (255, 36, 255),  # 6 - Magenta
    (182, 36, 182),  # 7 - Dark Magenta
    (0, 255, 36),    # 8 - Green
    (0, 182, 36),    # 9 - Dark Green
    (36, 255, 255),  # 10 - Light Cyan
    (36, 182, 182),  # 11 - Cyan
    (255, 255, 36),  # 12 - Yellow
    (182, 182, 36),  # 13 - Dark Yellow
    (182, 182, 182), # 14 - Light Grey
    (255, 255, 255) # 15 - White
]


def getClosestPaletteColor(r, g, b):
    closest_color_index = 1
    closest_distance = float('inf')
    
    for i in range(1, len(palette)):
        pr, pg, pb = palette[i]
        distance = (r - pr) ** 2 + (g - pg) ** 2 + (b - pb) ** 2
        if distance < closest_distance:
            closest_distance = distance
            closest_color_index = i

    return closest_color_index


def convert_image(input_file, data_only, mask_only, use_mot, lang, create_bin, no_size_info):

    # Open the input image
    with Image.open(input_file) as img:
        img = img.convert("RGBA")
        width, height = img.size
        wwidth = (width - 1) // 16 + 1

        # Prepare output data
        output_data = bytearray()
        
        binary_prefix = '0b' if not use_mot else '%'
        hexa_prefix = '0x' if not use_mot else '$'
        input_file_name = input_file.split('/')[-1].split('.')[0]
        sufix_last_line = True

        if lang == 'asm':                       
            block_prefix = f"{input_file_name}_data:\n\n\t; Imported from {input_file}\n"
            block_suffix = "\n"
            line_prefix = "\tdc.w"
            line_suffix = '\n'
            value_separator = ', '
            binary_prefix = '0b' if not use_mot else '%'
            hexa_prefix = '0x' if not use_mot else '$'

            print(block_prefix, end='')
            if not no_size_info:
                # Add size info at the beginning
                print(f"\n{line_prefix} {wwidth*16} ; Width: {wwidth*16}px", end=line_suffix)
                print(f"{line_prefix} {height} ; Height: {height}px\n", end=line_suffix)
                output_data.extend((wwidth*16).to_bytes(2, byteorder='big'))
                output_data.extend(height.to_bytes(2, byteorder='big'))
            else:
                print(f"\t; Width:{wwidth*16}px / Height:{height}px\n\n", end='')

        elif lang == 'c':
            block_prefix = f"__uint16_t {input_file_name}_data[] = {{\n\n\t// Imported from {input_file}\n\t// Width: {wwidth*16}px, Height: {height}px, Total size: {wwidth*8*height}bytes\n\n"
            block_suffix = "\n};\n"
            line_prefix = "\t"
            line_suffix = ',\n'
            value_separator = ', '
            binary_prefix = '0b' 
            hexa_prefix = '0x'
            sufix_last_line = False

            print(block_prefix, end='')
       
        for y in range(height):
            print(f"{line_prefix}", end='')
            for x in range(0, wwidth):
                words = [0,0,0,0]
                mask = 0
                for bit in range(16):
                    pixel_x = x * 16 + bit
                    bmask = 0
                    if pixel_x < width:
                        r, g, b, a = img.getpixel((pixel_x, y))
                        if a < 128:
                            color_index = 0  # Transparent
                            bmask = 1
                        else:
                            color_index = getClosestPaletteColor(r, g, b)
                            bmask = 0                       
                    else:                        
                        color_index = 0
                        bmask = 1

                    for outbit in range(4):
                        if color_index & (1 << outbit):
                            words[outbit] |= (1 << (15 - bit))                    
                    bmask <<= (15 - bit)
                    mask |= bmask
                    
                if not data_only:
                    print(f"{binary_prefix}{mask:016b}", end=value_separator if not (data_only or (mask_only and x == wwidth -1)) else '')
                    output_data.extend(mask.to_bytes(2, byteorder='big'))

                if not mask_only:
                    print(f"{hexa_prefix}{words[0]:04X}", end=value_separator)
                    print(f"{hexa_prefix}{words[1]:04X}", end=value_separator)
                    print(f"{hexa_prefix}{words[2]:04X}", end=value_separator)
                    print(f"{hexa_prefix}{words[3]:04X}", end=(value_separator if (x < wwidth -1) else ''))            
                    for i in range(4):
                        output_data.extend(words[i].to_bytes(2, byteorder='big'))

            if sufix_last_line or y < height -1:
                 print(line_suffix, end='')
              
        print(block_suffix, end='')

    if create_bin:
        bin_file = input_file.rsplit('.', 1)[0] + '.bin'
        with open(bin_file, 'wb') as bf:
            bf.write(output_data)
        print(f"\nBinary data written to {bin_file}")

def main():

    parser = argparse.ArgumentParser(description="Convert a PNG image 8bits wide to Atari ST 4bpp C or ASM source code.", epilog="By Carles Oriol 2025")
    parser.add_argument("input", help="Input PNG file")
    parser.add_argument("--dataonly", default=False, help="Image only without mask", action="store_true")
    parser.add_argument("--maskonly", default=False, help="Mask only without data", action="store_true")
    parser.add_argument("--usemot", default=False, help="Use Motorola syntax. Only in asm 0x->$ 0b->%%", action="store_true")


    # argument for output language, can only bc c or asm
    parser.add_argument("--lang", default="asm", help="Output language (asm or c)", choices=["asm", "c"])



    parser.add_argument("--createbin", default=False, help="Create a binary .bin file with the data", action="store_true")
    parser.add_argument("--nosizeinfo", default=False, help="Don't add size information in first 2 words", action="store_true")
    args = parser.parse_args()

    convert_image(args.input, args.dataonly, args.maskonly, args.usemot, args.lang, args.createbin, args.nosizeinfo)

if __name__ == "__main__":
    main()
