#ifndef __UI_SET_LOGIC_H__
#define __UI_SET_LOGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void uiSetLogicInit(ld_scene_t* ptScene);
void uiSetLogicLoop(ld_scene_t* ptScene);
void uiSetLogicQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif
