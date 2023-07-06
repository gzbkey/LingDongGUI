#ifndef _LD_WINDOW_H_
#define _LD_WINDOW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"
#include "ldImage.h"

#define ldWindow_t ldImage_t


ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,ldColor bgColor,uint32_t imageAddr,uint16_t maxImageNum,bool isPng,bool isTransparent,bool isHidden);
//void ldWindowLoop(ldWindow *info,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
//void nImageDelete(uint16_t nameId);
void pWindowDel(ldWindow_t *widget);

//void pImageSetAddr(ldImage *info,uint32_t addr);
//void nImageSetAddr(uint16_t nameId,uint32_t addr);
//bool pImageAddList(ldImage *info,uint32_t addr,uint16_t speedMs);
//bool nImageAddList(uint16_t nameId,uint32_t addr,uint16_t speedMs);

//void pImageSetColorType(ldImage *info,uint8_t colorType,ldColor writeColor);
//void nImageSetColorType(uint16_t nameId,uint8_t colorType,ldColor writeColor);



#ifdef __cplusplus
}
#endif

#endif //_LD_WINDOW_H_
