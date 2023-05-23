#ifndef _LDCOMMON_H_
#define _LDCOMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"

#include "arm_2d.h"
#include <math.h>
#include <assert.h>
#include "arm_2d_helper.h"
#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d_disp_adapter_0.h"
//#include "ldMemory.h"
#include "xString.h"
#include "xList.h"

#ifndef SET_BITS
#define   SET_BITS(data,value)          ((data)|=(value))
#endif
#ifndef CLR_BITS
#define   CLR_BITS(data,value)          ((data)&=~(value))
#endif
#ifndef SETBIT
#define   SETBIT(data,move)          ((data)|=(1<<(move)))
#endif
#ifndef CLRBIT
#define   CLRBIT(data,move)          ((data)&=~(1<<(move)))
#endif
#ifndef GETBIT
#define   GETBIT(data,move)          (((data)>>(move))&0x01)
#endif
#ifndef PUTBIT
#define   PUTBIT(data,value,move)    ((value)?SETBIT(data,move):CLRBIT(data,move))
#endif
#ifndef GET16H
#define   GET16H(data)              (((data)>>8)&0xFF)
#endif
#ifndef GET16L
#define   GET16L(data)              ((data)&0xFF)
#endif
#ifndef CONNECT16
#define   CONNECT16(H,L)            (((H)<<8)+(L))
#endif
#ifndef GET32H
#define   GET32H(data)              (((data)>>24)&0xFF)
#endif
#ifndef GET32MH
#define   GET32MH(data)              (((data)>>16)&0xFF)
#endif
#ifndef GET32ML
#define   GET32ML(data)              (((data)>>8)&0xFF)
#endif
#ifndef GET32L
#define   GET32L(data)              ((data)&0xFF)
#endif
#ifndef CONNECT32
#define   CONNECT32(H,MH,ML,L)           (((H)<<24)+((MH)<<16)+((ML)<<8)+(L))
#endif
#ifndef SWAP
#define SWAP(x,y) ((x) = (x)+(y),(y)=(x)-(y),(x)=(x)-(y))
#endif

typedef enum{
    widgetTypeNone,
    widgetTypeBackground,
    widgetTypeWindow,
    widgetTypeButton,
    widgetTypeImage,
    widgetTypeText,
    widgetTypeLineEdit,
    widgetTypeDashboardSimple,
    widgetTypeGraph,
    widgetTypeCheckBox,
    widgetTypeSlider,
    widgetTypeProgressBar,
    widgetTypeGauge,
    widgetTypeQRCode,
    widgetTypeDateTime,
    widgetTypeIconSlider,
    widgetTypeComboBox,
    widgetTypeIconRotateAround,
    widgetTypeNumber,
    widgetTypeStaticText,
    widgetTypeStartupImage,
    widgetTypeArc,
}ldWidgetType;

#define LD_IMPL_VERS(__COLOUR_FORMAT, __WIDTH, __HEIGHT,__IMG_ADDR)             \
    .tTile = {                                                                  \
        .tRegion = {                                                            \
            .tSize = {                                                          \
                .iWidth = (__WIDTH),                                            \
                .iHeight =(__HEIGHT),                                           \
            },                                                                  \
        },                                                                      \
        .tInfo = {                                                              \
            .bIsRoot = true,                                                    \
            .bHasEnforcedColour = true,                                         \
            .bVirtualResource = true,                                           \
            .tColourInfo = {                                                    \
                .chScheme = (__COLOUR_FORMAT),                                  \
            },                                                                  \
        },                                                                      \
    },                                                                          \
    .Load       = &__disp_adapter0_vres_asset_loader,                  \
    .Depose     = &__disp_adapter0_vres_buffer_deposer,                \
    .pTarget    = __IMG_ADDR,

    
#define LD_IMPL_TILE(__COLOUR_FORMAT, __WIDTH, __HEIGHT,__IMG_ADDR)             \
    .tRegion = {                                                                \
        .tSize = {                                                              \
            .iWidth = (__WIDTH),                                                \
            .iHeight =(__HEIGHT),                                               \
        },                                            \
    },                                            \
    .tInfo = {                                            \
        .bIsRoot = true,                                            \
        .bHasEnforcedColour = true,                                            \
        .tColourInfo = {                                            \
            .chScheme = (__COLOUR_FORMAT),                                            \
        },                                            \
    },                                            \
    .pchBuffer = (uint8_t *)__IMG_ADDR,

    
#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
#define RES_TILE(src)                          src.tTile
#define RES_IMG_ADDR(src)                      src.pTarget
#else
#define RES_TILE(src)                          src
#define RES_IMG_ADDR(src)                      src.pchBuffer
#endif
    
    
#define LD_COMMON_ATTRIBUTES  ldWidgetType widgetType; \
                             ldGeometry geometry; \
                             uint16_t nameId; \
                              ldWidgetType parentType; \
                             void * parentWidget; \
                             xListNode *childList; \
                             bool isHidden:1

                             
                             
typedef struct{
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
}ldGeometry;


typedef struct{
    LD_COMMON_ATTRIBUTES;
}ldCommon;

typedef struct{
    int16_t x;
    int16_t y;
}ldPoint;

typedef struct{
    int16_t width;
    int16_t height;
    uint32_t addr;
    uint8_t ascii;
}ldNumImgInfo;

typedef struct{
    int16_t width;
    int16_t height;
    uint32_t addr;
    uint8_t ascii;
}ldDateTimeImgInfo;

typedef enum
{
    hLeft,
    hCenter,
    hRight
}ldHorizontalAlign;

typedef enum
{
    vTop,
    vCenter,
    vBottom
}ldVerticalAlign;

#define ldColor    COLOUR_INT

extern uint16_t cfgMonitorWidth;
extern uint16_t cfgMonitorHeight;

extern volatile uint16_t gYear;
extern volatile uint8_t gMonth,gDay,gHour,gMinute,gSecond,gWeek;

void ldCommonAutoLoop(ldCommon* pWidget,arm_2d_tile_t *ptTile,bool bIsNewFrame);

void ldCommonWidgetHidRefresh(uint8_t *hidBuf);




void ldCommonCmdRefreshNormal(uint8_t *dat);
void ldCommonCmdRefreshFixed(uint8_t *dat);
























extern xListNode ldWidgetLink;


void *ldMalloc(uint32_t size);

void ldFree(void *p);

void *ldRealloc(void *ptr,uint32_t newSize);

#define XMALLOC     ldMalloc
#define XFREE       ldFree

#define LD_MALLOC_WIDGET_INFO(widgetTypedef)      (widgetTypedef*)ldMalloc(sizeof(widgetTypedef))

#define LD_MALLOC_STRING(str)      (uint8_t *)ldMalloc((strlen((const char *)str)+1)*sizeof(uint8_t))


ldPoint ldGetGlobalPos(ldCommon *widget);

xListNode* ldGetWidgetInfoById(uint16_t nameId);
xListNode* ldGetWidgetInfoByPos(int16_t x,int16_t y);

bool ldTimeOut(uint16_t ms, int64_t *plTimer,bool isReset);
void ldDelWidget(ldCommon *widget);

#ifdef __cplusplus
}
#endif

#endif //_LDCOMMON_H_
