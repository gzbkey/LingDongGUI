#ifndef _UI_WIDGET_H_
#define _UI_WIDGET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"
#include "srcWidget.h"

void uiWidgetInit(arm_2d_scene_t *pScene,uint8_t page);
void uiWidgetLoop(arm_2d_scene_t *pScene,uint8_t page);
void uiWidgetQuit(arm_2d_scene_t *pScene, uint8_t page);

#ifdef __cplusplus
}
#endif

#endif //_UI_WIDGET_H_
