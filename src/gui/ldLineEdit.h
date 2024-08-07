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

#ifndef __LD_LINE_EDIT_H__
#define __LD_LINE_EDIT_H__

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
#ifdef __LD_LINE_EDIT_IMPLEMENT__
#undef __LD_LINE_EDIT_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_LINE_EDIT_INHERIT__)
#undef __LD_LINE_EDIT_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldLineEdit_t ldLineEdit_t;

struct ldLineEdit_t
{
    implement(ldBase_t);
    ARM_PRIVATE(
            ld_scene_t *ptScene;
    )
    arm_2d_font_t *ptFont;
    uint16_t kbNameId;
    ldColor textColor;
    uint8_t *pText;
    uint8_t textMax;
    bool isEditing:1;
    bool isCorner:1;
    ldEditType_t editType:2;
    bool hasFloatPoint:1;
};

ldLineEdit_t* ldLineEdit_init(ld_scene_t *ptScene, ldLineEdit_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont, uint8_t textMax);
void ldLineEdit_depose( ldLineEdit_t *ptWidget);
void ldLineEdit_on_load( ldLineEdit_t *ptWidget);
void ldLineEdit_on_frame_start( ldLineEdit_t *ptWidget);
void ldLineEdit_show(ld_scene_t *pScene, ldLineEdit_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldLineEditSetText(ldLineEdit_t* ptWidget,uint8_t *pText);
void ldLineEditSetKeyboard(ldLineEdit_t* ptWidget,uint16_t kbNameId);
void ldLineEditSetType(ldLineEdit_t* ptWidget,ldEditType_t editType);

#define ldLineEditInit(nameId,parentNameId,x,y,width,height,ptFont,textMax) \
        ldLineEdit_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptFont,textMax)

#define ldLineEditSetHidden                ldBaseSetHidden
#define ldLineEditMove                     ldBaseMove
#define ldLineEditSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
