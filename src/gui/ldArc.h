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

#ifndef _LD_ARC_H_
#define _LD_ARC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isWaitRefresh:1;
    ldColor bgColor;
    ldColor fgColor;
    ldColor parentColor;
    uint32_t srcAddr;
    uint32_t maskAddr;
    uint16_t startAngle_x10;
    uint16_t endAngle_x10;
    uint16_t rotationAngle_x10;

}ldArc_t;

ldArc_t* ldArcInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint32_t srcQuarterAddr, uint32_t maskQuarterAddr, uint16_t parentColor);
void ldArcFrameUpdate(ldArc_t* pWidget);
void ldArcLoop(ldArc_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldArcDel(ldArc_t *pWidget);

void ldArcSetAngle(ldArc_t *pWidget,float bgStart,float bgEnd,float fgStart,float fgEnd);
void ldArcSetRotationAngle(ldArc_t *pWidget,float rotationAngle);

#define ldArcSetHidden          ldBaseSetHidden
#define ldArcMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_ARC_H_
