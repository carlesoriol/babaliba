#ifndef SPECCYPAL_H
#define SPECCYPAL_H

#include <sys/types.h>

static __uint16_t palette[16] = {

    0x0000, // Color 0 - Transparent
    0x0000, // Color 1 - Black
    0x0107, // Color 2 - Blue
    0x0105, // Color 3 - Dark Blue
    0x0710, // Color 4 - Red
    0x0510, // Color 5 - Dark Red
    0x0717, // Color 6 - Magenta
    0x0515, // Color 7 - Dark Magenta
    0x0071, // Color 8 - Green  
    0x0051, // Color 9 - Dark Green
    0x0177, // Color 10 - Light Cyan
    0x0155, // Color 11 - Cyan
    0x0771, // Color 12 - Yellow
    0x0551, // Color 13 - Dark Yellow
    0x0555, // Color 14 - Light Grey
    0x0777  // Color 15 - White 
};

#define COLOR_BACKGROUND   0
#define COLOR_BLACK         1
#define COLOR_BLUE          2
#define COLOR_DARK_BLUE     3
#define COLOR_RED           4
#define COLOR_DARK_RED      5
#define COLOR_MAGENTA       6
#define COLOR_DARK_MAGENTA  7
#define COLOR_GREEN         8
#define COLOR_DARK_GREEN    9
#define COLOR_LIGHT_CYAN    10
#define COLOR_CYAN          11
#define COLOR_YELLOW        12
#define COLOR_DARK_YELLOW   13
#define COLOR_GREY          14
#define COLOR_WHITE         15

#endif // SPECCYPAL_H