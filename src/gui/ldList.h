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

#ifndef __LD_LIST_H__
#define __LD_LIST_H__

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
#ifdef __LD_LIST_IMPLEMENT__
#undef __LD_LIST_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_LIST_INHERIT__)
#undef __LD_LIST_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldList_t ldList_t;

struct ldList_t
{
    implement(ldBase_t);

    arm_2d_location_t clickItemPos;
    arm_2d_align_t tAlign;
    arm_2d_font_t *ptFont;
    const uint8_t **ppItemStrGroup;
    ldColor bgColor;
    ldColor textColor;
    ldColor selectColor;
    int16_t offset;
    int16_t _offset;
    uint8_t itemHeight;
    uint8_t itemCount;
    uint8_t selectItem;
    ldBody_t padding;// 边框到内容中间的部分
    ldBody_t margin;// 边框以外的区域
    bool isMoveReset:1;
    bool isHoldMove:1;
    bool isCorner:1;
    bool isTransparent:1;
};

ldList_t* ldList_init(ld_scene_t *ptScene, ldList_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
void ldList_depose( ldList_t *ptWidget);
void ldList_on_load( ldList_t *ptWidget);
void ldList_on_frame_start( ldList_t *ptWidget);
void ldList_show(ld_scene_t *pScene, ldList_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldListSetItemHeight(ldList_t* ptWidget,uint8_t itemHeight);
void ldListSetText(ldList_t* ptWidget,const uint8_t *pStrArray[],uint8_t arraySize, arm_2d_font_t *ptFont);

void ldListSetTextColor(ldList_t* ptWidget,ldColor textColor);
void ldListSetAlign(ldList_t *ptWidget,arm_2d_align_t tAlign);
void ldListSetBgColor(ldList_t *ptWidget, ldColor bgColor);
void ldListSetSelectColor(ldList_t* ptWidget,ldColor selectColor);
void ldListSetItemWidget(ldList_t* ptWidget,uint8_t itemNum,ldBase_t* childWidget);
int8_t ldListGetSelectItem(ldList_t* ptWidget);
void ldListSetSelectItem(ldList_t* ptWidget,int8_t itemNum);

#define ldListInit(nameId,parentNameId,x,y,width,height) \
        ldList_init(ptScene,NULL,nameId,parentNameId,x,y,width,height)

#define ldListSetHidden                ldBaseSetHidden
#define ldListMove                     ldBaseMove
#define ldListSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
