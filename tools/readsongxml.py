#!/usr/bin/env python3

import xml.etree.ElementTree as ET
import sys

DEFAULT_SONG_XML = '../data/babaliba_song.xml'

TIME_MULTIPLIER = 5

notes = ["C", "CS", "D", "DS", "E", "F", "FS", "G", "GS", "A", "AS", "B"]

def printnote(fnote, duration):
    note = fnote % 12
    escale = fnote // 12
    if fnote != 0:
        print(f" NOTE_A({notes[note]},{escale}), ENV_SINGLE_DECAY(0x4000), ", end="")
    print(f"SND_WAIT({duration*TIME_MULTIPLIER}), """)


def read_song_xml(file_path):
    tree = ET.parse(file_path)
    root = tree.getroot()
    
    # find element <tracks> <height>   
    song_length = int(root.find('height').text)
    track = root.find('track')
    
    # extract extension and path from file_path
    name = file_path.split('/')[-1].split('.')[0]

    print(f"unsigned char fx_{name}[] = {{\nSND_MIXER, CHANNEL_A_TONE,")

    index = 0
    note = -1
    for cell in track.findall('cell'):        
        newindex = int(cell.find('index').text) 
        if note != -1:
            printnote(note, newindex - index)
            
        index = newindex
        note = int(cell.find('note').text)        
        if int(cell.find('instrument').text) == 0:
            note = 0
        #print (f"Index {index}: Note {note}")

    #print( note, song_length - index)    
    printnote(note, song_length - index)

    print("VOLUME_A(VOL_MUTE), SND_END")

    print("};")

read_song_xml(sys.argv[1] if len(sys.argv) > 1 else DEFAULT_SONG_XML)