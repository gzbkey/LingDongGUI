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

#ifndef __LD_BUTTON_H__
#define __LD_BUTTON_H__




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
#ifdef __LD_BUTTON_IMPLEMENT__
#undef __LD_BUTTON_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_BUTTON_INHERIT__)
#undef __LD_BUTTON_INHERIT__
#define __ARM_2D_INHERIT__
#endif

#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldButton_t ldButton_t;

struct ldButton_t
{
    implement(ldBase_t);
//    ARM_PRIVATE(
//            ld_scene_t *ptScene;
//    )
    arm_2d_tile_t *ptReleaseImgTile;
    arm_2d_tile_t *ptReleaseMaskTile;
    arm_2d_tile_t *ptPressImgTile;
    arm_2d_tile_t *ptPressMaskTile;
    arm_2d_tile_t *ptSelectMaskTile;
    uint32_t keyValue;
    arm_2d_font_t *ptFont;
    uint8_t *pStr;
    ldColor charColor;
    ldColor releaseColor;
    ldColor pressColor;
    ldColor selectColor;
    bool isTransparent : 1;
    bool isCheckable : 1;
    bool isChecked : 1;
    bool isPressed : 1;
    bool isSelected : 1;
    bool isCorner : 1;
};

ldButton_t *ldButton_init(ld_scene_t *ptScene, ldButton_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
void ldButton_depose(ldButton_t *ptWidget);
void ldButton_on_load(ldButton_t *ptWidget);
void ldButton_on_frame_start(ldButton_t *ptWidget);
void ldButton_show(ld_scene_t *ptScene, ldButton_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldButtonSetColor(ldButton_t* ptWidget, ldColor releaseColor, ldColor pressColor);
void ldButtonSetImage(ldButton_t* ptWidget,arm_2d_tile_t* ptReleaseImgTile,arm_2d_tile_t* ptReleaseMaskTile,arm_2d_tile_t* ptPressImgTile,arm_2d_tile_t* ptPressMaskTile);
void ldButtonSetSelectImage(ldButton_t* ptWidget,arm_2d_tile_t* ptSelectMaskTile,ldColor selectColor);
void ldButtonSetTransparent(ldButton_t* ptWidget,bool isTransparent);
void ldButtonSetRoundCorner(ldButton_t* ptWidget,bool isCorner);
void ldButtonSetSelect(ldButton_t* ptWidget,bool isSelected);
void ldButtonSetFont(ldButton_t *ptWidget, arm_2d_font_t *ptFont);
void ldButtonSetText(ldButton_t* ptWidget,uint8_t *pStr);
void ldButtonSetTextColor(ldButton_t* ptWidget,ldColor charColor);
void ldButtonSetCheckable(ldButton_t *ptWidget,bool isCheckable);
void ldButtonSetKeyValue(ldButton_t *ptWidget,uint32_t value);

#define ldButtonInit(nameId,parentNameId,x,y,width,height) \
        ldButton_init(ptScene,NULL,nameId,parentNameId,x,y,width,height)

#define ldButtonSetHidden               ldBaseSetHidden
#define ldButtonMove                    ldBaseMove
#define ldButtonSetOpacity              ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
