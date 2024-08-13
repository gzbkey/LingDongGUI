
/* Generated on Tue Aug 13 16:32:30 2024 from -i */
/* Re-sized : False */
/* Rotated : 0.0 deg */



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



ARM_SECTION("arm2d.asset.c_bmp_ProgressBarBg_bmp_GRAY8")
const uint8_t c_bmp_ProgressBarBg_bmp_GRAY8[1*30] = {
/* -0- */
0x58, 
/* -1- */
0x5d, 
/* -2- */
0x63, 
/* -3- */
0x68, 
/* -4- */
0x6e, 
/* -5- */
0x73, 
/* -6- */
0x79, 
/* -7- */
0x7e, 
/* -8- */
0x84, 
/* -9- */
0x89, 
/* -10- */
0x8f, 
/* -11- */
0x95, 
/* -12- */
0x9a, 
/* -13- */
0xa0, 
/* -14- */
0xa6, 
/* -15- */
0xab, 
/* -16- */
0xb1, 
/* -17- */
0xb6, 
/* -18- */
0xbc, 
/* -19- */
0xc1, 
/* -20- */
0xc7, 
/* -21- */
0xcc, 
/* -22- */
0xd2, 
/* -23- */
0xd7, 
/* -24- */
0xdd, 
/* -25- */
0xe3, 
/* -26- */
0xe9, 
/* -27- */
0xee, 
/* -28- */
0xf4, 
/* -29- */
0xf9, 
};

ARM_SECTION("arm2d.asset.c_bmp_ProgressBarBg_bmp_RGB565")
const uint16_t c_bmp_ProgressBarBg_bmp_RGB565[1*30] = {
/* -0- */
0x0686, 
/* -1- */
0x0e87, 
/* -2- */
0x16a8, 
/* -3- */
0x1ea9, 
/* -4- */
0x26c9, 
/* -5- */
0x2eca, 
/* -6- */
0x36cb, 
/* -7- */
0x3eec, 
/* -8- */
0x46ed, 
/* -9- */
0x4eee, 
/* -10- */
0x570f, 
/* -11- */
0x5f0f, 
/* -12- */
0x6730, 
/* -13- */
0x6f31, 
/* -14- */
0x7732, 
/* -15- */
0x7f53, 
/* -16- */
0x8f54, 
/* -17- */
0x9754, 
/* -18- */
0x9f75, 
/* -19- */
0xa776, 
/* -20- */
0xaf97, 
/* -21- */
0xb798, 
/* -22- */
0xbf99, 
/* -23- */
0xc7b9, 
/* -24- */
0xcfba, 
/* -25- */
0xd7bb, 
/* -26- */
0xdfdc, 
/* -27- */
0xe7dd, 
/* -28- */
0xeffe, 
/* -29- */
0xf7ff, 
};

ARM_SECTION("arm2d.asset.c_bmp_ProgressBarBg_bmp_CCCN888")
const uint32_t c_bmp_ProgressBarBg_bmp_CCCN888[1*30]= {
/* -0- */
0xff00d235, 
/* -1- */
0xff08d33b, 
/* -2- */
0xff11d542, 
/* -3- */
0xff19d649, 
/* -4- */
0xff22d84f, 
/* -5- */
0xff2ad956, 
/* -6- */
0xff33db5d, 
/* -7- */
0xff3bdc64, 
/* -8- */
0xff44de6a, 
/* -9- */
0xff4cdf71, 
/* -10- */
0xff55e178, 
/* -11- */
0xff5de27f, 
/* -12- */
0xff66e485, 
/* -13- */
0xff6ee58c, 
/* -14- */
0xff77e793, 
/* -15- */
0xff7fe89a, 
/* -16- */
0xff88eaa0, 
/* -17- */
0xff90eba7, 
/* -18- */
0xff99edae, 
/* -19- */
0xffa1eeb4, 
/* -20- */
0xffaaf0bb, 
/* -21- */
0xffb2f1c2, 
/* -22- */
0xffbbf3c9, 
/* -23- */
0xffc3f4cf, 
/* -24- */
0xffccf6d6, 
/* -25- */
0xffd4f7dd, 
/* -26- */
0xffddf9e4, 
/* -27- */
0xffe5faea, 
/* -28- */
0xffeefcf1, 
/* -29- */
0xfff6fdf8, 
};


extern const arm_2d_tile_t c_tile_ProgressBarBg_bmp_GRAY8;
ARM_SECTION("arm2d.tile.c_tile_ProgressBarBg_bmp_GRAY8")
const arm_2d_tile_t c_tile_ProgressBarBg_bmp_GRAY8 = {
    .tRegion = {
        .tSize = {
            .iWidth = 1,
            .iHeight = 30,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_GRAY8,
        },
    },
    .pchBuffer = (uint8_t*)c_bmp_ProgressBarBg_bmp_GRAY8,
};




extern const arm_2d_tile_t c_tile_ProgressBarBg_bmp_RGB565;
ARM_SECTION("arm2d.tile.c_tile_ProgressBarBg_bmp_RGB565")
const arm_2d_tile_t c_tile_ProgressBarBg_bmp_RGB565 = {
    .tRegion = {
        .tSize = {
            .iWidth = 1,
            .iHeight = 30,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_RGB565,
        },
    },
    .phwBuffer = (uint16_t*)c_bmp_ProgressBarBg_bmp_RGB565,
};




extern const arm_2d_tile_t c_tile_ProgressBarBg_bmp_CCCN888;

ARM_SECTION("arm2d.tile.c_tile_ProgressBarBg_bmp_CCCN888")
const arm_2d_tile_t c_tile_ProgressBarBg_bmp_CCCN888 = {
    .tRegion = {
        .tSize = {
            .iWidth = 1,
            .iHeight = 30,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_RGB888,
        },
    },
    .pwBuffer = (uint32_t*)c_bmp_ProgressBarBg_bmp_CCCN888,
};




#if defined(__clang__)
#   pragma clang diagnostic pop
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_warning=1296
#endif


