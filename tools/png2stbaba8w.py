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


def convert_image(input_file, mode, num):

    # Open the input image
    with Image.open(input_file) as img:
        img = img.convert("RGBA")
        width, height = img.size                
        nitems = width // 8
        
        bitsperpixel = 1 if mode == 'mono' else 4
        
        input_file_name = input_file.split('/')[-1].split('.')[0]

        print( f"""#ifndef {input_file_name.upper()}_DATA_H
#define {input_file_name.upper()}_DATA_H
\t// Imported from {input_file}\n\t// Width: {width}px, Height: {height}px, Items: {nitems}, Mode: {mode}, Total size: {nitems*height*bitsperpixel} bytes

#include <sys/types.h>

const __uint8_t {input_file_name}_data[{nitems}][{height*bitsperpixel}] = {{
    """)

        for x in range(nitems):
            print( f"\t// Item {x}\n\t{{" )            
            for y in range(height):
                byte = [0,0,0,0] if mode == 'col' else 0
                for bit in range(8):
                    pixel_x = x * 8 + bit
                    r, g, b, a = img.getpixel((pixel_x, y))
                    if a < 128:
                        color_index = 0  # Transparent
                    else:
                        color_index = getClosestPaletteColor(r, g, b)
                    
                    if mode == 'mono':
                        if color_index != 0:
                            byte |= (1 << (7 - bit))
                    else:  # color mode
                        byte[0] |= ((color_index & 0x1) << (7 - bit))
                        byte[1] |= (((color_index >> 1) & 0x1) << (7 - bit))
                        byte[2] |= (((color_index >> 2) & 0x1) << (7 - bit))
                        byte[3] |= (((color_index >> 3) & 0x1) << (7 - bit))

                if mode == 'mono':
                    if num == 'hex':
                        print(f"\t0x{byte:02X}, ")
                    else:
                        print(f"\t0b{byte:08b}, ")
                else:
                    if num == 'hex':
                        print(f"\t0x{byte[0]:02X}, 0x{byte[1]:02X}, 0x{byte[2]:02X}, 0x{byte[3]:02X}, ")
                    else:
                        print(f"\t0b{byte[0]:08b}, 0b{byte[1]:08b}, 0b{byte[2]:08b}, 0b{byte[3]:08b}, ")

            print("\t},\n")

        print("};")

        print(f"#endif // {input_file_name.upper()}_DATA_H")

def main():

    parser = argparse.ArgumentParser(description="Convert a PNG image 8bits wide to Atari ST 4bpp C source code.", epilog="By Carles Oriol 2025")
    parser.add_argument("input", help="Input PNG file")
    parser.add_argument("--mode", default="mono", help="Output language (mono or col)", choices=["mono", "col"])
    parser.add_argument("--num", default="bin", help="Output in binary or hexadecimal", choices=["bin", "hex"])

    args = parser.parse_args()

    convert_image(args.input, args.mode, args.num)

if __name__ == "__main__":
    main()
