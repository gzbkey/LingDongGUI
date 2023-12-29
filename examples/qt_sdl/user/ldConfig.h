#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

// base config
#define LD_CFG_COLOR_DEPTH                        16   // 8 16 32
#define LD_CFG_SCEEN_WIDTH                        500
#define LD_CFG_SCEEN_HEIGHT                       500
#define LD_CFG_PFB_WIDTH                          LD_CFG_SCEEN_WIDTH
#define LD_CFG_PFB_HEIGHT                         10
#define LD_MEM_SIZE                               (32*1024) //BYTE

#define USE_DIRTY_REGION                          1
#define USE_VIRTUAL_RESOURCE                      0
#define USE_OPACITY                               0
#define USE_TLSF                                  1
#define USE_RADIA_MENU_SCALE                      1

//debug config
#define LD_DEBUG                                  0
#define __DISP0_CFG_DEBUG_DIRTY_REGIONS__         1

// 以下不用修改
#define __GLCD_CFG_COLOUR_DEPTH__                 LD_CFG_COLOR_DEPTH
#define __DISP0_CFG_DISABLE_NAVIGATION_LAYER__    1
#define __DISP0_CFG_PFB_BLOCK_WIDTH__             LD_CFG_PFB_WIDTH
#define __DISP0_CFG_PFB_BLOCK_HEIGHT__            LD_CFG_PFB_HEIGHT
#define __DISP0_CFG_COLOUR_DEPTH__                LD_CFG_COLOR_DEPTH
#define __DISP0_CFG_SCEEN_WIDTH__                 LD_CFG_SCEEN_WIDTH
#define __DISP0_CFG_SCEEN_HEIGHT__                LD_CFG_SCEEN_HEIGHT
#define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__     USE_VIRTUAL_RESOURCE

#if __GLCD_CFG_COLOUR_DEPTH__ != LD_CFG_COLOR_DEPTH
#error parameter configuration error. (arm_2d_cfg.h) __GLCD_CFG_COLOUR_DEPTH__ not equal to LD_CFG_COLOR_DEPTH
#endif

bool ldCfgTouchGetPoint(int16_t *x,int16_t *y);

// user key num
#define KEY_NUM_UP   0
#define KEY_NUM_DOWN   1
#define KEY_NUM_LEFT   2
#define KEY_NUM_RIGHT   3
#define KEY_NUM_ENTER   4
#define KEY_NUM_ESC   5



#ifdef __cplusplus
}
#endif

#endif //_LD_CONFIG_H_
