#include <stdio.h>
#include <stdint.h>
#include <osbind.h>

#include "sound.h"
#include "sounds.h"


/* Global state for the sound effect */
int shot_active = 0;
int shot_volume = 0;

unsigned long saved_sp;


// Call this once to trigger the sound
void trigger_shot() {

 long old = Super(0);
    shot_active = 1;
    shot_volume = 15; // Max volume

    // Register 7: Mixer (1 = off, 0 = on)
    // Bits: [Noise C, B, A] [Tone C, B, A]
    // 0x36 = 00110110 (Noise A ON, Tone A OFF)
    WRITE_PSG(7, 0x36); 
    
    // Register 6: Noise Frequency (0-31)
    WRITE_PSG(6, 0x05); 
     Super(old);
}

// Call this function once every frame (VBL)
void update_sfx() {
    if (shot_active) {
        if (shot_volume > 0) {
            WRITE_PSG(8, shot_volume); // Channel A Volume
            shot_volume--;             // Decay volume
        } else {
            WRITE_PSG(8, 0);           // Silence
            shot_active = 0;
        }
    }
}

void remove_click_sound(void)
{
    long old = Super(0);
    #define CLICK_MEM ((volatile uint8_t *)0x484)
    *CLICK_MEM = CLICK_MEM[0] & 0b11111110; // Clear bit 0 to disable click sound
    Super(old);
}


volatile unsigned long counter = 0;

void* old_vbl_vector = 0;

// VBL interrupt routine
void vbl_handler_c();
__asm__(
"   .text\n"
"   .globl _vbl_handler_c\n"
"_vbl_handler_c:\n"
"   movem.l %d0-%d7/%a0-%a6,-(%sp)   | Save all registers\n"
"   addq.l  #1,_counter               | Increment counter\n"
"   jsr     _update_sfx               | Call update_sfx\n"
"   movem.l (%sp)+,%d0-%d7/%a0-%a6   | Restore all registers\n"
"   rts                               | Return from subroutine\n"
);

#define VBL_LIST_ADDR 0x456UL 
#define NVBLS_ADDR    0x454UL

int slot = -1;

void install_vbl_interrupt() {
    void **vbl_list;
    int nvbls;    
    long old_stack;

    // 1. Enter Supervisor mode to access system variables
    old_stack = Super(0);

    // 2. Get the pointer to the VBL queue and the number of slots
    vbl_list = *((void ***)VBL_LIST_ADDR);
    nvbls = *((int *)NVBLS_ADDR);

    // 3. Find an empty slot (containing NULL)
    for (int i = 0; i < nvbls; i++) {
        if (vbl_list[i] == NULL) {
            slot = i;
            break;
        }
    }
    if (slot != -1) {
        printf("Installing VBL in slot %d...\n", slot);
        vbl_list[slot] = vbl_handler_c;
    }

    Super(old_stack);
}

void remove_vbl_interrupt()
{
    void **vbl_list;

    if( slot == -1 )
        return; // not installed

    long old_stack = Super(0);
 
    vbl_list = *((void ***)VBL_LIST_ADDR);
    vbl_list[slot] = NULL;

    Super(old_stack);
}

/* A simple "bing" sound in Dosound format */
/* Format: {Register, Value} ... 0xFF (End) */



unsigned char fx_click[] = {
    0, 0x20, 1, 0x00,  /* Very high pitch */
    7, 0xFE,           /* Enable Tone A only */
    8, 0x10,           /* Volume: Use Envelope */
    11, 0x00, 12, 0x05, /* Fast Envelope speed */
    13, 0x09,          /* Shape: Single Decay */
    SND_END             /* End */
};



unsigned char fx_falling[] = {
    7, 0xFE,           /* Enable Tone A */
    8, 15,             /* Max Volume */
    0, 0x40, 1, 0,     /* Pitch 1 */
    SND_WAIT(10),              /* Wait 10 frames (128 + 10) */
    0, 0x80, 1, 0,     /* Pitch 2 (Lower) */
    SND_WAIT(10),               /* Wait 10 frames */
    0, 0xC0, 1, 0,     /* Pitch 3 (Lower) */
    SND_WAIT(10),               /* Wait 10 frames */
    0, 0x00, 1, 1,     /* Pitch 4 (Lower) */
    SND_WAIT(10),               /* Wait 10 frames */
    8, 0,              /* Silence */
    SND_END             /* End */
};


unsigned char fx_bomb_drop[] = {
    7, 0xF6,           /* Enable Tone A AND Noise A (11110110) */
    6, 0x05,           /* High pitched noise (wind) */
    8, 0x10,           /* Use Envelope for volume */
    11, 0x00, 12, 0x40, /* Long envelope period */
    13, 0x09,          /* Single Decay */
    0, 0x20, 1, 0,     /* Start pitch very high */
    SND_WAIT(10),               /* Short wait */
    0, 0x60,           /* Lower... */
    SND_WAIT(10),
    0, 0xA0,           /* Lower... */
    SND_WAIT(10),
    SND_END
};


    
unsigned char fx_explosion2[] = {
    0x00, 0xFE, // Canal A Freqüència fina (molt baixa)
    0x01, 0x0F, // Canal A Freqüència grossa (molt baixa)
    0x06, 0x1F, // Soroll al mínim de freqüència (més greu)
    0x07, 0xF6, // Mixer: Soroll I TO al Canal A (més contundència)
    0x08, 0x10, // Volum per Envolupant
    0x0B, 0x00, // Envolupant Low
    0x0C, 0x30, // Envolupant High (valor alt = so més llarg i pesat)
    0x0D, 0x00, // Forma: Decaïment
    0xFF, 0x00  // Fi
};

unsigned char fx_full_bomb[] = {
    /* --- THE FALL --- */
    7, 0xFE, 8, 15,    /* Tone On, Vol Max */
    0, 0x40, 1, 0, SND_WAIT(1), /* Pitch 1, Wait 1 frame */
    0, 0x80, 1, 0, SND_WAIT(1), /* Pitch 2, Wait 1 frame */
    0, 0xC0, 1, 0, SND_WAIT(1), /* ... continue sliding ... */
    0, 0x00, 1, 1, SND_WAIT(1), 
    0, 0x40, 1, 1, SND_WAIT(1),
    
    /* --- THE EXPLOSION --- */
    6, 0x1F,           /* Set Noise Frequency */
    7, 0xF7,           /* Switch to Noise Only */
    8, 0x10,           /* Use Envelope */
    11, 0x00, 12, 0x40, /* Env speed */
    13, 0x00,          /* Shape: Fade out */
    SND_END
};

unsigned char silence[] = {8, 0, 9, 0, 10, 0, SND_END};

unsigned char fx_step[] = {
    7, 0xFE,           /* Enable Tone A */
    0, 0x08, 1, 0x00,  /* Higher pitch than bomb_timer (0x08 vs 0x10) */
    8, 10,             /* Medium volume */
    SND_END             /* End */
};

unsigned char fx_bomb_timerO[] = {
    /* --- TIC (Higher pitch) --- */
    7, 0xFE,           /* Enable Tone A */
    0, 0x10, 1, 0x00,  /* Very high pitch */
    8, 0x10,           /* Volume: Use Envelope */
    11, 0x00, 12, 0x02, /* Very fast envelope */
    13, 0x09,          /* Shape: Single Decay */
    
    SND_WAIT(1),              /* WAIT: 32 frames (~0.6 seconds) */

    /* --- TAC (Slightly lower pitch) --- */
    0, 0x30, 1, 0x00,  /* Slightly lower pitch */
    13, 0x09,          /* Re-trigger Envelope */
    
    SND_WAIT(32),               /* WAIT: 32 frames */
    
    //255, 1             /* LOOP back to the beginning! */
    SND_END
};



unsigned char fx_walk1[] = {  
    
    CHANNEL_A_FINE, 0x0e,              // Scale 8?
    CHANNEL_A_COARSE, 0xe0,            /* Very high pitch */
    SND_MIXER(CHANNEL_A_TONE),           /* Enable Tone A */    
    VOLUME_A(10),           /* Volume: Use Envelope */        
    SND_WAIT(1),
    
    VOLUME_A(VOL_MUTE),
    SND_WAIT(2), 

    CHANNEL_A_FINE, 0x01,
    CHANNEL_A_COARSE, 0x00,
    SND_MIXER(CHANNEL_A_TONE),           /* Enable Tone A */    
    VOLUME_A(12),           /* Volume: Use Envelope */        
    SND_WAIT(2),
    
    VOLUME_A(VOL_MUTE),
    SND_END
};

unsigned char fx_escala[] = {
    SND_MIXER(CHANNEL_A_TONE),  /* Enable Channel A Tone only (Bit 0=0) */
    
    // Do
    NOTE_A(C,5),    
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s) 
   
    // Re
    NOTE_A(D,5),
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s) 
   
    // Mi
    NOTE_A(E,5),
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s

    // Fa
    NOTE_A(F,5),
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s)

    // Sol
    NOTE_A(G,5),
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s

    // La
    NOTE_A(A,5),
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s  
    // Si
    NOTE_A(B,5),
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s

    //Do
    NOTE_A(C,6),
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s

    SND_END    /* End of data */
};


unsigned char fx_escala2[] = {
    SND_MIXER(CHANNEL_A_TONE),  /* Enable Channel A Tone only (Bit 0=0) */
    
    // Do
    CHANNEL_A_FINE, C5_FINE,  /* Channel A Pitch Low */
    CHANNEL_A_COARSE, C5_COARSE,  /* Channel A Pitch High */    
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    0x82, 0x20,   // <--- N * (1/50s) 
   
    // Re
    CHANNEL_A_FINE, D5_FINE,  /* Channel A Pitch Low */
    CHANNEL_A_COARSE, D5_COARSE,  /* Channel A Pitch High */    
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    0x82, 0x20,
   
    // Mi
    CHANNEL_A_FINE, E5_FINE,  /* Channel A Pitch Low */
    CHANNEL_A_COARSE, E5_COARSE,  /* Channel A Pitch High */    
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    0x82, 0x20,

    // Fa
    CHANNEL_A_FINE, F5_FINE,  /* Channel A Pitch Low */
    CHANNEL_A_COARSE, F5_COARSE,  /* Channel A Pitch High */    
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    0x82, 0x20,

    // Sol
    CHANNEL_A_FINE, G5_FINE,  /* Channel A Pitch Low */
    CHANNEL_A_COARSE, G5_COARSE,  /* Channel A Pitch High */    
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */   
    SND_WAIT(20),   // <--- N * (1/50s

    // La
    CHANNEL_A_FINE, A5_FINE,  /* Channel A Pitch Low */
    CHANNEL_A_COARSE, A5_COARSE,  /* Channel A Pitch High */    
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s  
    // Si
    CHANNEL_A_FINE, B5_FINE,  /* Channel A Pitch Low */
    CHANNEL_A_COARSE, B5_COARSE,  /* Channel A Pitch High */    
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s

    //Do
    CHANNEL_A_FINE, C6_FINE,  /* Channel A Pitch Low */
    CHANNEL_A_COARSE, C6_COARSE,  /* Channel A Pitch High */    
    ENV_SINGLE_DECAY_HOLD(0x4000), /* Envelope Shape: Single Decay */
    SND_WAIT(20),   // <--- N * (1/50s

    SND_END    /* End of data */
};

static const unsigned char musica[] = {
    
        SND_MIXER(CHANNEL_A_TONE),          // Mixer
        CHANNEL_A_FINE,      C4_FINE, 
        CHANNEL_A_COARSE,    C4_COARSE,           // Nota 1 (Do4)
        VOLUME_A(VOL_MAX),             // Volum On

        SND_WAIT(50),          // Retard 1 segon

        CHANNEL_A_FINE,      A5_FINE, 
        CHANNEL_A_COARSE,    A5_COARSE,          // Nota 2 (La4)

        SND_WAIT(50),          // Retard 1 segon

        VOLUME_A(0),           // Volum Off
        SND_END            // Fi
    
};

static char test_seq[] = {
    7, 254,  // Mixer: Canal A On
    8, 15,   // Volum A: Max
    0, 255,  // Nota molt greu
    1, 15,
    SND_WAIT(100), // Espera 2 segons
    8, 0,    // Volum A: Off
    0xFF, 0  // Fi
};

/*
char snd[] = { 0,222,1,1,8,16,11,0,12,64,13,9,7,254,
               0xff,30,0,170,13,9,0xff,10,
               0,123,13,9,0xff,20,0,170,13,9,0xff,20,
               0,102,13,9,0xff,20,0,123,13,9,0xff,20,
               0,170,13,9,0xff,10,0,250,13,9,0xff,10,
               0,222,13,9,0xff,30,7,255,0xff,0 };
*/

char snd_put_bomb[] = {3,3,9,16,13,1,12,55,0x80,10,SND_WAIT(3),2,0,255,0};

 
void play_random_beep() {
    static unsigned char beep[] = {
        0, 0, 1, 0,    /* Frequency (to be filled) */
        7, 0xFE, 8, 12, /* Tone A on, Fixed Volume 12 */
        SND_END 
    };
    
    // Randomize the frequency bytes
    beep[1] = (unsigned char)(rand() % 255);
    beep[3] = (unsigned char)(rand() % 4);
    
    Dosound(beep);
}

/* Global variables to control the sound from your main program */
volatile int fall_active = 0;
int fall_period = 100; // Start high pitch

void falling_vbl_handler(void) {
    if (!fall_active) return;

    // 1. Update the PSG Register 0 & 1 (Channel A Pitch)
    *((volatile char*)0xFF8800) = 0;
    *((volatile char*)0xFF8802) = (char)(fall_period & 0xFF);
    *((volatile char*)0xFF8800) = 1;
    *((volatile char*)0xFF8802) = (char)((fall_period >> 8) & 0x0F);

    // 2. Slide the pitch down (increase the period)
    fall_period += 8; 

    // 3. Stop when it gets too low/bassy
    if (fall_period > 1500) {
        fall_active = 0;
        // Kill volume
        *((volatile char*)0xFF8800) = 8;
        *((volatile char*)0xFF8802) = 0;
    }
}

void crida_directa_dosound(const unsigned char *ptr) {
    __asm__ volatile (
        
        "move.l %0,-(sp) \n\t"  // Empenta el punter a la seqüència
        "move.w #32,-(sp) \n\t" // Funció 32 (Dosound)
        "trap #14         \n\t" // XBIOS
        "addq.l #6,sp     \n\t" // Neteja la pila
        :
        : "g"(ptr)              // Entrada: el punter a la música
        : "d0", "d1", "d2", "a0", "a1", "a2" // Registres que el TOS pot modificar
    );
}

volatile long *frclock = (volatile long *)0x466L;

char mouse;
typedef struct {
    void (*midivec)();
    void (*vkbderr)();
    void (*vmiderr)();
    void (*statvec)();
    void (*mousevec)(); /* The vector we care about */
    void (*clockvec)();
    void (*joyvec)();
} KBDVBASE;
void my_mouse_handler(char *buf) {
    /* buf[0] contains button state:
       bit 0: Right button (reversed in some docs, usually 0x01)
       bit 1: Left button (usually 0x02)
    */
    mouse = buf[0];
}
void (*old_mouse_vec)(void);





int main() {
    // Basic Game Loop       

    remove_click_sound();    
    //install_vbl_interrupt();

    
    KBDVBASE *kv = (KBDVBASE *)Kbdvbase();    
    old_mouse_vec = kv->mousevec;
    Initmous(1, NULL, my_mouse_handler);

    int running = 1;
    char clicked = 0;
    
    while(running != 0) {
        
        //printf("VBL Count: %ld Mouse buttons: 0x%02X\n", counter, mouse);
        //printf("."); // Indicate loop iteration        
        if (Bconstat(2)) {
            /* Bconin(2) gets the key. 
               The lower 8 bits are the ASCII code.
               The upper 16 bits contain the Scancode.
            */
            uint32_t key = Bconin(2);
            char ascii = (char)(key & 0xFF);

            if (ascii == ' ') {
                printf("Shot triggered! VBL count: %ld\n", counter);
                trigger_shot();
            } else if (ascii == 'q' || ascii == 'Q') {
                running = 0;
            } else if (ascii == 'b' || ascii == 'B') {
                Dosound(fx_get_item);
            } else if (ascii == 'c' || ascii == 'C') {
                Dosound(fx_click);
            } else if (ascii == 's' || ascii == 'S') {
                Dosound(fx_step);
            } else if (ascii == 'f' || ascii == 'F') {
                Dosound(snd_put_bomb);
            } else if (ascii == 'd' || ascii == 'D') {
                Dosound(fx_bomb_drop);
            
            } else if (ascii == 'e' || ascii == 'E') {
                Dosound(fx_explosion);
            } else if (ascii == 'r' || ascii == 'R') {
                Dosound(fx_explosion2);

            } else if (ascii == 'u' || ascii == 'Y') {
                Dosound(fx_lostlife);


            } else if (ascii == 'x' || ascii == 'X') {
                Dosound(fx_full_bomb);
            } else if (ascii == 't' || ascii == 'T') {
                Dosound(fx_bomb_tic);
            } else if (ascii == 'y' || ascii == 'T') {
                Dosound(fx_bomb_tac);
            } else if (ascii == 'w' || ascii == 'W') {
                printf("walking sound\n");
                Dosound(fx_walk1);               
            }
            else if (ascii == 'n' || ascii == 'N') {             

                Dosound((const char *)musica);                
                /*
                long old_stack = Super(0);                
                printf("frclock: %ld\n", *frclock);   
                *(volatile long *)0x486L = (long)musica;             
                Super(old_stack);*/
            }
            else if (ascii == 'p' || ascii == 'P') {
                Dosound(fx_escala);     
            }
            else if (ascii == 'o' || ascii == 'O') {
                Dosound(fx_escala);     
            }
            else if (ascii == 'l' || ascii == 'L') {
                Dosound(fx_babaliba_song_short);
            }
            else if (ascii == 'k' || ascii == 'K') {
                Dosound(fx_mute);
            }
            else
            {
                printf("Key pressed: ASCII=%c (0x%02X)\n", ascii, (unsigned char)ascii);
            }

            while (Bconstat(2) != 0)
                (void)Bconin(2);
        }        

        if( (mouse & 0x02) && !clicked ) {
            // Left button clicked
            printf("Left mouse button clicked!\n");
            Dosound((const char *)musica);       
            clicked = 1;
        } else if( !(mouse & 0x02) ) {
            // Left button released
            clicked = 0;
        }

        //update_sfx(); // Update sound effects state
        
        Vsync(); // Wait for VBL
    }

    Initmous(1, NULL, old_mouse_vec);
    //remove_vbl_interrupt();    
    return 0;
}