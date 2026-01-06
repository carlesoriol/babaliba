#! /usr/bin/env python3


# Convert multicolor image to monochrome image
# using one color for paint, another for mask and third for background

import argparse
import PIL
from PIL import Image

FORMAT_MODES = ['interlaced_mask_first', 'interlaced_image_first',
                'mask_first', 'mask_only', 'image_first', 'image_only']
INTERLACED_MASK_FIRST = 0
INTERLACED_IMAGE_FIRST = 1
MASK_FIRST = 2
MASK_ONLY = 3
IMAGE_FIRST = 4
IMAGE_ONLY = 5

LANGUAGES = ['c', 'python', 'asm', 'none']
LANGUAGE_C = 0
LANGUAGE_PYTHON = 1
LANGUAGE_ASM = 2
LANGUAGE_NONE = 3

NUMBERS_FORMATS = ['bin', 'hex', 'dec']
NUMBER_BINARY = 0
NUMBER_HEX = 1
NUMBER_DEC = 2


def autoColors(im):
    # try to auto detect the 3 colors
    # 1st color is the close to image borders = background color
    # 2nd color is the close to image contour = mask color
    # 3rd color is the paint color

    # 3 colors
    colors = []
    border_count = {}
    mask_count = {}

    # add the 3 colors
    # global colors, border_count, mask_count
    for y in range(im.height):
        for x in range(im.width):
            color = im.getpixel((x, y))
            if color in colors:
                continue
            assert len(
                colors) <= 3,  f"Image {filename} has more than 3 colors"
            colors.append(color)
            border_count[color] = 0
            mask_count[color] = 0

    for y in range(im.height):
        color = im.getpixel((0, y))
        border_count[color] += 1
        c1, c2 = color, color
        for x in range(1, im.width):
            color2 = im.getpixel((x, y))
            if color2 != c2:
                if c1 == c2:
                    mask_count[color2] += 1
                c1 = c2
                c2 = color2
        if c1 != c2:
            mask_count[c1] += 1
        border_count[c2] += 1

    for x in range(im.width):
        color = im.getpixel((x, 0))
        border_count[color] += 1
        c1, c2 = color, color
        for y in range(0, im.height):
            color2 = im.getpixel((x, y))
            if color2 != c2:
                if c1 == c2:
                    mask_count[color2] += 1
                c1 = c2
                c2 = color2
        if c1 != c2:
            mask_count[c1] += 1
        border_count[c2] += 1

    border_col = [key for key, val in border_count.items(
    ) if val == max(border_count.values())][0]
    colors.remove(border_col)
    mask_count.pop(border_col)
    mask_col = [key for key, val in mask_count.items() if val ==
                max(mask_count.values())][0]
    colors.remove(mask_col)
    color = colors[0]

    return color, mask_col, border_col


def decodeColor(tcol):
    color = None
    try:
        if len(tcol) == 6:
            color = tuple(int(tcol[i:i+2], 16) for i in (0, 2, 4))
        else:
            color = eval(tcol)
    except:
        print(f"Error decoding color {tcol}\n")
        exit(-1)

    return color


filename = "images/clau1.png"
outbinfilename = "images_s/clau1.bin"

outbin_image = key = bytes([])  # image or combined image and mask
outbin_mask = key = bytes([])

parser = argparse.ArgumentParser(
    prog="demask.py", description="Convert Image to monochrome source code image with mask.\r\nAlso export it if necessary to binary file.",
    epilog="by Carles Oriol 2023", formatter_class=argparse.RawTextHelpFormatter)
parser.add_argument('filename', help='Image file to convert to source code')
parser.add_argument('-l', '--lang', choices=LANGUAGES,
                    default='asm', help='Export language')
parser.add_argument('-n', '--numbers', choices=NUMBERS_FORMATS,
                    default='bin', help='Number format')
parser.add_argument('-f', '--format', default='interlaced_mask_first',
                    choices=FORMAT_MODES, help='Export format')
parser.add_argument('-i', '--invert_mask', default=False,
                    action='store_true', help='Invert the mask bits')
parser.add_argument('-p', '--prefix', default="image_",
                    help='Object name prefix')
parser.add_argument('-c', '--image_color', metavar="color", default="",
                    help='Overwrites calculated image color. Colors Format: RRGGBB in hex or (R,G,B) in decimal without spaces')
parser.add_argument('-m', '--mask_color', metavar="color",
                    default="", help='Overwrites calculated mask color')
parser.add_argument('-b', '--background_color', dest="border_color", metavar="color",
                    default="", help='Overwrites calculated transparent background color')
parser.add_argument('-ic', '--info_colors', default=False,
                    action='store_true', help='Shows colors information in image header. ')
parser.add_argument('-o', '--output_bin',
                    help='Generate binary file with image data using the specified format.')
parser.add_argument('-a', '--append_bin', default=False,
                    action='store_true', help='Append to binary file if already exists.')
args = parser.parse_args()

export_mode = FORMAT_MODES.index(args.format)
export_lang = LANGUAGES.index(args.lang)
export_numbers = NUMBERS_FORMATS.index(args.numbers)
export_invert_mask = args.invert_mask
filename = args.filename
outbinfilename = args.output_bin
prefix = args.prefix
outbin_append = args.append_bin

# load image
im = Image.open(filename).convert("RGB")
color, mask_color, border_color = autoColors(im)

if args.image_color != "":
    color = decodeColor(args.image_color)
if args.mask_color != "":
    mask_color = decodeColor(args.mask_color)
if args.border_color != "":
    border_color = decodeColor(args.border_color)

info_colors = args.info_colors

result_width = int(im.width/8)
result_width *= 8
if result_width != im.width:
    result_width += 8

imagename = filename.split('.')[0].split('/')[-1]
line_prefix = ""
line_sufix = ""
last_line_sufix = ""

if export_lang == LANGUAGE_C:
    print(f"// Image {imagename}")
    print(f"// Width: {result_width} Height: {im.height}")
    if info_colors:
        print(f"// Colors: {color} {mask_color} {border_color}")
    print()

    print(f"const uint8_t {prefix}{imagename}[] = " + "{\n\t", end="")
    line_prefix = "\t"
    line_sufix = ", \n"
    last_line_sufix = ""

elif export_lang == LANGUAGE_ASM:

    print(f"; Image {imagename}")
    print(f"; Width: {result_width} Height: {im.height}")
    if info_colors:
        print(f"; Colors: {color} {mask_color} {border_color}")
    print()
    print(f"{prefix}{imagename}:\n\tdb ", end="")
    line_prefix = "\tdb "
    line_sufix = "\n"
    last_line_sufix = ""

elif export_lang == LANGUAGE_PYTHON:

    print(f"# Image {imagename}")
    print(f"# Width: {result_width} Height: {im.height}")
    if info_colors:
        print(f"#Colors: {color} {mask_color} {border_color}")
    print()
    print(f"{prefix}{imagename} = [ ", end="")
    line_prefix = "\t"
    line_sufix = ", \n"
    last_line_sufix = ""

tmask = ""
timage = ""
for y in range(0, im.height):
    maskline = ""
    imageline = ""
    line_outbin_image = key = bytes([])
    line_outbin_mask = key = bytes([])
    tline_prefix = line_prefix
    if y == 0:
        tline_prefix = ""
    for x in range(0, result_width, 8):
        if x != 0:
            maskline += ", "
            imageline += ", "
        bmask = 0
        bpixel = 0
        for b in range(0, 8):
            pcolor = im.getpixel((min(x+b, im.width-1), y))
            if pcolor == border_color:
                bmask |= (0b10000000) >> b
            if pcolor == color:
                bpixel |= (0b10000000) >> b

        if export_invert_mask:
            bmask = ~bmask & 0xff

        if export_numbers == NUMBER_BINARY:
            maskline += f"0b{bmask:08b}"
            imageline += f"0b{bpixel:08b}"
        elif export_numbers == NUMBER_HEX:
            maskline += f"0x{bmask:02x}"
            imageline += f"0x{bpixel:02x}"
        else:
            maskline += f"{bmask}"
            imageline += f"{bpixel}"

        line_outbin_mask += bytes({bmask})
        line_outbin_image += bytes({bpixel})

    iline_sufix = line_sufix
    if y == im.height-1:  # and export_lang in (LANGUAGE_C, LANGUAGE_PYTHON):
        iline_sufix = last_line_sufix

    if export_mode == INTERLACED_MASK_FIRST:
        if export_lang != LANGUAGE_NONE:
            print(tline_prefix + maskline + ", " +
                  imageline + iline_sufix, end="")
        outbin_image += line_outbin_mask
        outbin_image += line_outbin_image
    elif export_mode == INTERLACED_IMAGE_FIRST:
        if export_lang != LANGUAGE_NONE:
            print(tline_prefix+imageline + ", " +
                  maskline + iline_sufix, end="")
        outbin_image += line_outbin_image
        outbin_image += line_outbin_mask
    else:
        outbin_mask += line_outbin_mask
        outbin_image += line_outbin_image

    tmask += tline_prefix + maskline + iline_sufix
    timage += tline_prefix + imageline + iline_sufix

if export_mode == IMAGE_ONLY:
    if export_lang != LANGUAGE_NONE:
        print(timage, end=last_line_sufix)

if export_mode == IMAGE_FIRST:
    if export_lang != LANGUAGE_NONE:
        print(timage, end=line_sufix)
        print()
        print(tline_prefix+tmask, end="")
    outbin_image += outbin_mask

if export_mode == MASK_ONLY:
    if export_lang != LANGUAGE_NONE:
        print(tmask, end=last_line_sufix)
    outbin_image = outbin_mask

if export_mode == MASK_FIRST:
    if export_lang != LANGUAGE_NONE:
        print(tmask, end=line_sufix)
        print()
        print(tline_prefix+timage, end="")
    outbin_image = outbin_mask + outbin_image

if export_lang == LANGUAGE_C:
    print("\n};\n")

elif export_lang == LANGUAGE_PYTHON:
    print(" ]\n")

elif export_lang == LANGUAGE_ASM:
    print("\n")

if outbinfilename:
    with open(outbinfilename, "ab" if outbin_append else "wb") as f:
        f.write(outbin_image)
