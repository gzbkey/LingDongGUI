#ifndef __LD_BASE_H__
#define __LD_BASE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "arm_extra_controls.h"
#include "arm_2d_helper_control.h"
#include "xLog.h"

#include "ldConfig.h"

typedef enum{
widgetTypeBackground,
widgetTypeWindow,
widgetTypeButton,
widgetTypeImage,
widgetTypeText,
widgetTypeLineEdit,
widgetTypeGraph,
widgetTypeCheckBox,
widgetTypeSlider,
widgetTypeProgressBar,
widgetTypeGauge,
widgetTypeQRCode,
widgetTypeDateTime,
widgetTypeIconSlider,
widgetTypeComboBox,
widgetTypeArc,
widgetTypeRadialMenu,
widgetTypeScrollSelecter,
widgetTypeLabel,
widgetTypeTable,
widgetTypeKeyboard,
}ldWidgetType_t;


#define ldColor                                 COLOUR_INT

#define CURSOR_WIDTH                            2

#define MEM_MODE_FREERTOS_HEAP4                   (0)
#define MEM_MODE_TLFS                             (1)
#define MEM_MODE_STDLIB                           (2)
#define MEM_MODE_USER                             (3)


//#define LD_BASE                         ARM_PROTECTED( \
//                                            arm_2d_control_node_t *ptNext; \
//                                            arm_2d_control_node_t *ptParent; \
//                                            arm_2d_control_node_t *ptChildList; \
//                                        ) \
//                                        arm_2d_region_t tRegion; \
//                                        ldWidgetType_t widgetType; \
//                                        uint16_t nameId


#define LD_CHK_PTR_RET(ptr,retValue)             if ((ptr) == NULL) { return retValue; }


extern arm_2d_control_node_t *ldWidgetNode;


extern void *ldMalloc(uint32_t size);
extern void *ldCalloc(uint32_t num,uint32_t size);
extern void ldFree(void *p);
extern void *ldRealloc(void *ptr,uint32_t newSize);

bool ldTimeOut(uint16_t ms, int64_t *pTimer,bool isReset);

void ldNodeAdd(arm_2d_control_node_t *parent, arm_2d_control_node_t *child);

void ldBaseColor(arm_2d_tile_t* pTile,arm_2d_region_t* pRegion, ldColor color,uint8_t opacity);
void ldBaseImage(arm_2d_tile_t* pTile,arm_2d_region_t *pRegion,arm_2d_tile_t* pImgTile,arm_2d_tile_t* pMaskTile,ldColor color,uint8_t opacity);


#ifdef __cplusplus
}
#endif

#endif
