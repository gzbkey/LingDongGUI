/*
 * Copyright (c) 2023-2025 Ou Jianbo (59935554@qq.com). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LD_BASE_H__
#define __LD_BASE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "arm_2d_helper_control.h"
#include "arm_2d_example_controls.h"
#include "xLog.h"
#include "xBtnAction.h"
#include "ldMem.h"
#include "ldConfig.h"
#include "ldMsg.h"
#include "xQueue.h"

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
    widgetTypeAnimation,
    widgetTypeList,
    widgetTypeMessageBox,
    widgetTypeCalendar,
}ldWidgetType_t;


#ifndef SET_BITS
#define SET_BITS(data,value)            ((data)|=(value))
#endif
#ifndef CLR_BITS
#define CLR_BITS(data,value)            ((data)&=~(value))
#endif
#ifndef SETBIT
#define SETBIT(data,move)               ((data)|=(1<<(move)))
#endif
#ifndef CLRBIT
#define CLRBIT(data,move)               ((data)&=~(1<<(move)))
#endif
#ifndef GETBIT
#define GETBIT(data,move)               (((data)>>(move))&0x01)
#endif
#ifndef PUTBIT
#define PUTBIT(data,value,move)         ((value)?SETBIT(data,move):CLRBIT(data,move))
#endif

#define   GET16H(data)                  (((data)>>8)&0xFF)
#define   GET16L(data)                  ((data)&0xFF)
#define   CONNECT16(H,L)                (((H)<<8)+(L))

#define   GET32H(data)                  (((data)>>24)&0xFF)
#define   GET32MH(data)                 (((data)>>16)&0xFF)
#define   GET32ML(data)                 (((data)>>8)&0xFF)
#define   GET32L(data)                  ((data)&0xFF)
#define   CONNECT32(H,MH,ML,L)          (((H)<<24)+((MH)<<16)+((ML)<<8)+(L))

//btn占用0-9
#define SIGNAL_NO_OPERATION             BTN_NO_OPERATION
#define SIGNAL_PRESS                    BTN_PRESS           // value = x (2Byte) + y (2Byte)
#define SIGNAL_HOLD_DOWN                BTN_HOLD_DOWN       // value = x offset(2Byte) + y offset(2Byte) + x (2Byte) + y (2Byte)
#define SIGNAL_RELEASE                  BTN_RELEASE         // value = x speed(2Byte) + y speed(2Byte) + x (2Byte) + y (2Byte)

#define SIGNAL_CLICKED_ITEM             12                  // value = item num
#define SIGNAL_FINISHED                 13
#define SIGNAL_VALUE_CHANGED            14

#define GET_SIGNAL_OFFSET_X(dat)        ((dat>>48)&0xFFFF)
#define GET_SIGNAL_OFFSET_Y(dat)        ((dat>>32)&0xFFFF)

#define GET_SIGNAL_SPEED_X(dat)         ((dat>>48)&0xFFFF)
#define GET_SIGNAL_SPEED_Y(dat)         ((dat>>32)&0xFFFF)

#define GET_SIGNAL_VALUE_X(dat)         ((dat>>16)&0xFFFF)
#define GET_SIGNAL_VALUE_Y(dat)         (dat&0xFFFF)

#define ldColor                         COLOUR_INT

#define CURSOR_WIDTH                    (2)

#define MEM_MODE_FREERTOS_HEAP4         (0)
#define MEM_MODE_TLFS                   (1)
#define MEM_MODE_STDLIB                 (2)
#define MEM_MODE_USER                   (3)

#define ANGLE_2_RADIAN(angle)           ((float)(angle)*0.0174533f)

#ifndef LD_SELECT_COLOR
#define LD_SELECT_COLOR                 __RGB(255, 0, 0)
#endif

#ifndef LD_SELECT_OPACITY
#define LD_SELECT_OPACITY               (ptWidget->use_as__ldBase_t.opacity/4)
#endif

#define LD_BASE_WIDGET_SELECT \
do{ \
    if(ptWidget->use_as__ldBase_t.isSelected&&ptWidget->use_as__ldBase_t.isSelectable){ \
        if(ptWidget->use_as__ldBase_t.isCorner){ \
            draw_round_corner_border(&tTarget,&tTarget_canvas,LD_SELECT_COLOR,(arm_2d_border_opacity_t){LD_SELECT_OPACITY,LD_SELECT_OPACITY,LD_SELECT_OPACITY,LD_SELECT_OPACITY},(arm_2d_corner_opacity_t){LD_SELECT_OPACITY,LD_SELECT_OPACITY,LD_SELECT_OPACITY,LD_SELECT_OPACITY}); \
        }else{ \
            arm_2d_draw_box(&tTarget,&tTarget_canvas,2,LD_SELECT_COLOR,LD_SELECT_OPACITY); \
        } \
    } \
}while(0)

typedef struct ld_scene_t ld_scene_t;
typedef void (*ldPageFunc_t)(ld_scene_t*);
typedef void (*ldDrawFunc_t)(ld_scene_t *,arm_2d_tile_t *,bool);
typedef struct ldPageFuncGroup_t ldPageFuncGroup_t;

typedef void (*ldLoadFunc_t)(ld_scene_t *ptScene, void *ptWidget);
typedef void (*ldDeposeFunc_t)(ld_scene_t *ptScene, void *ptWidget);
typedef void (*ldFrameStartFunc_t)(ld_scene_t *ptScene, void *ptWidget);
typedef void (*ldFrameCompleteFunc_t)(ld_scene_t *ptScene, void *ptWidget);
typedef void (*ldShowFunc_t)(ld_scene_t *ptScene, void *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

typedef struct {
    ldDeposeFunc_t depose;
    ldLoadFunc_t load;
    ldFrameStartFunc_t frameStart;
    ldFrameCompleteFunc_t frameComplete;
    ldShowFunc_t show;
}ldBaseWidgetFunc_t;


struct ldPageFuncGroup_t{
    ldPageFunc_t init;
    ldPageFunc_t loop;
    ldPageFunc_t quit;
    ldDrawFunc_t draw;
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    char *pageName;
#endif
};

struct ld_scene_t {
    implement(arm_2d_scene_t);
//ARM_PRIVATE(
    bool bUserAllocated;
    arm_2d_region_list_item_t tDirtyRegionItem;
    arm_2d_control_enumerator_t tEnum;
//)
    const ldPageFuncGroup_t *ldGuiFuncGroup;
    arm_2d_control_node_t *ptNodeRoot;
    xQueue_t *ptMsgQueue;
};

typedef struct {
    arm_2d_region_t itemRegion;
    arm_2d_region_t tTempItemRegion;
}ldBaseItemRegion_t;

typedef struct {
    implement(arm_2d_control_node_t);
    arm_2d_region_t tTempRegion;
    const ldBaseWidgetFunc_t *ptGuiFunc;
    ldAssn_t *ptAssn;
    ldBaseItemRegion_t *ptItemRegionList;
    void *pInfo;
    ldWidgetType_t widgetType;
    uint16_t nameId;
    uint8_t opacity;
    uint8_t itemCount;
    bool isDirtyRegionUpdate:1;
    bool isDirtyRegionAutoReset:1;
    bool isHidden:1;
    bool isSelected:1;
    bool isSelectable:1;
    bool isCorner:1;
}ldBase_t;

typedef enum{
    typeString,
    typeInt,
    typeFloat
}ldEditType_t;

typedef struct {
    uint8_t top;
    uint8_t bottom;
    uint8_t left;
    uint8_t right;
}ldBody_t;

typedef struct {
    uint16_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t week;
    bool isEnable:1;
}ldBaseRTC_t;

typedef int64_t ldTimer_t;

bool __ldTimeOut(uint16_t ms, bool isReset, ldTimer_t *pTimer);

#define ldTimeOut(__ms, isReset, ...) ({static int64_t arm_2d_safe_name(s_lTimestamp); \
                                          __ldTimeOut(__ms, \
                                                      isReset, \
                                                      (&arm_2d_safe_name(s_lTimestamp),##__VA_ARGS__));})

void ldBaseNodeAdd(arm_2d_control_node_t *parent, arm_2d_control_node_t *child);
void ldBaseNodeRemove(arm_2d_control_node_t *ptNode);
#if (USE_LOG_LEVEL>=LOG_LEVEL_NONE)
void ldBaseNodeTreePrint(arm_2d_control_node_t *ptNodeRoot, int depth);
#endif
void* ldBaseGetWidget(arm_2d_control_node_t *ptNodeRoot, uint16_t nameId);
void ldBaseColor(arm_2d_tile_t* ptTile, arm_2d_region_t* ptRegion, ldColor color, uint8_t opacity);
void ldBaseImage(arm_2d_tile_t* ptTile, arm_2d_region_t *ptRegion, arm_2d_tile_t* ptImgTile, arm_2d_tile_t* ptMaskTile, ldColor color, uint8_t opacity);
void ldBaseImageScale(arm_2d_tile_t *ptTile, arm_2d_region_t *ptRegion, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile,float scale,arm_2d_op_trans_msk_opa_t *ptOP,uint8_t opacity,bool bIsNewFrame);
void ldBaseLabel(arm_2d_tile_t *ptTile, arm_2d_region_t *ptRegion, uint8_t *pStr, arm_2d_font_t *ptFont, arm_2d_align_t tAlign, ldColor textColor, uint8_t opacity);
ldWidgetType_t ldBaseGetWidgetType(ldBase_t *ptWidget);
uint16_t ldBaseGetNameId(ldBase_t *ptWidget);
uint16_t ldBaseGetOpacity(ldBase_t *ptWidget);
bool ldBaseIsHidden(ldBase_t* ptWidget);
bool ldBaseIsSelected(ldBase_t *ptWidget);
bool ldBaseIsSelectable(ldBase_t *ptWidget);
bool ldBaseIsCorner(ldBase_t *ptWidget);
arm_2d_location_t ldBaseGetRelativeLocation(ldBase_t *ptWidget,arm_2d_location_t tLocation);
arm_2d_location_t ldBaseGetAbsoluteLocation(ldBase_t *ptWidget,arm_2d_location_t tLocation);
void ldBaseDrawLine(arm_2d_tile_t *pTile,int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t lineSize, ldColor color,uint8_t opacityMax, uint8_t opacityMin);
ldBase_t* ldBaseGetParent(ldBase_t* ptWidget);
ldBase_t* ldBaseGetChildList(ldBase_t* ptWidget);
void ldBaseBgMove(ld_scene_t *ptScene, int16_t bgWidth,int16_t bgHeight,int16_t offsetX,int16_t offsetY);
arm_2d_region_t ldBaseGetAlignRegion(arm_2d_region_t parentRegion,arm_2d_region_t childRegion,arm_2d_align_t tAlign);
arm_2d_control_node_t *ldBaseGetRootNode(arm_2d_control_node_t *ptNode);
int16_t ldBaseAutoVerticalGridAlign(arm_2d_region_t widgetRegion, int16_t currentOffset, uint8_t itemCount, uint8_t itemHeight, uint8_t space);

void ldBaseSetCenter(ldBase_t *ptWidget);
void ldBaseSetHidden(ldBase_t* ptWidget,bool isHidden);
void ldBaseSetOpacity(ldBase_t *ptWidget, uint8_t opacity);
void ldBaseSetSelectable(ldBase_t* ptWidget,bool isSelectable);
void ldBaseSetSelect(ldBase_t* ptWidget,bool isSelect);
void ldBaseSetCorner(ldBase_t* ptWidget,bool isCorner);

void ldBaseSetRegion(ldBase_t* ptWidget,arm_2d_region_t region);
void ldBaseMove(ldBase_t* ptWidget,int16_t x,int16_t y);
void ldBaseResize(ldBase_t* ptWidget,arm_2d_size_t size);
void ldBaseSetX(ldBase_t* ptWidget,int16_t x);
void ldBaseSetY(ldBase_t* ptWidget,int16_t y);
void ldBaseSetWidth(ldBase_t* ptWidget,int16_t width);
void ldBaseSetHeight(ldBase_t* ptWidget,int16_t height);
arm_2d_region_t ldBaseGetRegion(ldBase_t* ptWidget);
arm_2d_location_t ldBaseGetLocation(ldBase_t* ptWidget);
arm_2d_size_t ldBaseGetSize(ldBase_t* ptWidget);
int16_t ldBaseGetX(ldBase_t* ptWidget);
int16_t ldBaseGetY(ldBase_t* ptWidget);
int16_t ldBaseGetWidth(ldBase_t* ptWidget);
int16_t ldBaseGetHeight(ldBase_t* ptWidget);
uint8_t ldBaseZeller(uint16_t year, uint8_t month, uint8_t day);

#define ldBaseGetWidgetById(nameId)     ldBaseGetWidget(ptScene->ptNodeRoot, nameId)

#if USE_VIRTUAL_RESOURCE == 1

struct arm_2d_vres_font_t {
    implement(arm_2d_font_t);
    uint32_t startAddr;
    arm_2d_vres_t childTileMask;
};
typedef struct arm_2d_vres_font_t arm_2d_vres_font_t;
arm_2d_vres_t *ldBaseGetVresImage(uint32_t addr);
arm_2d_vres_font_t* ldBaseGetVresFont(uint32_t addr);

#endif

typedef enum {
    NAV_UP, NAV_DOWN, NAV_LEFT, NAV_RIGHT, NAV_ENTER, NAV_BACK
} ldNavDir_t;
void ldBaseFocusNavigateInit(void);
void ldBaseFocusNavigate(ld_scene_t *ptScene, ldNavDir_t tDir);

#ifdef __cplusplus
}
#endif

#endif
