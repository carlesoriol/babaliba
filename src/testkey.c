#include <stdio.h>
#include <sys/types.h>


// --- IKBD Definitions ---
// The Atari ST IKBD sends 128 possible scan codes (0x00 to 0x7F)



volatile unsigned char joy1_state = 0;
volatile unsigned char joy1_header = 0;
volatile unsigned char joy1_extra = 0;
volatile int jchanged = 0;

/* This is the interrupt handler. It must be as fast as possible. */
/* The IKBD passes a pointer to the packet in the A0 register. */
void  my_joy_handler(unsigned char *packet) {
    /* packet[0] is the header (0xFD for Joy 1, FE Joy 0) */
    /* packet[1] is the state byte */
    joy1_header = packet[0];
    joy1_state = packet[1];
    joy1_extra = packet[2];
    
    jchanged ++;
    
    //printf("Joy: %02X\n", joy1_data);
    
}


typedef struct {
    void (*midivec)();   // MIDI interrupt
    void (*vkbdvec)();   // Keyboard interrupt
    void (*vmidvec)();   // MIDI error
    void (*statvec)();   // Status packet
    void (*mousevec)();  // Mouse packet
    void (*clockvec)();  // Clock packet
    void (*joyvec)();    // Joystick packet
} KBDVBASE;

void (*old_joy_vec)(void);

void install_joystick() {
    KBDVBASE *vecs = (KBDVBASE *)Kbdvbase();

    /* 1. Disable mouse so it doesn't interfere */
     /* 2. Enable Joystick reporting */
    old_joy_vec = vecs->joyvec;
    vecs->joyvec = my_joy_handler;
    Ikbdws(1, "\022\024");
   
    
    /* 3. Install our handler into the joyvec slot */

}

void uninstall_joystick() {
    KBDVBASE *vecs = (KBDVBASE *)Kbdvbase();
    
    // Restore old joystick handler
    vecs->joyvec = old_joy_vec;
     Ikbdws(1, "\025\010");

}

// Example usage:
void main() 
{

    install_joystick();
    while (1) {


        /*if ((Cconis() != 0)) 
        {
            char c = Crawcin(); // Read the key (Cconin() or Crawcin() for raw)            
            if (c == 'X' || c == 'x') {
                printf("Exiting.\n");
                break;
            }

            printf("Key pressed: %c %d\n", c, c);
        }*/

        if (Bconstat(2)) {           // Is a key waiting?
            long lkey;
            long key;
            int scan_code;
            lkey = Bconin(2);         // Get the key
            scan_code = (lkey >> 16) & 0xFF; // Extract scan code
            key = lkey &0xFF;               // Extract ASCII character

            //if (scan_code == 0x3B) {             // F1 was pressed            }
            printf("Key pressed: %c Scan code: %02X lk:%08X \n", key , scan_code, lkey);

            if ( key == 'x') {
                printf("Exiting.\n");
                break;
            }
        }

        if( jchanged != 0 ) {
            
            printf("Joy : %02X data : %02X extra: %02X jch: %d\n", joy1_header, joy1_state, joy1_extra, jchanged);
            jchanged = 0;
        }

        //printf(".");

        // Your main loop code goes here (e.g., drawing, logic, etc.)
    }

    uninstall_joystick();
}