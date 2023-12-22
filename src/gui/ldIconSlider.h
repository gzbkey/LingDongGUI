#ifndef _LD_ICONSLIDER_H_
#define _LD_ICONSLIDER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    uint32_t imgAddr;
    uint8_t* pName;
}ldIconInfo_t;

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isCorner:1;
    bool isHorizontalScroll:1;
    bool hasHorizontalBorder:1;
    bool hasVerticalBorder:1;
    bool isWaitMove:1;
    bool isAutoMove:1;
    bool isHoldMove:1;
    ldFontDict_t* pFontDict;
    ldIconInfo_t* pIconInfoList;
    uint8_t rowCount:4;
    uint8_t columnCount:4;
    uint8_t iconSpace;//icon 间隔
    int16_t iconWidth;
    uint16_t iconMax;
    uint16_t iconCount;
    int16_t scrollOffset;
    uint8_t pageMax;
    uint8_t selectIconOrPage;
    uint8_t moveOffset;
ldColor charColor;

}ldIconSlider_t;

ldIconSlider_t* ldIconSliderInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, int16_t iconWidth, uint8_t iconSpace, uint8_t columnCount, uint8_t rowCount, uint8_t pageMax, ldFontDict_t* pFontDict);
void ldIconSliderFrameStart(ldIconSlider_t* pWidget);
void ldIconSliderLoop(ldIconSlider_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldIconSliderDel(ldIconSlider_t *pWidget);
void ldIconSliderAddIcon(ldIconSlider_t *pWidget, uint32_t imageAddr, uint8_t *pNameStr);
void ldIconSliderSetHorizontalScroll(ldIconSlider_t *pWidget,bool isHorizontal);
void ldIconSliderSetSpeed(ldIconSlider_t *pWidget, uint8_t speed);

#define ldIconSliderSetHidden          ldBaseSetHidden
#define ldIconSliderMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_ICONSLIDER_H_
