#ifndef __UI_PRINT_LOGIC_H__
#define __UI_PRINT_LOGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void uiPrintLogicInit(ld_scene_t* ptScene);
void uiPrintLogicLoop(ld_scene_t* ptScene);
void uiPrintLogicQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif
