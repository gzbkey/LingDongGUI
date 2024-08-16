#ifndef __UI_LOGO_LOGIC_H__
#define __UI_LOGO_LOGIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

void uiLogoLogicInit(ld_scene_t* ptScene);
void uiLogoLogicLoop(ld_scene_t* ptScene);
void uiLogoLogicQuit(ld_scene_t* ptScene);

#if __cplusplus
}
#endif

#endif
