

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

color_names = [
    "TRANSPARENT",
    "BLACK",
    "BLUE",
    "DARK_BLUE",
    "RED",
    "DARK_RED",
    "MAGENTA",
    "DARK_MAGENTA",
    "GREEN",
    "DARK_GREEN",
    "LIGHT_CYAN",
    "CYAN",
    "YELLOW",
    "DARK_YELLOW",
    "GREY",
    "WHITE" ]


# Create imgblocks for alternate colors
extra_blocks = {
    #ID, imgid, [(fromcolor, tocolor)])
    100: (9, [(WHITE, MAGENTA)]),
    101: (9, [(WHITE, YELLOW)]),
    
    102: (28, [(MAGENTA, WHITE), (DARK_MAGENTA, GREY)]),
    103: (29, [(MAGENTA, WHITE), (DARK_MAGENTA, GREY)]),
    104: (30, [(MAGENTA, WHITE), (DARK_MAGENTA, GREY)]),
    105: (31, [(MAGENTA, WHITE), (DARK_MAGENTA, GREY)]),

    106: (28, [(MAGENTA, CYAN), (DARK_MAGENTA, GREY)]),    
    107: (29, [(MAGENTA, CYAN), (DARK_MAGENTA, GREY)]),
    108: (30, [(MAGENTA, CYAN), (DARK_MAGENTA, GREY)]),
    109: (31, [(MAGENTA, CYAN), (DARK_MAGENTA, GREY)]),
    
    110: (28, [(MAGENTA, YELLOW), (DARK_MAGENTA, DARK_YELLOW)]),    
    111: (29, [(MAGENTA, YELLOW), (DARK_MAGENTA, DARK_YELLOW)]),
    112: (30, [(MAGENTA, YELLOW), (DARK_MAGENTA, DARK_YELLOW)]),
    113: (31, [(MAGENTA, YELLOW), (DARK_MAGENTA, DARK_YELLOW)]),

    114: (28, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)]),    
    115: (29, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)]),
    116: (30, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)]),
    117: (31, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)]),

    118: (19, [(MAGENTA, WHITE), (YELLOW, MAGENTA)]),
    119: (19, [(MAGENTA, LIGHT_CYAN), (YELLOW, WHITE)]),
    120: (19, [(MAGENTA, WHITE), (YELLOW, RED)]),
    121: (19, [(YELLOW, DARK_MAGENTA), (MAGENTA, YELLOW)]),

    122: (18, [(WHITE, YELLOW), (LIGHT_CYAN, MAGENTA)]),
    123: (18, [(WHITE, MAGENTA), (LIGHT_CYAN, WHITE)]),
    
    124:(19, [(YELLOW, WHITE)]),
    125:(19, [(YELLOW, GREY), (MAGENTA, WHITE)]),

    126: (27, [(WHITE, RED), (GREY, DARK_RED)]),

    127: (9, [(WHITE, LIGHT_CYAN)]),
    128: (18, [(LIGHT_CYAN, MAGENTA)]),
    129: (18, [(WHITE, MAGENTA), (LIGHT_CYAN, YELLOW)]),
    
    130: (11, [(WHITE, BLACK),(BLUE, LIGHT_CYAN), (DARK_BLUE, CYAN)]),
    131: (11, [(WHITE, BLACK),(BLUE, YELLOW), (DARK_BLUE, DARK_YELLOW)]),
    132: (11, [(WHITE, BLACK),(BLUE, GREEN), (DARK_BLUE, DARK_GREEN)]),
    133: (11, [(WHITE, RED), (GREY, DARK_RED), (BLUE, WHITE), (DARK_BLUE, GREY)]),

    134: (9, [(WHITE, RED)]),

    135: (10, [(WHITE,YELLOW),(LIGHT_CYAN, WHITE), (CYAN, WHITE)]),
    136: (10, [(WHITE,MAGENTA), (DARK_BLUE,WHITE), (LIGHT_CYAN, RED), (CYAN, DARK_RED)]),
    137: (10, [(WHITE,RED), (DARK_BLUE,WHITE), (LIGHT_CYAN, DARK_RED), (CYAN, WHITE)]),
    138: (10, [(DARK_BLUE,GREY), (LIGHT_CYAN, MAGENTA), (CYAN, DARK_MAGENTA)]),
    139: (10, [(WHITE, YELLOW), (LIGHT_CYAN, WHITE), (CYAN, GREEN)]),
    140: (10, [(LIGHT_CYAN, YELLOW), (CYAN, DARK_YELLOW)]),
    141: (10, [(DARK_BLUE,WHITE)]),
    142: (10, [(CYAN, DARK_GREEN),(LIGHT_CYAN, GREEN)]),
    
    143: (46, [(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)]),

    144: (37, [(DARK_MAGENTA, RED)] ),
    145: (38, [(DARK_MAGENTA, RED)] ),

    146: (65, [(MAGENTA,RED),(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)]),
    147: (75, [(MAGENTA,RED),(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)]),
    148: (65, [(GREEN, YELLOW), (DARK_GREEN, DARK_YELLOW)]),
    149: (75, [(GREEN, YELLOW), (DARK_GREEN, DARK_YELLOW)]),

    150: (32, [(MAGENTA, LIGHT_CYAN), (DARK_MAGENTA, CYAN)]),
    151: (33, [(MAGENTA, LIGHT_CYAN), (DARK_MAGENTA, CYAN)]),
    152: (34, [(MAGENTA, LIGHT_CYAN), (DARK_MAGENTA, CYAN)]),
    153: (35, [(MAGENTA, LIGHT_CYAN), (DARK_MAGENTA, CYAN)]),

    155: (32, [(YELLOW, RED), (MAGENTA, YELLOW), (DARK_MAGENTA, YELLOW)]),
    156: (33, [(YELLOW, RED), (MAGENTA, YELLOW), (DARK_MAGENTA, YELLOW)]),
    157: (34, [(YELLOW, RED), (MAGENTA, YELLOW), (DARK_MAGENTA, YELLOW)]),
    158: (35, [(YELLOW, RED), (MAGENTA, YELLOW), (DARK_MAGENTA, YELLOW)]),

    159: (20, [(GREEN, YELLOW), (DARK_GREEN, DARK_YELLOW)]),

    160: (32, [(YELLOW, DARK_GREEN), (MAGENTA, GREEN), (DARK_MAGENTA, WHITE)]),
    161: (33, [(YELLOW, DARK_GREEN), (MAGENTA, GREEN), (DARK_MAGENTA, WHITE)]),
    162: (34, [(YELLOW, DARK_GREEN), (MAGENTA, GREEN), (DARK_MAGENTA, WHITE)]),
    163: (35, [(YELLOW, DARK_GREEN), (MAGENTA, GREEN), (DARK_MAGENTA, WHITE)]),

    165: (32, [(MAGENTA, LIGHT_CYAN), (DARK_MAGENTA, CYAN)]),
    166: (33, [(MAGENTA, LIGHT_CYAN), (DARK_MAGENTA, CYAN)]),
    167: (34, [(MAGENTA, LIGHT_CYAN), (DARK_MAGENTA, CYAN)]),
    168: (35, [(MAGENTA, LIGHT_CYAN), (DARK_MAGENTA, CYAN)]),

    169: (26, [(WHITE, LIGHT_CYAN), (GREY, CYAN), (MAGENTA, WHITE), (YELLOW, RED)]),


    #coixins
    170: (22, [(RED, LIGHT_CYAN), (DARK_RED, CYAN), (YELLOW, WHITE)]),
    171: (22, [(RED, GREEN), (DARK_RED, DARK_MAGENTA), (YELLOW, RED)]),
    172: (22, [(RED, MAGENTA), (DARK_RED, DARK_GREEN), (YELLOW, WHITE)]),
    173: (22, [(YELLOW, MAGENTA), (RED, YELLOW), (DARK_RED, DARK_YELLOW)]),
    174: (22, [(RED, WHITE), (DARK_RED, BLACK)]),

    #pufs
    180: (21, [(MAGENTA, LIGHT_CYAN), (DARK_MAGENTA, CYAN)]),
    181: (21, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)]),
    182: (21, [(YELLOW,RED), (MAGENTA, YELLOW), (DARK_MAGENTA, DARK_YELLOW)]),
    183: (21, [(MAGENTA, RED), (DARK_MAGENTA, DARK_RED)]),
    184: (21, [(MAGENTA, WHITE), (DARK_MAGENTA, GREY)]),

    #jerres
    185: (24, [(GREEN,MAGENTA), (DARK_GREEN,DARK_MAGENTA)]),
    186: (24, [(GREEN,RED), (DARK_GREEN,DARK_RED)]),
    187: (24, [(GREEN,YELLOW), (DARK_GREEN,DARK_YELLOW)]),
    188: (24, [(GREEN,LIGHT_CYAN), (DARK_GREEN,CYAN)]),

    #taules
    190: (23, [(YELLOW, WHITE), (DARK_YELLOW, GREY), (GREEN, RED)]),


    191: (47, [(WHITE, LIGHT_CYAN), (GREY, CYAN), (GREEN,MAGENTA), (DARK_GREEN,DARK_MAGENTA)]),
    192: (48, [(WHITE, LIGHT_CYAN), (GREY, CYAN), (GREEN,MAGENTA), (DARK_GREEN,DARK_MAGENTA)]),
    193: (49, [(WHITE, LIGHT_CYAN), (GREY, CYAN), (GREEN,MAGENTA), (DARK_GREEN,DARK_MAGENTA)]),
    194: (50, [(WHITE, LIGHT_CYAN), (GREY, CYAN), (GREEN,MAGENTA), (DARK_GREEN,DARK_MAGENTA)]),

    #esglaons
    195: (14, [(DARK_MAGENTA, DARK_GREEN), (MAGENTA, GREEN)]),
    196: (14, [(DARK_MAGENTA, CYAN), (MAGENTA, LIGHT_CYAN)]),

    200: (32, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)]),
    201: (33, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)]),
    202: (34, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)]),
    203: (35, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)]),

    210: (15, [(WHITE, YELLOW), (GREY, DARK_YELLOW)] ),
    211: (15, [(WHITE, MAGENTA), (GREY, DARK_MAGENTA)] ),
    212: (15, [(WHITE, LIGHT_CYAN), (GREY, CYAN)] ),
    
    215: (16, [(WHITE, YELLOW), (GREY, DARK_YELLOW)] ),    
    216: (16, [(WHITE, LIGHT_CYAN), (GREY, CYAN)] ),

    220: (51, [(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)] ),
    221: (52, [(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)] ),
    222: (53, [(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)] ),
    223: (54, [(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)] ),
    224: (55, [(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)] ),

    225: (56, [(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)] ),

    226: (37, [(MAGENTA, BLUE), (DARK_MAGENTA, DARK_BLUE)] ),
    227: (38, [(MAGENTA, BLUE), (DARK_MAGENTA, DARK_BLUE)] ),

    228: (41, [(GREEN, RED), (DARK_GREEN, DARK_RED)] ),
    229: (42, [(GREEN, RED), (DARK_GREEN, DARK_RED)] ),
    230: (41, [(GREEN, YELLOW), (DARK_GREEN, DARK_YELLOW)] ),
    231: (42, [(GREEN, YELLOW), (DARK_GREEN, DARK_YELLOW)] ),
    232: (41, [(GREEN, WHITE), (DARK_GREEN, GREY)] ),
    233: (42, [(GREEN, WHITE), (DARK_GREEN, GREY)] ),
    
    234: (41, [(GREEN, WHITE), (DARK_GREEN, GREY), (DARK_BLUE,GREY), (BLUE, GREY)] ),
    235: (42, [(GREEN, WHITE), (DARK_GREEN, GREY), (DARK_BLUE,GREY), (BLUE, GREY)] ),

    236: (37, [(MAGENTA, RED), (DARK_MAGENTA, DARK_RED)] ),
    237: (38, [(MAGENTA, RED), (DARK_MAGENTA, DARK_RED)] ),
    238: (37, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)] ),
    239: (38, [(MAGENTA, GREEN), (DARK_MAGENTA, DARK_GREEN)] ),
    240: (37, [(MAGENTA, YELLOW), (DARK_MAGENTA, DARK_YELLOW)] ),
    241: (38, [(MAGENTA, YELLOW), (DARK_MAGENTA, DARK_YELLOW)] ),

    242: (39, [(DARK_YELLOW, LIGHT_CYAN), (DARK_RED, DARK_BLUE)] ),
    243: (40, [(DARK_YELLOW, LIGHT_CYAN), (DARK_RED, DARK_BLUE)] ),

    244: (41, [(GREEN, LIGHT_CYAN), (DARK_GREEN, CYAN)] ),
    245: (42, [(GREEN, LIGHT_CYAN), (DARK_GREEN, CYAN)] ),

    246: (62, [(BLUE, LIGHT_CYAN), (DARK_BLUE, CYAN)] ),
    247: (63, [(BLUE, LIGHT_CYAN), (DARK_BLUE, CYAN)] ),

    248: (62, [(BLUE, GREEN), (DARK_BLUE, DARK_GREEN)] ),
    249: (63, [(BLUE, GREEN), (DARK_BLUE, DARK_GREEN)] ),
                               
    250: (62, [(BLUE, RED), (DARK_BLUE, DARK_RED)] ),
    251: (63, [(BLUE, RED), (DARK_BLUE, DARK_RED)] ),
    
    252: (64, [(GREEN, MAGENTA), (DARK_GREEN, DARK_MAGENTA)]),    
    253: (64, [(GREEN, YELLOW), (DARK_GREEN, DARK_YELLOW)]),

    254: (14, [(MAGENTA, YELLOW), (DARK_MAGENTA, DARK_YELLOW)]),
}


# nomenclator

BLOCK_NAMES = { 1: ["Johny Jones", "left", "a"],
2: ["Johny Jones", "left", "b"],
3: ["Johny Jones", "up", "a"],
4: ["Johny Jones", "up", "b"],
5: ["Johny Jones", "down", "a"],
6: ["Johny Jones", "down", "b"],
7: ["Johny Jones", "right", "a"],
8: ["Johny Jones", "right", "b"],
9: ["Garden fence", "white", ""],
10: ["Tiling cyan", "", ""],
11: ["Tiling symbol", "white", "blue"],
12: ["Hole", "", ""],
13: ["Wall", "", ""],
14: ["Stair step", "magenta", ""],
15: ["Stones", "white", ""],
16: ["Stones with chains", "white", ""],
17: ["TNT", "", ""],
18: ["Flower 1", "cyan", "white"],
19: ["Flower 2", "magenta", "yellow"],
20: ["Hedge", "", ""],
21: ["Pouf", "magenta", ""],
22: ["Cushion", "red", ""],
23: ["Table", "yellow", "green"],
24: ["Jar", "green", ""],
25: ["Spider web ", "white", ""],
26: ["Flowerpot", "white", ""],
27: ["Plantpot", "white", ""],
28: ["Fountain", "magenta", "bottom right"],
29: ["Fountain", "magenta", "bottom left"],
30: ["Fountain", "magenta", "top right"],
31: ["Fountain", "magenta", "top left"],
32: ["Carpet", "magenta", "bottom left"],
33: ["Carpet", "magenta", "top left"],
34: ["Carpet", "magenta", "top right"],
35: ["Carpet", "magenta", "bottom right"],
36: ["Arrow", "right", "green"],
37: ["Crab", "a", "magenta"],
38: ["Crab", "b", "magenta"],
39: ["Bad guy", "left", "yellow"],
40: ["Bad guy", "right", "yellow"],
41: ["Sword bad guy", "left", "green"],
42: ["Sword bad guy", "right", "green"],
43: ["Crocodile", "close", "left"],
44: ["Crocodile", "open", "left"],
45: ["Princess", "left", "down"],
46: ["Key", "green", ""],
47: ["B", "white", "green"],
48: ["A", "white", "green"],
49: ["L", "white", "green"],
50: ["I", "white", "green"],
51: ["Gate", "green", "left bottom"],
52: ["Gate", "green", "left top"],
53: ["Gate", "green", "center top"],
54: ["Gate", "green", "right top"],
55: ["Gate", "green", "right bottom"],
56: ["Gate door", "green", ""],
57: ["Treasure chest", "magenta", "left"],
58: ["Treasure chest", "magenta", "right"],
59: ["Treasure", "a", ""],
60: ["Treasure", "b", ""],
61: ["Spider thread", "", ""],
62: ["Spider ", "a", ""],
63: ["Spider ", "b", ""],
64: ["Snake", "body", ""],
65: ["Snake", "head", "right"],
66: ["Prisoner", "left", ""],
67: ["Princess", "right", "up"],
68: ["Johny Jones", "life", ""],
69: ["Arrow", "right", "cyan"],
70: ["Empty", "", ""],
71: ["Prisoner", "right", ""],
72: ["Empty", "blue", ""],
73: ["Crocodile", "close", "right"],
74: ["Crocodile", "open", "right"],
75: ["Snake", "head", "left"],
76: ["Princess", "right", "down"],
77: ["Princess", "left", "up"],
78: ["", "", ""],
79: ["", "", ""],
80: ["", "", ""],
81: ["", "", ""],
82: ["", "", ""],
83: ["", "", ""],
84: ["", "", ""],
85: ["", "", ""],
86: ["", "", ""],
87: ["", "", ""],
88: ["", "", ""],
89: ["", "", ""],
90: ["", "", ""],
91: ["", "", ""],
92: ["", "", ""],
93: ["", "", ""],
94: ["", "", ""],
95: ["", "", ""],
96: ["", "", ""],
97: ["", "", ""],
98: ["", "", ""],
99: ["", "", ""],
100: ["", "", ""],
101: ["", "", ""],
102: ["", "", ""],
103: ["", "", ""],
104: ["", "", ""],
105: ["", "", ""],
106: ["", "", ""],
107: ["", "", ""],
108: ["", "", ""],
109: ["", "", ""],
110: ["", "", ""],
111: ["", "", ""],
112: ["", "", ""],
113: ["", "", ""],
114: ["", "", ""],
115: ["", "", ""],
116: ["", "", ""],
117: ["", "", ""],
118: ["", "", ""],
119: ["", "", ""],
120: ["", "", ""],
121: ["", "", ""],
122: ["", "", ""],
123: ["", "", ""],
124: ["", "", ""],
125: ["", "", ""],
126: ["", "", ""],
127: ["", "", ""],
128: ["", "", ""],
129: ["", "", ""],
130: ["", "", ""],
131: ["", "", ""],
132: ["", "", ""],
133: ["", "", ""],
134: ["", "", ""],
135: ["", "", ""],
136: ["", "", ""],
137: ["", "", ""],
138: ["", "", ""],
139: ["", "", ""],
140: ["", "", ""],
141: ["", "", ""],
142: ["", "", ""],
143: ["", "", ""],
144: ["", "", ""],
145: ["", "", ""],
146: ["", "", ""],
147: ["", "", ""],
148: ["", "", ""],
149: ["", "", ""],
150: ["", "", ""],
151: ["", "", ""],
152: ["", "", ""],
153: ["", "", ""],
154: ["", "", ""],
155: ["", "", ""],
156: ["", "", ""],
157: ["", "", ""],
158: ["", "", ""],
159: ["", "", ""],
160: ["", "", ""],
161: ["", "", ""],
162: ["", "", ""],
163: ["", "", ""],
164: ["", "", ""],
165: ["", "", ""],
166: ["", "", ""],
167: ["", "", ""],
168: ["", "", ""],
169: ["", "", ""],
170: ["", "", ""],
171: ["", "", ""],
172: ["", "", ""],
173: ["", "", ""],
174: ["", "", ""],
175: ["", "", ""],
176: ["", "", ""],
177: ["", "", ""],
178: ["", "", ""],
179: ["", "", ""],
180: ["", "", ""],
181: ["", "", ""],
182: ["", "", ""],
183: ["", "", ""],
184: ["", "", ""],
185: ["", "", ""],
186: ["", "", ""],
187: ["", "", ""],
188: ["", "", ""],
189: ["", "", ""],
190: ["", "", ""],
191: ["", "", ""],
192: ["", "", ""],
193: ["", "", ""],
194: ["", "", ""],
195: ["", "", ""],
196: ["", "", ""],
197: ["", "", ""],
198: ["", "", ""],
199: ["", "", ""],
200: ["", "", ""],
201: ["", "", ""],
202: ["", "", ""],
203: ["", "", ""],
204: ["", "", ""],
205: ["", "", ""],
206: ["", "", ""],
207: ["", "", ""],
208: ["", "", ""],
209: ["", "", ""],
210: ["", "", ""],
211: ["", "", ""],
212: ["", "", ""],
213: ["", "", ""],
214: ["", "", ""],
215: ["", "", ""],
216: ["", "", ""],
217: ["", "", ""],
218: ["", "", ""],
219: ["", "", ""],
220: ["", "", ""],
221: ["", "", ""],
222: ["", "", ""],
223: ["", "", ""],
224: ["", "", ""],
225: ["", "", ""],
226: ["", "", ""],
227: ["", "", ""],
228: ["", "", ""],
229: ["", "", ""],
230: ["", "", ""],
231: ["", "", ""],
232: ["", "", ""],
233: ["", "", ""],
234: ["", "", ""],
235: ["", "", ""],
236: ["", "", ""],
237: ["", "", ""],
238: ["", "", ""],
239: ["", "", ""],
240: ["", "", ""],
241: ["", "", ""],
242: ["", "", ""],
243: ["", "", ""],
244: ["", "", ""],
245: ["", "", ""],
246: ["", "", ""],
247: ["", "", ""],
248: ["", "", ""],
249: ["", "", ""],
250: ["", "", ""],
251: ["", "", ""],
252: ["", "", ""],
253: ["", "", ""],
254: ["", "", ""],
255: ["", "", ""],
}


def block_name(block_id):
    
    if (block_id not in extra_blocks) or (BLOCK_NAMES[block_id][0] != ""):
        name = " ".join(part for part in BLOCK_NAMES[block_id] if part)
        return name if name else f"Block {block_id}"

    extrablock = extra_blocks[block_id]    
    base_name = BLOCK_NAMES[extrablock[0]][0] 
    name_ext = BLOCK_NAMES[extrablock[0]][1]
    variant = BLOCK_NAMES[extrablock[0]][2]

    for replacement in extrablock[1]:
        fromcolor, tocolor = replacement
        if name_ext.upper() == color_names[fromcolor] :
            name_ext = f"{color_names[tocolor].lower()}"
                
        if variant.upper() == color_names[fromcolor] :
            variant = f"{color_names[tocolor].lower()}"
        
    rets = f"{base_name} {name_ext} {variant}".replace("  ", " ").strip()

    return rets