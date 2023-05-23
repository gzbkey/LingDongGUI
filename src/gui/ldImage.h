#ifndef _LDIMAGE_H_
#define _LDIMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"


typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isInitEnd:1;
//    bool isMaskColor:1;
    bool isPng:1;
    bool isHwDec:1;
    bool isTransparent:1;//window专用
    ldColor bgColor;
    
    //定时切换用
    uint32_t _timeUnit;
    int64_t _lastTimeLog;
    uint16_t speedMs;
    
    
#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
    arm_2d_vres_t resource;
#else
    arm_2d_tile_t resource;
#endif
    
    
    ldColor specialColor;//maskColor charColor
    
    uint32_t* imgList;
    uint16_t imgMax;
    uint16_t imgCount;
    uint16_t imgNow;
    uint8_t textAlpha;
}ldImage;

ldImage* ldImageInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,ldColor bgColor,uint32_t imageAddr,uint16_t maxImageNum,bool isPng,bool isHwDec,bool isHidden);
void ldImageLoop(ldImage *info,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void pImageDel(ldImage *widget);

void pImageSetAddr(ldImage *info,uint32_t addr);
void nImageSetAddr(uint16_t nameId,uint32_t addr);
bool pImageAddList(ldImage *info,uint32_t addr,uint16_t speedMs);
bool nImageAddList(uint16_t nameId,uint32_t addr,uint16_t speedMs);

void pImageSetColorType(ldImage *info,uint8_t colorType,ldColor writeColor);
void nImageSetColorType(uint16_t nameId,uint8_t colorType,ldColor writeColor);



#ifdef __cplusplus
}
#endif

#endif //_LDIMAGE_H_
