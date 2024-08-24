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

#ifndef __LD_TEXT_H__
#define __LD_TEXT_H__

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
#ifdef __LD_TEXT_IMPLEMENT__
#undef __LD_TEXT_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_TEXT_INHERIT__)
#undef __LD_TEXT_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldText_t ldText_t;

struct ldText_t
{
    implement(ldBase_t);
//ARM_PRIVATE(
//    ld_scene_t *ptScene;
//)
    arm_2d_helper_pi_slider_t tPISlider;
    arm_2d_tile_t *ptImgTile;
    arm_2d_font_t *ptFont;
    uint8_t *pStr;
    ldColor bgColor;
    ldColor textColor;
    int16_t scrollOffset;
    int16_t _scrollOffset;
    int16_t strHeight;
    bool isTransparent:1;
    bool isMoveReset:1;
    bool _isTopScroll:1;
    bool _isBottomScroll:1;
    bool _isStatic:1;
};

ldText_t* ldText_init(ld_scene_t *ptScene, ldText_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont,bool isScroll);
void ldText_depose( ldText_t *ptWidget);
void ldText_on_load( ldText_t *ptWidget);
void ldText_on_frame_start( ldText_t *ptWidget);
void ldText_show(ld_scene_t *pScene, ldText_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldTextSetTransparent(ldText_t* ptWidget,bool isTransparent);
void ldTextSetText(ldText_t* ptWidget,uint8_t *pStr);
void ldTextSetStaticText(ldText_t* ptWidget,const uint8_t *pStr);
void ldTextSetTextColor(ldText_t* ptWidget,ldColor charColor);
void ldTextSetBgImage(ldText_t *ptWidget, arm_2d_tile_t *ptImgTile);
void ldTextSetBgColor(ldText_t *ptWidget, ldColor bgColor);
void ldTextScrollSeek(ldText_t *ptWidget,int16_t offset);
void ldTextScrollMove(ldText_t *ptWidget, int8_t moveValue);

#define ldTextInit(nameId,parentNameId,x,y,width,height,ptFont,isScroll) \
        ldText_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptFont,isScroll)

#define ldTextSetHidden                ldBaseSetHidden
#define ldTextMove                     ldBaseMove
#define ldTextSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
