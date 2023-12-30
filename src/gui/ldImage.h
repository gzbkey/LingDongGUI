#ifndef _LD_IMAGE_H_
#define _LD_IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

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
void ldImageFrameUpdate(ldImage_t* pWidget);
void ldImageLoop(ldImage_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldImageDel(ldImage_t *pWidget);

void ldImageSetBgColor(ldImage_t *pWidget,ldColor bgColor);
void ldImageSetOpacity(ldImage_t *pWidget, uint8_t opacity);
void ldImageSetImage(ldImage_t *pWidget, uint32_t imageAddr, bool isWithMask);
void ldImageSetGrayscale(ldImage_t *pWidget, uint8_t grayBit, ldColor writeColor);

#define ldImageSetHidden          ldBaseSetHidden
#define ldImageMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_IMAGE_H_
