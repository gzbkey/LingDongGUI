#!/usr/bin/python
# -*- coding: utf-8 -*-

# *************************************************************************************************
#  Arm 2D project
#  @file        img2c.py
#  @brief       image to C-array converter
#
# *************************************************************************************************
#
# * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
# *
# * SPDX-License-Identifier: Apache-2.0
# *
# * Licensed under the Apache License, Version 2.0 (the License); you may
# * not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# * www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an AS IS BASIS, WITHOUT
# * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# */

import sys
from PIL import Image
import numpy as np
import time
import argparse
import os


class ColorType:
    COLOUR_SZ_1BIT = 0
    COLOUR_SZ_2BIT = 1
    COLOUR_SZ_4BIT = 2
    COLOUR_SZ_8BIT = 3
    COLOUR_SZ_16BIT = 4
    COLOUR_SZ_32BIT = 5
    COLOUR_SZ_24BIT = 6

    COLOUR_SZ_MASK = 0x07 << 1
    COLOUR_ENDIAN_MASK = 1 << 4
    COLOUR_ALPHA_MASK = 1 << 0
    COLOUR_VARIANT_MASK = 0x03 << 5

    GRAY8 = COLOUR_SZ_8BIT << 1
    RGB565 = COLOUR_SZ_16BIT << 1
    RGB888 = COLOUR_SZ_32BIT << 1
    RGBA8888 = (COLOUR_SZ_32BIT << 1) | (1 << 0)
    MASK_A1 = (COLOUR_SZ_1BIT << 1) | COLOUR_VARIANT_MASK
    MASK_A2 = COLOUR_SZ_2BIT << 1
    MASK_A4 = COLOUR_SZ_4BIT << 1
    MASK_A8 = COLOUR_SZ_8BIT << 1


class BinaryTracker:
    def __init__(self, binfile):
        self.offset = 0
        self.binfile = binfile
        self.offsets = {}
        self.has_alpha = False
                
    def write_header_file(self, header_path, prefix):
        with open(header_path, 'w') as f:
            for name, offset in sorted(self.offsets.items()):
                name=name.replace('__', '_')
                name=name.strip('_').upper()
                print(f'{name}:0x{offset:08x}', file=f)
            if not self.has_alpha:
                prefix=prefix.strip('_').upper()
                print(f'{prefix}:0xFFFFFFFF', file=f)
                
    def write_data(self, data, align=4, name=None, width=0, height=0, color_type=0):
        start_offset = self.offset
        
        data_length = data.nbytes if isinstance(data, np.ndarray) else len(data)
        self.binfile.write(data_length.to_bytes(4, byteorder='little'))
        self.binfile.write(width.to_bytes(2, byteorder='little'))
        self.binfile.write(height.to_bytes(2, byteorder='little'))
        self.binfile.write(bytes([color_type]))
        self.binfile.write(bytes([0] * 7))
        self.offset += 16

        if align > 1:
            current_position = self.offset
            pad = (align - (current_position % align)) % align
            if pad > 0:
                self.binfile.write(bytes([0] * pad))
                self.offset += pad
        
        if isinstance(data, np.ndarray):
            data_le = data.astype(data.dtype.newbyteorder('<'))
            data_le.tofile(self.binfile)
            self.offset += data_le.nbytes
        else:
            self.binfile.write(data)
            self.offset += len(data)
            
        if align > 1:
            current_position = self.offset
            pad = (align - (current_position % align)) % align
            if pad > 0:
                self.binfile.write(bytes([0] * pad))
                self.offset += pad
            
        if name:
            self.offsets[name] = start_offset
            
        return start_offset
hdr="""
/* Generated on {0} from {1} */
/* Re-sized : {2} */
/* Rotated : {3} deg */



#include "arm_2d.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#   pragma clang diagnostic ignored "-Wcast-qual"
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_suppress=1296
#endif

"""

tailDataGRAY8="""

extern const arm_2d_tile_t c_tile{0}GRAY8;
ARM_SECTION(\"arm2d.tile.c_tile{0}GRAY8\")
const arm_2d_tile_t c_tile{0}GRAY8 = {{
    .tRegion = {{
        .tSize = {{
            .iWidth = {1},
            .iHeight = {2},
        }},
    }},
    .tInfo = {{
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {{
            .chScheme = ARM_2D_COLOUR_GRAY8,
        }},
    }},
    {3}c_bmp{0}GRAY8,
}};

"""

tailDataRGB565="""

extern const arm_2d_tile_t c_tile{0}RGB565;
ARM_SECTION(\"arm2d.tile.c_tile{0}RGB565\")
const arm_2d_tile_t c_tile{0}RGB565 = {{
    .tRegion = {{
        .tSize = {{
            .iWidth = {1},
            .iHeight = {2},
        }},
    }},
    .tInfo = {{
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {{
            .chScheme = ARM_2D_COLOUR_RGB565,
        }},
    }},
    {3}c_bmp{0}RGB565,
}};

"""

tailDataRGB888="""

extern const arm_2d_tile_t c_tile{0}CCCN888;

ARM_SECTION(\"arm2d.tile.c_tile{0}CCCN888\")
const arm_2d_tile_t c_tile{0}CCCN888 = {{
    .tRegion = {{
        .tSize = {{
            .iWidth = {1},
            .iHeight = {2},
        }},
    }},
    .tInfo = {{
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {{
            .chScheme = ARM_2D_COLOUR_RGB888,
        }},
    }},
    {3}c_bmp{0}CCCN888,
}};

"""

tailDataRGBA8888="""

extern const arm_2d_tile_t c_tile{0}CCCA8888;

ARM_SECTION(\"arm2d.tile.c_tile{0}CCCA8888\")
const arm_2d_tile_t c_tile{0}CCCA8888 = {{
    .tRegion = {{
        .tSize = {{
            .iWidth = {1},
            .iHeight = {2},
        }},
    }},
    .tInfo = {{
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {{
            .chScheme = ARM_2D_COLOUR_BGRA8888,
        }},
    }},
    {3}c_bmp{0}CCCA8888,
}};

"""

tailAlpha="""


extern const arm_2d_tile_t c_tile{0}Mask;

ARM_SECTION(\"arm2d.tile.c_tile{0}Mask\")
const arm_2d_tile_t c_tile{0}Mask = {{
    .tRegion = {{
        .tSize = {{
            .iWidth = {1},
            .iHeight = {2},
        }},
    }},
    .tInfo = {{
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {{
            .chScheme = ARM_2D_COLOUR_8BIT,
        }},
    }},
    .pchBuffer = (uint8_t *)c_bmp{0}Alpha,
}};
"""

tail1BitAlpha="""


extern const arm_2d_tile_t c_tile{0}A1Mask;

ARM_SECTION(\"arm2d.tile.c_tile{0}A1Mask\")
const arm_2d_tile_t c_tile{0}A1Mask = {{
    .tRegion = {{
        .tSize = {{
            .iWidth = {1},
            .iHeight = {2},
        }},
    }},
    .tInfo = {{
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {{
            .chScheme = ARM_2D_COLOUR_MASK_A1,
        }},
    }},
    .pchBuffer = (uint8_t *)c_bmp{0}A1Alpha,
}};
"""


tail2BitAlpha="""


extern const arm_2d_tile_t c_tile{0}A2Mask;

ARM_SECTION(\"arm2d.tile.c_tile{0}A2Mask\")
const arm_2d_tile_t c_tile{0}A2Mask = {{
    .tRegion = {{
        .tSize = {{
            .iWidth = {1},
            .iHeight = {2},
        }},
    }},
    .tInfo = {{
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {{
            .chScheme = ARM_2D_COLOUR_MASK_A2,
        }},
    }},
    .pchBuffer = (uint8_t *)c_bmp{0}A2Alpha,
}};
"""

tail4BitAlpha="""


extern const arm_2d_tile_t c_tile{0}A4Mask;

ARM_SECTION(\"arm2d.tile.c_tile{0}A4Mask\")
const arm_2d_tile_t c_tile{0}A4Mask = {{
    .tRegion = {{
        .tSize = {{
            .iWidth = {1},
            .iHeight = {2},
        }},
    }},
    .tInfo = {{
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {{
            .chScheme = ARM_2D_COLOUR_MASK_A4,
        }},
    }},
    .pchBuffer = (uint8_t *)c_bmp{0}A4Alpha,
}};
"""


tailAlpha2="""


extern const arm_2d_tile_t c_tile{0}Mask2;

ARM_SECTION(\"arm2d.tile.c_tile{0}Mask2\")
const arm_2d_tile_t c_tile{0}Mask2 = {{
    .tRegion = {{
        .tSize = {{
            .iWidth = {1},
            .iHeight = {2},
        }},
    }},
    .tInfo = {{
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {{
            .chScheme = ARM_2D_CHANNEL_8in32,
        }},
    }},
    .nAddress = ((intptr_t)c_bmp{0}CCCA8888) + 3,
}};
"""


tail="""

#if defined(__clang__)
#   pragma clang diagnostic pop
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_warning=1296
#endif

"""

def main(argv):

    parser = argparse.ArgumentParser(description='image to C array converter (v1.2.5)')

    parser.add_argument('-i', nargs='?', type = str,  required=False, help="Input file (png, bmp, etc..)")
    parser.add_argument('-o', nargs='?', type = str,  required=False, help="output C file containing RGB56/RGB888/Gray8 and alpha values arrays")

    parser.add_argument('--name', nargs='?',type = str, required=False, help="A specified array name")
    parser.add_argument('--format', nargs='?',type = str, default="all", help="RGB Format (rgb565, rgb32, gray8, mask, all)")
    parser.add_argument('--dim', nargs=2,type = int, help="Resize the image with the given width and height")
    parser.add_argument('--rot', nargs='?',type = float, default=0.0, help="Rotate the image with the given angle in degrees")
    parser.add_argument('--a1', action='store_true', help="Generate 1bit alpha-mask")
    parser.add_argument('--a2', action='store_true', help="Generate 2bit alpha-mask")
    parser.add_argument('--a4', action='store_true', help="Generate 4bit alpha-mask")

    args = parser.parse_args()

    if args.i == None or args.i == "" :
        parser.print_help()
        exit(1)
    inputfile = args.i
    basename = os.path.basename(inputfile).split('.')[0]


    outputfile = args.o
    if outputfile == None or outputfile == "":
        outputfile = basename + ".c"

    binfile = os.path.splitext(outputfile)[0] + ".bin"
    header_file = os.path.splitext(outputfile)[0] + ".txt"
    binary_tracker = BinaryTracker(open(binfile, 'wb'))
    
    arr_name = args.name
    if arr_name == None or arr_name == "":
        arr_name = basename

    if args.format != 'rgb565' and \
        args.format != 'rgb32' and \
        args.format != 'gray8' and \
        args.format != 'mask' and \
        args.format != 'all':
        parser.print_help()
        exit(1)

    try:
        image = Image.open(inputfile)
    except FileNotFoundError:
        print("Cannot open image file %s" % (inputfile))
        sys.exit(2)


    # rotation
    if args.rot != 0.0:
        image = image.rotate(args.rot)


    # resizing
    resized = False
    if args.dim != None:
        image = image.resize((args.dim[0], args.dim[1]))
        resized = True


    mode = image.mode

    # Modes supported by Pillow

    # 1 (1-bit pixels, black and white, stored with one pixel per byte), the value is in 0-1.
    # L (8-bit pixels, black and white), the value is in 0-255.
    # P (8-bit pixels, mapped to any other mode using a color palette), the value is in 0-255.
    # RGB (3×8-bit pixels, true color), the value is in [0-255, 0-255, 0-255].
    # RGBA (4×8-bit pixels, true color with transparency mask), the value is in [0-255, 0-255, 0-255, 0-255]
    # CMYK (4×8-bit pixels, color separation)
    # YCbCr (3×8-bit pixels, color video format)
    # LAB (3×8-bit pixels, the L*a*b color space)
    # HSV (3×8-bit pixels, Hue, Saturation, Value color space)
    # I (32-bit signed integer pixels)
    # F (32-bit floating point pixels)
    # LA (L with alpha)
    # PA (P with alpha)
    # RGBX (true color with padding)
    # RGBa (true color with premultiplied alpha)
    # La (L with premultiplied alpha)
    # I;16 (16-bit unsigned integer pixels)
    # I;16L (16-bit little endian unsigned integer pixels)
    # I;16B (16-bit big endian unsigned integer pixels)
    # I;16N (16-bit native endian unsigned integer pixels)
    # BGR;15 (15-bit reversed true colour)
    # BGR;16 (16-bit reversed true colour)
    # BGR;24 (24-bit reversed true colour)
    # BGR;32 (32-bit reversed true colour)

    # handle {P, LA, RGB, RGBA} for now
    if mode == 'P' or mode == 'LA':
        image = image.convert('RGBA')
        mode = 'RGBA'
    if mode == 'L':
        image = image.convert('RGB')
        mode = 'RGB'
    (row, col) = image.size
    data = np.asarray(image)

    # C Array format width
    WIDTH_ALPHA = 16
    WIDTH_GRAY8 = 32
    WIDTH_RGB565 = 16
    WIDTH_RGB32 = 16

    with open(outputfile,"w") as o:

        # insert header
        print(hdr.format(time.asctime( time.localtime(time.time())), argv[0], resized, args.rot), file=o)

        if mode == "RGBA":
            alpha_data = data[...,3].astype(np.uint8)
            alpha_offset = binary_tracker.write_data(alpha_data, 
                                                   name=f"{arr_name}_ALPHA",
                                                   width=row, 
                                                   height=col,
                                                   color_type=ColorType.MASK_A8)
            binary_tracker.has_alpha = True
            print('ARM_ALIGN(4) ARM_SECTION(\"arm2d.asset.c_bmp%sAlpha\")' % (arr_name), file=o)
            # alpha channel array available
            print('static const uint8_t c_bmp%sAlpha[%d*%d] = {' % (arr_name, row, col),file=o)
            cnt = 0
            for eachRow in data:
                lineWidth=0
                print("/* -%d- */" % (cnt), file=o)
                for eachPix in eachRow:
                    alpha = eachPix[3]
                    if lineWidth % WIDTH_ALPHA == (WIDTH_ALPHA - 1):
                        print("0x%02x," %(alpha) ,file=o)
                    else:
                        print("0x%02x" %(alpha), end =", ",file=o)
                    lineWidth+=1
                cnt+=1
                print('',file=o)
            print('};\r\n', file=o)

            # 1-bit Alpha channel
            if args.a1 or args.format == 'all' or args.format == 'mask':
                def RevBitPairPerByte(byteArr):
                    return ((byteArr & 0x01) << 7) | ((byteArr & 0x80) >> 7) | ((byteArr & 0x40) >> 5) | ((byteArr & 0x02) << 5) | \
                        ((byteArr & 0x04) << 3) | ((byteArr & 0x20) >> 3) | ((byteArr & 0x10) >> 1) | ((byteArr & 0x08) << 1)

                print('ARM_ALIGN(4) ARM_SECTION("arm2d.asset.c_bmp%sA1Alpha")' % (arr_name), file=o)
                print('static const uint8_t c_bmp%sA1Alpha[%d*%d] = {' % (arr_name, (row+7)//8, col), file=o)
                cnt = 0
                all_bytes = []
                alpha = data[...,3].astype(np.uint8)
                for eachRow in alpha:
                    lineWidth=0
                    print("/* -%d- */" % (cnt), file=o)

                    bitsArr = np.unpackbits(eachRow.astype(np.uint8))

                    # generate indexes for MSB bit every byte
                    idx = np.arange(0, np.size(bitsArr), 8)
                    idx = np.reshape(idx, (1,-1))

                    # extraction + endianness conversion
                    msbBits = bitsArr[idx] & 0x80 >> 7
                    packedBytes = RevBitPairPerByte(np.packbits(msbBits))

                    for elt in packedBytes:
                        all_bytes.append(elt)
                        if lineWidth % WIDTH_ALPHA == (WIDTH_ALPHA-1):
                            print("0x%02x," %(elt) ,file=o)
                        else:
                            print("0x%02x" %(elt), end =", ",file=o)
                        lineWidth+=1
                    cnt+=1
                    print('',file=o)
                print('};\n', file=o)

                all_bytes = np.array(all_bytes, dtype=np.uint8)
                a1_offset = binary_tracker.write_data(all_bytes,
                                                    name=f"{arr_name}_A1ALPHA",
                                                    width=row,
                                                    height=col,
                                                    color_type=ColorType.MASK_A1)

            # 2-bit Alpha channel
            if args.a2 or args.format == 'all' or args.format == 'mask':
                def RevBitPairPerByte(byteArr):
                    return ((byteArr & 0x03) << 6) |  ((byteArr & 0xc0) >> 6) | ((byteArr & 0x30) >> 2 ) | ((byteArr & 0x0c) << 2)

                print('ARM_ALIGN(4) ARM_SECTION(\"arm2d.asset.c_bmp%sA2Alpha\")' % (arr_name), file=o)
                print('static const uint8_t c_bmp%sA2Alpha[%d*%d] = {' % (arr_name, (row+3)//4, col),file=o)
                cnt = 0
                all_bytes = []
                alpha = data[...,3].astype(np.uint8)
                for eachRow in alpha:
                    lineWidth=0
                    print("/* -%d- */" % (cnt), file=o)

                    bitsArr = np.unpackbits(eachRow.astype(np.uint8))

                    # generate indexes for MSB bit pair every byte
                    idx = np.arange(0, np.size(bitsArr), 8)
                    idx=np.reshape(np.column_stack((idx+0, idx+1)), (1,-1))

                    # extraction + endianness conversion
                    packedBytes = RevBitPairPerByte(np.packbits(bitsArr[idx]))

                    for elt in packedBytes:
                        all_bytes.append(elt)
                        if lineWidth % WIDTH_ALPHA == (WIDTH_ALPHA-1):
                            print("0x%02x," %(elt) ,file=o)
                        else:
                            print("0x%02x" %(elt), end =", ",file=o)
                        lineWidth+=1
                    cnt+=1
                    print('',file=o)
                print('};\r\n', file=o)

                all_bytes = np.array(all_bytes, dtype=np.uint8)
                a2_offset = binary_tracker.write_data(all_bytes,
                                                    name=f"{arr_name}_A2ALPHA",
                                                    width=row,
                                                    height=col,
                                                    color_type=ColorType.MASK_A2)

            # 4-bit Alpha channel
            if args.a4 or args.format == 'all' or args.format == 'mask':
                def RevBitQuadPerByte(byteArr):
                    return ((byteArr & 0x0f) << 4) |  ((byteArr & 0xf0) >> 4)

                print('ARM_ALIGN(4) ARM_SECTION(\"arm2d.asset.c_bmp%sA4Alpha\")' % (arr_name), file=o)
                print('static const uint8_t c_bmp%sA4Alpha[%d*%d] = {' % (arr_name, (row+1)//2, col),file=o)
                cnt = 0
                all_bytes = []
                alpha = data[...,3].astype(np.uint8)
                for eachRow in alpha:
                    lineWidth=0
                    print("/* -%d- */" % (cnt), file=o)

                    bitsArr = np.unpackbits(eachRow.astype(np.uint8))

                    # generate indexes for MSB bit quadruplet every byte
                    idx = np.arange(0, np.size(bitsArr), 8)
                    idx=np.reshape(np.column_stack(
                            (np.column_stack((idx+0, idx+1)), np.column_stack((idx+2, idx+3)))),
                            (1,-1))

                    # extraction + endianness conversion
                    packedBytes = RevBitQuadPerByte(np.packbits(bitsArr[idx]))

                    for elt in packedBytes:
                        all_bytes.append(elt)
                        if lineWidth % WIDTH_ALPHA == (WIDTH_ALPHA - 1):
                            print("0x%02x," %(elt) ,file=o)
                        else:
                            print("0x%02x" %(elt), end =", ",file=o)
                        lineWidth+=1
                    cnt+=1
                    print('',file=o)
                print('};\r\n', file=o)

                all_bytes = np.array(all_bytes, dtype=np.uint8)
                a4_offset = binary_tracker.write_data(all_bytes,
                                                    name=f"{arr_name}_A4ALPHA",
                                                    width=row,
                                                    height=col,
                                                    color_type=ColorType.MASK_A4)


        # Gray8 channel array
        if args.format == 'gray8' or args.format == 'all':

            R = (data[...,0]).astype(np.uint16)
            G = (data[...,1]).astype(np.uint16)
            B = (data[...,2]).astype(np.uint16)
            # merge
            RGB = np.rint((R + G + B)/3).astype(np.uint8)

            gray8_offset = binary_tracker.write_data(RGB.astype(np.uint8), 
                                                   name=f"{arr_name}_GRAY8",
                                                   width=row, 
                                                   height=col,
                                                   color_type=ColorType.GRAY8)
            print('',file=o)
            print('ARM_SECTION(\"arm2d.asset.c_bmp%sGRAY8\")' % (arr_name), file=o)
            print('const uint8_t c_bmp%sGRAY8[%d*%d] = {' % (arr_name, row, col), file=o)
            cnt = 0
            for eachRow in RGB:
                lineWidth=0
                print("/* -%d- */" % (cnt), file=o)
                for eachPix in eachRow:
                    if lineWidth % WIDTH_GRAY8 == (WIDTH_GRAY8 - 1):
                        print("0x%02x," %(eachPix) ,file=o)
                    else:
                        print("0x%02x" %(eachPix), end =", ", file=o)
                    lineWidth+=1
                print('',file=o)
                cnt+=1
            print('};', file=o)
            buffStr='pchBuffer'
            typStr='uint8_t'

        # RGB565 channel array
        if args.format == 'rgb565' or args.format == 'all':
            R = (data[...,0]>>3).astype(np.uint16) << 11
            G = (data[...,1]>>2).astype(np.uint16) << 5
            B = (data[...,2]>>3).astype(np.uint16)
            # merge
            RGB = R | G | B

            rgb565_offset = binary_tracker.write_data(RGB.astype(np.uint16), 
                                                    name=f"{arr_name}_RGB565",
                                                    width=row, 
                                                    height=col,
                                                    color_type=ColorType.RGB565)
            print('',file=o)
            print('ARM_SECTION(\"arm2d.asset.c_bmp%sRGB565\")' % (arr_name), file=o)
            print('const uint16_t c_bmp%sRGB565[%d*%d] = {' % (arr_name, row, col), file=o)
            cnt = 0
            for eachRow in RGB:
                lineWidth=0
                print("/* -%d- */" % (cnt), file=o)
                for eachPix in eachRow:
                    if lineWidth % WIDTH_RGB565 == (WIDTH_RGB565 - 1):
                        print("0x%04x," %(eachPix) ,file=o)
                    else:
                        print("0x%04x" %(eachPix), end =", ", file=o)
                    lineWidth+=1
                print('',file=o)
                cnt+=1
            print('};', file=o)
            buffStr='phwBuffer'
            typStr='uint16_t'



        if args.format == 'rgb32' or args.format == 'all':
            R = data[...,0].astype(np.uint32) << 16
            G = data[...,1].astype(np.uint32) << 8
            B = data[...,2].astype(np.uint32)
            if mode == "RGBA":
                A = data[...,3].astype(np.uint32) << 24
            else:
                # alpha chanel forced to 0xFF
                A = 0xff << 24
            # merge
            RGB = R | G | B | A

            data_name = f"{arr_name}_RGBA8888" if mode == "RGBA" else f"{arr_name}_RGB888"
            color_type = ColorType.RGBA8888 if mode == "RGBA" else ColorType.RGB888
            rgb32_offset = binary_tracker.write_data(RGB.astype(np.uint32), 
                                                   name=data_name,
                                                   width=row, 
                                                   height=col,
                                                   color_type=color_type)
            print('',file=o)
            if mode == "RGBA":
                print('ARM_SECTION(\"arm2d.asset.c_bmp%sCCCA8888\")' % (arr_name), file=o)
                print('const uint32_t c_bmp%sCCCA8888[%d*%d] = {' % (arr_name, row, col), file=o)
            else:
                print('ARM_SECTION(\"arm2d.asset.c_bmp%sCCCN888\")' % (arr_name), file=o)
                print('const uint32_t c_bmp%sCCCN888[%d*%d]= {' % (arr_name, row, col), file=o)

            cnt = 0
            for eachRow in RGB:
                lineWidth=0
                print("/* -%d- */" % (cnt), file=o)
                for eachPix in eachRow:
                    if lineWidth % WIDTH_RGB32 == (WIDTH_RGB32 - 1):
                        print("0x%08x," %(eachPix) ,file=o)
                    else:
                        print("0x%08x" %(eachPix), end =", ", file=o)
                    lineWidth+=1
                print('',file=o)
                cnt+=1
            print('};', file=o)
            buffStr='pwBuffer'
            typStr='uint32_t'

        # insert tail
        if args.format == 'gray8' or args.format == 'all':
            buffStr='pchBuffer'
            typStr='uint8_t'
            print(tailDataGRAY8.format(arr_name, str(row), str(col), "."+buffStr+" = ("+typStr+"*)"), file=o)

        if args.format == 'rgb565' or args.format == 'all':
            buffStr='phwBuffer'
            typStr='uint16_t'
            print(tailDataRGB565.format(arr_name, str(row), str(col), "."+buffStr+" = ("+typStr+"*)"), file=o)

        if args.format == 'rgb32' or args.format == 'all':
            buffStr='pwBuffer'
            typStr='uint32_t'
            if mode == "RGBA":
                print(tailDataRGBA8888.format(arr_name, str(row), str(col), "."+buffStr+" = ("+typStr+"*)"), file=o)
                print(tailAlpha2.format(arr_name, str(row), str(col)), file=o)
            else :
                print(tailDataRGB888.format(arr_name, str(row), str(col), "."+buffStr+" = ("+typStr+"*)"), file=o)


        if mode == "RGBA":
            print(tailAlpha.format(arr_name, str(row), str(col)), file=o)

            if args.a1 or args.format == 'all' or args.format == 'mask':
                print(tail1BitAlpha.format(arr_name, str(row), str(col)), file=o)

            if args.a2 or args.format == 'all' or args.format == 'mask':
                print(tail2BitAlpha.format(arr_name, str(row), str(col)), file=o)

            if args.a4 or args.format == 'all' or args.format == 'mask':
                print(tail4BitAlpha.format(arr_name, str(row), str(col)), file=o)


        print(tail.format(arr_name, str(row), str(col)), file=o)
        
        binary_tracker.write_header_file(header_file, arr_name.upper())
        binary_tracker.binfile.close()

if __name__ == '__main__':
    main(sys.argv[1:])
