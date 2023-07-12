#ifndef _LDIMAGE_H_
#define _LDIMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"

#define USE_IMAGE_OPACITY        0

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isWithMask:1;
    bool isTransparent:1;//window专用
    bool isColor:1;
    ldColor bgColor;
    ldColor specialColor;//maskColor charColor
#if USE_IMAGE_OPACITY == 1
    uint8_t opacity;
#endif
}ldImage_t;

ldImage_t* ldImageInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,bool isColor,ldColor bgColor,uint32_t imageAddr,bool isWithMask,bool isHidden);
void ldImageLoop(ldImage_t *info,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void pImageDel(ldImage_t *widget);

void pImageSetOpacity(ldImage_t *info, uint8_t opacity);
void nImageSetOpacity(uint16_t nameId, uint8_t opacity);

void pImageSetAddr(ldImage_t *info,uint32_t addr);
void nImageSetAddr(uint16_t nameId,uint32_t addr);

void pImageSetGrayscale(ldImage_t *info, uint8_t grayBit, ldColor writeColor);
void nImageSetGrayscale(uint16_t nameId, uint8_t grayBit, ldColor writeColor);



#ifdef __cplusplus
}
#endif

#endif //_LDIMAGE_H_
