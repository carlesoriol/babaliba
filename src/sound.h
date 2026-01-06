#define CHANNEL_A_FINE 0 
#define CHANNEL_A_COARSE 1 
#define CHANNEL_B_FINE 2 
#define CHANNEL_B_COARSE 3 
#define CHANNEL_C_FINE 4 
#define CHANNEL_C_COARSE 5 
#define NOISE_PERIOD(VALUE)0x06, VALUE
#define CHANNELNUM 7 
#define PORT_STATUS 7 
#define SND_MIXER(VALUE) 7, VALUE
#define VOLUME_A(VALUE) 8, VALUE 
#define VOLUME_B(VALUE) 9, VALUE 
#define VOLUME_C(VALUE) 0x0a, VALUE
#define MUTE_A VOLUME_A(0)
#define MUTE_B VOLUME_B(0)
#define MUTE_C VOLUME_C(0)
#define MUTE_ALL VOLUME_A(0), VOLUME_B(0), VOLUME_C(0)
#define ENV_PERIOD_FINE 0x0b
#define ENV_PERIOD_COARSE 0x0c 
#define ENV_SHAPE 0x0d
#define ENV_VOLUME 0x0f
#define ENV_9 9 
#define WRITE 128

#define USE_ENVELOPE_A VOLUME_A(0x10)
#define USE_ENVELOPE_B VOLUME_B(0x10)
#define USE_ENVELOPE_C VOLUME_C(0x10)

/*00xx	00-03	Single Decay	High to Low, then silence ( \___ )
01xx	04-07	Single Attack	Low to High, then silence ( /|___ )
1000	08	Decay Loop (Sawtooth)	High to Low, repeats ( |\|\|\|\ )
1001	09	Single Decay & Hold Low	High to Low, stays at zero ( \___ ) ?
1010	0A	Decay-Attack Loop (Triangle)	High to Low, then Low to High, repeats ( \/\/\/\/ )
1011	0B	Decay & Hold High	High to Low, then jumps to High and stays ( \|‾‾‾ )
1100	0C	Attack Loop (Inverted Sawtooth)	Low to High, repeats ( /|/|/|/| )
1101	0D	Attack & Hold High	Low to High, stays at High ( /‾‾‾ )
1110	0E	Attack-Decay Loop (Inverted Triangle)	Low to High, then High to Low, repeats ( /\/\/\/\ )
1111	0F	Attack & Hold Low	Low to High, then drops to zero and stays ( /|___ )

Single envelope bits
Bit 3: Continue
Bit 2: Attack
Bit 1: Alternate
Bit 0: Hold
*/

#define NOTE_A(NOTE,SCALE) CHANNEL_A_FINE, NOTE##SCALE##_FINE, CHANNEL_A_COARSE, NOTE##SCALE##_COARSE    
#define NOTE_B(NOTE,SCALE) CHANNEL_B_FINE, NOTE##SCALE##_FINE, CHANNEL_B_COARSE, NOTE##SCALE##_COARSE    
#define NOTE_C(NOTE,SCALE) CHANNEL_C_FINE, NOTE##SCALE##_FINE, CHANNEL_C_COARSE, NOTE##SCALE##_COARSE    
#define TONE_A(VALUE) CHANNEL_A_FINE, (VALUE&0xff), CHANNEL_A_COARSE, (VALUE>>8)   
#define TONE_B(VALUE) CHANNEL_B_FINE, (VALUE&0xff), CHANNEL_B_COARSE, (VALUE>>8)   
#define TONE_C(VALUE) CHANNEL_C_FINE, (VALUE&0xff), CHANNEL_C_COARSE, (VALUE>>8)   
#define ENV_PERIOD(NOTESPEED) ENV_PERIOD_FINE, (NOTESPEED&0xff), ENV_PERIOD_COARSE, (NOTESPEED>>8)
#define TONE_A(VALUE) CHANNEL_A_FINE, (VALUE&0xff), CHANNEL_A_COARSE, (VALUE>>8)   

#define ENV_SINGLE_DECAY(NOTESPEED)        ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b0000 
#define ENV_SINGLE_DECAY_2(NOTESPEED)      ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b0001 
#define ENV_SINGLE_DECAY_3(NOTESPEED)      ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b0010 
#define ENV_SINGLE_DECAY_4(NOTESPEED)      ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b0011 
#define ENV_SINGLE_ATTACK(NOTESPEED)       ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b0100 
#define ENV_SINGLE_ATTACK_2(NOTESPEED)     ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b0101 
#define ENV_SINGLE_ATTACK_3(NOTESPEED)     ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b0110 
#define ENV_SINGLE_ATTACK_4(NOTESPEED)     ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b0111
#define ENV_SAWTOOTH(NOTESPEED)            ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b1000
#define ENV_SINGLE_DECAY_HOLD(NOTESPEED)   ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b1001
#define ENV_TRIANGLE(NOTESPEED)            ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b1010
#define ENV_DECAY_HOLD_HIGH(NOTESPEED)     ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b1011
#define ENV_INVERTED_SAWTOOTH(NOTESPEED)   ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b1100
#define ENV_ATTACK_HOLD_HIGH(NOTESPEED)    ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b1101
#define ENV_INVERTED_TRIANGLE(NOTESPEED)   ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b1110
#define ENV_ATTACK_HOLD_LOW(NOTESPEED)     ENV_PERIOD(NOTESPEED), ENV_SHAPE, 0b1111

#define SND_WAIT(TIME) 0x82, TIME // Wait in frames N * (1/50s) 
#define SND_END 255, 0

#define LOAD_TEMP_REG 0x80
#define USE_TEMP_REG 0x81    

#define VOL_ENVELOPE 0x10
#define VOL_MUTE 0
#define VOL_MAX 15

// mixer tone
#define CHANNEL_A_TONE    0b11111110 
#define CHANNEL_B_TONE    0b11111101
#define CHANNEL_C_TONE    0b11111011 

// mixer noise
#define CHANNEL_A_NOISE   0b11110111
#define CHANNEL_B_NOISE   0b11101111
#define CHANNEL_C_NOISE   0b11011111

// mixer values must be joined with & AND (not with ! or)

#define A_OFF 0X00 
#define B_OFF 
#define C_OFF 
#define AB_OFF 
#define AC_OFF 
#define BC_OFF 
#define ABC_OFF

/* Tone production */

/* Sound */

/* Tone production */
#define C1_COARSE 0x0E
#define C1_FINE 0x0EE
#define CS1_COARSE 0x0E
#define CS1_FINE 0x018
#define D1_COARSE 0x0D
#define D1_FINE 0x04D
#define DS1_COARSE 0x0C
#define DS1_FINE 0x08E
#define E1_COARSE 0x0B
#define E1_FINE 0x0DA
#define F1_COARSE 0x0B
#define F1_FINE 0x031
#define FS1_COARSE 0x0A
#define FS1_FINE 0x092
#define G1_COARSE 0x09
#define G1_FINE 0x0F9
#define GS1_COARSE 0x09
#define GS1_FINE 0x06B
#define A1_COARSE 0x08
#define A1_FINE 0x0E3
#define AS1_COARSE 0x08
#define AS1_FINE 0x063
#define B1_COARSE 0x07
#define B1_FINE 0x0E9
#define C2_COARSE 0x07
#define C2_FINE 0x077
#define CS2_COARSE 0x07
#define CS2_FINE 0x00C
#define D2_COARSE 0x06
#define D2_FINE 0x0A6
#define DS2_COARSE 0x06
#define DS2_FINE 0x047
#define E2_COARSE 0x05
#define E2_FINE 0x0ED
#define F2_COARSE 0x05
#define F2_FINE 0x098
#define FS2_COARSE 0x05
#define FS2_FINE 0x049
#define G2_COARSE 0x04
#define G2_FINE 0x0FD
#define GS2_COARSE 0x04
#define GS2_FINE 0x0B5
#define A2_COARSE 0x04
#define A2_FINE 0x071
#define AS2_COARSE 0x04
#define AS2_FINE 0x031
#define B2_COARSE 0x03
#define B2_FINE 0x0F4
#define C3_COARSE 0x03
#define C3_FINE 0x0BB
#define CS3_COARSE 0x03
#define CS3_FINE 0x086
#define D3_COARSE 0x03
#define D3_FINE 0x053
#define DS3_COARSE 0x03
#define DS3_FINE 0x023
#define E3_COARSE 0x02
#define E3_FINE 0x0F6
#define F3_COARSE 0x02
#define F3_FINE 0x0CC
#define FS3_COARSE 0x02
#define FS3_FINE 0x0A4
#define G3_COARSE 0x02
#define G3_FINE 0x07E
#define GS3_COARSE 0x02
#define GS3_FINE 0x05B
#define A3_COARSE 0x02
#define A3_FINE 0x038
#define AS3_COARSE 0x02
#define AS3_FINE 0x019
#define B3_COARSE 0x01
#define B3_FINE 0x0FA
#define C4_COARSE 0x01
#define C4_FINE 0x0DE
#define CS4_COARSE 0x01
#define CS4_FINE 0x0C3
#define D4_COARSE 0x01
#define D4_FINE 0x0AA
#define DS4_COARSE 0x01
#define DS4_FINE 0x092
#define E4_COARSE 0x01
#define E4_FINE 0x07B
#define F4_COARSE 0x01
#define F4_FINE 0x066
#define FS4_COARSE 0x01
#define GS4_FINE 0x02D
#define A4_COARSE 0x01
#define A4_FINE 0x01C
#define AS4_COARSE 0x01
#define AS4_FINE 0x00C
#define B4_COARSE 0x00
#define B4_FINE 0x0FD
#define C5_COARSE 0x00
#define C5_FINE 0x0EF
#define CS5_COARSE 0x00
#define CS5_FINE 0x0E1
#define D5_COARSE 0x00
#define FS4_FINE 0x052
#define G4_COARSE 0x01
#define G4_FINE 0x03F
#define GS4_COARSE 0x01
#define GS4_FINE 0x02D
#define A4_COARSE 0x01
#define A4_FINE 0x01C
#define AS4_COARSE 0x01
#define AS4_FINE 0x00C
#define B4_COARSE 0x00
#define B4_FINE 0x0FD
#define C5_COARSE 0x00
#define C5_FINE 0x0EF
#define CS5_COARSE 0x00
#define CS5_FINE 0x0E1
#define D5_COARSE 0x00
#define D5_FINE 0x0D5
#define DS5_COARSE 0x00
#define DS5_FINE 0x0C9
#define E5_COARSE 0x00
#define E5_FINE 0x0BE
#define F5_COARSE 0x00
#define F5_FINE 0x0B3
#define FS5_COARSE 0x00
#define FS5_FINE 0x0A9
#define G5_COARSE 0x00
#define G5_FINE 0x0A0
#define GS5_COARSE 0x00
#define GS5_FINE 0x097
#define A5_COARSE 0x00
#define A5_FINE 0x08E
#define AS5_COARSE 0x00
#define AS5_FINE 0x086
#define B5_COARSE 0x00
#define B5_FINE 0x07F
#define C6_COARSE 0x00
#define C6_FINE 0x077
#define CS6_COARSE 0x00
#define CS6_FINE 0x071
#define D6_COARSE 0x00
#define D6_FINE 0x06A
#define DS6_COARSE 0x00
#define DS6_FINE 0x064
#define E6_COARSE 0x00
#define E6_FINE 0x05F
#define F6_COARSE 0x00
#define F6_FINE 0x059
#define FS6_COARSE 0x00
#define FS6_FINE 0x054
#define G6_COARSE 0x00
#define G6_FINE 0x050
#define GS6_COARSE 0x00
#define GS6_FINE 0x04B
#define A6_COARSE 0x00
#define A6_FINE 0x047
#define AS6_COARSE 0x00
#define AS6_FINE 0x043
#define B6_COARSE 0x00
#define B6_FINE 0x03F
#define C7_COARSE 0x00
#define C7_FINE 0x03C
#define CS7_COARSE 0x00
#define CS7_FINE 0x038
#define D7_COARSE 0x00
#define D7_FINE 0x035
#define DS7_COARSE 0x00
#define DS7_FINE 0x032
#define E7_COARSE 0x00
#define E7_FINE 0x02F
#define F7_COARSE 0x00
#define F7_FINE 0x02D
#define FS7_COARSE 0x00
#define FS7_FINE 0x02A
#define G7_COARSE 0x00
#define G7_FINE 0x028
#define GS7_COARSE 0x00
#define GS7_FINE 0x026
#define A7_COARSE 0x00
#define A7_FINE 0x024
#define AS7_COARSE 0x00
#define AS7_FINE 0x021
#define B7_COARSE 0x00
#define B7_FINE 0x020

#define PSG_SELECT ((volatile char *)0xFFFF8800)
#define PSG_WRITE  ((volatile char *)0xFFFF8802)

#define WRITE_PSG(reg, value) *PSG_SELECT = (char)(reg); *PSG_WRITE = (char)(value);  

// Example: Play a tone on Channel A
/*void play_tone() {
    WRITE_PSG(0, 0xFE); // Fine tune frequency
    WRITE_PSG(1, 0x00); // Coarse tune
    WRITE_PSG(7, 0xFE); // Enable Channel A (Mixer)
    WRITE_PSG(8, 0x0F); // Set Volume to Max
}*/