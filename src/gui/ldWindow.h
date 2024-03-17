#ifndef _LD_WINDOW_H_
#define _LD_WINDOW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "ldImage.h"

#define ldWindow_t ldImage_t



ldWindow_t* ldWindowInit(arm_2d_scene_t *pScene,uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldWindowDel(ldWindow_t *pWidget);

void ldWindowSetTransparent(ldWindow_t *pWidget,bool isTransparent);

#define ldWindowSetHidden        ldBaseSetHidden
#define ldWindowSetBgColor       ldImageSetBgColor
#define ldWindowSetImage         ldImageSetImage
#define ldWindowMove             ldBaseMove


#ifdef __cplusplus
}
#endif

#endif //_LD_WINDOW_H_
