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

#ifndef __LD_LABEL_H__
#define __LD_LABEL_H__

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
#ifdef __LD_LABEL_IMPLEMENT__
#undef __LD_LABEL_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_LABEL_INHERIT__)
#undef __LD_LABEL_INHERIT__
#define __ARM_2D_INHERIT__
#endif

#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldLabel_t ldLabel_t;

struct ldLabel_t
{
    implement(ldBase_t);
    ARM_PRIVATE(
            ld_scene_t *ptScene;
    )
    arm_2d_align_t tAlign;
    arm_2d_tile_t *ptImgTile;
    arm_2d_font_t *ptFont;
    ldColor bgColor;
    ldColor textColor;
    uint8_t* pStr;
    bool isTransparent:1;
};

ldLabel_t* ldLabel_init(ld_scene_t *ptScene, ldLabel_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,arm_2d_font_t *ptFont);
void ldLabel_depose( ldLabel_t *ptWidget);
void ldLabel_on_load( ldLabel_t *ptWidget);
void ldLabel_on_frame_start( ldLabel_t *ptWidget);
void ldLabel_show(ld_scene_t *ptScene, ldLabel_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldLabelSetTransparent(ldLabel_t* ptWidget,bool isTransparent);
void ldLabelSetText(ldLabel_t* ptWidget,uint8_t *pStr);
void ldLabelSetTextColor(ldLabel_t* ptWidget,ldColor textColor);
void ldLabelSetAlign(ldLabel_t *ptWidget,arm_2d_align_t tAlign);
void ldLabelSetBgImage(ldLabel_t *ptWidget, arm_2d_tile_t *ptImgTile);
void ldLabelSetBgColor(ldLabel_t *ptWidget, ldColor bgColor);
void ldLabelSetOpacity(ldLabel_t *ptWidget, uint8_t opacity);

#define ldLabelInit(nameId,parentNameId,x,y,width,height,ptFont) \
        ldLabel_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptFont)

#define ldLabelSetHidden                ldBaseSetHidden
#define ldLabelMove                     ldBaseMove
#define ldLabelSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
