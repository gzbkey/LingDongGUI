#ifndef __UI_WAIT_LOGIC_H__
#define __UI_WAIT_LOGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void uiWaitLogicInit(ld_scene_t* ptScene);
void uiWaitLogicLoop(ld_scene_t* ptScene);
void uiWaitLogicQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif
