#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
#define USE_VIRTUAL_RESOURCE                      1
#else
#define USE_VIRTUAL_RESOURCE                      0
#endif

#define LD_CFG_SCEEN_WIDTH                        480
#define LD_CFG_SCEEN_HEIGHT                       272
#define LD_CFG_COLOR_DEPTH                        16

bool ldCfgTouchGetPoint(int16_t *x,int16_t *y);

#ifdef __cplusplus
}
#endif

#endif //_LD_CONFIG_H_
