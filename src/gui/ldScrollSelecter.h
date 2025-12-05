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

#ifndef __LD_SCROLL_SELECTER_H__
#define __LD_SCROLL_SELECTER_H__

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
#ifdef __LD_SCROLL_SELECTER_IMPLEMENT__
#undef __LD_SCROLL_SELECTER_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_SCROLL_SELECTER_INHERIT__)
#undef __LD_SCROLL_SELECTER_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldScrollSelecter_t ldScrollSelecter_t;

struct ldScrollSelecter_t
{
    implement(ldBase_t);
    arm_2d_font_t *ptFont;
    arm_2d_tile_t *ptImgTile;
    arm_2d_tile_t *ptMaskTile;
    arm_2d_tile_t *ptIndicatorTile;
    const uint8_t **ppItemStrGroup;
    int16_t scrollOffset;
    int16_t itemSpace;
    ldColor charColor;
    ldColor bgColor;
    ldColor indicatorColor;
    uint8_t itemSelect;
    uint8_t moveOffset;
    uint8_t itemCount;
    bool isTransparent:1;
    bool isWaitMove:1;
    bool isAutoMove:1;
    bool isEdit:1;
};

ldScrollSelecter_t* ldScrollSelecter_init(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont);
void ldScrollSelecter_depose(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget);
void ldScrollSelecter_on_load(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget);
void ldScrollSelecter_on_frame_start(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget);
void ldScrollSelecter_on_frame_complete(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget);
void ldScrollSelecter_show(ld_scene_t *pScene, ldScrollSelecter_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldScrollSelecterSetItems(ldScrollSelecter_t* ptWidget, const uint8_t *pStrArray[], uint8_t arraySize);
void ldScrollSelecterSetTextColor(ldScrollSelecter_t* ptWidget,ldColor charColor);
void ldScrollSelecterSetBackgroundColor(ldScrollSelecter_t* pWidget,ldColor bgColor);
void ldScrollSelecterSetBackgroundImage(ldScrollSelecter_t* pWidget,arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile);
void ldScrollSelecterSetTransparent(ldScrollSelecter_t* pWidget,bool isTransparent);
void ldScrollSelecterSetSpeed(ldScrollSelecter_t *pWidget, uint8_t speed);
void ldScrollSelecterSelectItem(ldScrollSelecter_t *pWidget, int8_t itemNum);
void ldScrollSelecterSelectText(ldScrollSelecter_t *ptWidget, uint8_t *text);
void ldScrollSelecterSetEditMode(ldScrollSelecter_t *ptWidget, bool isEdit);
uint8_t *ldScrollSelecterCurrentText(ldScrollSelecter_t *ptWidget);

#define ldScrollSelecterInit(nameId,parentNameId,x,y,width,height,ptFont) \
        ldScrollSelecter_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptFont)

#define ldScrollSelecterSetHidden                ldBaseSetHidden
#define ldScrollSelecterMove                     ldBaseMove
#define ldScrollSelecterSetOpacity               ldBaseSetOpacity
#define ldScrollSelecterSetSelectable            ldBaseSetSelectable
#define ldScrollSelecterSetSelect                ldBaseSetSelect
#define ldScrollSelecterSetCorner                ldBaseSetCorner

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
