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
import re

# Import zhRGB565 compression modules
try:
    from __img2c_zhRGB565 import (
        encode_rgb565_rle_only,
        generate_c_array as generate_rle_c_array,
        encode_rgb565_rle_diff,
        generate_c_array as generate_diff_c_array
    )
    ZHRGB565_AVAILABLE = True
except ImportError:
    ZHRGB565_AVAILABLE = False

# Import LMSK compression modules
try:
    from __img2c_lmsk import (
        load_mask_from_image,
        encode_lmsk,
    )
    LMSK_AVAILABLE = True
except ImportError:
    LMSK_AVAILABLE = False


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
    def __init__(self, binfile, xip_offset=0):
        self.offset = 0
        self.binfile = binfile
        self.offsets = {}
        self.data_offsets = {}
        self.xip_macros = []
        self.xip_offset = xip_offset
        self.has_alpha = False

    def make_macro_name(self, name):
        name = name.replace('__', '_')
        name = name.strip('_').upper()
        return name

    def write_header_file(self, header_path, prefix):
        with open(header_path, 'w') as f:
            for name, offset in sorted(self.offsets.items()):
                macro_name = self.make_macro_name(name)
                print(f'{macro_name}:0x{offset:08x}', file=f)
            if not self.has_alpha:
                prefix = self.make_macro_name(prefix)
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

        data_offset = self.offset

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
            self.data_offsets[name] = data_offset
            macro_name = self.make_macro_name(name)
            self.xip_macros.append((macro_name, self.xip_offset + data_offset))

        return data_offset
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
#if (USE_XIP == 1)
    .pchBuffer = XIP_BASE + IMAGE_{3}_XIP_OFFSET,
#else
    .pchBuffer = (uint8_t *)c_bmp{0}GRAY8,
#endif
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
#if (USE_XIP == 1)
    .phwBuffer = (uint16_t *)(XIP_BASE + IMAGE_{3}_XIP_OFFSET),
#else
    .phwBuffer = (uint16_t *)c_bmp{0}RGB565,
#endif
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
#if (USE_XIP == 1)
    .pwBuffer = (uint32_t *)(XIP_BASE + IMAGE_{3}_XIP_OFFSET),
#else
    .pwBuffer = (uint32_t *)c_bmp{0}CCCN888,
#endif
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
#if (USE_XIP == 1)
    .pwBuffer = (uint32_t *)(XIP_BASE + IMAGE_{3}_XIP_OFFSET),
#else
    .pwBuffer = (uint32_t *)c_bmp{0}CCCA8888,
#endif
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
            .chScheme = ARM_2D_COLOUR_MASK_A8,
        }},
    }},
#if (USE_XIP == 1)
    .pchBuffer = XIP_BASE + IMAGE_{3}_XIP_OFFSET,
#else
    .pchBuffer = (uint8_t *)c_bmp{0}Alpha,
#endif
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
#if (USE_XIP == 1)
    .pchBuffer = XIP_BASE + IMAGE_{3}_XIP_OFFSET,
#else
    .pchBuffer = (uint8_t *)c_bmp{0}A1Alpha,
#endif
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
#if (USE_XIP == 1)
    .pchBuffer = XIP_BASE + IMAGE_{3}_XIP_OFFSET,
#else
    .pchBuffer = (uint8_t *)c_bmp{0}A2Alpha,
#endif
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
#if (USE_XIP == 1)
    .pchBuffer = XIP_BASE + IMAGE_{3}_XIP_OFFSET,
#else
    .pchBuffer = (uint8_t *)c_bmp{0}A4Alpha,
#endif
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
#if (USE_XIP == 1)
    .nAddress = (intptr_t)(XIP_BASE + IMAGE_{3}_XIP_OFFSET) + 3,
#else
    .nAddress = ((intptr_t)c_bmp{0}CCCA8888) + 3,
#endif
}};
"""


tailDataZHRGB565="""

extern const arm_2d_tile_t c_tile{0}ZHRGB565;
ARM_SECTION(\"arm2d.tile.c_tile{0}ZHRGB565\")
const arm_2d_tile_t c_tile{0}ZHRGB565 = {{
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
#if (USE_XIP == 1)
    .phwBuffer = (uint16_t *)(XIP_BASE + IMAGE_{3}_XIP_OFFSET),
#else
    .phwBuffer = (uint16_t *)c_zhrgb{0},
#endif
}};

"""


tailDataLMSK="""

extern const arm_2d_tile_t c_tile{0}LMSK;
ARM_SECTION(\"arm2d.tile.c_tile{0}LMSK\")
const arm_2d_tile_t c_tile{0}LMSK = {{
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
            .chScheme = ARM_2D_COLOUR_MASK_A8,
        }},
    }},
#if (USE_XIP == 1)
    .pchBuffer = XIP_BASE + IMAGE_{3}_XIP_OFFSET,
#else
    .pchBuffer = (uint8_t *)c_lmsk{0},
#endif
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

    parser = argparse.ArgumentParser(description='image to C array converter (v2.1.0)')

    parser.add_argument('-i', nargs='?', type = str,  required=False, help="Input file (png, bmp, etc..)")
    parser.add_argument('-o', nargs='?', type = str,  required=False, help="output C file containing RGB56/RGB888/Gray8 and alpha values arrays")

    parser.add_argument('--name', nargs='?',type = str, required=False, help="A specified array name")
    parser.add_argument('--format', nargs='?',type = str, default="all", help="RGB Format (rgb565, rgb32, gray8, mask, zhRGB565, lmsk, all)")
    parser.add_argument('--dim', nargs=2,type = int, help="Resize the image with the given width and height")
    parser.add_argument('--rot', nargs='?',type = float, default=0.0, help="Rotate the image with the given angle in degrees")
    parser.add_argument('--a1', action='store_true', help="Generate 1bit alpha-mask")
    parser.add_argument('--a2', action='store_true', help="Generate 2bit alpha-mask")
    parser.add_argument('--a4', action='store_true', help="Generate 4bit alpha-mask")
    parser.add_argument('--a8', action='store_true', help="Generate 8bit alpha-mask")
    parser.add_argument('--border', action='store_true', help="Add a 1pix border")
    parser.add_argument("--no-gradient", action="store_true", help="Disable gradient detection algorithm. (Repeat>62 still uses GRADIENT tag like C encoder.)")
    parser.add_argument("--gradient-tolerant", type=int, default=2, choices=[0, 1, 2, 3], help="Gradient tolerant (0~3). Default: 2.")
    parser.add_argument("--alpha-bits", type=int, default=6, choices=[1, 2, 3, 4, 5, 6, 7, 8], help="Alpha Bits (1~8). Default: 6.")
    parser.add_argument('--xip-offset', type=lambda x: int(x, 0), default=0, help="XIP base offset for this image bin")

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
    binary_tracker = BinaryTracker(open(binfile, 'wb'), xip_offset=args.xip_offset)

    arr_name = args.name
    if arr_name == None or arr_name == "":
        arr_name = basename

    if args.format != 'rgb565' and \
        args.format != 'rgb32' and \
        args.format != 'gray8' and \
        args.format != 'mask' and \
        args.format != 'zhRGB565' and \
        args.format != 'lmsk' and \
        args.format != 'all':
        parser.print_help()
        exit(1)

    if args.a1:
        args.format = ''

    if args.a2:
        args.format = ''

    if args.a4:
        args.format = ''

    if args.a8:
        args.format = ''

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

    # add 1 pixel border
    if args.border:
        data = np.asarray(image)
        pad_val = 0
        if data.ndim == 2:
            padded = np.pad(data, 1, mode='constant', constant_values=pad_val)
        else:
            padded = np.pad(data, ((1, 1), (1, 1), (0, 0)),
                            mode='constant', constant_values=pad_val)
        image = Image.fromarray(padded, mode=mode)

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
    origin_image = image

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
                        print("0x%02x" %(alpha), end =", ", file=o)
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

        # Lossless Compressed Mask (lmsk)
        if (args.format == 'lmsk') or (args.format == 'all'):
            if not LMSK_AVAILABLE:
                print("Warning: LMSK compression library not available, skipping LMSK format", file=sys.stderr)
            else:
                # Detect alpha presence robustly
                has_alpha = ("A" in origin_image.mode) or (origin_image.mode == "P" and "transparency" in origin_image.info)

                if has_alpha:
                    img = origin_image.convert("RGBA")
                    alpha = img.getchannel("A")
                    mask_raw = alpha.tobytes()
                else:
                    img = origin_image.convert("L")
                    mask_raw = img.tobytes()

                mask_compressed = encode_lmsk(mask_raw,
                    width=img.width,
                    height=img.height,
                    raw=False,
                    no_gradient=args.no_gradient,
                    tolerant=args.gradient_tolerant,
                    alpha_bits=args.alpha_bits)

                compressed_array = np.frombuffer(mask_compressed, dtype=np.uint8)
                lmsk_offset = binary_tracker.write_data(compressed_array,
                                                    name=f"{arr_name}_LMSK",
                                                    width=img.width,
                                                    height=img.height,
                                                    color_type=ColorType.MASK_A8)

                print('',file=o)
                print('extern const uint8_t c_lmsk%s[%d];\n' % (arr_name, len(mask_compressed)), file=o)
                print('ARM_ALIGN(4) ARM_SECTION(\"arm2d.asset.c_lmsk%s\")' % (arr_name), file=o)
                print('const uint8_t c_lmsk%s[%d] = {' % (arr_name, len(mask_compressed)), file=o)

                for i in range(0, len(mask_compressed), 16):
                    chunk = mask_compressed[i:i+16]
                    hex_strings = [f"0x{b:02x}" for b in chunk]
                    line = ", ".join(hex_strings)
                    o.write("    ")
                    o.write(line)
                    o.write(",\n")

                print('};', file=o)

        # zhRGB565 compressed format
        if (args.format == 'zhRGB565') or (args.format == 'all'):
            if not ZHRGB565_AVAILABLE:
                print("Warning: zhRGB565 compression library not available, skipping zhRGB565 format", file=sys.stderr)
            else:
                # Convert to RGB565 first
                R = (data[...,0]>>3).astype(np.uint16) << 11
                G = (data[...,1]>>2).astype(np.uint16) << 5
                B = (data[...,2]>>3).astype(np.uint16)
                RGB = R | G | B

                # Use RLE+DIFF compression for better gradient compression
                compressed_data, compressed_size, compression_ratio = encode_rgb565_rle_diff(RGB.flatten(), row, col)
                compression_method = "RLE+DIFF"

                if compressed_data is not None:
                    # Write compressed data to bin
                    compressed_array = np.array(compressed_data, dtype=np.uint16)
                    zh_offset = binary_tracker.write_data(compressed_array,
                                                      name=f"{arr_name}_ZHRGB565",
                                                      width=row,
                                                      height=col,
                                                      color_type=ColorType.RGB565)

                    print('',file=o)
                    print('/* ============================================ */', file=o)
                    print('/* zhRGB565 compressed data (%s) */' % compression_method, file=o)
                    print('/* Original size: %d bytes */' % (row * col * 2), file=o)
                    print('/* Compressed size: %d bytes */' % (compressed_size * 2), file=o)
                    print('/* Compression ratio: %.2f%% */' % compression_ratio, file=o)
                    print('/* ============================================ */', file=o)
                    print('',file=o)

                    # Generate compressed C array
                    c_code = generate_diff_c_array(compressed_data, compressed_size, row, col, compression_ratio, inputfile, arr_name)

                    # Write the compressed data (extract just the array part)
                    lines = c_code.split('\n')
                    in_array = False
                    for line in lines:
                        if 'const uint16_t' in line and '[' in line:
                            in_array = True
                            # Add ARM_SECTION directive and extern declaration for zhRGB565
                            # Extract the original array name and convert to ARM format
                            match = re.search(r'const uint16_t (\w+)\[(.*?)\]', line)

                            if match:
                                original_name = match.group(1)
                                # Convert to ARM format: c_zhRGB565_ + name
                                arm_name = f'c_zhrgb{arr_name}'
                                array_size = match.group(2) if match.group(2) else ''
                                # Generate extern declaration
                                extern_decl = f"extern const uint16_t {arm_name}[{array_size}];"
                                print(extern_decl, file=o)
                                print('ARM_SECTION("arm2d.asset.c_zhrgb%s")' % (arr_name), file=o)
                                # Generate the actual array definition with modified name
                                modified_line = line.replace(original_name, arm_name)
                                print(modified_line, file=o)

                            continue
                        elif line.strip() == '};':
                            if in_array:
                                print(line, file=o)
                                break
                        elif in_array:
                            print(line, file=o)
                else:
                    print("Warning: RLE compression failed for zhRGB565 format", file=sys.stderr)


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

        # insert XIP offset macros
        if binary_tracker.xip_macros:
            print("\n#ifndef XIP_BASE", file=o)
            print("#define XIP_BASE ((uint8_t *)0x00000000)", file=o)
            print("#endif", file=o)
            printed_macros = set()
            for macro_name, offset in binary_tracker.xip_macros:
                if macro_name in printed_macros:
                    continue
                printed_macros.add(macro_name)
                print(f"\n#ifndef IMAGE_{macro_name}_XIP_OFFSET", file=o)
                print(f"#define IMAGE_{macro_name}_XIP_OFFSET 0x{offset:08X}", file=o)
                print(f"#endif", file=o)
            print("", file=o)

        # insert tail
        if args.format == 'gray8' or args.format == 'all':
            macro_name = binary_tracker.make_macro_name(f"{arr_name}_GRAY8")
            print(tailDataGRAY8.format(arr_name, str(row), str(col), macro_name), file=o)

        if args.format == 'rgb565' or args.format == 'all':
            macro_name = binary_tracker.make_macro_name(f"{arr_name}_RGB565")
            print(tailDataRGB565.format(arr_name, str(row), str(col), macro_name), file=o)

        if args.format == 'rgb32' or args.format == 'all':
            if mode == "RGBA":
                macro_name = binary_tracker.make_macro_name(f"{arr_name}_RGBA8888")
                print(tailDataRGBA8888.format(arr_name, str(row), str(col), macro_name), file=o)
                print(tailAlpha2.format(arr_name, str(row), str(col), macro_name), file=o)
            else:
                macro_name = binary_tracker.make_macro_name(f"{arr_name}_RGB888")
                print(tailDataRGB888.format(arr_name, str(row), str(col), macro_name), file=o)

        if args.format == 'zhRGB565' or args.format == 'all':
            macro_name = binary_tracker.make_macro_name(f"{arr_name}_ZHRGB565")
            print(tailDataZHRGB565.format(arr_name, str(row), str(col), macro_name), file=o)

        if args.format == 'lmsk' or args.format == 'all':
            macro_name = binary_tracker.make_macro_name(f"{arr_name}_LMSK")
            print(tailDataLMSK.format(arr_name, str(row), str(col), macro_name), file=o)

        if mode == "RGBA":
            macro_name = binary_tracker.make_macro_name(f"{arr_name}_ALPHA")
            print(tailAlpha.format(arr_name, str(row), str(col), macro_name), file=o)

            if args.a1 or args.format == 'all' or args.format == 'mask':
                macro_name = binary_tracker.make_macro_name(f"{arr_name}_A1ALPHA")
                print(tail1BitAlpha.format(arr_name, str(row), str(col), macro_name), file=o)

            if args.a2 or args.format == 'all' or args.format == 'mask':
                macro_name = binary_tracker.make_macro_name(f"{arr_name}_A2ALPHA")
                print(tail2BitAlpha.format(arr_name, str(row), str(col), macro_name), file=o)

            if args.a4 or args.format == 'all' or args.format == 'mask':
                macro_name = binary_tracker.make_macro_name(f"{arr_name}_A4ALPHA")
                print(tail4BitAlpha.format(arr_name, str(row), str(col), macro_name), file=o)


        print(tail.format(arr_name, str(row), str(col)), file=o)

        binary_tracker.write_header_file(header_file, arr_name.upper())
        binary_tracker.binfile.close()

if __name__ == '__main__':
    main(sys.argv[1:])
