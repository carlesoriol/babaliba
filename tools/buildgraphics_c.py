#! /usr/bin/env python3

import os
import sys
import subprocess

from extra_blocks import extra_blocks, BLOCK_NAMES

#GRAPHICS_DIR = os.path.join(os.path.dirname(__file__), '..', 'data','babalibac')
GRAPHICS_DIR = "data/babalibac"
TOOLS_DIR = "tools"

# list all files in the graphics directory
files = os.listdir(GRAPHICS_DIR)

# all filenames start with 'babalibapal' and end with '.png'
# get the XX number of every file name  files that match the pattern 'babalibapalXX.png'
numbers = []
for filename in files:
    if filename.startswith('babalibapal') and filename.endswith('.png'):
        number = filename[len('babalibapal'):-len('.png')]
        numbers.append(int(number))
numbers.sort()

# create a C header file with an array of all the graphics
print('#ifndef GRAPHICS_DATA_H')
print('#define GRAPHICS_DATA_H\n')
print('#include <sys/types.h>\n')
print('#include "speccypal.h"\n')

print(f"\n\t// Elements: {len(numbers)}, Total size graphics data: {len(numbers*384)}bytes\n")

aliases = []

#include all graphics data headers
for number in numbers:

    alias = (f"babalibapal{number}_data")
    if BLOCK_NAMES[number][0] != "":
        print (f"// {BLOCK_NAMES[number][0]} {BLOCK_NAMES[number][1]} {BLOCK_NAMES[number][2]}")
        alias = (f"{BLOCK_NAMES[number][0]}_{BLOCK_NAMES[number][1]}_{BLOCK_NAMES[number][2]}")+"_data"
        alias = alias.replace(" ", "_").replace("__", "_").replace("__", "_").lower()
        print(f"#define {alias.lower()} babalibapal{number}_data")

    aliases.append(alias)

    # execute ./png2st.py --dataonly --lang c /home/carles/Projectes/68000/sources/babaliba/tools/./../data/babalibac/babalibapal1.png
    cmd = [ TOOLS_DIR +'/png2stbaba.py', '--dataonly', '--lang', 'c', GRAPHICS_DIR + f'/babalibapal{number}.png' ]
    result = subprocess.run( cmd, stdout=subprocess.PIPE )
    print( result.stdout.decode('utf-8') )    

# create map array

print('__uint16_t *graphics[255] = { // 255 to book space for modifieds{\n')
number = 1
for alias in aliases:
    print ( f"\t{alias},  // {number} ")
    number += 1
    
print('};\n\n')

print('#endif // GRAPHICS_DATA_H\n')




