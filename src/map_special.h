#ifndef MAP_SPECIAL_H
#define MAP_SPECIAL_H

#include <sys/types.h>


typedef struct
{
    __uint8_t room_number; // high nibble = x, low nibble = y
    __uint8_t mbadguy_y; // high nibble = 0 - horizonta, 1 - vertical snake, 2 - vertical spider, low nibble = y pos
    __uint8_t mbadguy_minx_maxx; // all bad guys start in minx (low nibble) and move to maxx (high nibble)    
    __uint8_t image1;
    __uint8_t image2;
    __uint8_t special;
} bgroom;

const bgroom bgrooms[] = {

    // horizontals
    {0xe0, 0x03, 0x64, 226, 227},
    {0xc3, 0x03, 0x86, 228, 229},
    {0xb6, 0x02, 0x31, 236, 237},
    {0xb4, 0x03, 0x31, 230, 231},        
    {0xb1, 0x03, 0x30, 236, 237},
    {0xa0, 0x02, 0x62, 238, 239},
    {0x76, 0x02, 0x86, 236, 237},
    {0x90, 0x02, 0x74, 236, 237},
    {0x84, 0x01, 0x64, 234, 235},
    {0x85, 0x02, 0x73, 242, 243},
    {0x54, 0x02, 0x86, 242, 243},
    {0x66, 0x02, 0x63,  41,  42},
    {0x65, 0x01, 0x63, 242, 243},
    {0x64, 0x02, 0x53, 244, 245},
    {0x63, 0x02, 0x73, 144, 145},
    {0x40, 0x03, 0x86, 244, 245},
    {0x32, 0x01, 0x86, 230, 231},
    {0x17, 0x03, 0x85, 228, 229},
    {0x14, 0x02, 0x90, 242, 243}, // WARNING BUG if you enter from left on it direct die
    {0x12, 0x01, 0x20, 242, 243}, // WARNING BUG if you enter from left on it direct die
    {0x07, 0x02, 0x74, 242, 243},
    {0x02, 0x02, 0x31, 240, 241},    
    {0x01, 0x02, 0x62,  41,  42},

    // verticals 
    // spiders
    {0x68, 0x15, 0x31, 246, 247}, // must be cyan
    {0x58, 0x16, 0x31, 62, 63},
    {0x48, 0x16, 0x32, 248, 249}, // must be green    
    {0x38, 0x17, 0x31, 250, 251}, // must be red  
    // snakes    
    {0x20, 0x24, 0x31, 148, 149 }, // yellow cua 253
    {0x10, 0x24, 0x31, 65, 75 }, // green cua 64
    {0x00, 0x27, 0x31, 146, 147 }, // magenta cua 252
};

const __uint8_t item_magenta_key = 143;
const __uint8_t item_magenta_door = 225;
const __uint8_t item_green_key = 46;
const __uint8_t item_green_door = 56;
const __uint8_t item_prisoner = 66;
const __uint8_t item_spell_B = 47;
const __uint8_t item_spell_A = 48;
const __uint8_t item_spell_L = 49;
const __uint8_t item_spell_I = 50;
const __uint8_t item_bomb = 17;


const __uint8_t empty_block = 0;
const __uint8_t clean_block = 70;
const __uint8_t block_hole = 12;
const __uint8_t block_top_green_door = 53;
const __uint8_t block_top_magenta_door = 222;

const __uint8_t map_badguy_r = 39; // random bad guy ID not for paiting
const __uint8_t map_badguy_m = 50; // moving bad guy ID not for paiting


// special positions 

__uint16_t item_pos_magenta_key = 6576;
__uint16_t item_pos_magenta_door = 904;
__uint16_t item_pos_green_key = 433;
__uint16_t item_pos_green_door = 2465;
__uint16_t item_pos_spell_B1 = 6576;
__uint16_t item_pos_spell_A1 = 1922;
__uint16_t item_pos_spell_B2 = 433;
__uint16_t item_pos_spell_A2 = 5795;
__uint16_t item_pos_spell_L = 6242;
__uint16_t item_pos_spell_I = 6374; 
__uint16_t item_pos_spell_B3 = 2662;
__uint16_t item_pos_spell_A3 = 246;   

__uint16_t item_pos_prisoner = 6517; // screen 6,8 pos 7,3

__uint16_t specialLocations[] = {
    6576, 904, 433, 2465, // 6517 is prisoner not included 
    6576, 1922, 433, 5795, 6242, 
    6374, 2662, 246
};

typedef struct
{
    int entryscreenx; 
    int entryscreeny;
    int entryx;
    int entryy;
    int drawscreenx; // drawing screen (basically only changes stairs colors)
    int drawscreeny;
    int direction; // 1 = up/left, -1 = down/right
    int exitscreenx;
    int exitscreeny;
    int exitx;
    int exity;  
    int exitdirection; 

} stairs;

stairs stairs_data[] = {
    // 
    // baixada aranyes magenta
    {   8, // entry screen x
        5, // entry screen y
        5, // entry x
        5, // entry y
        8, // draw screen x
        6, // draw screen y
        -1, // direction
        3, // exit screen x
        8, // exit screen y
        0, // exit x
        3, // exit y
        MOVE_RIGHT },

    // retorn aranyes magenta
    {   3, // entry screen x
        8, // entry screen y
        -1, // entry x
        3, // entry y
        8, // draw screen x
        6, // draw screen y
        1, // direction
        8, // exit screen x
        5, // exit screen y
        5, // exit x
        4, // exit y
        MOVE_UP },

    { // de l'1 al 2
        10, // entry screen x
        3, // entry screen y
        5, // entry x
        -1, // entry y
        10, // draw screen x
        2, // draw screen y
        1, // direction
        2, // exit screen x
        2, // exit screen y
        6, // exit x
        0, // exit y
        MOVE_DOWN },
    
    { // del 2 a l'1
        2, // entry screen x
        2, // entry screen y
        6, // entry x
        -1, // entry y
        10, // draw screen x
        2, // draw screen y
        -1, // direction
        10, // exit screen x
        3, // exit screen y
        5, // exit x
        0, // exit y
        MOVE_DOWN },

    { // del 2 al 3
        0, // entry screen x
        7, // entry screen y
        5, // entry x
        5, // entry y
        0, // draw screen x
        8, // draw screen y
        1, // direction
        5, // exit screen x
        4, // exit screen y
        7, // exit x
        0, // exit y
        MOVE_DOWN },

    { // del 3 al 2
        5, // entry screen x
        4, // entry screen y
        7, // entry x
        -1, // entry y
        0, // draw screen x
        8, // draw screen y
        -1, // direction
        0, // exit screen x
        7, // exit screen y
        5, // exit x
        4, // exit y
        MOVE_UP },
    
    { // del 3 al 4
        3, // entry screen x
        6, // entry screen y
        6, // entry x
        5, // entry y
        3, // draw screen x
        7, // draw screen y
        1, // direction
        6, // exit screen x
        6, // exit screen y
        5, // exit x
        4, // exit y
        MOVE_UP },

    { // del 4 al 3
        6, // entry screen x
        6, // entry screen y
        5, // entry x
        5, // entry y
        3, // draw screen x
        7, // draw screen y
        -1, // direction
        3, // exit screen x
        6, // exit screen y
        6, // exit x
        4, // exit y
        MOVE_UP },

    
};

#endif // MAP_SPECIAL_H