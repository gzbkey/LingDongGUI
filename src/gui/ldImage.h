#ifndef _LDIMAGE_H_
#define _LDIMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"
#include "ldConfig.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isWithMask:1;
    bool isTransparent:1;//window专用
    bool isColor:1;
    ldColor bgColor;
    ldColor specialColor;//maskColor charColor
#if USE_OPACITY == 1
    uint8_t opacity;
#endif
}ldImage_t;

ldImage_t* ldImageInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,uint32_t imageAddr,bool isWithMask);
void ldImageLoop(ldImage_t *widget,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldImageDel(ldImage_t *widget);

void ldImageSetBgColor(ldImage_t *widget,ldColor bgColor);
void ldImageSetHidden(ldImage_t *widget,bool isHidden);
void ldImageSetOpacity(ldImage_t *widget, uint8_t opacity);
void ldImageSetImage(ldImage_t *widget, uint32_t imageAddr, bool isWithMask);
void ldImageSetGrayscale(ldImage_t *widget, uint8_t grayBit, ldColor writeColor);



#ifdef __cplusplus
}
#endif

#endif //_LDIMAGE_H_
