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

#ifndef __LD_ARC_H__
#define __LD_ARC_H__

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
#ifdef __LD_ARC_IMPLEMENT__
#undef __LD_ARC_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_ARC_INHERIT__)
#undef __LD_ARC_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldArc_t ldArc_t;

struct ldArc_t
{
    implement(ldBase_t);
//    ARM_PRIVATE(
//            ld_scene_t *ptScene;
//    )
    arm_2d_tile_t *ptImgTile;
    arm_2d_tile_t *ptMaskTile;
    ldColor color[2];
    ldColor parentColor;
    uint16_t startAngle_x10[2];
    uint16_t endAngle_x10[2];
    uint16_t rotationAngle_x10;
    bool isClockwise:1;
};

ldArc_t* ldArc_init(ld_scene_t *ptScene, ldArc_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,arm_2d_tile_t *ptQuarterImgTile,arm_2d_tile_t *ptQuarterMaskTile,ldColor parentColor);
void ldArc_depose( ldArc_t *ptWidget);
void ldArc_on_load( ldArc_t *ptWidget);
void ldArc_on_frame_start( ldArc_t *ptWidget);
void ldArc_show(ld_scene_t *pScene, ldArc_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldArcSetBgAngle(ldArc_t *pWidget,float bgStart,float bgEnd);
void ldArcSetFgAngle(ldArc_t *pWidget,float fgEnd);
void ldArcSetRotationAngle(ldArc_t *pWidget,float rotationAngle);
void ldArcSetColor(ldArc_t *pWidget,ldColor bgColor,ldColor fgColor);

#define ldArcInit(nameId,parentNameId,x,y,width,height,ptQuarterImgTile,ptQuarterMaskTile,parentColor) \
        ldArc_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptQuarterImgTile,ptQuarterMaskTile,parentColor)

#define ldArcSetHidden                ldBaseSetHidden
#define ldArcMove                     ldBaseMove
#define ldArcSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
