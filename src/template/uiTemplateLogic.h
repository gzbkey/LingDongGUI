#ifndef __UI_TEMPLATE_LOGIC_H__
#define __UI_TEMPLATE_LOGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void uiTemplateLogicInit(ld_scene_t* ptScene);
void uiTemplateLogicLoop(ld_scene_t* ptScene);
void uiTemplateLogicQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif
