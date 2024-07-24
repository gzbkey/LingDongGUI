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

#ifndef __LD__RADIAL_MENU_H__
#define __LD__RADIAL_MENU_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-declarations"
#pragma clang diagnostic ignored "-Wmicrosoft-anon-tag"
#pragma clang diagnostic ignored "-Wpadded"
#endif



/* OOC header, please DO NOT modify  */
#ifdef __LD__RADIAL_MENU_IMPLEMENT__
#undef __LD__RADIAL_MENU_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD__RADIAL_MENU_INHERIT__)
#undef __LD__RADIAL_MENU_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldRadialMenu_t ldRadialMenu_t;

#ifndef USE_RADIA_MENU_SCALE
#define USE_RADIA_MENU_SCALE     1
#endif

typedef struct {
    arm_2d_tile_t *ptImgTile;
    arm_2d_tile_t *ptMaskTile;
    arm_2d_region_t itemRegion;
    arm_2d_region_t tTempItemRegion;
    uint8_t count;
#if USE_RADIA_MENU_SCALE == 1
    uint8_t scalePercent;
#endif
    uint16_t angle;
    arm_2d_op_trans_msk_opa_t op;
}ldRadialMenuItem_t;

struct ldRadialMenu_t
{
    implement(ldBase_t);
    ARM_PRIVATE(
            ld_scene_t *ptScene;
    )

    ldRadialMenuItem_t *pItemList;
    uint8_t itemCount;
    uint8_t itemMax;
    uint16_t xAxis;
    uint16_t yAxis;
    arm_2d_location_t originPos;
    int16_t offsetAngle;
    int16_t nowAngle;
    uint8_t targetItem;
    uint8_t selectItem;
    uint32_t timer;
    uint8_t *pShowList;
    bool isMove:1;
    bool isWaitInit:1;
};

ldRadialMenu_t* ldRadialMenu_init(ld_scene_t *ptScene, ldRadialMenu_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint16_t xAxis, uint16_t yAxis, uint8_t itemMax);
void ldRadialMenu_depose( ldRadialMenu_t *ptWidget);
void ldRadialMenu_on_load( ldRadialMenu_t *ptWidget);
void ldRadialMenu_on_frame_start( ldRadialMenu_t *ptWidget);
void ldRadialMenu_show(ld_scene_t *pScene, ldRadialMenu_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldRadialMenuAddItem(ldRadialMenu_t *ptWidget,arm_2d_tile_t *ptImgTile,arm_2d_tile_t *ptMaskTile);
void ldRadialMenuSelectItem(ldRadialMenu_t *ptWidget,uint8_t num);
void ldRadialMenuOffsetItem(ldRadialMenu_t *ptWidget,int8_t offset);

#define ldRadialMenuInit(nameId,parentNameId,x,y,width,height,xAxis,yAxis,itemMax) \
        ldRadialMenu_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,xAxis,yAxis,itemMax)

#define ldRadialMenuSetHidden                ldBaseSetHidden
#define ldRadialMenuMove                     ldBaseMove
#define ldRadialMenuSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
