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

#ifndef __LD_PROGRESS_WHEEL_H__
#define __LD_PROGRESS_WHEEL_H__

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

#define __PROGRESS_WHEEL_IMPLEMENT__

/* OOC header, please DO NOT modify  */
#ifdef __LD_PROGRESS_WHEEL_IMPLEMENT__
#undef __LD_PROGRESS_WHEEL_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_PROGRESS_WHEEL_INHERIT__)
#undef __LD_PROGRESS_WHEEL_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldProgressWheel_t ldProgressWheel_t;

struct ldProgressWheel_t
{
    implement(ldBase_t);
    int16_t iProgress;
    progress_wheel_t tWheel;
};

ldProgressWheel_t* ldProgressWheel_init(ld_scene_t *ptScene, ldProgressWheel_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
void ldProgressWheel_depose(ld_scene_t *ptScene, ldProgressWheel_t *ptWidget);
void ldProgressWheel_on_load(ld_scene_t *ptScene, ldProgressWheel_t *ptWidget);
void ldProgressWheel_on_frame_start(ld_scene_t *ptScene, ldProgressWheel_t *ptWidget);
void ldProgressWheel_on_frame_complete(ld_scene_t *ptScene, ldProgressWheel_t *ptWidget);
void ldProgressWheel_show(ld_scene_t *pScene, ldProgressWheel_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

#define ldProgressWheelInit(nameId,parentNameId,x,y,width,height) \
        ldProgressWheel_init(ptScene,NULL,nameId,parentNameId,x,y,width,height)

void ldProgressWheelSetProgress(ldProgressWheel_t *ptWidget,int16_t value);//0-1000
void ldProgressWheelSetWheelColor(ldProgressWheel_t *ptWidget,ldColor wheelColor);
void ldProgressWheelSetDotColor(ldProgressWheel_t *ptWidget,ldColor dotColor,bool isEnable);

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
