#!/usr/bin/env python3

import os
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide" # to avoid error in header export

import pygame
import sys
import re

from extra_blocks import extra_blocks, block_name

CURRENT_PATH = os.path.dirname(os.path.abspath(__file__))+"/"
IMAGES_FOLDER = CURRENT_PATH + "../data/babalibac/"
MAP_FOLDER = CURRENT_PATH + "../data/"
MAP_BACK_FOLDER = CURRENT_PATH + "../old/"

# Initialize Pygame
pygame.init()
# Set up display
WIDTH, HEIGHT = 1280, 720
screen = pygame.display.set_mode((WIDTH, HEIGHT), pygame.RESIZABLE)
pygame.display.set_caption("BABALIBA Map Creator")
font = pygame.font.SysFont(None, 24)
pygame.key.set_repeat(300, 60) 

# Set up clock
clock = pygame.time.Clock()
FPS = 60

MAP_WIDTH = 150
MAP_HEIGHT = 45
SCREEN_WIDTH = 10
SCREEN_HEIGHT = 5
SCREENS_WIDTH = MAP_WIDTH // SCREEN_WIDTH
SCREENS_HEIGHT = MAP_HEIGHT // SCREEN_HEIGHT

VISIBLE_SCREEN_WIDTH = 10
VISIBLE_SCREEN_HEIGHT = 5
CURRENT_SCREEN = [14, 4] # x,y in screens


DM_ONLYLABYRINTH = 1
DM_SCREENGRID = 2
DM_ONLYLABYRINTHCOL = 4

drawing_mode = DM_SCREENGRID

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

TRANSPARENT = 0
BLACK = 1
BLUE = 2
DARK_BLUE = 3
RED = 4
DARK_RED = 5
MAGENTA = 6
DARK_MAGENTA = 7
GREEN = 8
DARK_GREEN = 9
LIGHT_CYAN = 10
CYAN = 11
YELLOW = 12
DARK_YELLOW = 13
GREY = 14
WHITE = 15

WHOLE_MAP = [0] * (MAP_WIDTH * MAP_HEIGHT)

UNDO_BUFFER = []

MAX_UNDO_DEPTH = 1000
REDO_BUFFER = []

def keep_undo_buffer( keepredo=False):
    global UNDO_BUFFER, WHOLE_MAP, REDO_BUFFER
    # keep a copy of the current map in the undo buffer
    UNDO_BUFFER.append(WHOLE_MAP.copy())

    if not keepredo:
        # clear redo buffer on new action        
        REDO_BUFFER = []        


def keep_redo_buffer():
    global REDO_BUFFER, WHOLE_MAP    
    # keep a copy of the current map in the redo buffer
    REDO_BUFFER.append(WHOLE_MAP.copy())    
    
def restore_from_undo_buffer():
    global UNDO_BUFFER, WHOLE_MAP
    keep_redo_buffer()
    if len(UNDO_BUFFER) > 0:
        WHOLE_MAP = UNDO_BUFFER.pop()
        
def restore_from_redo_buffer():
    global REDO_BUFFER, WHOLE_MAP
    keep_undo_buffer( keepredo=True )
    if len(REDO_BUFFER) > 0:
        WHOLE_MAP = REDO_BUFFER.pop()
    
def replaceColor( surface, old_color, new_color):
    for x in range(surface.get_width()):
        for y in range(surface.get_height()):
            if surface.get_at((x, y))[:3] == old_color:
                surface.set_at((x, y), new_color)
    return surface

def getMainBlockColor( nblock ):
    if nblock not in image_blocks:
        return 0
    block = image_blocks[nblock]
    indexes = [0]*16
    max_index = 0
    max_index_value = 0

    for x in range(block.get_width()):
        for y in range(block.get_height()):
            color = block.get_at((x, y))[:3]
            if color in palette:
                ci = palette.index(color)
                if ci > 1:
                    indexes[ci] += 1
                    if indexes[ci] > max_index_value and ci != TRANSPARENT:
                        max_index_value = indexes[ci]
                        max_index = ci
    
    return max_index
    


# Load resources

image_blocks = {}
for filename in os.listdir(IMAGES_FOLDER):
    if filename.endswith(".png"):        
        img = pygame.image.load(
            os.path.join(IMAGES_FOLDER, filename)).convert_alpha()
        block_index = int(re.search(r'\d+', (filename.split('.')[0])).group())
        image_blocks[block_index] = img        


for index, extra_block in extra_blocks.items():
    base_img_id = extra_block[0]
    s = image_blocks[base_img_id].copy()    
    for color_pair in extra_block[1]:        
        replaceColor( s, palette[color_pair[0]], palette[color_pair[1]] )                            
    image_blocks[index] = s    

image_blocks_list = list(image_blocks.keys())
image_blocks_list.sort()

block_size = 64
current_screen_pos = [0,0]
current_drawing_item = 9


color_indexes = [0]*256
def build_colorIndexes():
    for i in range(256):
        color_indexes[i] = getMainBlockColor(i)

build_colorIndexes()

if "--exportcolorindexes" in sys.argv:
    
    print("#ifndef BLOCKS_MAIN_COLOR_H\n")
    print("const __uint8_t blocks_main_color[] = {\n\t", end="")
    for i in range(256):
        print(f"0x{color_indexes[i]:02X}, ", end="")
        if i%16 == 15:
            print(f"\t// from block {i-i%16}\n\t", end="")
    print("};\n#endif // BLOCKS_MAIN_COLOR_H\n")
    sys.exit(0)



def get_image_block(index):
    #now useless
    return image_blocks[index]
                
def next_image_block(ibindex):
    global image_blocks_list
    i = image_blocks_list.index(ibindex)    
    i = (i+1) % len(image_blocks_list)
    return image_blocks_list[i]

def previous_image_block(ibindex):
    global image_blocks_list
    i = image_blocks_list.index(ibindex)    
    i = (i-1) % len(image_blocks_list)    
    return image_blocks_list[i]

def load_map():
    global WHOLE_MAP, redraw, MAP_HEIGHT, MAP_WIDTH
    with open(MAP_FOLDER+"map.bin", "rb") as f:
        for p in range(MAP_HEIGHT * MAP_WIDTH):                                      
            WHOLE_MAP[p] = int.from_bytes(f.read(1), "little")        
    keep_undo_buffer()

load_map()

# Main game loop
running = True
redraw = True

# actions
NOACTION        = 0b00000000
SETATMOUSE      = 0b00000001
SETATCURSOR     = 0b00000010  
CLEARATCURSOR   = 0b00000100
GETATMOUSE      = 0b00001000
GETATCURSOR     = 0b00010000
CLEARATMOUSE    = 0b00100000

action = NOACTION
infomode = 0

while running:

    action = NOACTION

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        # check if shift is pressed
        keys = pygame.key.get_pressed()
        shift = keys[pygame.K_LSHIFT] or keys[pygame.K_RSHIFT]

        # if resized event
        if event.type == pygame.VIDEORESIZE:
            WIDTH, HEIGHT = event.size            
            redraw = True        

        if event.type == pygame.KEYDOWN:

            if event.key == pygame.K_ESCAPE:
                running = False

            if event.key == pygame.K_F11:
                pygame.display.toggle_fullscreen()
                WIDTH, HEIGHT = pygame.display.get_surface().get_size()         
                redraw = True

            if event.key == pygame.K_LEFT or event.key == pygame.K_o:                
                if not shift:
                    current_screen_pos[0] -= 1                
                    if current_screen_pos[0] < 0:
                        if CURRENT_SCREEN[0] > 0:
                            CURRENT_SCREEN[0] -= 1        
                            current_screen_pos[0] = 9    
                        else:
                            current_screen_pos[0] = 0  
                else:
                    if CURRENT_SCREEN[0] > 0:
                        CURRENT_SCREEN[0] -= 1

                redraw = True

            if event.key == pygame.K_RIGHT or event.key == pygame.K_p:
                if not shift:
                    current_screen_pos[0] += 1
                    if current_screen_pos[0] > 9:
                        if CURRENT_SCREEN[0] < SCREENS_WIDTH - 1:
                            CURRENT_SCREEN[0] += 1            
                            current_screen_pos[0] = 0
                        else:
                            current_screen_pos[0] = 9
                else:
                    if CURRENT_SCREEN[0] < SCREENS_WIDTH - 1:
                        CURRENT_SCREEN[0] += 1
                
                redraw = True

            if event.key == pygame.K_UP or event.key == pygame.K_q:
                if not shift:
                    current_screen_pos[1] -= 1
                    if current_screen_pos[1] < 0:
                        if CURRENT_SCREEN[1] > 0:
                            CURRENT_SCREEN[1] -= 1
                            current_screen_pos[1] = 4
                        else:
                            current_screen_pos[1] = 0
                else:
                    if CURRENT_SCREEN[1] > 0:
                        CURRENT_SCREEN[1] -= 1
                redraw = True


            if event.key == pygame.K_DOWN or event.key == pygame.K_a:
                if not shift:
                    current_screen_pos[1] += 1
                    if current_screen_pos[1] > 4:
                        if CURRENT_SCREEN[1] < SCREENS_HEIGHT - 1:
                            CURRENT_SCREEN[1] += 1
                            current_screen_pos[1] = 0
                        else:
                            current_screen_pos[1] = 4
                else:
                    if CURRENT_SCREEN[1] < SCREENS_HEIGHT - 1:
                        CURRENT_SCREEN[1] += 1
                redraw = True

            if event.key == pygame.K_F4:
                if os.path.exists(MAP_FOLDER+"map.bin"):
                    bak_index = 1
                    while os.path.exists(MAP_BACK_FOLDER+f"map.{bak_index:03}.bak"):
                        bak_index += 1
                    os.rename(MAP_FOLDER+"map.bin", MAP_BACK_FOLDER+f"map.{bak_index:03}.bak")
                    print(f"Backup created: map.{bak_index:03}.bak")

                with open(MAP_FOLDER+"map.bin", "wb") as f:
                    for p in range(MAP_HEIGHT * MAP_WIDTH):                         
                        f.write(bytes([WHOLE_MAP[p]]))
                print("Map saved to map.bin")
                keep_undo_buffer()

            if event.key == pygame.K_F2:
                # save map.png image
                BLOCK_SAVE_SIZE = 24
                if drawing_mode & (DM_ONLYLABYRINTH|DM_ONLYLABYRINTHCOL) == 0:
                    map_image = pygame.Surface((MAP_WIDTH * BLOCK_SAVE_SIZE, MAP_HEIGHT * BLOCK_SAVE_SIZE))
                    # fill with black                
                    map_image.fill((0, 0, 0))
                    for y in range(MAP_HEIGHT):
                        for x in range(MAP_WIDTH):
                            pos = x + (y * MAP_WIDTH)
                            block_index = WHOLE_MAP[pos]
                            if block_index > 0:
                                s = get_image_block(block_index).copy()                                                    
                                resized_img = pygame.transform.scale(s, (BLOCK_SAVE_SIZE, BLOCK_SAVE_SIZE))
                                map_image.blit(resized_img, (x * BLOCK_SAVE_SIZE, y * BLOCK_SAVE_SIZE))
                    pygame.image.save(map_image, "map.png")
                    print("Map image saved to map.png")
                elif (drawing_mode & DM_ONLYLABYRINTH) != 0:
                    # create surface monochrome
                    map_image = pygame.Surface((MAP_WIDTH, MAP_HEIGHT), depth = 8)
                    map_image.fill(255)
                    for y in range(MAP_HEIGHT):
                        for x in range(MAP_WIDTH):
                            pos = x + (y * MAP_WIDTH)
                            if WHOLE_MAP[pos] != 0:                            
                                map_image.set_at((x, y), 0)
                    pygame.image.save(map_image, "map_labyrinth.png")
                    print("Map labyrinth image saved to map_labyrinth.png")
                elif (drawing_mode & DM_ONLYLABYRINTHCOL) != 0:
                    # create surface with colors
                    map_image = pygame.Surface((MAP_WIDTH, MAP_HEIGHT))
                    map_image.fill((0,0,0))
                    for y in range(MAP_HEIGHT):
                        for x in range(MAP_WIDTH):
                            pos = x + (y * MAP_WIDTH)
                            block_index = WHOLE_MAP[pos]
                            if block_index != 0:                            
                                p = color_indexes[block_index]
                                map_image.set_at((x, y), palette[p])
                    pygame.image.save(map_image, "map_labyrinth_color.png")
                    print("Map labyrinth color image saved to map_labyrinth_color.png")


            if event.key == pygame.K_F3:
                load_map()
                redraw = True

            if event.key == pygame.K_SPACE:
                action |= SETATCURSOR

            if event.key == pygame.K_x:
                action |= GETATCURSOR

            if event.key == pygame.K_z and not keys[pygame.K_LCTRL]:
                action |= CLEARATCURSOR

            if event.key == pygame.K_1:
                # Drawing mode xor ONLYlabyrinth
                drawing_mode = (drawing_mode & ~DM_ONLYLABYRINTHCOL) ^ DM_ONLYLABYRINTH
                redraw = True
            if event.key == pygame.K_2:
                # Drawing mode xor ONLYlabyrinthColor
                drawing_mode = (drawing_mode & ~DM_ONLYLABYRINTH) ^ DM_ONLYLABYRINTHCOL
                redraw = True

            if event.key == pygame.K_g:
                # Drawing mode xor SCREENGRID
                drawing_mode ^= DM_SCREENGRID
                redraw = True

            # event type undo CTRL+Z
            if event.key == pygame.K_z and keys[pygame.K_LCTRL]:
                print("Undo")
                restore_from_undo_buffer()
                redraw = True

            # event type redo CTRL+Y
            if event.key == pygame.K_y and keys[pygame.K_LCTRL]:
                print("Redo")
                restore_from_redo_buffer()
                redraw = True

            if event.key == pygame.K_i:
                infomode ^= 1
                redraw = True
                
        if (event.type == pygame.KEYDOWN and event.key == pygame.K_w) or (event.type == pygame.MOUSEWHEEL and event.y < 0 and not shift):
            current_drawing_item = next_image_block(current_drawing_item)            
            redraw = True

        if (event.type == pygame.KEYDOWN and event.key) == pygame.K_s or (event.type == pygame.MOUSEWHEEL and event.y > 0 and not shift):
            current_drawing_item = previous_image_block(current_drawing_item)
            redraw = True
            
        if (event.type == pygame.KEYDOWN and event.key == pygame.K_e) or (event.type == pygame.MOUSEWHEEL and event.y > 0 and shift):
            block_size += 4                
            redraw = True

        if (event.type == pygame.KEYDOWN and event.key == pygame.K_d) or (event.type == pygame.MOUSEWHEEL and event.y < 0 and shift):
            block_size -= 4                
            if block_size < 8:
                block_size = 8                
            redraw = True

        if( event.type == pygame.MOUSEMOTION ):            
            redraw = True

        if( event.type == pygame.MOUSEBUTTONDOWN and event.button == 1 ):
            action |= SETATMOUSE

        if( event.type == pygame.MOUSEBUTTONDOWN and event.button == 3 ):  # right button          
            action |= GETATMOUSE

        if( event.type == pygame.MOUSEBUTTONDOWN and event.button == 2 ):   # middle button
            action |= CLEARATMOUSE

    offsetx = ((WIDTH // block_size) // 2) - (SCREEN_WIDTH // 2)
    offsety = ((HEIGHT // block_size) // 2) - (SCREEN_HEIGHT // 2)
    mousepos = pygame.mouse.get_pos()
    mox = (mousepos[0] // block_size)
    moy = (mousepos[1] // block_size)
    # calculate map position at mouse
    mix = mox - offsetx + (CURRENT_SCREEN[0] * SCREEN_WIDTH)
    miy = moy - offsety + (CURRENT_SCREEN[1] * SCREEN_HEIGHT) 
    current_pos = (CURRENT_SCREEN[1] * SCREEN_HEIGHT + (current_screen_pos[1])) * MAP_WIDTH + CURRENT_SCREEN[0] * SCREEN_WIDTH + (current_screen_pos[0])

    if (action & SETATCURSOR) != 0:
        keep_undo_buffer()
        WHOLE_MAP[current_pos] = current_drawing_item                          
        redraw = True
        
    if (action & SETATMOUSE) != 0:
        smpos = (miy * MAP_WIDTH) + mix
        if smpos >= 0 and smpos < len(WHOLE_MAP):
            keep_undo_buffer()
            WHOLE_MAP[smpos] = current_drawing_item
            redraw = True
            

    if (action & CLEARATCURSOR) != 0:
        keep_undo_buffer()
        WHOLE_MAP[current_pos] = 0
        redraw = True
        

    if (action & GETATCURSOR) != 0:
        keep_undo_buffer()
        p = WHOLE_MAP[current_pos]
        if p > 0:
            current_drawing_item = p
        redraw = True
        
    if (action & GETATMOUSE) != 0:
        smpos = (miy * MAP_WIDTH) + mix
        if smpos >= 0 and smpos < len(WHOLE_MAP):    
            keep_undo_buffer()  
            p = WHOLE_MAP[smpos]      
            if p > 0:
                current_drawing_item = p
                redraw = True      
                

    if (action & CLEARATMOUSE) != 0:
        smpos = (miy * MAP_WIDTH) + mix
        if smpos >= 0 and smpos < len(WHOLE_MAP):    
            keep_undo_buffer()  
            WHOLE_MAP[smpos] = 0
            redraw = True            

    # Drawing code goes here
    if redraw:        
        screen.fill((0, 0, 0) if (drawing_mode & DM_ONLYLABYRINTH) == 0 else (255,255,255))  # Fill screen with black

        for y in range(HEIGHT // block_size):
            for x in range(WIDTH // block_size):
                mx = x - offsetx
                my = y - offsety
                posy = CURRENT_SCREEN[1] * SCREEN_HEIGHT + my
                posx = CURRENT_SCREEN[0] * SCREEN_WIDTH + mx
                pos = posx + (posy * MAP_WIDTH)
                if posx < 0 or posy < 0 or posy >= MAP_HEIGHT or posy >= MAP_WIDTH:
                    continue

                inscreen = mx >= 0 and mx < VISIBLE_SCREEN_WIDTH and my >= 0 and my < VISIBLE_SCREEN_HEIGHT
                if inscreen and (drawing_mode & DM_SCREENGRID) != 0:
                    pygame.draw.rect(screen, (64, 64, 64), (x*block_size, y*block_size, block_size, block_size), 1)                                

                outoffbonds = (CURRENT_SCREEN[1] * SCREEN_HEIGHT + my) >= MAP_HEIGHT or (CURRENT_SCREEN[0] * SCREEN_WIDTH + mx) >= MAP_WIDTH

                if not outoffbonds:
                    if pos < len(WHOLE_MAP) and pos >= 0:
                        if WHOLE_MAP[pos] > 0 and pos < len(WHOLE_MAP) and pos >= 0:
                            block_index = WHOLE_MAP[pos]

                            if (drawing_mode & (DM_ONLYLABYRINTH|DM_ONLYLABYRINTHCOL)) != 0:
                                # draw filled rectangle
                                if drawing_mode & DM_ONLYLABYRINTH:
                                    pygame.draw.rect(screen, (0, 0, 0), (x*block_size, y*block_size, block_size, block_size), 0)                                
                                else:
                                    p = color_indexes[block_index]
                                    pygame.draw.rect(screen, palette[p], (x*block_size, y*block_size, block_size, block_size), 0)                                
                            else:
                                s = get_image_block(block_index).copy()                                                    
                                if not inscreen:
                                    s.set_alpha(128)
                                resized_img = pygame.transform.scale(s, (block_size, block_size))
                                screen.blit(resized_img, (x*block_size, y*block_size))
                                
                    
        cmx = current_screen_pos[0] + offsetx
        cmy = current_screen_pos[1] + offsety

        pygame.draw.rect(screen, (255, 32, 32), (cmx*block_size, cmy*block_size, block_size, block_size), 3)        
        pygame.draw.rect(screen, (255, 128, 128), (mox*block_size, moy*block_size, block_size, block_size), 3)
        
        s = get_image_block(current_drawing_item).copy()
        s.set_alpha(64)
        resized_img = pygame.transform.scale(s, (block_size, block_size))
        screen.blit(resized_img, (cmx*block_size, cmy*block_size))

        # current drawing item
        size = WIDTH//11
        i = current_drawing_item
        s = get_image_block(i).copy()        
        resized_img = pygame.transform.scale(s, (size, size))
        current_top = size//2
        screen.blit(resized_img, (WIDTH - size - 10,  HEIGHT//2 - current_top))
        i2 = i
        size = (size * 90) // 100

        while current_top <  HEIGHT//2 :            
            size = (size * 80) // 100
            current_top = current_top + size + 10

            i = previous_image_block(i)
            s = get_image_block(i).copy()
            resized_img = pygame.transform.scale(s, (size, size))            
            screen.blit(resized_img, (WIDTH - size - 10,  HEIGHT//2 - current_top))

            i2 = next_image_block(i2)
            s = get_image_block(i2).copy()
            resized_img = pygame.transform.scale(s, (size, size))
            screen.blit(resized_img, (WIDTH - size - 10,  HEIGHT//2 + current_top - size))
            
        # write CURRENT_SCREEN and current position
        
        if infomode == 0:
            towrite = f"Screen: {CURRENT_SCREEN[0]},{CURRENT_SCREEN[1]}  Pos: {current_screen_pos[0]},{current_screen_pos[1]}    "
        else:
            towrite = f"Map xy: {CURRENT_SCREEN[0]*SCREEN_WIDTH+current_screen_pos[0]}, {CURRENT_SCREEN[1]*SCREEN_HEIGHT+current_screen_pos[1]}   #: {current_pos}      "
        text = font.render(towrite, True, (255, 255, 255))
        screen.blit(text, (10, HEIGHT - 30))

        text = font.render(str(current_drawing_item) + " - " +block_name(current_drawing_item), True, (255, 255, 128))
        screen.blit(text, (350, HEIGHT - 30))

        pygame.display.flip()

        redraw = False

    clock.tick(FPS)

pygame.quit()
sys.exit()
