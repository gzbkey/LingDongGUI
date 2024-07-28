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

#ifndef __LD_GRAPH_H__
#define __LD_GRAPH_H__

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
#ifdef __LD_GRAPH_IMPLEMENT__
#undef __LD_GRAPH_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_GRAPH_INHERIT__)
#undef __LD_GRAPH_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct {
    ldColor seriesColor;
    uint16_t *pValueList;
    uint16_t valueCountMax;
    uint8_t lineSize;
}ldGraphSeries_t;

typedef struct ldGraph_t ldGraph_t;

struct ldGraph_t
{
    implement(ldBase_t);
    ARM_PRIVATE(
            ld_scene_t *ptScene;
    )
    uint16_t xAxisMax;
    uint16_t yAxisMax;
    uint16_t xAxisOffset;
    float xScale;
    float yScale;
    arm_2d_tile_t *ptPointMaskTile;
    uint8_t frameSpace;
    uint8_t seriesCount;
    uint8_t seriesMax;
    uint8_t gridOffset;
    ldGraphSeries_t *pSeries;
    bool isFrame:1;
    bool isCorner:1;
};

ldGraph_t* ldGraph_init(ld_scene_t *ptScene, ldGraph_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t seriesMax);
void ldGraph_depose( ldGraph_t *ptWidget);
void ldGraph_on_load( ldGraph_t *ptWidget);
void ldGraph_on_frame_start( ldGraph_t *ptWidget);
void ldGraph_show(ld_scene_t *pScene, ldGraph_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldGraphSetAxis(ldGraph_t *ptWidget, uint16_t xAxis, uint16_t yAxis, uint16_t xAxisOffset);
void ldGraphSetAxisOffset(ldGraph_t *ptWidget,uint16_t xAxisOffset);
void ldGraphSetFrameSpace(ldGraph_t *ptWidget, uint8_t frameSpace, bool isCorner);
void ldGraphSetGridOffset(ldGraph_t *ptWidget, uint8_t gridOffset);
void ldGraphSetPointImageMask(ldGraph_t *ptWidget, arm_2d_tile_t *ptPointMaskTile);
int8_t ldGraphAddSeries(ldGraph_t *ptWidget, ldColor seriesColor, uint8_t lineSize, uint16_t pointMax);
void ldGraphSetValue(ldGraph_t *ptWidget,uint8_t seriesNum,uint16_t valueNum,uint16_t value);
void ldGraphMoveAdd(ldGraph_t *ptWidget,uint8_t seriesNum,uint16_t newValue);

#define ldGraphInit(nameId,parentNameId,x,y,width,height,seriesMax) \
        ldGraph_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,seriesMax)

#define ldGraphSetHidden                ldBaseSetHidden
#define ldGraphMove                     ldBaseMove
#define ldGraphSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
