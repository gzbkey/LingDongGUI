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

typedef struct {
    arm_2d_tile_t       *ptImgTile;
    arm_2d_tile_t       *ptMaskTile;
    ldColor             maskColor;
    arm_2d_op_trans_msk_opa_t op;
    arm_2d_point_float_t rotationCentre;
    float               radian;
} ldClockPointer_t;

struct ldClock_t
{
    implement(ldBase_t);
    ldClockPointer_t pointerInfo[3];
    arm_2d_tile_t *ptBgImgTile;
    arm_2d_tile_t *ptBgMaskTile;
    ldColor bgMaskColor;
    uint32_t lastTotalSeconds;
    bool isStepSecond:1;
};

ldClock_t* ldClock_init(ld_scene_t *ptScene, ldClock_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
#define ldClockInit(nameId,parentNameId,x,y,width,height) \
        ldClock_init(ptScene,NULL,nameId,parentNameId,x,y,width,height)
void ldClock_depose(ld_scene_t *ptScene, ldClock_t *ptWidget);
void ldClock_on_load(ld_scene_t *ptScene, ldClock_t *ptWidget);
void ldClock_on_frame_start(ld_scene_t *ptScene, ldClock_t *ptWidget);
void ldClock_on_frame_complete(ld_scene_t *ptScene, ldClock_t *ptWidget);
void ldClock_show(ld_scene_t *pScene, ldClock_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldClockSetBackgroundImage(ldClock_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile, ldColor maskColor);
void ldClockSetHourPointerImage(ldClock_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile, ldColor maskColor, float x, float y);
void ldClockSetMinutePointerImage(ldClock_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile, ldColor maskColor, float x, float y);
void ldClockSetSecondPointerImage(ldClock_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile, ldColor maskColor, float x, float y);

void ldClockSetStepSecond(ldClock_t *ptWidget, bool isStepSecond);

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
