/*
 * Copyright 2023-2024 Ou Jianbo (59935554@qq.com)
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

#ifndef _LD_GRAPH_H_
#define _LD_GRAPH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    ldColor seriesColor;
    uint16_t *pValueList;
    uint16_t valueCountMax;
    uint8_t lineSize;
}ldGraphSeries_t;

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isFrame:1;
    bool isCorner:1;
    uint16_t xAxisMax;
    uint16_t yAxisMax;
    uint16_t xAxisOffset;
    float xScale;
    float yScale;
    uintptr_t pointImgAddr;
    int16_t pointImgWidth;
    uint8_t frameSpace;
    uint8_t seriesCount;
    uint8_t seriesMax;
    uint8_t gridOffset;
    ldGraphSeries_t *pSeries;

}ldGraph_t;

ldGraph_t* ldGraphInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t seriesMax);
void ldGraphFrameUpdate(ldGraph_t* pWidget);
void ldGraphLoop(ldGraph_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldGraphDel(ldGraph_t *pWidget);

void ldGraphSetAxis(ldGraph_t *pWidget, uint16_t xAxis, uint16_t yAxis, uint16_t xAxisOffset);
void ldGraphSetAxisOffset(ldGraph_t *pWidget,uint16_t xAxisOffset);
void ldGraphSetFrameSpace(ldGraph_t *pWidget, uint8_t frameSpace, bool isCorner);
void ldGraphSetGridOffset(ldGraph_t *pWidget, uint8_t gridOffset);
void ldGraphSetPointImageMask(ldGraph_t *pWidget, uintptr_t addr, int16_t width);
int8_t ldGraphAddSeries(ldGraph_t *pWidget, ldColor seriesColor, uint8_t lineSize, uint16_t pointMax);
void ldGraphSetValue(ldGraph_t *pWidget,uint8_t seriesNum,uint16_t valueNum,uint16_t value);
void ldGraphMoveAdd(ldGraph_t *pWidget,uint8_t seriesNum,uint16_t newValue);

#define ldGraphSetHidden          ldBaseSetHidden
#define ldGraphMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_GRAPH_H_
