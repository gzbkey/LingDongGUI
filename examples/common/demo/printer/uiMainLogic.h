#ifndef __UI_MAIN_LOGIC_H__
#define __UI_MAIN_LOGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void uiMainLogicInit(ld_scene_t* ptScene);
void uiMainLogicLoop(ld_scene_t* ptScene);
void uiMainLogicQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif
