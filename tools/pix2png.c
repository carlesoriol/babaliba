#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "raylib.h"

struct image_formats_def {
    uint16_t width;
    uint16_t height;
    uint8_t bpp;
} ;

struct image_formats_def  image_formats[3] = {
    {320, 200, 4},
    {640, 200, 2},  
    {640, 400, 1} };

uint8_t COLOR_LEVELS_3_TO_8[8] = {0x00, 0x24, 0x48, 0x6D, 0x91, 0xB6, 0xDA, 0xFF};

void *readFile( const char *fileName, long *psize)
{
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL)
    {
        printf("Error: file not found: %s\n", fileName);
        psize = 0;
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    *psize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    void *buffer = malloc(*psize);
    fread(buffer, *psize, 1, fp);
    fclose(fp);
    return buffer;
}



int main(int argc, char *argv[])
{
    InitWindow(1, 1, "");

    if( argc < 2 )
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *fileName = argv[1];

    long size;
    uint16_t *pw = readFile(fileName, &size);
    // switch WORDS
    for (int i = 0; i < size / 2; i++)
    {
        uint16_t w = pw[i];
        pw[i] = (w >> 8) | (w << 8);
    }
     

    int format = pw[0];
    Color palette[16];

    for (int c=0; c < 16; c++)
    {
        uint16_t col = pw[1+c];
        palette[c] = (Color){ COLOR_LEVELS_3_TO_8[(col >> 8) & 0x07], COLOR_LEVELS_3_TO_8[(col >> 4) & 0x07], COLOR_LEVELS_3_TO_8[col & 0x07], 255};
    }
  

    Image image = GenImageColor(image_formats[format].width, image_formats[format].height, BLACK);
    for (int c = 0; c < image_formats[format].width * image_formats[format].height; c++)
    {
        int pos = (c >> 4) * image_formats[format].bpp;
        int offset = c & 0x0f;
        int ioffset = 15 - offset;
        uint16_t bitmask = 0x8000 >> offset;

        int ncolor = 0;
        for (int bppi = 0; bppi < image_formats[format].bpp; bppi++)
            ncolor += (((pw[17 + pos + bppi] & bitmask) >> (ioffset)) << bppi);

        ImageDrawPixel(&image, c % image_formats[format].width, c / image_formats[format].width, palette[ncolor]);
    }

    free(pw);

    char exportName[256];
    snprintf(exportName, sizeof(exportName), "%s.png", fileName);
    ExportImage(image, exportName);
    CloseWindow();

    return 0;
}

// gcc -o pix2png pix2png.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O2 -D_DEFAULT_SOURCE -I. -I/home/atari/Develop/raylib/src -I/home/atari/Develop/raylib/src/external -I/usr/local/include -I/home/atari/Develop/raylib/src/external/glfw/include -L. -L/home/atari/Develop/raylib/src -L/home/atari/Develop/raylib/src -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic -DPLATFORM_DESKTOP -DPLATFORM_DESKTOP_GLFW
