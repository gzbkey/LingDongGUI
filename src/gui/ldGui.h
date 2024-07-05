#ifndef __LD_GUI_H__
#define __LD_GUI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldBase.h"
#include "ldWindow.h"
#include "ldImage.h"
#include "ldButton.h"

void ldGuiDraw(arm_2d_scene_t *ptScene,
               const arm_2d_tile_t *ptTile,
               bool bIsNewFrame);


#ifdef __cplusplus
}
#endif

#endif
