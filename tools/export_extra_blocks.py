#!/usr/bin/env python3

import re
import sys

from extra_blocks import extra_blocks, BLOCK_NAMES, block_name, color_names
print("""

#ifndef GRAPHICSMOD_DATA_H
#define GRAPHICSMOD_DATA_H

#include <sys/types.h>
#include "speccypal.h"

// Graphics modifiers
#define GM_NO_CHANGE 	0b00000000
#define FLIP_HORIZONTAL 0b00000001
#define FLIP_VERTICAL 	0b00000010

struct graphics_modifier 
{
	const __uint8_t index;
	const __uint8_t graphic_modifiers; // e.g., FLIP_HORIZONTAL, FLIP_VERTICAL
	const __uint8_t srcimage_data;
	const __uint8_t color_modifiers[4]; // up to 4 color modifications 0 =  means no change
    const __uint8_t padding; // align to 8 bytes      
};

    """ )

print("const struct graphics_modifier graphics_modifiers[] = {\n")
print(f"\t// Items: {len(extra_blocks)}, will be extra  {len(extra_blocks)*384} bytes in game memory\n")

tindex = ""

for index, extrablock in extra_blocks.items():

    eb_name = block_name(index)
    
    print(f"\t{{\t// {eb_name} ")
    print(f"\t\t{index}, GM_NO_CHANGE, {extrablock[0]}, ", end= "")    
    newname = BLOCK_NAMES[extrablock[0]][0]+ " " + BLOCK_NAMES[extrablock[0]][1] + " " + BLOCK_NAMES[extrablock[0]][2]
    print(f" // from: {BLOCK_NAMES[extrablock[0]][0]+ " " + BLOCK_NAMES[extrablock[0]][1] + " " + BLOCK_NAMES[extrablock[0]][2]} ")
    print("\t\t{", end="")

    
    for replacement in range(4):
        if replacement < len(extrablock[1]):
            fromcolor, tocolor = extrablock[1][replacement]
            print(f" COLOR_{color_names[fromcolor]} << 4 | COLOR_{color_names[tocolor]}", end="")
        else:
            print(" 0", end="")
        if replacement < 4:
            print(",", end="")

    newname = eb_name.replace(" ", "_").replace("__", "_").replace("__", "_").lower()
    tindex += f"#define {newname} babalibapal{str(index)}_data\n"
    
    print("} },\n")

print("};\n\n" )

print("""\n\n#define N_GRAPHICS_MODIFIERS (sizeof(graphics_modifiers)/sizeof(graphics_modifiers[0]))""")
      

print("\n\n#endif // GRAPHICSMOD_DATA_H")

