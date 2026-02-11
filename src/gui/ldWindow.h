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

#ifndef __LD_WINDOW_H__
#define __LD_WINDOW_H__

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
#ifdef __LD_WINDOW_IMPLEMENT__
#undef __LD_WINDOW_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_WINDOW_INHERIT__)
#undef __LD_WINDOW_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldWindow_t ldWindow_t;

typedef struct ldPadding_t
{
    int16_t left;
    int16_t top;
    int16_t right;
    int16_t bottom;
}ldPadding_t;

struct ldWindow_t
{
    implement(ldBase_t);
    ldColor bgColor;
    arm_2d_tile_t *ptImgTile;
    arm_2d_tile_t *ptMaskTile;
    bool isTransparent:1;
    ldPadding_t *pLayoutPaddingGroup;
    ldLayoutType_t layoutTpye:2;
    bool isLayoutUpdate:1;
};

ldWindow_t* ldWindow_init(ld_scene_t *ptScene, ldWindow_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
#define ldWindowInit(nameId,parentNameId,x,y,width,height) \
        ldWindow_init(ptScene,NULL,nameId,parentNameId,x,y,width,height)
void ldWindow_depose(ld_scene_t *ptScene, ldWindow_t *ptWidget);
void ldWindow_on_load(ld_scene_t *ptScene, ldWindow_t *ptWidget);
void ldWindow_on_frame_start(ld_scene_t *ptScene, ldWindow_t *ptWidget);
void ldWindow_on_frame_complete(ld_scene_t *ptScene, ldWindow_t *ptWidget);
void ldWindow_show(ld_scene_t *pScene, ldWindow_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldWindowSetColor(ldWindow_t *ptWidget,ldColor bgColor);
void ldWindowSetImage(ldWindow_t *ptWidget, arm_2d_tile_t* ptImgTile, arm_2d_tile_t* ptMaskTile);

void ldWindowSetLayout(ldWindow_t *ptWidget, ldLayoutType_t type);
void ldWindowSetPaddingGroup(ldWindow_t *ptWidget, ldPadding_t *pPaddingGroup);//Local variables forbidden

ldColor ldWindowGetColor(ldWindow_t *ptWidget);


#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
