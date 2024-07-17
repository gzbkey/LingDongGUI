#ifndef __UI_PAGE_LOGIC_TEMPLATE_H__
#define __UI_PAGE_LOGIC_TEMPLATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void uiPageLogicTemplateInit(ld_scene_t* ptScene);
void uiPageLogicTemplateLoop(ld_scene_t* ptScene);
void uiPageLogicTemplateQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif
