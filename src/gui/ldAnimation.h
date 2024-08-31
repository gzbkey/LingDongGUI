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

#ifndef __LD_ANIMATION_H__
#define __LD_ANIMATION_H__

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
#ifdef __LD_ANIMATION_IMPLEMENT__
#undef __LD_ANIMATION_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_ANIMATION_INHERIT__)
#undef __LD_ANIMATION_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldAnimation_t ldAnimation_t;

struct ldAnimation_t
{
    implement(ldBase_t);

    arm_2d_tile_t *ptImgTile;
    arm_2d_region_t showRegion;
    int64_t frameTimer;
    uint16_t periodMs;
};

ldAnimation_t* ldAnimation_init(ld_scene_t *ptScene, ldAnimation_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_tile_t *ptImgTile, uint16_t periodMs);
void ldAnimation_depose( ldAnimation_t *ptWidget);
void ldAnimation_on_load( ldAnimation_t *ptWidget);
void ldAnimation_on_frame_start( ldAnimation_t *ptWidget);
void ldAnimation_show(ld_scene_t *pScene, ldAnimation_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

#define ldAnimationInit(nameId,parentNameId,x,y,width,height,ptImgTile,periodMs) \
        ldAnimation_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptImgTile,periodMs)

#define ldAnimationSetHidden                ldBaseSetHidden
#define ldAnimationMove                     ldBaseMove
#define ldAnimationSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
