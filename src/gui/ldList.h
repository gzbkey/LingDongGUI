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

//void listItemProcess(ldList_t *ptWidget,uint8_t itemNum,arm_2d_tile_t *ptTile,arm_2d_region_t *ptRegion,arm_2d_location_t clickPos,bool bIsNewFrame)
typedef void (*ldListItemFunc_t)(ldList_t *,uint8_t,arm_2d_tile_t*,arm_2d_region_t*,arm_2d_location_t,bool);

struct ldList_t
{
    implement(ldBase_t);

    arm_2d_location_t clickItemPos;
    ldListItemFunc_t ptItemFunc;
    int16_t offset;
    int16_t _offset;
    uint8_t itemHeight;
    uint8_t itemCount;
    uint8_t selectItem;
    bool isMoveReset:1;
    bool isHoldMove:1;
};

ldList_t* ldList_init(ld_scene_t *ptScene, ldList_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
void ldList_depose( ldList_t *ptWidget);
void ldList_on_load( ldList_t *ptWidget);
void ldList_on_frame_start( ldList_t *ptWidget);
void ldList_show(ld_scene_t *pScene, ldList_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldListSetItemFunc(ldList_t *ptWidget,ldListItemFunc_t ptItemFunc);

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