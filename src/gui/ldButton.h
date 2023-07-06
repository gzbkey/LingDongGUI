#ifndef _LD_BUTTON_H_
#define _LD_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"

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
    arm_2d_tile_t tReleaseImgRes;
#if USE_VIRTUAL_RESOURCE == 1
    uintptr_t reserve1;
    void      *reserve2;
    void      *reserve3;
#endif
    arm_2d_tile_t tPressImgRes;
#if USE_VIRTUAL_RESOURCE == 1
    uintptr_t reserve4;
    void      *reserve5;
    void      *reserve6;
#endif
    arm_2d_tile_t tSelectMaskRes;
#if USE_VIRTUAL_RESOURCE == 1
    uintptr_t reserve4;
    void      *reserve5;
    void      *reserve6;
#endif
    
    ldChar_t *ptTextInfo;
    ldColor releaseColor;
    ldColor pressColor;
    ldColor selectColor;
    ldColor charColor;
    uint32_t keyValue;
}ldButton_t;

ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,ldColor releaseColor,ldColor pressColor,ldColor selectColor,uint32_t releaseImgAddr,uint32_t pressImgAddr,uint32_t selectMaskAddr,ldColor charColor,bool isWithMask,bool isTransparent,bool isHidden);

//ldWindow* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,ldColor bgColor,uint32_t imageAddr,uint16_t maxImageNum,bool isPng,bool isTransparent,bool isHidden);
void ldButtonLoop(ldButton_t *info,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
////void nImageDelete(uint16_t nameId);
//void pWindowDel(ldWindow *widget);

//void pImageSetAddr(ldImage *info,uint32_t addr);
//void nImageSetAddr(uint16_t nameId,uint32_t addr);
//bool pImageAddList(ldImage *info,uint32_t addr,uint16_t speedMs);
//bool nImageAddList(uint16_t nameId,uint32_t addr,uint16_t speedMs);

//void pImageSetColorType(ldImage *info,uint8_t colorType,ldColor writeColor);
//void nImageSetColorType(uint16_t nameId,uint8_t colorType,ldColor writeColor);



#ifdef __cplusplus
}
#endif

#endif //_LD_BUTTON_H_
