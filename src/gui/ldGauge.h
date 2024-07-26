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

#ifndef __LD_GAUGE_H__
#define __LD_GAUGE_H__

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
#ifdef __LD_GAUGE_IMPLEMENT__
#undef __LD_GAUGE_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_GAUGE_INHERIT__)
#undef __LD_GAUGE_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldGauge_t ldGauge_t;

struct ldGauge_t
{
    implement(ldBase_t);
    ARM_PRIVATE(
            ld_scene_t *ptScene;
    )
    arm_2d_tile_t *ptBgImgTile;
    arm_2d_tile_t *ptBgMaskTile;
    arm_2d_tile_t *ptPointerImgTile;
    arm_2d_tile_t *ptPointerMaskTile;
    int16_t centreOffsetX;//0 = centre
    int16_t centreOffsetY;//0 = centre
    int16_t pointerOriginOffsetX;
    int16_t pointerOriginOffsetY;
    int16_t angle_x10;
    ldColor keyingOrMaskColor;
    arm_2d_op_trans_msk_opa_t op;
    arm_2d_region_t targetDirtyRegion;
    bool isKeying:1;
};

ldGauge_t* ldGauge_init(ld_scene_t *ptScene, ldGauge_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_tile_t *ptBgImgTile, arm_2d_tile_t *ptBgMaskTile, int16_t centreOffsetX, int16_t centreOffsetY);
void ldGauge_depose( ldGauge_t *ptWidget);
void ldGauge_on_load( ldGauge_t *ptWidget);
void ldGauge_on_frame_start( ldGauge_t *ptWidget);
void ldGauge_show(ld_scene_t *pScene, ldGauge_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldGaugeSetPointerImage(ldGauge_t *ptWidget,arm_2d_tile_t *ptPointerImgTile,arm_2d_tile_t *ptPointerMaskTile,int16_t pointerOriginOffsetX,int16_t pointerOriginOffsetY);
void ldGaugeSetPointerColor(ldGauge_t *ptWidget,ldColor color);
void ldGaugeSetAngle(ldGauge_t *ptWidget, float angle);

#define ldGaugeInit(nameId,parentNameId,x,y,width,height,ptBgImgTile,ptBgMaskTile,centreOffsetX,centreOffsetY) \
        ldGauge_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptBgImgTile,ptBgMaskTile,centreOffsetX,centreOffsetY)

#define ldGaugeSetHidden                ldBaseSetHidden
#define ldGaugeMove                     ldBaseMove
#define ldGaugeSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
