#ifndef _LD_BUTTON_H_
#define _LD_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"
#include "ldConfig.h"

typedef struct{
    arm_2d_tile_t *ptRes;
    arm_2d_font_t *ptFont;
    uint8_t* pStr;
}ldChar_t;

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isWithMask:1;
    bool isTransparent:1;
    bool isCheckable:1;
    bool isChecked:1;
    bool isPressed:1;
    bool isSelected:1;
    uint32_t releaseImgAddr;
    uint32_t pressImgAddr;
    uint32_t selectMaskAddr;
    ldChar_t *ptTextInfo;
    ldColor releaseColor;
    ldColor pressColor;
    ldColor selectColor;
    ldColor charColor;
    uint32_t keyValue;
}ldButton_t;

ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldButtonLoop(ldButton_t *widget,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldButtonDel(ldButton_t *widget);

void ldButtonSetColor(ldButton_t* widget, ldColor releaseColor, ldColor pressColor);
void ldButtonSetImage(ldButton_t* widget,uint32_t releaseImgAddr,uint32_t pressImgAddr,bool isWithMask);
void ldButtonSetSelectImage(ldButton_t* widget,uint32_t selectMaskAddr,ldColor selectColor);
void ldButtonSetCharColor(ldButton_t* widget,ldColor charColor);
void ldButtonSetTransparent(ldButton_t* widget,bool isTransparent);
void ldButtonSetHidden(ldButton_t* widget,bool isHidden);

#ifdef __cplusplus
}
#endif

#endif //_LD_BUTTON_H_
