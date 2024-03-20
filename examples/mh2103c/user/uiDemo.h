#ifndef _UI_DEMO_H_
#define _UI_DEMO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"
#include "image.h"

void uiDemoInit(arm_2d_scene_t *pScene,uint8_t page);
void uiDemoLoop(arm_2d_scene_t *pScene,uint8_t page);
void uiDemoQuit(arm_2d_scene_t *pScene,uint8_t page);

#ifdef __cplusplus
}
#endif

#endif //_UI_DEMO_H_
