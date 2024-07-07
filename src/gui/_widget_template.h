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

#ifndef __<CONTROL_NAME>_H__
#define __<CONTROL_NAME>_H__


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
#ifdef __<CONTROL_NAME>_IMPLEMENT__
#undef __<CONTROL_NAME>_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__<CONTROL_NAME>_INHERIT__)
#undef __<CONTROL_NAME>_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct template_t template_t;

struct template_t
{

ARM_PRIVATE(
    arm_2d_scene_t *ptScene;
)
    /* place your public member here */
    
};

extern
ARM_NONNULL(1)
template_t* template_init(arm_2d_scene_t *ptScene, template_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);

void template_depose( template_t *ptWidget);

void template_on_load( template_t *ptWidget);

void template_on_frame_start( template_t *ptWidget);

void template_show(arm_2d_scene_t *pScene, template_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
