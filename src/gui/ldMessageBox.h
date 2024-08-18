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

#ifndef __LD_MESSAGE_BOX_H__
#define __LD_MESSAGE_BOX_H__

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
#ifdef __LD_MESSAGE_BOX_IMPLEMENT__
#undef __LD_MESSAGE_BOX_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_MESSAGE_BOX_INHERIT__)
#undef __LD_MESSAGE_BOX_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldMessageBox_t ldMessageBox_t;

typedef void (*ldMsgBoxFunc_t)(ldMessageBox_t *);

struct ldMessageBox_t
{
    implement(ldBase_t);


    arm_2d_tile_t *ptBgImgTile;
    arm_2d_tile_t *ptBgMaskTile;
    arm_2d_font_t *ptFont;
    const uint8_t *pTitleStr;
    const uint8_t **ppBtnStrGroup;
    const uint8_t *pMsgStr;
    ldMsgBoxFunc_t ptFunc;
    ldBody_t padding;// 边框到内容中间的部分
    arm_2d_region_t btnRegion;
    ldColor titleStrColor;
    ldColor msgStrColor;
    ldColor btnStrColor;
    ldColor bgColor;
    ldColor releaseColor;
    ldColor pressColor;
    uint8_t btnCount;
    uint8_t titleHeight;
    uint8_t msgHeight;
    bool isCorner:1;
    uint8_t isBtnPressed:3;
    int8_t clickNum:2;
};

ldMessageBox_t* ldMessageBox_init(ld_scene_t *ptScene, ldMessageBox_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont);
void ldMessageBox_depose( ldMessageBox_t *ptWidget);
void ldMessageBox_on_load( ldMessageBox_t *ptWidget);
void ldMessageBox_on_frame_start( ldMessageBox_t *ptWidget);
void ldMessageBox_show(ld_scene_t *pScene, ldMessageBox_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldMessageBoxSetTitle(ldMessageBox_t* ptWidget,const uint8_t *pStr);
void ldMessageBoxSetMsg(ldMessageBox_t* ptWidget,const uint8_t *pStr);
void ldMessageBoxSetBtn(ldMessageBox_t* ptWidget,const uint8_t *pStrArray[],uint8_t arraySize);
void ldMessageBoxSetCallback(ldMessageBox_t* ptWidget,ldMsgBoxFunc_t ptFunc);

#define ldMessageBoxInit(nameId,parentNameId,width,height,ptFont) \
        ldMessageBox_init(ptScene,NULL,nameId,parentNameId,-1,-1,width,height,ptFont)

#define ldMessageBoxSetHidden                ldBaseSetHidden
#define ldMessageBoxMove                     ldBaseMove
#define ldMessageBoxSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
