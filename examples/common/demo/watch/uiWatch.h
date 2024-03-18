#ifndef _UI_WATCH_H_
#define _UI_WATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"
#include "srcWatch.h"

void uiWatchInit(arm_2d_scene_t *pScene,uint8_t page);
void uiWatchLoop(arm_2d_scene_t *pScene,uint8_t page);
void uiWatchQuit(arm_2d_scene_t *pScene, uint8_t page);

#ifdef __cplusplus
}
#endif

#endif //_UI_WATCH_H_
