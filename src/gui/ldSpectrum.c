/*
 * Copyright (c) 2023-2026 Ou Jianbo (59935554@qq.com). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define __LD_SPECTRUM_IMPLEMENT__

#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "ldSpectrum.h"

#define LD_SPECTRUM_RISE_SPEED     8
#define LD_SPECTRUM_FALL_SPEED     3
#define LD_SPECTRUM_PEAK_FALL_SPEED 1

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wcast-qual"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wunused-const-variable"
#pragma clang diagnostic ignored "-Wmissing-declarations"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

#include "arm_math.h"

#if __GLCD_CFG_COLOUR_DEPTH__ == 16
ARM_SECTION("arm2d.asset.c_bmp_spectrumBeam_png_RGB565")
const uint16_t c_bmp_spectrumBeam_png_RGB565[1*600] = {
    0xfca1, 0xfca1, 0xfca1, 0xfca1, 0xfca1, 0xfca1, 0xfca1, 0xfca1, 0xfca2, 0xfca2, 0xfca2, 0xfca2, 0xfca2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfcc2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfce2, 0xfd02, 0xfd02, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd03, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd23, 0xfd43, 0xfd43, 0xfd43, 0xfd43, 0xfd43, 0xfd43, 0xfd43, 0xfd43, 0xfd43, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd44, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd64, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd84, 0xfd85, 0xfd85, 0xfd85, 0xfd85, 0xfd85, 0xfd85, 0xfd85, 0xfd85, 0xfd85, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfda5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc5, 0xfdc6, 0xfdc6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xfde6, 0xf5e6, 0xf5e6, 0xf5e6, 0xf5e6, 0xf5e6, 0xf5e6, 0xf5e6, 0xede6, 0xede6, 0xede6, 0xede6, 0xede6, 0xede6, 0xede6, 0xe5e6, 0xe5e6, 0xe5e5, 0xe5e5, 0xe5e5, 0xe5e5, 0xe5e5, 0xdde5, 0xdde5, 0xdde5, 0xdde5, 0xdde5, 0xdde5, 0xdde5, 0xd5e5, 0xd5e5, 0xd5e5, 0xd5e5, 0xd5e5, 0xd5e5, 0xd5e5, 0xcde5, 0xcde5, 0xcde5, 0xcdc5, 0xcdc5, 0xcdc5, 0xcdc5, 0xc5c5, 0xc5c5, 0xc5c5, 0xc5c5, 0xc5c5, 0xc5c5, 0xc5c5, 0xbdc5, 0xbdc5, 0xbdc5, 0xbdc5, 0xbdc5, 0xbdc5, 0xbdc5, 0xb5c5, 0xb5c5, 0xb5c5, 0xb5c5, 0xb5c5, 0xb5c5, 0xb5c5, 0xadc5, 0xadc5, 0xadc5, 0xadc5, 0xadc5, 0xadc5, 0xadc5, 0xa5c5, 0xa5c5, 0xa5c5, 0xa5c5, 0xa5c5, 0xa5c5, 0xa5c5, 0x9dc5, 0x9dc5, 0x9dc5, 0x9dc5, 0x9dc5, 0x9dc5, 0x9dc5, 0x95c5, 0x95c5, 0x95c5, 0x95c5, 0x95c5, 0x95c5, 0x8dc5, 0x8dc5, 0x8dc5, 0x8dc5, 0x8dc5, 0x8dc5, 0x8dc5, 0x85c5, 0x85c5, 0x85c5, 0x85c5, 0x85c5, 0x85c5, 0x85c5, 0x7dc5, 0x7dc4, 0x7dc4, 0x7dc4, 0x7dc4, 0x7dc4, 0x7dc4, 0x75c4, 0x75c4, 0x75c4, 0x75c4, 0x75c4, 0x75c4, 0x75c4, 0x6dc4, 0x6dc4, 0x6dc4, 0x6dc4, 0x6dc4, 0x6dc4, 0x6dc4, 0x65c4, 0x65c4, 0x65a4, 0x65a4, 0x65a4, 0x65a4, 0x65a4, 0x5da4, 0x5da4, 0x5da4, 0x5da4, 0x5da4, 0x5da4, 0x5da4, 0x55a4, 0x55a4, 0x55a4, 0x55a4, 0x55a4, 0x55a4, 0x55a4, 0x4da4, 0x4da4, 0x4da4, 0x4da4, 0x4da4, 0x4da4, 0x4da4, 0x45a4, 0x45a4, 0x45a4, 0x45a4, 0x45a4, 0x45a4, 0x45a4, 0x3da4, 0x3da4, 0x3da4, 0x3da4, 0x3da4, 0x3da4, 0x3da4, 0x35a4, 0x35a4, 0x35a4, 0x35a4, 0x35a4, 0x35a4, 0x35a4, 0x2da4, 0x2da4, 0x2da4, 0x2da4, 0x2da4, 0x2da4, 0x2da4, 0x25a4, 0x25a4, 0x25a4, 0x25a4, 0x25a4, 0x25a4, 0x25a4, 0x1da4, 0x1da4, 0x1da4, 0x1da4, 0x1da4, 0x1da4, 0x1da3, 0x15a3, 0x15a3, 0x15a3, 0x15a3, 0x15a3, 0x15a3, 0x15a3, 0x0da3, 0x0da3, 0x0da3, 0x0da3, 0x0da3, 0x0da3, 0x0da3, 0x05a3, 0x05a3, 0x05a3, 0x05a3, 0x05a3, 0x05a3, 0x05a3};
const arm_2d_tile_t c_tile_spectrumBeam_png_RGB565 = {
    .tRegion = {
        .tSize = {
            .iWidth = 1,
            .iHeight = 600,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_RGB565,
        },
    },
    .phwBuffer = (uint16_t*)c_bmp_spectrumBeam_png_RGB565,
};
#else
ARM_SECTION("arm2d.asset.c_bmp_spectrumBeam_png_CCCA8888")
const uint32_t c_bmp_spectrumBeam_png_CCCA8888[1*600] = {
    0xffff960f, 0xffff960f, 0xffff960f, 0xffff960f, 0xffff960f, 0xffff960f, 0xffff960f, 0xffff970f, 0xffff9710, 0xffff9710, 0xffff9710, 0xffff9710, 0xffff9710, 0xffff9810, 0xffff9810, 0xffff9811, 0xffff9811, 0xffff9811, 0xffff9811, 0xffff9911, 0xffff9911, 0xffff9911, 0xffff9912, 0xffff9912, 0xffff9912, 0xffff9912, 0xffff9a12, 0xffff9a12, 0xffff9a12, 0xffff9a13, 0xffff9a13, 0xffff9a13, 0xffff9b13, 0xffff9b13, 0xffff9b13, 0xffff9b13, 0xffff9b14, 0xffff9b14, 0xffff9c14, 0xffff9c14, 0xffff9c14, 0xffff9c14, 0xffff9c14, 0xffff9c14, 0xffff9c15, 0xffff9d15, 0xffff9d15, 0xffff9d15, 0xffff9d15, 0xffff9d15, 0xffff9d15, 0xffff9e16, 0xffff9e16, 0xffff9e16, 0xffff9e16, 0xffff9e16, 0xffff9e16, 0xffff9f16, 0xffff9f17, 0xffff9f17, 0xffff9f17, 0xffff9f17, 0xffff9f17, 0xffffa017, 0xffffa017, 0xffffa018, 0xffffa018, 0xffffa018, 0xffffa018, 0xffffa018, 0xffffa118, 0xffffa118, 0xffffa119, 0xffffa119, 0xffffa119, 0xffffa119, 0xffffa219, 0xffffa219, 0xffffa219, 0xffffa219, 0xffffa21a, 0xffffa21a, 0xffffa31a, 0xffffa31a, 0xffffa31a, 0xffffa31a, 0xffffa31a, 0xffffa31b, 0xffffa31b, 0xffffa41b, 0xffffa41b, 0xffffa41b, 0xffffa41b, 0xffffa41b, 0xffffa41c, 0xffffa51c, 0xffffa51c, 0xffffa51c, 0xffffa51c, 0xffffa51c, 0xffffa51c, 0xffffa61d, 0xffffa61d, 0xffffa61d, 0xffffa61d, 0xffffa61d, 0xffffa61d, 0xffffa61d, 0xffffa71e, 0xffffa71e, 0xffffa71e, 0xffffa71e, 0xffffa71e, 0xffffa71e, 0xffffa81e, 0xffffa81e, 0xffffa81f, 0xffffa81f, 0xffffa81f, 0xffffa81f, 0xffffa91f, 0xffffa91f, 0xffffa91f, 0xffffa920, 0xffffa920, 0xffffa920, 0xffffaa20, 0xffffaa20, 0xffffaa20, 0xffffaa20, 0xffffaa21, 0xffffaa21, 0xffffaa21, 0xffffab21, 0xffffab21, 0xffffab21, 0xffffab21, 0xffffab22, 0xffffab22, 0xffffac22, 0xffffac22, 0xffffac22, 0xffffac22, 0xffffac22, 0xffffac23, 0xffffad23, 0xffffad23, 0xffffad23, 0xffffad23, 0xffffad23, 0xffffad23, 0xffffad23, 0xffffae24, 0xffffae24, 0xffffae24, 0xffffae24, 0xffffae24, 0xffffae24, 0xffffaf24, 0xffffaf25, 0xffffaf25, 0xffffaf25, 0xffffaf25, 0xffffaf25, 0xffffb025, 0xffffb025, 0xffffb026, 0xffffb026, 0xffffb026, 0xffffb026, 0xffffb026, 0xffffb126, 0xffffb126, 0xffffb127, 0xffffb127, 0xffffb127, 0xffffb127, 0xffffb227, 0xffffb227, 0xffffb227, 0xffffb228, 0xffffb228, 0xffffb228, 0xffffb328, 0xffffb328, 0xffffb328, 0xffffb328, 0xffffb328, 0xffffb329, 0xffffb429, 0xffffb429, 0xffffb429, 0xffffb429, 0xffffb429, 0xffffb429, 0xffffb42a, 0xffffb52a, 0xffffb52a, 0xffffb52a, 0xffffb52a, 0xffffb52a, 0xffffb52a, 0xffffb62b, 0xffffb62b, 0xffffb62b, 0xffffb62b, 0xffffb62b, 0xffffb62b, 0xffffb72b, 0xffffb72c, 0xffffb72c, 0xffffb72c, 0xffffb72c, 0xffffb72c, 0xffffb72c, 0xffffb82c, 0xffffb82d, 0xffffb82d, 0xffffb82d, 0xffffb82d, 0xffffb82d, 0xffffb92d, 0xffffb92d, 0xffffb92d, 0xffffb92e, 0xffffb92e, 0xffffb92e, 0xffffba2e, 0xffffba2e, 0xffffba2e, 0xffffba2e, 0xffffba2f, 0xffffba2f, 0xffffba2f, 0xffffbb2f, 0xffffbb2f, 0xffffbb2f, 0xffffbb2f, 0xffffbb30, 0xffffbb30, 0xffffbc30, 0xffffbc30, 0xffffbc30, 0xffffbc30, 0xffffbc30, 0xffffbc31, 0xffffbd31, 0xffffbd31, 0xffffbd31, 0xffffbd31, 0xffffbd31, 0xffffbd31, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xffffbe32, 0xfffebd31, 0xfffdbd31, 0xfffcbd31, 0xfffbbd31, 0xfffabd31, 0xfff9bd31, 0xfff8bd31, 0xfff6bd31, 0xfff5bd31, 0xfff4bd31, 0xfff3bd31, 0xfff2bd31, 0xfff1bd30, 0xfff0bd30, 0xffeebd30, 0xffedbd30, 0xffecbd30, 0xffebbd30, 0xffeabd30, 0xffe9bd30, 0xffe8bd30, 0xffe6bd30, 0xffe5bd30, 0xffe4bc2f, 0xffe3bc2f, 0xffe2bc2f, 0xffe1bc2f, 0xffe0bc2f, 0xffdebc2f, 0xffddbc2f, 0xffdcbc2f, 0xffdbbc2f, 0xffdabc2f, 0xffd9bc2f, 0xffd8bc2e, 0xffd6bc2e, 0xffd5bc2e, 0xffd4bc2e, 0xffd3bc2e, 0xffd2bc2e, 0xffd1bc2e, 0xffd0bc2e, 0xffcebc2e, 0xffcdbc2e, 0xffccbc2e, 0xffcbbb2d, 0xffcabb2d, 0xffc9bb2d, 0xffc8bb2d, 0xffc6bb2d, 0xffc5bb2d, 0xffc4bb2d, 0xffc3bb2d, 0xffc2bb2d, 0xffc1bb2d, 0xffc0bb2d, 0xffbebb2c, 0xffbdbb2c, 0xffbcbb2c, 0xffbbbb2c, 0xffbabb2c, 0xffb9bb2c, 0xffb8bb2c, 0xffb6bb2c, 0xffb5bb2c, 0xffb4bb2c, 0xffb3bb2c, 0xffb2ba2b, 0xffb1ba2b, 0xffb0ba2b, 0xffaeba2b, 0xffadba2b, 0xffacba2b, 0xffabba2b, 0xffaaba2b, 0xffa9ba2b, 0xffa8ba2b, 0xffa6ba2b, 0xffa5ba2a, 0xffa4ba2a, 0xffa3ba2a, 0xffa2ba2a, 0xffa1ba2a, 0xffa0ba2a, 0xff9eba2a, 0xff9dba2a, 0xff9cba2a, 0xff9bba2a, 0xff9aba2a, 0xff99ba2a, 0xff98b929, 0xff96b929, 0xff95b929, 0xff94b929, 0xff93b929, 0xff92b929, 0xff91b929, 0xff8fb929, 0xff8eb929, 0xff8db929, 0xff8cb929, 0xff8bb928, 0xff8ab928, 0xff89b928, 0xff87b928, 0xff86b928, 0xff85b928, 0xff84b928, 0xff83b928, 0xff82b928, 0xff81b928, 0xff7fb928, 0xff7eb827, 0xff7db827, 0xff7cb827, 0xff7bb827, 0xff7ab827, 0xff79b827, 0xff77b827, 0xff76b827, 0xff75b827, 0xff74b827, 0xff73b827, 0xff72b826, 0xff71b826, 0xff6fb826, 0xff6eb826, 0xff6db826, 0xff6cb826, 0xff6bb826, 0xff6ab826, 0xff69b826, 0xff67b826, 0xff66b826, 0xff65b725, 0xff64b725, 0xff63b725, 0xff62b725, 0xff61b725, 0xff5fb725, 0xff5eb725, 0xff5db725, 0xff5cb725, 0xff5bb725, 0xff5ab725, 0xff59b724, 0xff57b724, 0xff56b724, 0xff55b724, 0xff54b724, 0xff53b724, 0xff52b724, 0xff51b724, 0xff4fb724, 0xff4eb724, 0xff4db724, 0xff4cb724, 0xff4bb623, 0xff4ab623, 0xff49b623, 0xff47b623, 0xff46b623, 0xff45b623, 0xff44b623, 0xff43b623, 0xff42b623, 0xff41b623, 0xff3fb623, 0xff3eb622, 0xff3db622, 0xff3cb622, 0xff3bb622, 0xff3ab622, 0xff39b622, 0xff37b622, 0xff36b622, 0xff35b622, 0xff34b622, 0xff33b622, 0xff32b521, 0xff31b521, 0xff2fb521, 0xff2eb521, 0xff2db521, 0xff2cb521, 0xff2bb521, 0xff2ab521, 0xff29b521, 0xff27b521, 0xff26b521, 0xff25b520, 0xff24b520, 0xff23b520, 0xff22b520, 0xff21b520, 0xff1fb520, 0xff1eb520, 0xff1db520, 0xff1cb520, 0xff1bb520, 0xff1ab520, 0xff19b41f, 0xff17b41f, 0xff16b41f, 0xff15b41f, 0xff14b41f, 0xff13b41f, 0xff12b41f, 0xff11b41f, 0xff0fb41f, 0xff0eb41f, 0xff0db41f, 0xff0cb41e, 0xff0bb41e, 0xff0ab41e, 0xff09b41e, 0xff07b41e, 0xff06b41e, 0xff05b41e, 0xff04b41e, 0xff03b41e, 0xff02b41e, 0xff01b41e};
const arm_2d_tile_t c_tile_spectrumBeam_png_CCCA8888 = {
    .tRegion = {
        .tSize = {
            .iWidth = 1,
            .iHeight = 600,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_BGRA8888,
        },
    },
    .pwBuffer = (uint32_t*)c_bmp_spectrumBeam_png_CCCA8888,
};
#endif

const ldBaseWidgetFunc_t ldSpectrumFunc = {
    .depose = (ldDeposeFunc_t)ldSpectrum_depose,
    .load = (ldLoadFunc_t)ldSpectrum_on_load,
    .frameStart = (ldFrameStartFunc_t)ldSpectrum_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldSpectrum_on_frame_complete,
    .show = (ldShowFunc_t)ldSpectrum_show,
};

static void _fft_process(ldSpectrum_t *ptWidget, int16_t *pInputData, uint16_t len)
{
    uint16_t processLen = (len < ptWidget->fftSize) ? len : ptWidget->fftSize;

    int32_t dataSum = 0;
    for (uint16_t i = 0; i < processLen; i++)
    {
        dataSum += (pInputData[i] > 0) ? pInputData[i] : -pInputData[i];
    }

    if (dataSum < processLen)
    {
        memset(ptWidget->pValueList, 0, ptWidget->barCount);
        return;
    }

    for (uint16_t i = 0; i < processLen; i++)
    {
        ptWidget->pFftBuffer[i] = (float)pInputData[i];
    }
    for (uint16_t i = processLen; i < ptWidget->fftSize; i++)
    {
        ptWidget->pFftBuffer[i] = 0.0f;
    }

    arm_rfft_fast_instance_f32 *pInst = (arm_rfft_fast_instance_f32 *)ptWidget->pFftInstance;
    arm_rfft_fast_f32(pInst, ptWidget->pFftBuffer, ptWidget->pFftBuffer, 0);

    uint16_t magLen = ptWidget->fftSize / 2;
    arm_cmplx_mag_f32(ptWidget->pFftBuffer, ptWidget->pMagBuffer, magLen);

    int binsPerBar = magLen / ptWidget->barCount;
    if (binsPerBar < 1) binsPerBar = 1;

    float refMag = (float)ptWidget->fftSize * 32.0f / (float)ptWidget->gain;

    for (uint8_t i = 0; i < ptWidget->barCount; i++)
    {
        float sum = 0.0f;
        int startBin = i * binsPerBar;
        int endBin = startBin + binsPerBar;
        if (endBin > magLen) endBin = magLen;
        if (startBin < 1) startBin = 1;

        for (int j = startBin; j < endBin; j++)
        {
            sum += ptWidget->pMagBuffer[j];
        }

        float avg = sum / (endBin - startBin);
        float normalized = (avg / refMag) * 255.0f;
        if (normalized > 255.0f) normalized = 255.0f;

        ptWidget->pValueList[i] = (uint8_t)normalized;
    }
}

ldSpectrum_t* ldSpectrum_init(ld_scene_t *ptScene, ldSpectrum_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t barCount, uint16_t fftSize)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    uint8_t *pValueList = NULL;
    uint8_t *pDisplayValueList = NULL;
    uint8_t *pPeakValueList = NULL;
    ldSpectrumParticle_t *pParticles = NULL;
    float *pFftBuffer = NULL;
    float *pMagBuffer = NULL;
    arm_rfft_fast_instance_f32 *pFftInst = NULL;
    uint8_t defaultBarWidth = 8;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldSpectrum_t));
        pValueList = ldCalloc(1, barCount);
        pDisplayValueList = ldCalloc(1, barCount);
        pPeakValueList = ldCalloc(1, barCount);
        pParticles = ldCalloc(barCount, sizeof(ldSpectrumParticle_t));
        pFftBuffer = ldCalloc(1, fftSize * sizeof(float));
        pMagBuffer = ldCalloc(1, (fftSize / 2) * sizeof(float));
        pFftInst = ldCalloc(1, sizeof(arm_rfft_fast_instance_f32));

        if ((NULL == ptWidget) || (NULL == pValueList) || (NULL == pDisplayValueList) || 
            (NULL == pPeakValueList) || (NULL == pParticles) || (NULL == pFftBuffer) || 
            (NULL == pMagBuffer) || (NULL == pFftInst))
        {
            ldFree(ptWidget);
            ldFree(pValueList);
            ldFree(pDisplayValueList);
            ldFree(pPeakValueList);
            ldFree(pParticles);
            ldFree(pFftBuffer);
            ldFree(pMagBuffer);
            ldFree(pFftInst);
            LOG_ERROR("[init failed][spectrum] id:%d", nameId);
            return NULL;
        }
        arm_rfft_fast_init_f32(pFftInst, fftSize);
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot, parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeSpectrum;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldSpectrumFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity = 255;

    ptWidget->pValueList = pValueList;
    ptWidget->pDisplayValueList = pDisplayValueList;
    ptWidget->pPeakValueList = pPeakValueList;
    ptWidget->pParticles = pParticles;
    ptWidget->barCount = barCount;
    ptWidget->barWidth = defaultBarWidth;
    ptWidget->barColor = __RGB(255, 165, 0);
    ptWidget->bgColor = 0;
#if __GLCD_CFG_COLOUR_DEPTH__ == 16
    ptWidget->ptBarImgTile = (arm_2d_tile_t *)&c_tile_spectrumBeam_png_RGB565;
#else
    ptWidget->ptBarImgTile = (arm_2d_tile_t *)&c_tile_spectrumBeam_png_CCCA8888;
#endif
    ptWidget->ptBarMaskTile = NULL;
    ptWidget->ptBgImgTile = NULL;
    ptWidget->ptBgMaskTile = NULL;
    ptWidget->gain = 1;
    ptWidget->fftSize = fftSize;
    ptWidget->pFftInstance = pFftInst;
    ptWidget->pFftBuffer = pFftBuffer;
    ptWidget->pMagBuffer = pMagBuffer;

    LOG_INFO("[init][spectrum] id:%d, size:%d", nameId, (int)sizeof(*ptWidget));
    return ptWidget;
}

void ldSpectrum_depose(ld_scene_t *ptScene, ldSpectrum_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if (ptWidget->use_as__ldBase_t.widgetType != widgetTypeSpectrum)
    {
        return;
    }

    LOG_INFO("[depose][spectrum] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t *)ptWidget);

    ldFree(ptWidget->pFftInstance);
    ldFree(ptWidget->pFftBuffer);
    ldFree(ptWidget->pMagBuffer);
    ldFree(ptWidget->pValueList);
    ldFree(ptWidget->pDisplayValueList);
    ldFree(ptWidget->pPeakValueList);
    ldFree(ptWidget->pParticles);
    ldFree(ptWidget);
}

void ldSpectrum_on_load(ld_scene_t *ptScene, ldSpectrum_t *ptWidget)
{
    (void)ptScene;
    (void)ptWidget;
}

void ldSpectrum_on_frame_start(ld_scene_t *ptScene, ldSpectrum_t *ptWidget)
{
    (void)ptScene;
    if (ptWidget == NULL)
    {
        return;
    }

    for (uint8_t i = 0; i < ptWidget->barCount; i++)
    {
        int16_t target = ptWidget->pValueList[i];
        int16_t current = ptWidget->pDisplayValueList[i];

        if (target > current)
        {
            current += LD_SPECTRUM_RISE_SPEED;
            if (current > target)
            {
                current = target;
            }
        }
        else if (target < current)
        {
            if (current > LD_SPECTRUM_FALL_SPEED)
            {
                current -= LD_SPECTRUM_FALL_SPEED;
            }
            else
            {
                current = 0;
            }
        }

        ptWidget->pDisplayValueList[i] = (uint8_t)current;

        int16_t peak = ptWidget->pPeakValueList[i];
        if (current > peak)
        {
            peak = current;
        }
        else if (peak > current)
        {
            if (peak > LD_SPECTRUM_PEAK_FALL_SPEED)
            {
                peak -= LD_SPECTRUM_PEAK_FALL_SPEED;
            }
            else
            {
                peak = 0;
            }
        }
        ptWidget->pPeakValueList[i] = (uint8_t)peak;

        ldSpectrumParticle_t *pParticle = &ptWidget->pParticles[i];
        if (pParticle->active)
        {
            pParticle->offsetY += pParticle->speed;
            if (pParticle->offsetY > 30)
            {
                pParticle->active = 0;
            }
        }

        if (current > peak + 5)
        {
            if (!pParticle->active)
            {
                pParticle->active = 1;
                pParticle->offsetY = -(int16_t)(rand() % 10);
                pParticle->speed = (uint8_t)(1 + (rand() % 3));
            }
        }
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
}

void ldSpectrum_on_frame_complete(ld_scene_t *ptScene, ldSpectrum_t *ptWidget)
{
    (void)ptScene;
    (void)ptWidget;
}

void ldSpectrum_show(ld_scene_t *ptScene, ldSpectrum_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    (void)ptScene;
    (void)bIsNewFrame;

    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    const arm_2d_tile_t *ptBarTile = ptWidget->ptBarImgTile;

    arm_2d_region_t globalRegion;
    arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t *)ptWidget, &globalRegion, true);

    if (arm_2d_helper_pfb_is_region_active(ptTile, &globalRegion, true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if (ldBaseIsHidden((ldBase_t *)ptWidget))
            {
                break;
            }

            int16_t widgetHeight = ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
            int16_t widgetWidth = ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth;

            int16_t totalWidth = ptWidget->barCount * ptWidget->barWidth;
            int16_t barGap = (widgetWidth - totalWidth) / (ptWidget->barCount + 1);
            int16_t startX = barGap;

            if (ptWidget->ptBgImgTile != NULL || ptWidget->ptBgMaskTile != NULL)
            {
                ldBaseImage(&tTarget, NULL, ptWidget->ptBgImgTile, ptWidget->ptBgMaskTile, ptWidget->bgColor, ptWidget->use_as__ldBase_t.opacity);
            }
            else
            {
                ldBaseColor(&tTarget, NULL, ptWidget->bgColor, ptWidget->use_as__ldBase_t.opacity);
            }

            for (uint8_t i = 0; i < ptWidget->barCount; i++)
            {
                uint8_t displayValue = ptWidget->pDisplayValueList[i];
                uint8_t peakValue = ptWidget->pPeakValueList[i];
                uint16_t barHeight = (uint32_t)displayValue * widgetHeight / 255;

                int16_t barX = startX + i * (ptWidget->barWidth + barGap);

                if (barHeight > 0)
                {
                    int16_t barY = widgetHeight - barHeight;

                    int16_t gradientHeight = ptBarTile->tRegion.tSize.iHeight;
                    int16_t srcY = gradientHeight - widgetHeight + barY;
                    if (srcY < 0) srcY = 0;
                    if (srcY + barHeight > gradientHeight)
                    {
                        barHeight = gradientHeight - srcY;
                    }

                    arm_2d_tile_t childTile = impl_child_tile(*ptBarTile, 0, srcY, 1, barHeight);

                    arm_2d_region_t barRegion = {
                        .tLocation = {barX, barY},
                        .tSize = {ptWidget->barWidth, barHeight}
                    };

                    ldBaseImage(&tTarget, &barRegion, &childTile, NULL, 0, ptWidget->use_as__ldBase_t.opacity);
                }

                if (peakValue > displayValue)
                {
                    uint16_t peakHeight = (uint32_t)peakValue * widgetHeight / 255;
                    int16_t peakY = widgetHeight - peakHeight;

                    int16_t gradientHeight = ptBarTile->tRegion.tSize.iHeight;
                    int16_t peakSrcY = gradientHeight - widgetHeight + peakY;
                    if (peakSrcY < 0) peakSrcY = 0;
                    if (peakSrcY >= gradientHeight) peakSrcY = gradientHeight - 1;

                    arm_2d_tile_t peakTile = impl_child_tile(*ptBarTile, 0, peakSrcY, 1, 2);

                    arm_2d_region_t peakRegion = {
                        .tLocation = {barX, peakY},
                        .tSize = {ptWidget->barWidth, 2}
                    };

                    ldBaseImage(&tTarget, &peakRegion, &peakTile, NULL, 0, ptWidget->use_as__ldBase_t.opacity);

                    ldSpectrumParticle_t *pParticle = &ptWidget->pParticles[i];
                    if (pParticle->active)
                    {
                        int16_t particleY = peakY + pParticle->offsetY;

                        if (particleY >= 0 && particleY < widgetHeight)
                        {
                            arm_2d_region_t particleRegion = {
                                .tLocation = {barX, particleY},
                                .tSize = {ptWidget->barWidth, 1}
                            };
                            ldBaseColor(&tTarget, &particleRegion, ptWidget->barColor, ptWidget->use_as__ldBase_t.opacity);
                        }
                    }
                }
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldSpectrumSetData(ldSpectrum_t *ptWidget, int16_t *pAudioData, uint16_t len)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL || pAudioData == NULL || ptWidget->pFftInstance == NULL)
    {
        return;
    }

    _fft_process(ptWidget, pAudioData, len);
}

void ldSpectrumSetBarWidth(ldSpectrum_t *ptWidget, uint8_t barWidth)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->barWidth = barWidth;
}

void ldSpectrumSetBarImage(ldSpectrum_t *ptWidget, arm_2d_tile_t *ptBarImgTile, arm_2d_tile_t *ptBarMaskTile)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->ptBarImgTile = ptBarImgTile;
    ptWidget->ptBarMaskTile = ptBarMaskTile;
}

void ldSpectrumSetBarColor(ldSpectrum_t *ptWidget, ldColor barColor)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->barColor = barColor;
}

void ldSpectrumSetBgColor(ldSpectrum_t *ptWidget, ldColor bgColor)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->bgColor = bgColor;
}

void ldSpectrumSetBgImage(ldSpectrum_t *ptWidget, arm_2d_tile_t *ptBgImgTile, arm_2d_tile_t *ptBgMaskTile)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->ptBgImgTile = ptBgImgTile;
    ptWidget->ptBgMaskTile = ptBgMaskTile;
}

void ldSpectrumSetGain(ldSpectrum_t *ptWidget, uint8_t gain)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    if (gain < 1) gain = 1;
    if (gain > 10) gain = 10;
    ptWidget->gain = gain;
}
