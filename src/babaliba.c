/**
 * @file    babaliba.c
 * @author  Carles Oriol (carles@enging.com)
 * @brief   Atari ST conversion of Dinamic Babaliba game.
 * @version 1.0
 * @date    2026-01-01 // Happy New Year!
 * 
 * Original software belong to Dinamic this
 * this adaptation is released as a fanware project.
 * 
 * This source code and tools are licensed under a 
 * Creative Commons Attribution-NonCommercial (CC BY-NC)
 * 
 * All source code is published in github
 * https://github.com/carlesoriol/babaliba
 * 
 */

// #define DEBUG
const char* const version_string = "1.00";

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <osbind.h> // For XBIOS calls (Setexc)

#include "lib_screen.h"
#include "lib_graphics.h"

#include "speccypal.h"
#include "graphics.h"
#include "loading.h"
#include "graphicsmod.h"
#include "numbers.h"
#include "font.h"
#include "marges.h"
#include "map.h"


#if !defined(CATALAN) && !defined(SPANISH) && !defined(ENGLISH)
#define SPANISH
//#define CATALAN
//#define ENGLISH
#endif
#include "strings.h"

#define NO_MOVE 0
#define MOVE_LEFT 1
#define MOVE_UP 2
#define MOVE_DOWN 3
#define MOVE_RIGHT 4
#include "map_special.h"
#include "sounds.h"

__uint8_t savedResolution;
__uint16_t savedPalette[16];

#define false 0
#define true (0 == 0)
#define nullptr ((void *)0)

#define JOYSTICK_PORT_0 0xFE
#define JOYSTICK_PORT_1 0xFF

#define TICK_TIME 20

#define BOMB_TIME 150
#define EXPLOSION_TIME 325

int time;
int lives;
int tnts;

#define BLOCK_SIZE 24
#define BLOCK_SIZE_BYTES 384
__uint16_t *extra_graphics = nullptr;

int current_screen_x;
int current_screen_y;

#define MAP_WIDTH 150
#define MAP_HEIGHT 45
#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 5
#define SCREENS_WIDTH 15 // MAP_WIDTH / SCREEN_WIDTH
#define SCREENS_HEIGHT 9 // MAP_HEIGHT / SCREEN_HEIGHT

#define SCREEN_OFFSET_X 40
#define SCREEN_OFFSET_Y 16

typedef struct
{
    int x;
    int y;
    __uint8_t direction;
    __uint8_t step;
    __uint8_t aniframe;
    __uint8_t images[8]; // image pairs U1 U2 D1 D2 L1 L2 R1 R2
    __uint8_t redraw;
    int screenx;
    int screeny;
} sprite;

sprite jj = {
    9, 9,      // mapx, mapy
    MOVE_LEFT, // direction
    0,         // movingstep
    0,         // aniframe
    {1, 2, 3, 4, 5, 6, 7, 8},
    true // redraw
};

sprite rbadguy = { // random bad guy
    -1,
    -1,         // mapx, mapy
    MOVE_LEFT,  // direction
    0,          // movingstep
    0,          // aniframe
    {},         // images
    true};      // redraw

sprite mbadguy = { // moving bad guy
    4,
    3,          // mapx, mapy
    NO_MOVE,    // direction NO_MOVE = (mbadguy.direction != NO_MOVE) false
    0,          // movingstep
    0,          // aniframe
    {},         // images
    true};      // redraw   

int mBadGuyMin;
int mBadGuyMax;
int mBadGuyType;

__uint8_t drawSprite_topCleanImage;
__uint8_t drawSprite_bottomCleanImage;

int enterScreenPositionX;
int enterScreenPositionY;

long tickTime = 0;

__uint8_t collision = false;

// random badguy
const __uint8_t rBaudGuyImagePairs[3][2] =
{
    {39, 40},   // yellow
    {37, 38},   // green with sword
    {232, 233}, // white with sword
};
#define MIN_BADGUY_DISTANCE 3 // distance in blocks to appear minimim from jj

#define JOY_UP 0x01
#define JOY_DOWN 0x02
#define JOY_LEFT 0x04
#define JOY_RIGHT 0x08
#define JOY_FIRE 0x80

#define NO_ITEM 0b00000000
#define ITEM_GREEN_KEY 0b00000001
#define ITEM_MAGENTA_KEY 0b00000010
#define ITEM_PRISONER 0b00000100
#define ITEM_PRINCESS 0b00001000
#define ITEM_TREASURE 0b00010000

__uint8_t items = NO_ITEM;
__uint8_t babaliba_spell; // 0 -> 1 - 9

const __uint8_t verticalclear[24] = { // innecesari apuntar al clear 70 ja que son tot 0s
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

__uint8_t finished;
__uint8_t redrawScreen;

volatile unsigned char joy1_data = 0;

#define CHEAT_LIVES     0b00000001
#define CHEAT_TNTS      0b00000010
#define CHEAT_TIME      0b00000100
#define CHEAT_IMMUNITY  0b00001000
#define CHEAT_NOWALLS   0b00010000

__uint8_t cheats;

long loopCounter = 0;

long bombTick = 0; // != 0 if placed
int bombPosition = 0;
__uint8_t bombScreenX = 0;
__uint8_t bombScreenY = 0;
__uint8_t bombPositionX = 0;
__uint8_t bombPositionY = 0;
long explosionTick = 0; // != 0 if explosion ongoing
__uint8_t explosionScreenX = 0;
__uint8_t explosionScreenY = 0;


void VsyncFrames(int frames)
{
    for (int i = 0; i < frames; i++)
        Vsync();
}
// Address of the _vbclock system variable
#define VBCLOCK_ADDRESS 0x00000462L
// Function to read the VBL clock (requires Supervisor Mode)
long get_vbclock()
{
    return *(long *)VBCLOCK_ADDRESS;
}


void clear_key_buffer()
{
    while (Bconstat(2) != 0)
        (void)Bconin(2);
}

__uint8_t wait_for_key_timed(int maxtime) // or joystick fire
{
    long start = get_vbclock();
    
    clear_key_buffer();

    while (Bconstat(2) == 0 && (joy1_data & JOY_FIRE) == 0)
    {
        if( (maxtime != 0 && (get_vbclock() - start) >maxtime)) // timeout
            return 0;
        
        Vsync();
    }
    
    long key = Bconin(2);
    int scan_code = (key >> 16) & 0xFF;

    VsyncFrames(5);
    clear_key_buffer();
    while( (joy1_data & JOY_FIRE) != 0) 
        Vsync();    

    return scan_code;
}

#define wait_for_key() wait_for_key_timed(0);

static inline int screenPosToMapIndex(int x, int y) __attribute__((pure));
static inline int screenPosToMapIndex(int x, int y)
{
    return (current_screen_y * SCREEN_HEIGHT + y) * MAP_WIDTH + (current_screen_x * SCREEN_WIDTH + x);    
}

int inCurrentScreen(int position)
{
    int screenx = position % MAP_WIDTH / SCREEN_WIDTH;
    int screeny = position / MAP_WIDTH / SCREEN_HEIGHT;
    return (screenx == current_screen_x && screeny == current_screen_y);
}

__uint8_t getMapBlockFromScreenXY(int x, int y)
{
    return babaliba_map[screenPosToMapIndex(x, y)];    
}

__uint8_t getMapFromScreenXY(int x, int y)
{
    if (mbadguy.direction != NO_MOVE && x == mbadguy.x && y == mbadguy.y && (explosionTick == 0 || !(current_screen_x == explosionScreenX && current_screen_y == explosionScreenY))) // moving bad guy
        return map_badguy_m;    
    return getMapBlockFromScreenXY(x,y);
}

void drawTextBabaliba(const char *text, const int x, const int y, const __uint8_t color1, const __uint8_t color2, const __uint8_t bgcolor)
{
    drawText(text, font_data, 10, 5, x, y, color1, bgcolor);
    drawText(text, ((void *)font_data) + 5, 10, 5, x, y + 5, color2, bgcolor);
}
#define drawTextBabalibaCentered(A, B, C, D, E) drawTextBabaliba((A), (160 - ((strlen(A) * 4))), (B), (C), (D), (E))

void clearGameScreen()
{
    __uint8_t *pscr = (__uint8_t *)(videoAddress) + 2576; //__uint8_t *pscr = (__uint8_t *)(videoAddress) + 16 * 160; pscr += (int)(40 / 16) * 8;
    for (int y = 0; y < 120; y++)
    {
        pscr[1] = 0;
        pscr[3] = 0;
        pscr[5] = 0;
        pscr[7] = 0;
        pscr[120] = 0;
        pscr[122] = 0;
        pscr[124] = 0;
        pscr[126] = 0;
        memset(pscr + 8, 0, 112); // 16 * 7);
        pscr += 160;
    }
}

const void *getGraphics(const int index)
{
    // dinamically create modified graphics only first time they are requested
    void *ret = graphics[index - 1];
    if (ret != nullptr)
        return ret;

    // find in modifiers and create if necesary
    for (int i = 0; i < N_GRAPHICS_MODIFIERS; i++)
    {
        const struct graphics_modifier *mod = &graphics_modifiers[i];
        if (mod->index == index)
        {
            const __uint16_t *srcimage = (const __uint16_t *)graphics[mod->srcimage_data - 1];
            __uint16_t *dstimage = extra_graphics + (i * (BLOCK_SIZE_BYTES / 2));
            memcpy(dstimage, srcimage, BLOCK_SIZE_BYTES);
            graphics[mod->index - 1] = dstimage;
            for (int c = 0; c < 4; c++)
            {
                if (mod->color_modifiers[c] == 0)
                    break; // no more modifications
                imageReplaceColor(dstimage, BLOCK_SIZE, BLOCK_SIZE, mod->color_modifiers[c] >> 4, mod->color_modifiers[c] & 0x0F);
            }
            return dstimage;
        }
    }
    return nullptr;
}

int drawImage24Height = 24;
void drawImage24(const void *pimage, const int x, const int y) // image must be 16 bits aligned
{
    int startswith16 = ((x & 0b1111) == 0);

    const __uint8_t *image = (__uint8_t *)pimage;
    __uint8_t *pscr = (__uint8_t *)(videoAddress) + y * 160;
    pscr += (int)(x / 16) * 8;

    for (int y = 0; y < drawImage24Height; y++)
    {
        if (startswith16)
        {
            *((__uint32_t *)pscr) = *((__uint32_t *)image); // copy first 16 bits
            pscr += 4;
            image += 4;
            *((__uint32_t *)pscr) = *((__uint32_t *)image); // copy first 16 bits
            pscr += 4;
            image += 4;

            *pscr++ = *image++;
            pscr++;
            image++;
            *pscr++ = *image++;
            pscr++;
            image++;
            *pscr++ = *image++;
            pscr++;
            image++;
            *pscr++ = *image++;
            pscr++;
            image++;
        }
        else
        {
            const __uint8_t *image2 = image + 1;
            pscr++;
            *pscr++ = *image++;

            pscr++;
            image++;
            *pscr++ = *image++;

            pscr++;
            image++;
            *pscr++ = *image++;

            pscr++;
            image++;
            *pscr++ = *image++;

            *pscr++ = *image2++;
            image2++;
            image++;
            *pscr++ = *image++;

            *pscr++ = *image2++;
            image2++;
            image++;
            *pscr++ = *image++;

            *pscr++ = *image2++;
            image2++;
            image++;
            *pscr++ = *image++;

            *pscr++ = *image2++;
            image2++;
            image++;
            *pscr++ = *image++;

            image++;
        }

        pscr += 160 - 16;
    }
}

void drawImage8(const void *pimage, const int x, const int y, const int height)
{
    const __uint8_t *image = (__uint8_t *)pimage;
    __uint8_t *pscr = (__uint8_t *)(videoAddress) + y * 160;
    pscr += (int)(x / 16) * 8;
    if ((x & 0b1111) != 0)
        pscr++;

    for (int y = 0; y < height; y++)
    {
        *pscr++ = *image++;
        pscr++;
        *pscr++ = *image++;
        pscr++;
        *pscr++ = *image++;
        pscr++;
        *pscr++ = *image++;
        pscr++;

        pscr += 160 - 8;
    }
}

void drawNum(const int num, const int x, const int y, const __uint8_t color, const __uint8_t bgcolor)
{
    int c;
    c = num / 1000;
    drawChar(numbers_data[c], 8, x, y, color, bgcolor);
    c = (num / 100) % 10;
    drawChar(numbers_data[c], 8, x + 8, y, color, bgcolor);
    c = (num / 10) % 10;
    drawChar(numbers_data[c], 8, x + 16, y, color, bgcolor);
    c = num % 10;
    drawChar(numbers_data[c], 8, x + 24, y, color, bgcolor);
}

void drawTime()
{
    drawNum(time, 48, 160, COLOR_WHITE, COLOR_BACKGROUND);    
}

void drawScreenXY(int screenx, int screeny)
{
    int oldx = current_screen_x;
    int oldy = current_screen_y;
    current_screen_x = screenx;
    current_screen_y = screeny;

    // preload images for current screen
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {            
            int graphicindex = getMapBlockFromScreenXY(x, y);
            if (graphicindex == 0)
                continue; // empty tile
            (void)getGraphics(graphicindex);
        }
    }    

    // and draw
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            int graphicindex = getMapBlockFromScreenXY(x, y);

            if (graphicindex == 0)
                graphicindex = clean_block; // empty tile to delete previous graphics
            const void *image = getGraphics(graphicindex);
            if (image != nullptr)
                drawImage24(image, 24 * x + SCREEN_OFFSET_X, 24 * y + SCREEN_OFFSET_Y);
        }    
    }
    current_screen_x = oldx;
    current_screen_y = oldy;
}

void drawScreen()
{
    drawTime();
    drawScreenXY(current_screen_x, current_screen_y);
    // avoid false erasing of characters
    jj.step = 0;
    rbadguy.step = 0;
    jj.redraw = true;
    rbadguy.redraw = true;
    mbadguy.redraw = true;
}

#define CLICK_MEM ((volatile __uint8_t *)0x484)
__uint8_t old_click_sound = 0;
int old_kb_delay = 0;
int old_kb_rate = 0;

void remove_click_sound(void)
{
    // all in super mode    
    old_click_sound = CLICK_MEM[0];
    CLICK_MEM[0] &= 0b11111110; // Set bit 0 to enable click sound    
    int cv = Kbrate(-1, -1);
    old_kb_delay = cv >> 8;
    old_kb_rate = cv & 0xFF;
    Kbrate(1,1);
}

void restore_click_sound(void)
{
    // all in super mode    
    CLICK_MEM[0] = old_click_sound;
    Kbrate(old_kb_delay, old_kb_rate);
}

void my_joy_handler(unsigned char *packet)
{
    __uint8_t c = packet[0];
    
    if( c == JOYSTICK_PORT_1 )
        joy1_data = packet[2];
    else if (c == JOYSTICK_PORT_0 )
        joy1_data = packet[1];
    else
        joy1_data = 0;
}

typedef struct
{
    void (*midivec)();  // MIDI interrupt
    void (*vkbdvec)();  // Keyboard interrupt
    void (*vmidvec)();  // MIDI error
    void (*statvec)();  // Status packet
    void (*mousevec)(); // Mouse packet
    void (*clockvec)(); // Clock packet
    void (*joyvec)();   // Joystick packet
} KBDVBASE;

void (*old_joy_vec)(void);

void install_joystick()
{
    KBDVBASE *vecs = (KBDVBASE *)Kbdvbase();

    /* 1. Disable mouse so it doesn't interfere */
    char mouse_off[] = {0x12};
    Ikbdws(0, mouse_off);
    /* 2. Enable Joystick reporting */
    Ikbdws(1, "\022\024");   
    /* 3. Install our handler into the joyvec slot */
    old_joy_vec = vecs->joyvec;
    vecs->joyvec = my_joy_handler;
}
 

void uninstall_joystick()
{
    KBDVBASE *vecs = (KBDVBASE *)Kbdvbase();

    // Restore old joystick handler
    vecs->joyvec = old_joy_vec;
    char joy_on[] = {0x14};
    Ikbdws(0, joy_on);
    char cmd[] = {0x08};
    Ikbdws(0, cmd); // 0x08 = Enable mouse (relative mode)
}

void getrBadGuyPosition()
{
    __uint8_t found = false;
    rbadguy.step = 0;

    while (!found)
    {
        int randx = (rand() % SCREEN_WIDTH);
        int randy = (rand() % SCREEN_HEIGHT);

        // randx, randy have to be at least 4 blocks away from jj
        if ((abs(randx - jj.x) + abs(randy - jj.y)) < MIN_BADGUY_DISTANCE)
            continue; // too close

        // if bloc is occupied, try again
        if (babaliba_map[(current_screen_y * SCREEN_HEIGHT + randy) * MAP_WIDTH + (current_screen_x * SCREEN_WIDTH + randx)] != empty_block)
            continue; // occupied

        // Avoid rbadguy in the mbadguy path NOT IN ORIGINAL, to avoid too many gaphic overloads
        if( mbadguy.direction == MOVE_LEFT || mbadguy.direction == MOVE_RIGHT )
        {
            if( randy == mbadguy.y && randx >= mBadGuyMin && randx <= mBadGuyMax )
                continue; // occupied
        }
        else if( mbadguy.direction == MOVE_UP || mbadguy.direction == MOVE_DOWN )
        {
            if( randx == mbadguy.x && randy >= mBadGuyMin && randy <= mBadGuyMax )
                continue; // occupied
        }

        rbadguy.x = randx;
        rbadguy.y = randy;

        int ri = rand() % 3;
        for (int i = 0; i < 8; i++)
            rbadguy.images[i] = rBaudGuyImagePairs[ri][i & 1];

        found = true;
    }
}

__uint8_t moveMBadGuy()
{
    if ((mbadguy.direction == NO_MOVE))
        return false; // not a bad guy
    
    if (mBadGuyType == 0)
    {
        if (mbadguy.x == mBadGuyMin)
            mbadguy.direction = MOVE_RIGHT;
        else if (mbadguy.x == mBadGuyMax)
            mbadguy.direction = MOVE_LEFT;
        mbadguy.x += (mbadguy.direction == MOVE_LEFT) ? -1 : 1;
    }
    else 
    {
        if (mbadguy.y == mBadGuyMin)
            mbadguy.direction = MOVE_DOWN;
        else if (mbadguy.y == mBadGuyMax)
            mbadguy.direction = MOVE_UP;
        mbadguy.y += (mbadguy.direction == MOVE_UP) ? -1 : 1;
    }
    
    return true;
}

__uint8_t tryToMoveRBadGuy()
{
    if (rbadguy.x == -1 || rbadguy.y == -1)
        return false; // not placed yet

    int dir = rand() % 8;
    int newx = rbadguy.x;
    int newy = rbadguy.y;
    int dx = 0;
    int dy = 0;

    switch (dir)
    {
    case 0:
        dx = -1;
        break; // left
    case 1:        
        dx = 1;
        break; // right
    case 2:
        dy = -1;
        break; // up
    case 3:
        dy = 1;
        break; // down
    case 4:    // follow JJ on x
    case 5:    // double                
        if (rbadguy.x < jj.x)
            dx = 1;        
        else if (rbadguy.x > jj.x)
            dx = -1;   
        else
            return false; // no move             
        break;
    case 6: // follow JJ on y
        if (rbadguy.y < jj.y)        
            dy = 1;
        else if (rbadguy.y > jj.y)
            dy = -1;
        else
            return false; // no move
       
        break;
    case 7: // try to repeat last move
        switch (rbadguy.direction)
        {
        case MOVE_LEFT:
            dx = -1;
            break;
        case MOVE_RIGHT:
            dx = 1;
            break;
        case MOVE_UP:
            dy = -1;
            break;
        case MOVE_DOWN:
            dy = 1;
            break;
        default:
            return false; // no move
        }
        break;
    }

    if( current_screen_x == 5 && current_screen_y == 0 && ((items & ITEM_PRINCESS) == 0) )
    {
        dx *= -1;
        dy *= -1;
    }

    if( dx == 0 && dy == 0)
        return false; // no move

    rbadguy.direction = (dx == -1) ? MOVE_LEFT : (dx == 1) ? MOVE_RIGHT : (dy == -1) ? MOVE_UP : (dy == 1) ? MOVE_DOWN : rbadguy.direction;

    newx += dx;
    newy += dy;

    // check bounds
    if (newx < 0 || newx >= SCREEN_WIDTH ||
        newy < 0 || newy >= SCREEN_HEIGHT)
        return false; // out of bounds

    // check if block is free
    __uint8_t map_value = getMapFromScreenXY(newx, newy);
    if (map_value != empty_block)
        return false; // occupied

    // move bad guy
    rbadguy.x = newx;
    rbadguy.y = newy;

    return true;
}

void drawSprite(sprite *psprite)
{
    if (!psprite->direction)
        return;

    int offsetx = 0;
    int offsety = 0;
    int eraseback = false;

    if (psprite->step > 0)
    {
        psprite->step--;
        switch (psprite->direction)
        {
        case MOVE_UP:
            offsety = psprite->step * 8;
            break;
        case MOVE_DOWN:
            offsety = -psprite->step * 8;
            break;
        case MOVE_LEFT:
            offsetx = psprite->step * 8;
            break;
        case MOVE_RIGHT:
            offsetx = -psprite->step * 8;
            break;
        }
        eraseback = true;
    }

    psprite->screenx = psprite->x * 24 + SCREEN_OFFSET_X + offsetx;
    psprite->screeny = psprite->y * 24 + SCREEN_OFFSET_Y + offsety;
    drawImage24(getGraphics(psprite->images[(psprite->direction - 1) * 2 + psprite->aniframe]), psprite->screenx, psprite->screeny);

    if (eraseback)
    {
        if (psprite->direction == MOVE_UP)
        {
            drawImage24Height = 8;
            drawImage24(getGraphics(drawSprite_bottomCleanImage), psprite->screenx, psprite->screeny + 24);
            drawImage24Height = 24;
        }
        else if (psprite->direction == MOVE_DOWN)
        {
            drawImage24Height = 8;
            drawImage24(getGraphics(drawSprite_topCleanImage), psprite->screenx, psprite->screeny - 8);
            drawImage24Height = 24;
        }
        else if (psprite->direction == MOVE_LEFT)
        {
            drawChar(getGraphics(clean_block), 24, psprite->screenx + 24, psprite->screeny, COLOR_BLACK, COLOR_BACKGROUND);
        }
        else if (psprite->direction == MOVE_RIGHT)
        {
            drawChar(getGraphics(clean_block), 24, psprite->screenx - 8, psprite->screeny, COLOR_BLACK, COLOR_BACKGROUND);
        }
    }

    psprite->redraw = false;
    if( loopCounter & 1)
        psprite->aniframe ^= 1;
}

void drawTNTs()
{
    int c;
    drawImage24(arrow_right_green_data, 176, 144);
    if (tnts >= 10)
    {
        c = tnts / 10;
        drawChar(numbers_data[c], 8, 176, 153, COLOR_DARK_GREEN, NO_BG_COLOR);
        drawChar(numbers_data[c], 8, 176, 152, COLOR_BLACK, NO_BG_COLOR);
    }
    c = tnts % 10;
    drawChar(numbers_data[c], 8, 184, 153, COLOR_DARK_GREEN, NO_BG_COLOR);
    drawChar(numbers_data[c], 8, 184, 152, COLOR_BLACK, NO_BG_COLOR);
}

void drawLives()
{
    drawImage24(arrow_right_cyan_data, 232, 144);
    drawChar(numbers_data[lives], 8, 240, 153, COLOR_CYAN, NO_BG_COLOR);
    drawChar(numbers_data[lives], 8, 240, 152, COLOR_BLACK, NO_BG_COLOR);
}

const bgroom *getBGinRoom(__uint8_t room_number)
{
    for (int i = 0; i < (sizeof(bgrooms) / sizeof(bgroom)); i++)
    {
        if (bgrooms[i].room_number == room_number)
            return &bgrooms[i];
    }
    return nullptr;
}

void initScreen()
{
    const bgroom *current_bgroom = getBGinRoom(current_screen_x << 4 | current_screen_y);
    if( current_bgroom != nullptr )
    {
        mBadGuyType = (current_bgroom->mbadguy_y & 0xf0) >> 4;
        if( mBadGuyType == 0) // horizontal mover
        {
            mbadguy.y = current_bgroom->mbadguy_y & 0x0f;
            mBadGuyMin = current_bgroom->mbadguy_minx_maxx & 0x0F;
            mBadGuyMax = (current_bgroom->mbadguy_minx_maxx >> 4) & 0x0F;
            mbadguy.x = mBadGuyMin;
            mbadguy.direction = MOVE_RIGHT;
            //printf("MBG r:%02X xy:%d,%d marg:%d,%d       \r", current_bgroom->room_number, mbadguy.x, mbadguy.y, mBadGuyMinX, mBadGuyMaxX);
        }
        else if ( mBadGuyType == 1) // snake
        {            
            mbadguy.x = current_bgroom->mbadguy_y & 0x0f;
            mBadGuyMin = current_bgroom->mbadguy_minx_maxx & 0x0F;         // n
            mBadGuyMax = (current_bgroom->mbadguy_minx_maxx >> 4) & 0x0F;
            mbadguy.y = mBadGuyMin;
            mbadguy.direction = MOVE_UP;
        }
        else // spider
        {            
            mbadguy.x = current_bgroom->mbadguy_y & 0x0f;
            mBadGuyMin = current_bgroom->mbadguy_minx_maxx & 0x0F;
            mBadGuyMax = (current_bgroom->mbadguy_minx_maxx >> 4) & 0x0F;
            mbadguy.y = mBadGuyMax;
            mbadguy.direction = MOVE_DOWN;
        }
        
        mbadguy.images[0] = current_bgroom->image1;
        mbadguy.images[1] = current_bgroom->image2;
        mbadguy.images[2] = current_bgroom->image1;
        mbadguy.images[3] = current_bgroom->image2;
        mbadguy.images[4] = current_bgroom->image1;
        mbadguy.images[5] = current_bgroom->image2;
        mbadguy.images[6] = current_bgroom->image1;
        mbadguy.images[7] = current_bgroom->image2;
        mbadguy.redraw = true;
        mbadguy.step = 0;
    }
    else    
        mbadguy.direction = NO_MOVE;    

    jj.redraw = true;
    jj.step = 0;
    enterScreenPositionX = jj.x;
    enterScreenPositionY = jj.y;
    getrBadGuyPosition();
    if( current_screen_x == 5 && current_screen_y == 0 && ((items & ITEM_PRINCESS) == 0) )
    {        
        rbadguy.images[0] = rbadguy.images[2] = rbadguy.images[4] = rbadguy.images[6] = 45;
        rbadguy.images[1] = rbadguy.images[3] = rbadguy.images[5] = rbadguy.images[7] = 76;
    }
    rbadguy.redraw = true;
    clear_key_buffer();
    drawScreen();
    redrawScreen = false;
}

void pantallaInici()
{
    drawImage24(getGraphics(47),  64, 8); // B
    drawImage24(getGraphics(48),  88, 8); // A
    drawImage24(getGraphics(47), 112, 8); // B
    drawImage24(getGraphics(48), 136, 8); // A
    drawImage24(getGraphics(49), 160, 8); // L
    drawImage24(getGraphics(50), 184, 8); // I
    drawImage24(getGraphics(47), 208, 8); // B
    drawImage24(getGraphics(48), 232, 8); // A    
    drawTextBabaliba(version_string, 224, 33, COLOR_GREEN, COLOR_DARK_GREEN, COLOR_BLACK);
    drawTextBabalibaCentered(STR_TITLE_PRESS_KEY, 7*8, COLOR_LIGHT_CYAN, COLOR_MAGENTA, COLOR_BLACK);
    drawTextBabalibaCentered(STR_TITLE_START, 9*8, COLOR_MAGENTA, COLOR_LIGHT_CYAN, COLOR_BLACK);    
    drawTextBabalibaCentered(STR_TITLE_AUTHOR, 15*8, COLOR_LIGHT_CYAN, COLOR_WHITE, COLOR_BLACK);
    drawTextBabalibaCentered(STR_TITLE_COPYRIGHT, 17*8, COLOR_WHITE, COLOR_GREEN, COLOR_BLACK);
    drawTextBabalibaCentered(STR_TITLE_FANWARE, 21*8, COLOR_YELLOW, COLOR_GREY, COLOR_BLACK);    
    drawTextBabalibaCentered(STR_TITLE_CONVERTER, 23*8, COLOR_WHITE, COLOR_RED, COLOR_BLACK);
    fadePaletteIn(palette);
}

void drawItems()
{
    __uint8_t r = rand() & 0b11;
    //lletres aconseguides: blanc verd, falten: light cyan, magenta
    if( items & ITEM_PRINCESS )
        drawImage24(r & 0b1 ? princess_left_down_data : princess_right_up_data, 96, 144);
    else if( items & ITEM_MAGENTA_KEY)
        drawImage24(getGraphics(143), 96, 144);
    else
        drawImage24(getGraphics(clean_block), 96, 144); // empty key slot

    if( items & ITEM_PRISONER )
        drawImage24(r & 0b10 ? prisoner_left_data : prisoner_right_data, 120, 144);
    else
    {
        drawImage24(getGraphics(clean_block), 120, 144); // empty prisoner slot
        if( current_screen_x == 6 && current_screen_y == 8 )
            drawImage24(r & 0b10 ? prisoner_left_data : prisoner_right_data, 7*24+SCREEN_OFFSET_X, 3*24+SCREEN_OFFSET_Y); // draw prisoner in the screen  
    }
    if( items & ITEM_TREASURE )
        drawImage24(treasure_chest_magenta_left_data, 144, 144);
    else if( items & ITEM_GREEN_KEY )
        drawImage24(key_green_data, 144, 144);
    else
        drawImage24(getGraphics(clean_block), 144, 144); // empty treasure slot
}

void drawSpell()
{
    drawImage24(getGraphics((babaliba_spell > 1) ? 47 : 191), 64, 176); // B
    drawImage24(getGraphics((babaliba_spell > 2) ? 48 : 192), 88, 176);  // A
    drawImage24(getGraphics((babaliba_spell > 3) ? 47 : 191), 112, 176); // B
    drawImage24(getGraphics((babaliba_spell > 4) ? 48 : 192), 136, 176); // A
    drawImage24(getGraphics((babaliba_spell > 5) ? 49 : 193), 160, 176); // L
    drawImage24(getGraphics((babaliba_spell > 6) ? 50 : 194), 184, 176); // I
    drawImage24(getGraphics((babaliba_spell > 7) ? 47 : 191), 208, 176); // B
    drawImage24(getGraphics((babaliba_spell > 8) ? 48 : 192), 232, 176); // A
}

void drawGameBackground()
{

    drawImage8(marges_data[1], 32, 8, 8);
    drawImage8(marges_data[3], 280, 8, 8);
    drawImage8(marges_data[7], 32, 168, 8);
    drawImage8(marges_data[5], 280, 168, 8);
    drawImage8(marges_data[11], 32, 136, 8);
    drawImage8(marges_data[12], 280, 136, 8);
    for (int y = 16; y < 136; y += 8)
    {
        drawImage8(marges_data[0], 32, y, 8);
        drawImage8(marges_data[4], 280, y, 8);
    }
    for (int y = 144; y < 168; y += 8)
    {
        drawImage8(marges_data[0], 32, y, 8);
        drawImage8(marges_data[4], 280, y, 8);
    }
    for (int x = 40; x < 280; x += 8)
    {
        drawImage8(marges_data[2], x, 8, 8);
        drawImage8(marges_data[13], x, 136, 8);
        drawImage8(marges_data[6], x, 168, 8);
    }
    drawImage8(marges_data[16], 88, 136, 8);
    drawImage8(marges_data[10], 168, 136, 8);
    drawImage8(marges_data[15], 88, 168, 8);
    drawImage8(marges_data[9], 168, 168, 8);
    for (int y = 144; y < 168; y += 8)
    {
        drawImage8(marges_data[14], 88, y, 8);
        drawImage8(marges_data[8], 168, y, 8);
    }
    
    drawImage24(tnt_data, 200, 144);
    drawImage24(johny_jones_life_data, 256, 144);
    drawTextBabaliba(STR_TIME, 40, 146, COLOR_DARK_YELLOW, COLOR_CYAN, NO_BG_COLOR);

    drawSpell();
    drawTNTs();
    drawLives();
    drawItems();
    drawTime();
}


char filename[] = STR_SAVEGAME_FILENAME;
void saveGame(int slot)
{
    __uint16_t magic = 0xBABA;
    filename[4] = '1'+slot;
    FILE *f = fopen(filename, "wb");
    if (f != nullptr)
    {
        fwrite(&magic, sizeof(magic), 1, f); // start check
        fwrite(&current_screen_x, sizeof(current_screen_x), 1, f);
        fwrite(&current_screen_y, sizeof(current_screen_y), 1, f);        
        fwrite(&jj, sizeof(jj), 1, f);
        fwrite(&rbadguy, sizeof(rbadguy), 1, f);
        fwrite(&mbadguy, sizeof(mbadguy), 1, f);
        fwrite(&cheats, sizeof(cheats), 1, f);
        fwrite(&mBadGuyMin, sizeof(mBadGuyMin), 1, f);
        fwrite(&mBadGuyMax, sizeof(mBadGuyMax), 1, f);
        fwrite(&mBadGuyType, sizeof(mBadGuyType), 1, f);
        fwrite(&enterScreenPositionX, sizeof(enterScreenPositionX), 1, f);
        fwrite(&enterScreenPositionY, sizeof(enterScreenPositionY), 1, f);
        fwrite(&items, sizeof(items), 1, f);
        fwrite(&babaliba_spell, sizeof(babaliba_spell), 1, f);
        fwrite(&tnts, sizeof(tnts), 1, f);
        fwrite(&lives, sizeof(lives), 1, f);
        fwrite(&time, sizeof(time), 1, f);        
        fwrite(&loopCounter, sizeof(loopCounter), 1, f);        
        fwrite(&tickTime, sizeof(tickTime), 1, f);
        fwrite(&bombTick, sizeof(bombTick), 1, f);
        fwrite(&explosionTick, sizeof(explosionTick), 1, f);
        fwrite(&bombPosition, sizeof(bombPosition), 1, f);
        fwrite(&bombPositionX, sizeof(bombPositionX), 1, f);
        fwrite(&bombPositionY, sizeof(bombPositionY), 1, f);
        fwrite(&bombScreenX, sizeof(bombScreenX), 1, f);
        fwrite(&bombScreenY, sizeof(bombScreenY), 1, f);        
        fwrite(&explosionScreenX, sizeof(explosionScreenX), 1, f);
        fwrite(&explosionScreenY, sizeof(explosionScreenY), 1, f);                 
        for( int i = 0; i < sizeof(specialLocations)/sizeof(__uint16_t); i++)
            fwrite(&babaliba_map[specialLocations[i]], 1, 1, f);        
        fwrite(&magic, sizeof(magic), 1, f); // end check
        fclose(f);
    }
}

void cleanSpecialPositions()
{
     for( int i = 0; i < sizeof(specialLocations)/sizeof(__uint16_t); i++)
        babaliba_map[specialLocations[i]] = empty_block;
}

__uint8_t loadGame(int slot)
{    
    __uint16_t magic = 0x0000;
    filename[4] = '1'+slot;
    FILE *f = fopen(filename, "rb");
    if (f != nullptr)
    {
        fread(&magic, sizeof(magic), 1, f); // start check
        if( magic != 0xBABA )
        {
            fclose(f);
            return false; // invalid save game not affected. continues current game
        }
        cleanSpecialPositions();
        fread(&current_screen_x, sizeof(current_screen_x), 1, f);
        fread(&current_screen_y, sizeof(current_screen_y), 1, f);        
        fread(&jj, sizeof(jj), 1, f);
        fread(&rbadguy, sizeof(rbadguy), 1, f);
        fread(&mbadguy, sizeof(mbadguy), 1, f);
        fread(&cheats, sizeof(cheats), 1, f);
        fread(&mBadGuyMin, sizeof(mBadGuyMin), 1, f);
        fread(&mBadGuyMax, sizeof(mBadGuyMax), 1, f);
        fread(&mBadGuyType, sizeof(mBadGuyType), 1, f);
        fread(&enterScreenPositionX, sizeof(enterScreenPositionX), 1, f);
        fread(&enterScreenPositionY, sizeof(enterScreenPositionY), 1, f);
        fread(&items, sizeof(items), 1, f);
        fread(&babaliba_spell, sizeof(babaliba_spell), 1, f);
        fread(&tnts, sizeof(tnts), 1, f);
        fread(&lives, sizeof(lives), 1, f);
        fread(&time, sizeof(time), 1, f);        
        fread(&loopCounter, sizeof(loopCounter), 1, f);        
        fread(&tickTime, sizeof(tickTime), 1, f);
        fread(&bombTick, sizeof(bombTick), 1, f);
        fread(&explosionTick, sizeof(explosionTick), 1, f);
        fread(&bombPosition, sizeof(bombPosition), 1, f);
        fread(&bombPositionX, sizeof(bombPositionX), 1, f);
        fread(&bombPositionY, sizeof(bombPositionY), 1, f);
        fread(&bombScreenX, sizeof(bombScreenX), 1, f);
        fread(&bombScreenY, sizeof(bombScreenY), 1, f);        
        fread(&explosionScreenX, sizeof(explosionScreenX), 1, f);
        fread(&explosionScreenY, sizeof(explosionScreenY), 1, f);        
        for( int i = 0; i < sizeof(specialLocations)/sizeof(__uint16_t); i++)
            fread(&babaliba_map[specialLocations[i]], 1, 1, f);      
        fread(&magic, sizeof(magic), 1, f); // start check
        fclose(f);
        if( magic != 0xBABA )
        {
            finished = true;            
            return false; // invalid save
        }          
        babaliba_map[item_pos_prisoner] = (items & ITEM_PRISONER) ? empty_block : item_prisoner;
        int newtickTime = get_vbclock();             
        if (bombTick != 0)
        {
            babaliba_map[bombPosition] = item_bomb;
            bombTick = newtickTime - (bombTick-tickTime);
        }
        if( explosionTick != 0 )
            explosionTick = newtickTime - (explosionTick-tickTime);
        tickTime = newtickTime;
        return true;
    }
    return false;
}

void startGame()
{
    current_screen_x = 14;
    current_screen_y = 4;
    jj.x = 7;
    jj.y = 2;
    enterScreenPositionX = jj.x;
    enterScreenPositionY = jj.y;
    items = NO_ITEM;
    babaliba_spell = 0;    
    tnts = 20;
    lives = 5;
    time = 2000;
    bombTick = 0;
    explosionTick = 0;
    cleanSpecialPositions();

    babaliba_map[item_pos_green_key] = item_green_key;
    babaliba_map[item_pos_magenta_key] = item_magenta_key;
    babaliba_map[item_pos_green_door] = item_green_door;
    babaliba_map[item_pos_magenta_door] = item_magenta_door;    
    babaliba_map[item_pos_prisoner] = item_prisoner;
    
    #ifdef DEBUG
    cheats = CHEAT_LIVES | CHEAT_TNTS | CHEAT_TIME | CHEAT_IMMUNITY | CHEAT_NOWALLS;
    #else
    cheats = 0;
    #endif    
    loopCounter = 0;

    drawGameBackground();
    clear_key_buffer();
}

void fallScreen()
{
    Dosound( fx_fall );
    drawScreenXY(2, 8);                

    int py = 0;
    int leftC = 0;
    int rightC = 0;
    for( int y = 0; y < 27; y++ )
    {
        py = 24+y*8;

        __uint8_t r = rand() & 0b11;
        if( r == 1 )                    
            leftC^=1;
        if( r == 2 )
            rightC^=1;

        Vsync();
        if( y<9)
        {
            drawImage24(getGraphics(((y&1)!=0)?5:6), 144, 24+y*8);
            drawImage24Height = 8;
            drawImage24(getGraphics(clean_block), 144, py - 8);
            drawImage24Height = 24;
            drawImage24(getGraphics(((y&1)!=0)?5:6), 144, 24+y*8);
        }
        else if (y == 9)
            drawImage24(getGraphics(clean_block), 144, 88);

        drawImage24(getGraphics(((leftC)!=0)?73:74), 128, 88);
        drawImage24(getGraphics(((rightC)!=0)?43:44), 160, 88);

        VsyncFrames(5);
    }
    collision = true;       
}

void main()
{
    videoAddress = Logbase(); // get the logical pointer of the video RAM
    long olds = Super(0);     // Switch to supervisor mode
    srand(get_vbclock());
    SaveResolutionAndPalette(&savedResolution, savedPalette);
    SetResolution(0); // Set to 320x200x16
    
    remove_click_sound();
    install_joystick();
    hideCursor();
    SetpaletteMult(palette, 0);
    cls(COLOR_BACKGROUND);
    drawSprite_topCleanImage = clean_block;
    drawSprite_bottomCleanImage = clean_block;
    
    extra_graphics = (__uint16_t *)malloc(BLOCK_SIZE_BYTES * N_GRAPHICS_MODIFIERS);
    
    // preload
    // babaliba letters
    getGraphics(191);
    getGraphics(192);
    getGraphics(193);
    getGraphics(194);

__uint8_t startwithgameloaded = false;


    __uint8_t exitgame = false;
        
    drawImage( loading_data, 256, 192, 32, 4, false);
    fadePaletteIn(palette);        
    (void)wait_for_key_timed(250);

    
    while( !exitgame )
    {    
        startwithgameloaded = false;
    // title screen
    // splash screen
    __uint8_t ret = 0;
    fadePaletteOut(palette);
    cls(COLOR_BACKGROUND);
    pantallaInici();
    while( ret == 0)
    {
        Dosound( fx_babaliba_song );
        ret = wait_for_key_timed(600);        
        if( ret==1)
            exitgame = true;        
        else if (ret >= 0x3b && ret <= 0x3e) // F7 - Load game
        {
            tickTime = get_vbclock();
            if(loadGame(ret-0x3b))            
                startwithgameloaded = true;
            else
                ret = 0; // stay in title screen
        }
    }
    fadePaletteOut(palette);
    cls(COLOR_BACKGROUND);
    if( exitgame )
        break;
    Dosound( fx_mute );
        
    __uint8_t onTickTime = false;
    if ( !startwithgameloaded )
    {
        startGame();
        tickTime = get_vbclock();        
        initScreen();
    }
    else
    {
        drawGameBackground();
        drawScreen();
        drawSpell();
        drawTNTs();
        drawLives();
        drawItems();
        drawTime();
    }

    fadePaletteIn(palette);
    finished = false;

    while (!finished)
    {
        collision = false;        
        onTickTime = false;
        loopCounter++;

        long now = get_vbclock();
        if (now - tickTime > TICK_TIME) // approx 22 ticks per second
        {
            tickTime = now;
            if (time > 0 && !(cheats & CHEAT_TIME))
                time--;

            // update
            drawTime();
            rbadguy.redraw = true;
            onTickTime = true;
            drawItems();
        }

        unsigned char stick = joy1_data; // Read Joystick 1 (the primary joystick port)
        // printf("Joy data: %02X\r", stick);

        if (Bconstat(2) != 0)
        {
            long key = Bconin(2);
            int scan_code = (key >> 16) & 0xFF;
            char c = key & 0xFF;

            if (scan_code == 1) // ESC key
            {
                finished = true;
                break;
            }
            else if (jj.step == 0)            
                stick |= (scan_code == 0x10) ? JOY_UP : (scan_code == 0x1e) ? JOY_DOWN : (scan_code == 0x18) ? JOY_LEFT : (scan_code == 0x19) ? JOY_RIGHT : (scan_code == 0x39) ? JOY_FIRE : 0;                            

            if (scan_code == 0x44) // F10 Pause
            {                       
                drawTextBabalibaCentered(STR_PAUSE, 64, COLOR_WHITE, COLOR_DARK_RED, COLOR_BLACK);
                drawTextBabalibaCentered(STR_PRESS_A_KEY, 80, COLOR_WHITE, COLOR_DARK_YELLOW, COLOR_BLACK);
                VsyncFrames(50); // wait a bit to avoid immediate unpause
                clear_key_buffer();
                while (Bconstat(2) == 0)
                    Vsync();
                clear_key_buffer();                
                drawScreen();
            }

            if (scan_code == 0x43) // F9 - Pantalla POKES
            {                
                clearGameScreen();
                __uint8_t exitcheats = false;                      
                drawTextBabaliba(STR_POKES_P, 250, 36, COLOR_WHITE, COLOR_DARK_RED, COLOR_BLACK);                
                drawTextBabaliba(STR_POKES_O, 250, 47, COLOR_WHITE, COLOR_DARK_RED, COLOR_BLACK);                
                drawTextBabaliba(STR_POKES_K, 250, 58, COLOR_WHITE, COLOR_DARK_RED, COLOR_BLACK);                
                drawTextBabaliba(STR_POKES_E, 250, 69, COLOR_WHITE, COLOR_DARK_RED, COLOR_BLACK);                
                drawTextBabaliba(STR_POKES_S, 250, 80, COLOR_WHITE, COLOR_DARK_RED, COLOR_BLACK);                
                drawTextBabaliba(STR_POKES_LIVES, 48, 22, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                drawTextBabaliba(STR_POKES_BOMBS, 48, 33, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                drawTextBabaliba(STR_POKES_IMMUNITY, 48, 44, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                drawTextBabaliba(STR_POKES_TIME, 48, 55, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                drawTextBabaliba(STR_POKES_GREEN_KEY, 48, 66, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                drawTextBabaliba(STR_POKES_VIOLET_KEY, 48, 77, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                drawTextBabaliba(STR_POKES_PRINCESS, 48, 88, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                drawTextBabaliba(STR_POKES_PRISONER, 48, 99, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                drawTextBabaliba(STR_POKES_TREASURE, 48, 110, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                drawTextBabaliba(STR_POKES_SPELL, 48, 121, COLOR_YELLOW, COLOR_CYAN, COLOR_BLACK);
                    
                while( !exitcheats )
                {     
                    #define CHEAT_YES_NO(CONDITION, X, Y) \
                        if( CONDITION ) \
                            drawTextBabaliba(STR_YES, X, Y, COLOR_WHITE, COLOR_BLUE, COLOR_BLACK); \
                        else \
                            drawTextBabaliba(STR_NO, X, Y, COLOR_RED, COLOR_DARK_YELLOW, COLOR_BLACK);
                                                
                    CHEAT_YES_NO( cheats & CHEAT_LIVES, 208, 22 );
                    CHEAT_YES_NO( cheats & CHEAT_TNTS, 208, 33 );
                    CHEAT_YES_NO( cheats & CHEAT_IMMUNITY, 208, 44 );
                    CHEAT_YES_NO( cheats & CHEAT_TIME, 208, 55 );
                    CHEAT_YES_NO( items & ITEM_GREEN_KEY, 208, 66 );
                    CHEAT_YES_NO( items & ITEM_MAGENTA_KEY, 208, 77 );
                    CHEAT_YES_NO( items & ITEM_PRINCESS, 208, 88 );
                    CHEAT_YES_NO( items & ITEM_PRISONER, 208, 99 );
                    CHEAT_YES_NO( items & ITEM_TREASURE, 208, 110 );

                    if( cheats & CHEAT_NOWALLS ) // hiden cheat for debug                       
                        drawTextBabaliba( STR_W, 250, 121, COLOR_WHITE, COLOR_BLUE, COLOR_BLACK);
                    else
                        drawTextBabaliba( STR_W, 250, 121, COLOR_BACKGROUND, COLOR_BACKGROUND, COLOR_BACKGROUND);                    

                    __uint8_t waitkey = wait_for_key();                    
                    if ( waitkey == 0x02 ) // key 1
                        cheats ^= CHEAT_LIVES;
                    else if ( waitkey == 0x03 ) // key 2
                        cheats ^= CHEAT_TNTS;
                    else if ( waitkey == 0x04 ) // key 3
                        cheats ^= CHEAT_IMMUNITY;
                    else if ( waitkey == 0x05 ) // key 4
                        cheats ^= CHEAT_TIME;
                    else if ( waitkey >= 0x06 && waitkey <= 0x0A ) // keys 5 to 9 and 0 for items
                    {      
                        Dosound( fx_get_item );                  
                        if ( waitkey == 0x06 ) // key 5 clau verda     // elimina la porta lila
                        {                                   
                            items ^= ITEM_GREEN_KEY;                                    
                            babaliba_map[item_pos_magenta_door] = (items & ITEM_GREEN_KEY) ? empty_block : item_magenta_door;                            
                        }
                        else if ( waitkey == 0x07 ) // key 6 clau lila  // elimina la porta verda
                        {
                            items ^= ITEM_MAGENTA_KEY;                    
                            babaliba_map[item_pos_green_door] = (items & ITEM_MAGENTA_KEY) ? empty_block : item_green_door;
                        }
                        else if ( waitkey == 0x08 ) // key 7 princesa
                            items ^= ITEM_PRINCESS;                   
                        else if ( waitkey == 0x09 ) // key 8 prisionero        
                        {            
                            items ^= ITEM_PRISONER;                    
                            babaliba_map[item_pos_prisoner] = (items & ITEM_PRISONER) ? item_prisoner : empty_block;
                        }
                        else if ( waitkey == 0x0A ) // key 9 tesoro
                            items ^= ITEM_TREASURE;                    
                        drawItems();                        
                    }
                    else if ( waitkey == 0x0B ) // key 0 hechizo
                    {
                        babaliba_spell ++;
                        if( babaliba_spell > 9 )
                            babaliba_spell = 0;                        
                        drawSpell();
                    }
                    else if ( waitkey == 0x11 ) // key w                    
                        cheats ^= CHEAT_NOWALLS;                    
                    else                    
                        exitcheats = true;     

                }                
                drawScreen();
            }        

            if (scan_code >= 0x54 && scan_code <= 0x57) // F8 - Save game
            {
                saveGame(scan_code-0x54);
                drawTextBabalibaCentered(STR_GAME_SAVED, 80, COLOR_WHITE, COLOR_DARK_GREEN, COLOR_BLACK);
                VsyncFrames(25);
                drawScreen();
            }
            else if (scan_code >= 0x3b && scan_code <= 0x3e) // F7 - Load game
            {
                if(loadGame(scan_code-0x3b))
                {
                    drawScreen();
                    drawSpell();
                    drawTNTs();
                    drawLives();
                    drawItems();
                    drawTime();
                    drawTextBabalibaCentered(STR_GAME_LOADED, 80, COLOR_WHITE, COLOR_DARK_GREEN, COLOR_BLACK);
                    VsyncFrames(25);
                    drawScreen();                    
                }
                else
                {
                    if( finished ) // invalid load during game. game messed
                        break;
                }
            }

            clear_key_buffer();
        }

        if (jj.step == 0)
        {
            int blocked = 0;

            __uint8_t mapBlock = 0;
            
            if (stick != 0)
            {  
                jj.redraw = true;

                int dy = !!(stick & JOY_DOWN) - !!(stick & JOY_UP);
                int dx = (dy == 0) ? (!!(stick & JOY_RIGHT) - !!(stick & JOY_LEFT)) : 0; // evitant diagonals

                mapBlock = getMapFromScreenXY(jj.x + dx, jj.y + dy);                

                // extra movement for doors position
                if( (jj.y == 1) && (dy == -1) && ( (current_screen_x == 0 && current_screen_y == 1) || (current_screen_x == 6 && current_screen_y == 3) ) )                        
                {
                    jj.y--; // extra movement up after door                        
                    jj.redraw = false;
                }        
                                
                if( mapBlock == block_top_green_door || mapBlock == block_top_magenta_door )
                    mapBlock = empty_block; // allow to pass through door tops
                
                // jj moved check new position for items
                if( mapBlock == item_green_key ) // green key  
                {                                 
                    
           
                    items |= ITEM_GREEN_KEY;
                    babaliba_map[item_pos_green_door] = empty_block;
                    babaliba_map[item_pos_green_key] = empty_block;
                    mapBlock = empty_block;
                    drawItems();
                    Dosound( fx_get_item );     
                    VsyncFrames(13);              
                    drawImage24(getGraphics(clean_block),  // remove letter from screen
                                (jj.x + dx) * 24 + SCREEN_OFFSET_X,
                                (jj.y + dy) * 24 + SCREEN_OFFSET_Y);
                }
                else if( mapBlock == item_magenta_key ) // magenta key
                {                    
                    
                    items |= ITEM_MAGENTA_KEY;                    
                    babaliba_map[item_pos_magenta_door] = empty_block;
                    babaliba_map[item_pos_magenta_key] = empty_block;
                    mapBlock = empty_block;
                    drawItems();
                    Dosound( fx_get_item );     
                    VsyncFrames(13);              
                    drawImage24(getGraphics(clean_block),  // remove letter from screen
                                (jj.x + dx) * 24 + SCREEN_OFFSET_X,
                                (jj.y + dy) * 24 + SCREEN_OFFSET_Y);
                }
                else if( mapBlock == block_hole ) // hole
                {
                    fallScreen();
                    collision = true;
                }                 
                
                if( ( mapBlock == item_spell_B && (babaliba_spell == 1 || babaliba_spell == 3 || babaliba_spell ==7) ) ||
                    ( mapBlock == item_spell_A && (babaliba_spell == 2 || babaliba_spell == 4 || babaliba_spell ==8) ) || 
                    ( mapBlock == item_spell_L && (babaliba_spell == 5) ) ||
                    ( mapBlock == item_spell_I && (babaliba_spell == 6) ) )                    
                {                    
                    babaliba_map[screenPosToMapIndex(jj.x + dx, jj.y + dy)] = empty_block;
                    babaliba_spell++;
                    mapBlock = empty_block;
                    drawSpell();                    
                    Dosound( fx_get_item );     
                    VsyncFrames(13);              
                    drawImage24(getGraphics(clean_block),  // remove letter from screen
                                 (jj.x + dx) * 24 + SCREEN_OFFSET_X,
                                 (jj.y + dy) * 24 + SCREEN_OFFSET_Y);
                }
                
                if (mapBlock == item_prisoner) // prisoner
                {
                    babaliba_map[item_pos_prisoner] = empty_block;
                    clearGameScreen();
                    drawTextBabalibaCentered(STR_RESCUED_PRISONER, 68, COLOR_WHITE, COLOR_LIGHT_CYAN, COLOR_BLACK);
                    items |= ITEM_PRISONER;
                    Dosound( fx_got_special );
                    VsyncFrames(75);
                    (void)wait_for_key();                
                    drawScreen();
                    mapBlock = empty_block;
                }       

                if(cheats & CHEAT_NOWALLS) mapBlock = empty_block;

                if (mapBlock == empty_block && (dx != 0 || dy != 0) ) 
                {
                    jj.y += dy;                        
                    jj.x += dx;
                    jj.step = 3;                    
                }
                else
                    blocked = 1;

                jj.direction = (dx == -1) ? MOVE_LEFT : (dx == 1) ? MOVE_RIGHT : (dy == -1) ? MOVE_UP : (dy == 1) ? MOVE_DOWN : jj.direction;    
                
                if( (stick & JOY_FIRE) && bombTick == 0 && (tnts > 0 || (cheats & CHEAT_TNTS)) )
                {
                    bombPosition = screenPosToMapIndex(jj.x, jj.y);
                    babaliba_map[bombPosition] = item_bomb;
                    bombTick = tickTime;
                    bombPositionX = jj.x;
                    bombPositionY = jj.y;
                    bombScreenX = current_screen_x;
                    bombScreenY = current_screen_y;
                    tnts--;
                    drawTNTs();
                }
            }

            if( bombTick == 0) // avoid walk sound to overlap with bomb sound
            {
                if (jj.step > 0)                
                    Dosound(fx_walk);
                if (blocked)                
                    Dosound(fx_blocked);
            }
        }

        if (rbadguy.step == 0 && onTickTime) 
        {
            // try to move rBadGuy
            if (tryToMoveRBadGuy())
                rbadguy.step = 3;
            rbadguy.redraw = true;
        }
        if (mbadguy.step == 0 && (mbadguy.direction != NO_MOVE))
        {
            // try to move mBadGuy
            moveMBadGuy();
            mbadguy.step = 3;
            mbadguy.redraw = true;
        }

        // check if jj position is in screen
        if (jj.x < 0 || jj.x >= SCREEN_WIDTH || jj.y < 0 || jj.y >= SCREEN_HEIGHT)
        {
            //comprovar escales
            // iterate stais in stairs_data
            for ( int i = 0; i < sizeof(stairs_data)/sizeof(stairs_data[0]); i++ )
            {
                stairs *pstairs = &stairs_data[i];
                if( pstairs->entryscreenx == current_screen_x &&
                    pstairs->entryscreeny == current_screen_y &&
                    pstairs->entryx ==  jj.x &&
                    pstairs->entryy ==  jj.y )
                {                    
                    drawScreenXY(pstairs->drawscreenx, pstairs->drawscreeny);
                    jj.step = 0;
                    int direction = pstairs->direction; // -1 = up/left, 1 = down/right
                    // animate stairs
                    jj.direction = (direction == -1) ? MOVE_RIGHT : MOVE_LEFT;
                    for( int s = (direction == -1) ? 0 : 3; 
                            s != ((direction == -1) ? 4 : -1);
                            s += (direction*-1))
                    {
                        jj.x = 3 + s;
                        jj.y = s;
                        Dosound(fx_stairs);
                        drawSprite(&jj);
                        VsyncFrames(15);
                        drawImage24(getGraphics(clean_block), jj.x*24+SCREEN_OFFSET_X, jj.y*24+SCREEN_OFFSET_Y); // remove stairs image
                    }
                    jj.x = pstairs->exitx;
                    jj.y = pstairs->exity;
                    jj.direction = pstairs->exitdirection;
                    current_screen_x = pstairs->exitscreenx;
                    current_screen_y = pstairs->exitscreeny;                    
                    drawScreen();
                    break;     
                }
            }            

            // move screen to jj position            
            if (jj.x < 0)
            {
                current_screen_x--;
                jj.x = SCREEN_WIDTH - 1;
                if( current_screen_x < 0 )
                    current_screen_x = SCREENS_WIDTH - 1;
            }
            else if (jj.x >= SCREEN_WIDTH)
            {
                current_screen_x++;
                if (current_screen_x >= SCREENS_WIDTH)
                    current_screen_x = 0;
                jj.x = 0;
            }
            else if (jj.y < 0)
            {
                current_screen_y--;
                if( current_screen_y < 0 )
                    current_screen_y = SCREENS_HEIGHT - 1;
                jj.y = SCREEN_HEIGHT - 1;
            }
            else if (jj.y >= SCREEN_HEIGHT)
            {
                current_screen_y++;
                if (current_screen_y >= SCREENS_HEIGHT)
                    current_screen_y = 0;
                jj.y = 0;

                if( ( (current_screen_x == 0 && current_screen_y == 1) || (current_screen_x == 6 && current_screen_y == 3) ) )                        
                    jj.y = 1;
            }

            if ( current_screen_x == 4 && current_screen_y == 2 && ((items & ITEM_TREASURE) == 0) )
            {
                clearGameScreen();                
                drawTextBabalibaCentered(STR_FOUND_TREASURE, 68, COLOR_WHITE, COLOR_LIGHT_CYAN, COLOR_BLACK);
                Dosound( fx_got_special );
                VsyncFrames(75);                
                items |= ITEM_TREASURE;                
            }
            redrawScreen = true;            
        }

        Vsync(); // wait for VBLank befor drawing
        if (redrawScreen)
            initScreen();
        
        if( !collision)
        {
            if (rbadguy.redraw || (rbadguy.step > 0))
            {
                if( current_screen_x == 5 && current_screen_y == 0 && ((items & ITEM_PRINCESS) == 0) )
                {        
                    if( abs(jj.x - rbadguy.x) + abs(jj.y - rbadguy.y) > 4 )
                    {
                        rbadguy.images[0] = rbadguy.images[2] = rbadguy.images[4] = rbadguy.images[6] = 45;
                        rbadguy.images[1] = rbadguy.images[3] = rbadguy.images[5] = rbadguy.images[7] = 76;
                    }
                    else
                    {
                        rbadguy.images[0] = rbadguy.images[2] = rbadguy.images[4] = rbadguy.images[6] = 67;
                        rbadguy.images[1] = rbadguy.images[3] = rbadguy.images[5] = rbadguy.images[7] = 77;
                    }
                }

                if (rbadguy.x != -1 && rbadguy.y != -1 && (explosionTick == 0 || !(current_screen_x == explosionScreenX && current_screen_y == explosionScreenY)))
                    drawSprite(&rbadguy);
                
                rbadguy.redraw = false;
            }

            if ((mbadguy.redraw || (mbadguy.step > 0)) && (mbadguy.direction != NO_MOVE)  )
            {
                if (mBadGuyType == 1)
                    drawSprite_topCleanImage = 61;
                if (mBadGuyType == 2)
                {
                    if( mbadguy.images[0] == 65)
                        drawSprite_bottomCleanImage = 64;
                    else if (mbadguy.images[0] == 148)
                        drawSprite_bottomCleanImage = 253;
                    else if (mbadguy.images[0] == 146)
                        drawSprite_bottomCleanImage = 252;
                }

                if( (explosionTick == 0) || !(current_screen_x == explosionScreenX && current_screen_y == explosionScreenY))
                    drawSprite(&mbadguy);

                if( mBadGuyType != 0)
                {
                    drawSprite_topCleanImage = clean_block;
                    drawSprite_bottomCleanImage = clean_block;
                }
            }


            if( bombTick != 0 )
            {                                            
                if( bombScreenX == current_screen_x && bombScreenY == current_screen_y )               
                    drawImage24(getGraphics(17), bombPositionX * 24 + SCREEN_OFFSET_X, bombPositionY * 24 + SCREEN_OFFSET_Y); // bomb image
                       
                if( onTickTime )
                    Dosound((tickTime&0b1) ? fx_bomb_tic : fx_bomb_tac);                    
                
                if( tickTime - bombTick > BOMB_TIME ) // bomb explosion
                {
                    Dosound(fx_explosion);
                    Vsync();
                    long expstart = get_vbclock();
                    for( int c=0; c<16; c++)
                        Setcolor(c, 0x777);

                    while (get_vbclock() - expstart < 10) // approx 22 ticks per second
                    {                    
                        Setcolor(0, palette[rand()&0b1111]);
                        for( int c=1; c<50; c++)
                            __asm("nop");                                        
                    }                    
                    Setpalette(palette);
                    if( inCurrentScreen(bombPosition) )
                        collision = true;
                    
                    bombTick = 0;                    
                    explosionTick = tickTime;
                    explosionScreenX = bombScreenX;
                    explosionScreenY = bombScreenY; 
                    babaliba_map[bombPosition] = empty_block;

                    if( explosionScreenX == 5 && explosionScreenY == 0 && ((items & ITEM_PRINCESS) == 0))
                    {
                        clearGameScreen();     
                        drawTextBabalibaCentered(STR_PRINCESS_KILLED, 68, COLOR_YELLOW, COLOR_MAGENTA, COLOR_BLACK);
                        drawTextBabalibaCentered(STR_MISSION_FAILED, 108, COLOR_LIGHT_CYAN, COLOR_WHITE, COLOR_BLACK);  
                        Dosound( fx_game_over );                          
                        VsyncFrames(75);
                        (void)wait_for_key();   
                        finished = true;    
                        collision = false;
                    }
                    if( explosionScreenX == 6 && explosionScreenY == 8 && ((items & ITEM_PRISONER) == 0))
                    {
                        clearGameScreen();     
                        drawTextBabalibaCentered(STR_PRISONER_KILLED, 68, COLOR_YELLOW, COLOR_MAGENTA, COLOR_BLACK);
                        drawTextBabalibaCentered(STR_MISSION_FAILED, 108, COLOR_LIGHT_CYAN, COLOR_WHITE, COLOR_BLACK);              
                        Dosound( fx_game_over );              
                        VsyncFrames(75);
                        (void)wait_for_key();   
                        finished = true;    
                        collision = false;
                    }           
                }
            }

            if (jj.redraw || jj.step > 0)
                drawSprite(&jj);                                          

            if( explosionTick != 0 && onTickTime)
            {
                if( tickTime - explosionTick > EXPLOSION_TIME )
                    explosionTick = 0;                                   
            }            
        }
                
        Vsync();
        
        // check collision
        if(  ((rbadguy.direction != NO_MOVE) && jj.screenx < rbadguy.screenx + 24 && jj.screenx + 24 > rbadguy.screenx) &&
             (jj.screeny < rbadguy.screeny + 24 && jj.screeny + 24 > rbadguy.screeny) 
             || 
             ((mbadguy.direction != NO_MOVE) && jj.screenx < mbadguy.screenx + 24 && jj.screenx + 24 > mbadguy.screenx) &&
                (jj.screeny < mbadguy.screeny + 24 && jj.screeny + 24 > mbadguy.screeny) )            
        {
            if( !(cheats & CHEAT_IMMUNITY) && (explosionTick == 0 || 
                (explosionTick != 0 && !(current_screen_x == explosionScreenX && current_screen_y == explosionScreenY))) )
            {
                collision = true;
                Setcolor(0, 0x0710);
                Dosound( fx_lostlife );
                Setcolor(1, 0x0710);
            }
        }

        // add some delay
        Vsync();

        if( babaliba_spell == 0 && (items & (ITEM_PRINCESS | ITEM_PRISONER | ITEM_TREASURE)) == (ITEM_PRINCESS | ITEM_PRISONER | ITEM_TREASURE) )
        {    
            clearGameScreen();     
            drawTextBabalibaCentered(STR_HAVE_ALL, 68, COLOR_WHITE, COLOR_LIGHT_CYAN, COLOR_BLACK);
            drawTextBabalibaCentered(STR_COMPLETE_SPELL, 108, COLOR_WHITE, COLOR_LIGHT_CYAN, COLOR_BLACK);                
            babaliba_spell = 1;
            Dosound( fx_got_special );
            VsyncFrames(250);
            drawScreen();   
            
            babaliba_map[item_pos_spell_B1] = item_spell_B;
            babaliba_map[item_pos_spell_A1] = item_spell_A;
            babaliba_map[item_pos_spell_B2] = item_spell_B;
            babaliba_map[item_pos_spell_A2] = item_spell_A;
            babaliba_map[item_pos_spell_L] = item_spell_L;
            babaliba_map[item_pos_spell_I] = item_spell_I;
            babaliba_map[item_pos_spell_B3] = item_spell_B;
            babaliba_map[item_pos_spell_A3] = item_spell_A;
        }

        if (collision)
        {            
            drawLives();
            Setcolor(0, 0);
            Setcolor(1, 0);
            clearGameScreen();

            if( current_screen_x == 5 && current_screen_y == 0 && ((items & ITEM_PRINCESS) == 0) )
            {
                drawTextBabalibaCentered(STR_ALREADY_PRINCESS, 68, COLOR_WHITE, COLOR_LIGHT_CYAN, COLOR_BLACK);                
                items |= ITEM_PRINCESS;
                Dosound( fx_got_special );
                VsyncFrames(75);
                (void)wait_for_key();
                getrBadGuyPosition();
                drawScreen();
            }
            else
            {
                if (lives > 0 && !(cheats & CHEAT_LIVES))
                    lives--;
                drawLives();
                if (lives == 0)
                {
                    finished = true;
                    drawTextBabalibaCentered(STR_YOU_ARE_DEAD, 68, COLOR_WHITE, COLOR_LIGHT_CYAN, COLOR_BLACK);
                    drawTextBabalibaCentered(STR_MISSION_FAILED, 108, COLOR_LIGHT_CYAN, COLOR_WHITE, COLOR_BLACK);  
                    Dosound( fx_game_over );              
                    VsyncFrames(75); // wait 1,5 s = 50+25 frames
                    (void)wait_for_key();
                }
                else
                {
                    drawTextBabalibaCentered(STR_ONE_LESS_LIFE, 68, COLOR_CYAN, COLOR_DARK_GREEN, COLOR_BLACK);                
                    VsyncFrames(75); // wait 1,5 s = 50+25 frames                
                    jj.x = enterScreenPositionX;
                    jj.y = enterScreenPositionY;                
                    initScreen();
                }
            }
        }

        if( babaliba_spell == 9 )
        {    
            clearGameScreen();     
            drawTextBabalibaCentered(STR_YOU_DID_IT, 52, COLOR_WHITE, COLOR_GREEN, COLOR_BLACK);
            drawTextBabalibaCentered(STR_TIME_USED, 68, COLOR_GREEN, COLOR_WHITE, COLOR_BLACK);
            drawNum(2000-time, 144, 84, COLOR_WHITE, COLOR_BACKGROUND);    
            drawTextBabalibaCentered(STR_CONTINUE_ICE_GLUB, 100, COLOR_LIGHT_CYAN, COLOR_WHITE, COLOR_BLACK);    
            Dosound( fx_babaliba_song_short );                       
            VsyncFrames(75);
            (void)wait_for_key();   
            finished = true;       
        }

        if( time == 0 )
        {            
            clearGameScreen();     
            drawTextBabalibaCentered(STR_TIME_EXPIRED, 68, COLOR_WHITE, COLOR_LIGHT_CYAN, COLOR_BLACK);
            drawTextBabalibaCentered(STR_MISSION_FAILED, 108, COLOR_LIGHT_CYAN, COLOR_WHITE, COLOR_BLACK);                
            Dosound( fx_game_over );              
            VsyncFrames(75); // wait 1,5 s = 50+25 frames
            (void)wait_for_key();
            finished = true;
        }
    }

    fadePaletteOut(palette);
    cls(COLOR_BACKGROUND);
    clear_key_buffer();

    }  // while not exit

    Dosound( fx_mute );
    free(extra_graphics);
    uninstall_joystick();
    showCursor();
    restore_click_sound();
    RestoreResolutionAndPalette(savedResolution, savedPalette);
    Super(olds); // Restore previous supervisor state
}
