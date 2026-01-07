#ifndef LIBGRAPHICS_H
#define LIBGRAPHICS_H

// test dwaring a bw image

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "lib_screen.h"

__uint16_t* videoAddress;


#define NO_BG_COLOR 255 // for drawChar

__uint16_t bytecolors[16][4] =
{
    {0x0000, 0x0000, 0x0000, 0x0000},
    {0xFFFF, 0x0000, 0x0000, 0x0000},
    {0x0000, 0xFFFF, 0x0000, 0x0000},
    {0xFFFF, 0xFFFF, 0x0000, 0x0000},
    {0x0000, 0x0000, 0xFFFF, 0x0000},
    {0xFFFF, 0x0000, 0xFFFF, 0x0000},
    {0x0000, 0xFFFF, 0xFFFF, 0x0000},
    {0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
    {0x0000, 0x0000, 0x0000, 0xFFFF},
    {0xFFFF, 0x0000, 0x0000, 0xFFFF},
    {0x0000, 0xFFFF, 0x0000, 0xFFFF},
    {0xFFFF, 0xFFFF, 0x0000, 0xFFFF},
    {0x0000, 0x0000, 0xFFFF, 0xFFFF},
    {0xFFFF, 0x0000, 0xFFFF, 0xFFFF},
    {0x0000, 0xFFFF, 0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}
};

void plot_pixel(int x, int y, int color) {    
    __uint8_t *pscr = (__uint8_t*)(videoAddress) + y * 160;
    pscr += (int)(x / 16)*8;
    __uint16_t *address = (__uint16_t *)pscr;
    unsigned short mask = 0x8000 >> (x & 15);
    
    if (color & 1) address[0] |= mask; else address[0] &= ~mask;
    // Plane 1 (bit 1 of color)
    if (color & 2) address[1] |= mask; else address[1] &= ~mask;
    // Plane 2 (bit 2 of color)
    if (color & 4) address[2] |= mask; else address[2] &= ~mask;
    // Plane 3 (bit 3 of color)
    if (color & 8) address[3] |= mask; else address[3] &= ~mask;
}

void drawImageA16( const void *pimage, const int width, const int height, const int x0, const int y0)
{
    const __uint32_t *image = (__uint32_t*)pimage;
    int blockswidth = (width-1) / 16 + 1;
    int linejump = 40 - blockswidth * 2;
    __uint32_t *pscr = (__uint32_t*)(videoAddress)+ y0*40;
    pscr += (int)(x0 / 16)*2;

    for( int y = 0; y < height; y++)
    {         
        for( int x = 0; x < blockswidth; x++)
        {
            *pscr++ = *image++;
            *pscr++ = *image++;
        }
        pscr += linejump;    
    }
}

void drawImageZeroMaskA16( const void *pimage, const int width, const int height, const int x0, const int y0)
{
    const __uint16_t *image = (__uint16_t*)pimage;
    int blockswidth = (width-1) / 16 + 1;
    int linejump = 80 - blockswidth * 4;
    __uint16_t *pscr = (__uint16_t*)(videoAddress) + y0*80;
    pscr += (int)(x0 / 16)*4;
    __uint16_t mask;

    for( int y = 0; y < height; y++)
    {         
        for( int x = 0; x < blockswidth; x++)
        {
            mask = ~(image[0] | image[1] | image[2] | image[3]); // 0 is mask                        

            *pscr++ = *pscr & mask | *image++;            
            *pscr++ = *pscr & mask | *image++;            
            *pscr++ = *pscr & mask | *image++;            
            *pscr++ = *pscr & mask | *image++;            
        }
        pscr += linejump;    
    }
}

void drawImagePx(const void *pimage, const int width, const int height, const int x0, const int y0)
{
       int rbits = x0 & 0b00001111; // nombre de bits a rotar a la dreta
    const __uint16_t *image = (__uint16_t*)pimage;
    int blockswidth = (width-1) / 16 + 1;
    int linejump = (80 - blockswidth * 4) - 4;
    __uint16_t *pscr = (__uint16_t*)(videoAddress) + y0*80;
    pscr += (int)(x0 / 16)*4;    
    __uint16_t cword;         
    __uint16_t pword0, pword1, pword2, pword3;         
    
    for ( int y = 0; y < height; y++ )
    {   
        pword0 = 0;
        pword1 = 0;
        pword2 = 0;
        pword3 = 0;

        for( int x = 0; x < blockswidth; x++)
        {                                       
            cword = *image++;
            *pscr++ =((cword >> rbits) | pword0);
            pword0 = cword << (16 - rbits);            

            cword = *image++;
            *pscr++ = ((cword >> rbits) | pword1);
            pword1 = cword << (16 - rbits);            

            cword = *image++;
            *pscr++ = ((cword >> rbits) | pword2);
            pword2 = cword << (16 - rbits);            
            
            cword = *image++;
            *pscr++ = ((cword >> rbits) | pword3);
            pword3 = cword << (16 - rbits);        
            
        }      

        *pscr++ = pword0;
        *pscr++ = pword1;
        *pscr++ = pword2;
        *pscr++ = pword3;

        pscr += linejump;  
    } 
}


void drawImage( const void *pimage, const int width, const int height, const int x0, const int y0, const __uint8_t automask)
{
    if( (x0 & 0b11110000) == x0) // Si x0 és múltiple de 16 fer el dibuixat ràpid sense rotar
    {
        if( automask )
            drawImageZeroMaskA16( pimage, width, height, x0,  y0);
        else
            drawImageA16( pimage, width, height, x0, y0);
        return;
    }

    int rbits = x0 & 0b00001111; // nombre de bits a rotar a la dreta
    const __uint16_t *image = (__uint16_t*)pimage;
    int blockswidth = (width-1) / 16 + 1;
    int linejump = (80 - blockswidth * 4) - 4;
    __uint16_t *pscr = (__uint16_t*)(videoAddress) + y0*80;
    pscr += (int)(x0 / 16)*4;
    
    __uint16_t mask;
    __uint16_t prevmask;
    __uint16_t curmask;
    
    __uint16_t cword;         
    __uint16_t pword0, pword1, pword2, pword3;             
    
    mask = 0;

    for ( int y = 0; y < height; y++ )
    {   
        pword0 = 0;
        pword1 = 0;
        pword2 = 0;
        pword3 = 0;
        prevmask = 0xffff;

        for( int x = 0; x < blockswidth; x++)
        {        
            if ( automask )              
            {
                curmask = ~(image[0] | image[1] | image[2] | image[3]); // 0 is mask                                    
                mask = (curmask >> rbits) | (prevmask << (16 - rbits));
                prevmask = curmask;
            }

            cword = *image++;
            *pscr++ = (*pscr & mask) | ((cword >> rbits) | pword0);
            pword0 = cword << (16 - rbits);            

            cword = *image++;
            *pscr++ = (*pscr & mask) | ((cword >> rbits) | pword1);
            pword1 = cword << (16 - rbits);            

            cword = *image++;
            *pscr++ = (*pscr & mask) | ((cword >> rbits) | pword2);
            pword2 = cword << (16 - rbits);            
            
            cword = *image++;
            *pscr++ = (*pscr & mask) | ((cword >> rbits) | pword3);
            pword3 = cword << (16 - rbits);        
            
        }    
        
        if( automask )
            mask = (0xffff >> rbits) | (prevmask << (16 - rbits));        

        *pscr++ = (*pscr & mask) | pword0;
        *pscr++ = (*pscr & mask) | pword1;
        *pscr++ = (*pscr & mask) | pword2;
        *pscr++ = (*pscr & mask) | pword3;

        pscr += linejump;  
    }
}

  
void drawChar( const void *pimage, const int height, const int x0, const int y0, const __uint8_t color, const __uint8_t bgcolor) // width is always 8 if bgcolor = 255 then no background
{    
    const __uint8_t *image = (__uint8_t *)pimage;
    __uint8_t *pscr = (__uint8_t*)(videoAddress) + y0*160;
    pscr += (int)(x0 / 16)*8;
    if(x0 & 0b00001000) // odd position
        pscr += 1;

    __uint16_t *bytecolor = bytecolors[color];
    __uint16_t *bytebgcolor = NULL;
    if (bgcolor != NO_BG_COLOR)
        bytebgcolor = bytecolors[bgcolor];
    
    for ( int y = 0; y < height; y++ )    
    {                   
        __uint8_t val = *image++;

        if( bgcolor == NO_BG_COLOR )
        {            
            *pscr = (*pscr & ~val) | (val & (__uint8_t)bytecolor[0]);
            pscr += 2;
            *pscr = (*pscr & ~val) | (val & (__uint8_t)bytecolor[1]);
            pscr += 2;
            *pscr = (*pscr & ~val) | (val & (__uint8_t)bytecolor[2]);
            pscr += 2;
            *pscr = (*pscr & ~val) | (val & (__uint8_t)bytecolor[3]);
            pscr += 2;            
        }
        else
        {
            __uint8_t invval = ~val;        

            *pscr = (val & (__uint8_t)bytecolor[0]) | (invval & (__uint8_t)bytebgcolor[0]);
            pscr += 2;
            *pscr = (val & (__uint8_t)bytecolor[1]) | (invval & (__uint8_t)bytebgcolor[1]);
            pscr += 2;
            *pscr = (val & (__uint8_t)bytecolor[2]) | (invval & (__uint8_t)bytebgcolor[2]);
            pscr += 2;
            *pscr = (val & (__uint8_t)bytecolor[3]) | (invval & (__uint8_t)bytebgcolor[3]);
            pscr += 2;            
        }

        pscr += 160-8;
    }
}

void drawText( const char* text, const void *pimage, const int fontheight, const int drawheight, const int x0, const int y0, const __uint8_t color, const __uint8_t bgcolor)
{
    const __uint8_t *image = (__uint8_t *)pimage;
    int x = x0;    
    int y = y0;

    while( *text )
    {
        char c=-1;

        if (*text >= 'A' && *text <= 'Z')             
            c = *text - 'A';
        else if (*text >= '0' && *text <= '9')
            c = *text - '0' + 26;
        else if (*text == '\'')
            c = 36;
        else if (*text == 'a')
            c = 37;
        else if (*text == 'o')
            c = 38;
        else if (*text == 'i')
            c = 39;
        else if (*text == 'c')
            c = 40;
        else if (*text == '_')
            c = 41;
        else if (*text == '.')
            c = 42;            
            
        if (c != -1)
            drawChar( image + ( c * fontheight), drawheight, x, y, color, bgcolor);             
        text++;
        
        x+=8;        
        if( x >= 320-7 )
        {            
            x = x0;
            y += fontheight;
            if ( y >= 200 )
                return;
        }       
    }
}

void cls( const __uint8_t color )
{
    __uint16_t bytecolor[4];
     __uint16_t *pscr = (__uint16_t*)videoAddress;
    int screensizeinwords = 4000;

    for(int bit = 0; bit < 4; bit++)
        bytecolor[bit] = ( color & (1 << bit) ) ? 0xFFFF : 0x0000;
   
    while(screensizeinwords-- )
    {              
        *pscr++ = bytecolor[0];
        *pscr++ = bytecolor[1];
        *pscr++ = bytecolor[2];
        *pscr++ = bytecolor[3];        
    }
    
}

void imageReplaceColor( __uint16_t*image, const int width, const int height, const __uint8_t srcColor, const __uint8_t dstColor)
{
    int blockswidth = (width-1) / 16 + 1;
    int totalblocs = height * blockswidth;
    __uint16_t *dstcolorbits = bytecolors[dstColor];
    
    for( int bloc = 0; bloc < totalblocs; bloc++)
    {   
        __uint16_t mask = 0;
        for( int bit = 0; bit < 16; bit ++)
        {
            int value = image[0] & (1 << bit) ? 0b0001 : 0;
            value |= image[1] & (1 << bit) ? 0b0010 : 0;
            value |= image[2] & (1 << bit) ? 0b0100 : 0;
            value |= image[3] & (1 << bit) ? 0b1000 : 0; 

            if( value == srcColor )
                mask |= (1 << bit);
        }
        mask = ~mask;
        *image++ = (*image & mask) | (dstcolorbits[0] & ~mask);
        *image++ = (*image & mask) | (dstcolorbits[1] & ~mask);
        *image++ = (*image & mask) | (dstcolorbits[2] & ~mask);
        *image++ = (*image & mask) | (dstcolorbits[3] & ~mask);        
    }
}

void shiftImageRight( __uint16_t*image, const int width, const int height, const int rbits )
{
    int blockswidth = (width-1) / 16 + 1;   
    
    __uint16_t cword;         
    __uint16_t pword0, pword1, pword2, pword3;         
    
    for ( int y = 0; y < height; y++ )
    {   
        pword0 = 0;
        pword1 = 0;
        pword2 = 0;
        pword3 = 0;

        for( int x = 0; x < blockswidth; x++)
        {                      
            cword = *image;
            *image++ = (cword >> rbits) | pword0;
            pword0 = cword << (16 - rbits);            

            cword = *image;
            *image++ = (cword >> rbits) | pword1;
            pword1 = cword << (16 - rbits);            

            cword = *image;
            *image++ = (cword >> rbits) | pword2;
            pword2 = cword << (16 - rbits);            
            
            cword = *image;
            *image++ = (cword >> rbits) | pword3;
            pword3 = cword << (16 - rbits);            
        }
        
    }
}

static __uint16_t mirrorWord(const __uint16_t value) {
    __uint16_t result;

    __asm__ __volatile__ (
        // Start: value is in %%d0 (due to "=d" and "0" constraints)

        // 1. Swap bytes (Bits 15-8 <-> 7-0)
        "ror.w #8, %0\n\t"     // Rotate Right 8 places

        // 2. Swap nibbles within each byte (15-12 <-> 11-8 and 7-4 <-> 3-0)
        "move.w %0, %%d1\n\t"  // Copy %0 (input/output) to D1
        "and.w #0xf0f0, %0\n\t" // Mask: Keep bits 15-12, 7-4
        "and.w #0x0f0f, %%d1\n\t" // Mask: Keep bits 11-8, 3-0
        "ror.w #4, %0\n\t"     // Rotate Right 4 (for the 0xf0f0 part)
        "rol.w #4, %%d1\n\t"   // Rotate Left 4 (for the 0x0f0f part)
        "or.w %%d1, %0\n\t"    // Combine the swapped nibbles

        // 3. Swap 2-bit groups (15-14 <-> 13-12, etc.)
        "move.w %0, %%d1\n\t"  // Copy %0 to D1
        "and.w #0xcccc, %0\n\t" // Mask: Keep bits 15,14, 11,10, 7,6, 3,2
        "and.w #0x3333, %%d1\n\t" // Mask: Keep bits 13,12, 9,8, 5,4, 1,0
        "ror.w #2, %0\n\t"     // Rotate Right 2 (for the 0xcccc part)
        "rol.w #2, %%d1\n\t"   // Rotate Left 2 (for the 0x3333 part)
        "or.w %%d1, %0\n\t"    // Combine the swapped 2-bit groups

        // 4. Swap 1-bit groups (Adjacent pairs)
        "move.w %0, %%d1\n\t"  // Copy %0 to D1
        "and.w #0xaaaa, %0\n\t" // Mask: Keep even bits (15, 13, 11, ...)
        "and.w #0x5555, %%d1\n\t" // Mask: Keep odd bits (14, 12, 10, ...)
        "ror.w #1, %0\n\t"     // Rotate Right 1 (for the 0xaaaa part)
        "rol.w #1, %%d1\n\t"   // Rotate Left 1 (for the 0x5555 part)
        "or.w %%d1, %0\n\t"    // Combine the swapped adjacent bits

        : "=d" (result)          // Output Operand %0: Result must be in a Data Register (d0-d7)
        : "0" (value)            // Input Operand %0: Use the same location as output (d0)
        : "d1", "cc"             // Clobber: D1 is used as temporary storage, CC (condition codes) is modified
    );

    return result;
}

void mirrorImageH( __uint16_t*image, const int width, const int height )
{
    int blockswidth = (width -1) / 16 + 1;  
    int halfblockswidth = blockswidth / 2;  
    __uint16_t*imageEnd ;
    __uint16_t tempword;
    int x;    

    for ( int y = 0; y < height; y++ )
    {  
        imageEnd = image;
        imageEnd += ((blockswidth-1) * 4);

        for( x = 0; x < halfblockswidth; x++)
        {            
            // swap words
            tempword = mirrorWord(*image);
            *image++ = mirrorWord(*imageEnd);            
            *imageEnd++ = tempword;

            tempword = mirrorWord(*image);
            *image++ = mirrorWord(*imageEnd);            
            *imageEnd++ = tempword;
            
            tempword = mirrorWord(*image);
            *image++ = mirrorWord(*imageEnd);            
            *imageEnd++ = tempword;
            
            tempword = mirrorWord(*image);
            *image++ = mirrorWord(*imageEnd);            
            *imageEnd++ = tempword;            
        }                 
        
        if( blockswidth & 0b1 != 0 )
        {
            *image++ = mirrorWord(*image);
            *image++ = mirrorWord(*image);
            *image++ = mirrorWord(*image);
            *image++ = mirrorWord(*image);            
            image += blockswidth * 2 - 2;
        }
        else
        {
            image += blockswidth * 2;
        }        
    }     
}

__uint16_t*createShiftedImage( const void *psrcImage, const int width, const int height, const int rbits )
{
    const __uint16_t *srcImage = (const __uint16_t*)psrcImage;
    int blockswidth = (width -1) / 16 + 1;    
    __uint16_t*originalDstImage = (__uint16_t*) malloc( (blockswidth + 1) * 8 );

    //printf("Allocated %d bytes for shifted image\n", (blockswidth + 1) * 8  );
    __uint16_t*dstImage = originalDstImage;

    for ( int y = 0; y < height; y++ )
    {         
        for( int x = 0; x < blockswidth; x++)
        {
           *dstImage++ = *srcImage++;
           *dstImage++ = *srcImage++;
           *dstImage++ = *srcImage++;        
           *dstImage++ = *srcImage++;
        }
        
        *dstImage++ = 0;
        *dstImage++ = 0;
        *dstImage++ = 0;
        *dstImage++ = 0;
    }

    shiftImageRight( originalDstImage, width+16, height, rbits );
    return originalDstImage;
}

#endif // LIBGRAPHICS_H