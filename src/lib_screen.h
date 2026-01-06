#ifndef LIB_SCREEN_H
#define LIB_SCREEN_H

#include <sys/types.h>

const short LOW_RES = 0;
const short MEDIUM_RES = 1;


#define STR_ESC "\033"
#define STR_HIDE_CURSOR (STR_ESC "f")
#define STR_SHOW_CURSOR (STR_ESC "e")

// gets color value in the current palette
__uint16_t GetColor(short colorIndex) { return Setcolor(colorIndex, -1);}

// saves the current palette into a buffer
void SavePalette(__uint16_t* paletteBuffer)
{
    for(int i=0; i < 16; i++)
        paletteBuffer[i] = GetColor(i);
}

// changes the current resolution
void SetResolution(short resolution) { Setscreen(-1,-1, resolution); }

// saves the current resolution and its palette returs resolutiom
void SaveResolutionAndPalette( __uint8_t *presolution,  __uint16_t *ppalettte )
{
    *presolution = Getrez(); // Get current resolution
    SavePalette(ppalettte); // Save the palette
}

// restores the saved resolution and its palette
void RestoreResolutionAndPalette(__uint8_t presolution,  __uint16_t *ppalettte )
{
    Setscreen(-1,-1, presolution);
    Setpalette(ppalettte);
}

static __uint16_t tmppalette[16];
void SetpaletteMult(__uint16_t *palette, __uint8_t mult) // mult = 0..0x7
{
    for (int i = 0; i < 16; i++)
    {
        __uint16_t color = palette[i];
        __uint16_t r = ((color >> 8) & 0x0F) * mult;
        r /= 0x7;
        __uint16_t g = ((color >> 4) & 0x0F) * mult;
        g /= 0x7;
        __uint16_t b = (color & 0x0F) * mult;
        b /= 0x7;
        if (r > 7)
            r = 7;
        if (g > 7)
            g = 7;
        if (b > 7)
            b = 7;

        tmppalette[i] = (r << 8) | (g << 4) | b;
    }
    Setpalette(tmppalette);
}

void fadePaletteIn(__uint16_t *palette)
{
    for (int c = 0; c <= 0x7; c ++)
    {
        SetpaletteMult(palette, c);
        Vsync();
    }
}

void fadePaletteOut(__uint16_t *palette)
{
    for (int c = 0x7; c >= 0; c --)
    {
        SetpaletteMult(palette, c);
        Vsync();
    }
}

void hideCursor()
{
    printf(STR_HIDE_CURSOR);
}

void showCursor()
{
    printf(STR_SHOW_CURSOR);
}


#endif // LIB_SCREEN_H