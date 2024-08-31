#ifndef __UI_WIDGET_LOGIC_H__
#define __UI_WIDGET_LOGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void uiWidgetLogicInit(ld_scene_t* ptScene);
void uiWidgetLogicLoop(ld_scene_t* ptScene);
void uiWidgetLogicQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif