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

#ifndef __LD_SLIDER_H__
#define __LD_SLIDER_H__

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
#ifdef __LD_SLIDER_IMPLEMENT__
#undef __LD_SLIDER_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_SLIDER_INHERIT__)
#undef __LD_SLIDER_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldSlider_t ldSlider_t;

struct ldSlider_t
{
    implement(ldBase_t);
//    ARM_PRIVATE(
//            ld_scene_t *ptScene;
//    )
    uint16_t permille;
    ldColor bgColor;
    ldColor frameColor;
    ldColor indicColor;//indicator
    uint8_t indicWidth;
    uint8_t slimSize;
    arm_2d_tile_t *ptBgImgTile;
    arm_2d_tile_t *ptBgMaskTile;
    arm_2d_tile_t *ptIndicImgTile;//indicator
    arm_2d_tile_t *ptIndicMaskTile;
    bool isHorizontal:1;
};

ldSlider_t* ldSlider_init(ld_scene_t *ptScene, ldSlider_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
void ldSlider_depose( ldSlider_t *ptWidget);
void ldSlider_on_load( ldSlider_t *ptWidget);
void ldSlider_on_frame_start( ldSlider_t *ptWidget);
void ldSlider_show(ld_scene_t *pScene, ldSlider_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldSliderSetPercent(ldSlider_t *ptWidget,float percent);
void ldSliderSetHorizontal(ldSlider_t *ptWidget,bool isHorizontal);
void ldSliderSetImage(ldSlider_t *ptWidget,arm_2d_tile_t *ptBgImgTile,arm_2d_tile_t *ptBgMaskTile,arm_2d_tile_t *ptIndicImgTile,arm_2d_tile_t *ptIndicMaskTile);
void ldSliderSetColor(ldSlider_t *ptWidget,ldColor bgColor,ldColor frameColor,ldColor indicColor);
void ldSliderSetIndicatorWidth(ldSlider_t *ptWidget,uint8_t indicWidth);
void ldSliderSetSlimSize(ldSlider_t *ptWidget,uint8_t slimSize);

#define ldSliderInit(nameId,parentNameId,x,y,width,height) \
        ldSlider_init(ptScene,NULL,nameId,parentNameId,x,y,width,height)

#define ldSliderSetHidden                ldBaseSetHidden
#define ldSliderMove                     ldBaseMove
#define ldSliderSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
