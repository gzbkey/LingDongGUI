/*
 * Copyright (c) 2023-2025 Ou Jianbo (59935554@qq.com). All rights reserved.
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

#ifndef __LD_CLOCK_H__
#define __LD_CLOCK_H__

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
#ifdef __LD_CLOCK_IMPLEMENT__
#undef __LD_CLOCK_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_CLOCK_INHERIT__)
#undef __LD_CLOCK_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldClock_t ldClock_t;

struct ldClock_t
{
    implement(ldBase_t);
    arm_2d_tile_t *ptBgImgTile;
    arm_2d_tile_t *ptBgMaskTile;
    ldColor maskColor;
    spin_zoom_widget_t tPointers[2];
    meter_pointer_t tSecPointer;
    uint32_t wHour;
    uint32_t wMin;
    int32_t nSec;
};

ldClock_t* ldClock_init(ld_scene_t *ptScene, ldClock_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
#define ldClockInit(nameId,parentNameId,x,y,width,height) \
        ldClock_init(ptScene,NULL,nameId,parentNameId,x,y,width,height)
void ldClock_depose(ld_scene_t *ptScene, ldClock_t *ptWidget);
void ldClock_on_load(ld_scene_t *ptScene, ldClock_t *ptWidget);
void ldClock_on_frame_start(ld_scene_t *ptScene, ldClock_t *ptWidget);
void ldClock_on_frame_complete(ld_scene_t *ptScene, ldClock_t *ptWidget);
void ldClock_show(ld_scene_t *pScene, ldClock_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldClockSetBackground(ldClock_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile);
void ldClockSetMaskColor(ldClock_t *ptWidget, ldColor maskColor);
void ldClockSetHourPointer(ldClock_t *ptWidget, arm_2d_tile_t *ptHourPointerMaskTile, ldColor pointerColor, float x, float y);
void ldClockSetMinutePointer(ldClock_t *ptWidget, arm_2d_tile_t *ptMinutePointerMaskTile, ldColor pointerColor, float x, float y);
void ldClockSetSecondPointer(ldClock_t *ptWidget, arm_2d_tile_t *ptSecondPointerMaskTile, ldColor pointerColor, float x, float y);

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
