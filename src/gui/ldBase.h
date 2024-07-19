/*
 * Copyright (c) 2023-2024 Ou Jianbo (59935554@qq.com). All rights reserved.
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
#include "arm_extra_controls.h"
#include "arm_2d_helper_control.h"
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
}ldWidgetType_t;

//btn占用0-9
#define SIGNAL_NO_OPERATION         BTN_NO_OPERATION
#define SIGNAL_PRESS                BTN_PRESS           // value = x (2Byte) + y (2Byte)
#define SIGNAL_HOLD_DOWN            BTN_HOLD_DOWN       // value = x offset(2Byte) + y offset(2Byte) + x (2Byte) + y (2Byte)
#define SIGNAL_RELEASE              BTN_RELEASE         // value = x speed(2Byte) + y speed(2Byte) + x (2Byte) + y (2Byte)


#define ldColor                                 COLOUR_INT

#define CURSOR_WIDTH                            2

#define MEM_MODE_FREERTOS_HEAP4                   (0)
#define MEM_MODE_TLFS                             (1)
#define MEM_MODE_STDLIB                           (2)
#define MEM_MODE_USER                             (3)

typedef struct ld_scene_t ld_scene_t;
typedef void (*ldPageFunc_t)(ld_scene_t*);
typedef struct ldPageFuncGroup_t ldPageFuncGroup_t;

typedef void (*ldDeposeFunc_t)(void *);
typedef void (*ldShowFunc_t)(ld_scene_t*,void *,void *,bool);
typedef void (*ldFrameStartFunc_t)(void *);
typedef void (*ldLoadFunc_t)(void *);

typedef struct {
    ldDeposeFunc_t depose;
    ldLoadFunc_t load;
    ldFrameStartFunc_t frameStart;
    ldShowFunc_t show;
}ldBaseWidgetFunc_t;


struct ldPageFuncGroup_t{
    ldPageFunc_t init;
    ldPageFunc_t loop;
    ldPageFunc_t quit;
#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    uint8_t pageName[16];
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

typedef struct  {
    implement(arm_2d_control_node_t);
//    ARM_PRIVATE(
    arm_2d_region_t tTempRegion;
//)
    const ldBaseWidgetFunc_t *ptGuiFunc;
    ldAssn_t *ptAssn;
    ldWidgetType_t widgetType;
    uint16_t nameId;
    uint8_t opacity;
    bool isDirtyRegionUpdate:1;
    bool isDirtyRegionAutoReset:1;
    bool isHidden:1;
}ldBase_t;

bool ldTimeOut(uint16_t ms, int64_t *pTimer,bool isReset);

void ldBaseNodeAdd(arm_2d_control_node_t *parent, arm_2d_control_node_t *child);
void ldBaseNodeRemove(arm_2d_control_node_t *ptNodeRoot, arm_2d_control_node_t *ptNode);
#if (USE_LOG_LEVEL>=LOG_LEVEL_NONE)
void ldBaseNodeTreePrint(arm_2d_control_node_t *ptNodeRoot, int depth);
#endif
void* ldBaseGetWidget(arm_2d_control_node_t *ptNodeRoot, uint16_t nameId);
void ldBaseColor(arm_2d_tile_t* ptTile, arm_2d_region_t* ptRegion, ldColor color, uint8_t opacity);
void ldBaseImage(arm_2d_tile_t* ptTile, arm_2d_region_t *ptRegion, arm_2d_tile_t* ptImgTile, arm_2d_tile_t* ptMaskTile, ldColor color, uint8_t opacity);
void ldBaseLabel(arm_2d_tile_t *ptTile, arm_2d_region_t *ptRegion, uint8_t *pStr, arm_2d_font_t *ptFont, arm_2d_align_t tAlign, ldColor textColor, uint8_t opacity);
void ldBaseMove(ldBase_t* ptWidget,int16_t x,int16_t y);
void ldBaseSetHidden(ldBase_t* ptWidget,bool isHidden);
void ldBaseSetOpacity(ldBase_t *ptWidget, uint8_t opacity);

#ifdef __cplusplus
}
#endif

#endif
