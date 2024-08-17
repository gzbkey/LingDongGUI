#ifndef __UI_PRINTING_LOGIC_H__
#define __UI_PRINTING_LOGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void uiPrintingLogicInit(ld_scene_t* ptScene);
void uiPrintingLogicLoop(ld_scene_t* ptScene);
void uiPrintingLogicQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif
