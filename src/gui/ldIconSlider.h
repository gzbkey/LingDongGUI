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

#ifndef __LD_ICON_SLIDER_H__
#define __LD_ICON_SLIDER_H__

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
#ifdef __LD_ICON_SLIDER_IMPLEMENT__
#undef __LD_ICON_SLIDER_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_ICON_SLIDER_INHERIT__)
#undef __LD_ICON_SLIDER_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct {
    arm_2d_tile_t *ptImgTile;
    arm_2d_tile_t *ptMaskTile;
    const uint8_t* pName;
}ldIconInfo_t;

typedef struct ldIconSlider_t ldIconSlider_t;

struct ldIconSlider_t
{
    implement(ldBase_t);
    ARM_PRIVATE(
            ld_scene_t *ptScene;
    )
    arm_2d_font_t *ptFont;
    ldIconInfo_t* ptIconInfoList;
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
    bool isCorner:1;
    bool isHorizontalScroll:1;
    bool hasHorizontalBorder:1;
    bool hasVerticalBorder:1;
    bool isWaitMove:1;
    bool isAutoMove:1;
    bool isHoldMove:1;
    bool isScrollEn:1;
};

ldIconSlider_t* ldIconSlider_init( ld_scene_t *ptScene,ldIconSlider_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, int16_t iconWidth, uint8_t iconSpace, uint8_t columnCount, uint8_t rowCount, uint8_t pageMax, arm_2d_font_t* ptFont);
void ldIconSlider_depose( ldIconSlider_t *ptWidget);
void ldIconSlider_on_load( ldIconSlider_t *ptWidget);
void ldIconSlider_on_frame_start( ldIconSlider_t *ptWidget);
void ldIconSlider_show(ld_scene_t *pScene, ldIconSlider_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldIconSliderAddIcon(ldIconSlider_t *ptWidget,arm_2d_tile_t *ptImgTile,arm_2d_tile_t *ptMaskTile,const uint8_t* pNameStr);
void ldIconSliderSetHorizontalScroll(ldIconSlider_t *ptWidget,bool isHorizontal);
void ldIconSliderSetSpeed(ldIconSlider_t *ptWidget, uint8_t speed);

#define ldIconSliderInit(nameId,parentNameId,x,y,width,height,iconWidth,iconSpace,columnCount,rowCount,pageMax,ptFont) \
        ldIconSlider_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,iconWidth,iconSpace,columnCount,rowCount,pageMax,ptFont)

#define ldIconSliderSetHidden                ldBaseSetHidden
#define ldIconSliderMove                     ldBaseMove
#define ldIconSliderSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
