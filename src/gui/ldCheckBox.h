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

#ifndef __LD_CHECK_BOX_H__
#define __LD_CHECK_BOX_H__


#include "arm_2d.h"
#include "./__common.h"

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
#ifdef __LD_CHECK_BOX_IMPLEMENT__
#undef __LD_CHECK_BOX_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_CHECK_BOX_INHERIT__)
#undef __LD_CHECK_BOX_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldCheckBox_t ldCheckBox_t;

struct ldCheckBox_t
{
    implement(ldBase_t);
    ARM_PRIVATE(
            ld_scene_t *ptScene;
    )
//    arm_2d_align_t tAlign;
    ldColor bgColor;
    ldColor fgColor;
    ldColor charColor;
    arm_2d_tile_t* ptUncheckedImgTile;
    arm_2d_tile_t* ptCheckedImgTile;
    arm_2d_font_t *ptFont;
    uint8_t* pStr;
    uint16_t boxWidth;
    uint8_t radioButtonGroup;
    bool isChecked:1;
    bool isCorner:1;
    bool isWithUncheckedMask:1;
    bool isWithCheckedMask:1;
    bool isRadioButton:1;
};

ldCheckBox_t* ldCheckBox_init(ld_scene_t *ptScene, ldCheckBox_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
void ldCheckBox_depose( ldCheckBox_t *ptWidget);
void ldCheckBox_on_load( ldCheckBox_t *ptWidget);
void ldCheckBox_on_frame_start( ldCheckBox_t *ptWidget);
void ldCheckBox_show(ld_scene_t *pScene, ldCheckBox_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldCheckBoxSetColor(ldCheckBox_t* ptWidget,ldColor bgColor,ldColor fgColor);
void ldCheckBoxSetImage(ldCheckBox_t* ptWidget,uint16_t boxWidth,uintptr_t uncheckedImgAddr,bool isUncheckedMask,uintptr_t checkedImgAddr,bool isCheckedMask);
void ldCheckBoxSetText(ldCheckBox_t* ptWidget,arm_2d_font_t *ptFont,uint8_t *pStr);
void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* ptWidget,uint8_t num);
void ldCheckBoxSetCorner(ldCheckBox_t* ptWidget,bool isCorner);
void ldCheckBoxSetCharColor(ldCheckBox_t* ptWidget,ldColor charColor);

#define ldCheckBoxInit(nameId,parentNameId,x,y,width,height) \
        ldCheckBox_init(ptScene,NULL,nameId,parentNameId,x,y,width,height)

#define ldCheckBoxSetHidden                ldBaseSetHidden
#define ldCheckBoxMove                     ldBaseMove
#define ldCheckBoxSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
