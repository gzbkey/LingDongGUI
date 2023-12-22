#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "arm_2d_cfg.h"
#include "st7789v16bit.h"

#define LD_CFG_SCEEN_WIDTH                        ST7789V_LCD_WIDTH
#define LD_CFG_SCEEN_HEIGHT                       ST7789V_LCD_HEIGHT
#define LD_CFG_COLOR_DEPTH                        16   // 8 16 32

#define USE_VIRTUAL_RESOURCE                      0

#define USE_DIRTY_REGION                          1

#define USE_OPACITY                               0

#define USE_TLSF                                  1

#define USE_RADIA_MENU_SCALE                      1

#define LD_MEM_SIZE                               (16*1024) //BYTE

#define LD_DEBUG                                  0


// arm-2d config

// PFB尺寸
#define __DISP0_CFG_PFB_BLOCK_WIDTH__             LD_CFG_SCEEN_WIDTH
#define __DISP0_CFG_PFB_BLOCK_HEIGHT__            16
#define __DISP0_CFG_DEBUG_DIRTY_REGIONS__         0

// 以下不用修改
#define __DISP0_CFG_COLOUR_DEPTH__                LD_CFG_COLOR_DEPTH
#define __DISP0_CFG_SCEEN_WIDTH__                 LD_CFG_SCEEN_WIDTH
#define __DISP0_CFG_SCEEN_HEIGHT__                LD_CFG_SCEEN_HEIGHT
#define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__     USE_VIRTUAL_RESOURCE

#if __GLCD_CFG_COLOUR_DEPTH__ != LD_CFG_COLOR_DEPTH
#error parameter configuration error. (arm_2d_cfg.h) __GLCD_CFG_COLOUR_DEPTH__ not equal to LD_CFG_COLOR_DEPTH
#endif

bool ldCfgTouchGetPoint(int16_t *x,int16_t *y);












#ifdef __cplusplus
}
#endif

#endif //_LD_CONFIG_H_
