#ifndef _LD_GUI_H_
#define _LD_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"

#include "arm_2d.h"
#include <math.h>
#include <assert.h>
#include "arm_2d_helper.h"
#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d_disp_adapter_0.h"
//#include "ldMemory.h"
#include "xString.h"
#include "xList.h"

void ldGuiInit(void);
void ldGuiLoop(const arm_2d_tile_t *ptParent,bool bIsNewFrame);
    
    
    
#ifdef __cplusplus
}
#endif

#endif //_LD_GUI_H_
