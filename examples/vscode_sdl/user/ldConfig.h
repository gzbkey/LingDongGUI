#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

#define USE_VIRTUAL_RESOURCE                      1

#define LD_CFG_SCEEN_WIDTH                        240
#define LD_CFG_SCEEN_HEIGHT                       320
#define LD_CFG_COLOR_DEPTH                        16

#define USE_OPACITY                               0
#define USE_TLSF                                  1

#define USE_RADIA_MENU_SCALE                      1

#define LD_MEM_SIZE                               (32*1024) //BYTE

#define LD_DEBUG                                  1

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