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

#ifndef __LD_COMBO_BOX_H__
#define __LD_COMBO_BOX_H__

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
#ifdef __LD_COMBO_BOX_IMPLEMENT__
#undef __LD_COMBO_BOX_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_COMBO_BOX_INHERIT__)
#undef __LD_COMBO_BOX_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldComboBox_t ldComboBox_t;

struct ldComboBox_t
{
    implement(ldBase_t);
//    ARM_PRIVATE(
//            ld_scene_t *ptScene;
//    )
    arm_2d_tile_t *ptDropdownImgTile;
    arm_2d_tile_t *ptDropdownMaskTile;
    uint8_t itemMax;
    uint8_t itemCount;
    uint8_t itemSelect;
    uint8_t itemPreSelect;
    int16_t itemHeight;
    arm_2d_font_t *ptFont;
    const uint8_t **ppItemStrGroup;
    ldColor textColor;
    bool isExpand:1;
    bool isCorner:1;
};

ldComboBox_t* ldComboBox_init(ld_scene_t *ptScene, ldComboBox_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont);
void ldComboBox_depose( ldComboBox_t *ptWidget);
void ldComboBox_on_load( ldComboBox_t *ptWidget);
void ldComboBox_on_frame_start( ldComboBox_t *ptWidget);
void ldComboBox_show(ld_scene_t *pScene, ldComboBox_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldComboBoxSetItems(ldComboBox_t* ptWidget,const uint8_t *pStrArray[],uint8_t arraySize);

#define ldComboBoxInit(nameId,parentNameId,x,y,width,height,ptFont) \
        ldComboBox_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptFont)

#define ldComboBoxSetHidden                ldBaseSetHidden
#define ldComboBoxMove                     ldBaseMove
#define ldComboBoxSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
