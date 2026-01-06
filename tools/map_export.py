#!/usr/bin/env python3

import sys
import os
import re


CURRENT_PATH = os.path.dirname(os.path.abspath(__file__))+"/"
MAP_FOLDER = CURRENT_PATH + "../data/"
MAP_BACK_FOLDER = CURRENT_PATH + "../old/"

MAP_WIDTH = 150
MAP_HEIGHT = 45
WHOLE_MAP = [0] * (MAP_WIDTH * MAP_HEIGHT)

with open(MAP_FOLDER+"map.bin", "rb") as f:
    for p in range(MAP_HEIGHT * MAP_WIDTH):                                      
        WHOLE_MAP[p] = int.from_bytes(f.read(1), "little")        

print(f"""

#ifndef MAP_DATA_H
#define MAP_DATA_H

#include <sys/types.h>

__uint8_t babaliba_map[{MAP_WIDTH}*{MAP_HEIGHT}] = {{ // {MAP_WIDTH*MAP_HEIGHT} tiles
    """ )

for y in range(MAP_HEIGHT):
    print ("\t", end= "")
    for x in range(MAP_WIDTH):        
        tile = WHOLE_MAP[y * MAP_WIDTH + x]
        print(f"0x{tile:02X}", end= ", ")
        if x % 16 == 15:
            print("\n\t", end= "")
    print("")

print("""};

#endif // MAP_DATA_H\n""")