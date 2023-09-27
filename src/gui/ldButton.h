#ifndef _LD_BUTTON_H_
#define _LD_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

#define SELECT_COLOR_OPACITY     128

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
    ldChar_t *ptTextInfo;
    ldColor releaseColor;
    ldColor pressColor;
    ldColor selectColor;
}ldButton_t;

ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldButtonLoop(ldButton_t *pWidget,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldButtonDel(ldButton_t *pWidget);

void ldButtonSetColor(ldButton_t* pWidget, ldColor releaseColor, ldColor pressColor);
void ldButtonSetImage(ldButton_t* pWidget,uint32_t releaseImgAddr,bool isReleaseMask,uint32_t pressImgAddr,bool isPressMask);
void ldButtonSetSelectImage(ldButton_t* pWidget,uint32_t selectMaskAddr,ldColor selectColor);

void ldButtonSetTransparent(ldButton_t* pWidget,bool isTransparent);
void ldButtonSetRoundCorner(ldButton_t* pWidget,bool isCorner);
void ldButtonSetSelect(ldButton_t* pWidget,bool isSelected);

void ldButtonSetFont(ldButton_t *pWidget, ldFontDict_t *pFontDict);
void ldButtonSetText(ldButton_t* pWidget,uint8_t *pStr);
void ldButtonSetTextColor(ldButton_t* pWidget,ldColor charColor);
void ldButtonSetAlign(ldButton_t *pWidget,uint8_t align);

#define ldButtonSetHidden          ldBaseSetHidden
#define ldButtonMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_BUTTON_H_
