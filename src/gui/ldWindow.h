#ifndef _LD_WINDOW_H_
#define _LD_WINDOW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"
#include "ldImage.h"

#define ldWindow_t ldImage_t



ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldWindowDel(ldWindow_t *widget);

void ldWindowSetTransparent(ldImage_t *widget,bool isTransparent);

#define ldWindowSetHidden        ldImageSetHidden
#define ldWindowSetBgColor       ldImageSetBgColor
#define ldWindowSetImage         ldImageSetAddr


#ifdef __cplusplus
}
#endif

#endif //_LD_WINDOW_H_
