#ifndef _LD_BUTTON_H_
#define _LD_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"
#include "ldConfig.h"



typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isTransparent:1;
    bool isCheckable:1;
    bool isChecked:1;
    bool isPressed:1;
    bool isSelected:1;
    bool isReleaseMask:1;
    bool isPressMask:1;
    bool isCorner:1;
    uint32_t releaseImgAddr;
    uint32_t pressImgAddr;
    uint32_t selectMaskAddr;
    uint32_t keyValue;
    ldColor releaseColor;
    ldColor pressColor;
    ldColor selectColor;
    ldChar_t *ptTextInfo;
}ldButton_t;

ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldButtonLoop(ldButton_t *widget,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldButtonDel(ldButton_t *widget);

void ldButtonSetColor(ldButton_t* widget, ldColor releaseColor, ldColor pressColor);
void ldButtonSetImage(ldButton_t* widget,uint32_t releaseImgAddr,bool isReleaseMask,uint32_t pressImgAddr,bool isPressMask);
void ldButtonSetSelectImage(ldButton_t* widget,uint32_t selectMaskAddr,ldColor selectColor);

void ldButtonSetTransparent(ldButton_t* widget,bool isTransparent);
void ldButtonSetHidden(ldButton_t* widget,bool isHidden);
void ldButtonSetRoundCorner(ldButton_t* widget,bool isCorner);
void ldButtonSetSelect(ldButton_t* widget,bool isSelected);

void ldButtonSetFont(ldButton_t *widget, uint8_t maskType, uint32_t fontDictAddr, uint32_t fontSrcAddr, uint16_t lineOffset, int16_t descender);
void ldButtonSetText(ldButton_t* widget,uint8_t *pStr);
void ldButtonSetTextColor(ldButton_t* widget,ldColor charColor);
void ldButtonSetAlign(ldButton_t *widget,uint8_t align);
#ifdef __cplusplus
}
#endif

#endif //_LD_BUTTON_H_
