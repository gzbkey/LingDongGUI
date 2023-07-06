#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"
#include "ldImage.h"

#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
#define USE_VIRTUAL_RESOURCE                 1
#else
#define USE_VIRTUAL_RESOURCE                 0
#endif


bool ldCfgTouchGetPoint(int16_t *x,int16_t *y);

#ifdef __cplusplus
}
#endif

#endif //_LD_CONFIG_H_
