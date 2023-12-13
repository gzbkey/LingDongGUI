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

#ifndef _LD_GAUGE_H_
#define _LD_GAUGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isWithBgMask:1;
    uint8_t pointerImgType:2;
    uint32_t bgImgAddr;
    uint32_t pointerImgAddr;
    int16_t pointerWidth;
    int16_t pointerHeight;
    int16_t centreOffsetX;//0 = centre
    int16_t centreOffsetY;//0 = centre
    int16_t pointerOriginOffsetX;
    int16_t pointerOriginOffsetY;
    int16_t angle_x10;
    ldColor keyingOrMaskColor;
    arm_2d_op_trans_msk_opa_t op;
    arm_2d_region_t targetDirtyRegion;

//    arm_2d_helper_transform_t helper;
}ldGauge_t;

typedef enum{
    nomal,
    withMask,
    onlyMask,
    keying
} gaugePointerType_t;

ldGauge_t* ldGaugeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint32_t bgImgAddr, bool isBgMask);
void ldGaugeLoop(ldGauge_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldGaugeDel(ldGauge_t *pWidget);
void ldGaugeSetPointerImage(ldGauge_t *pWidget,uint32_t pointerImgAddr,int16_t pointerWidth,int16_t pointerHeight,int16_t pointerOriginOffsetX,int16_t pointerOriginOffsetY);
void ldGaugeSetCenterOffset(ldGauge_t *pWidget, int16_t centreOffsetX, int16_t centreOffsetY);
void ldGaugeSetAngle(ldGauge_t *pWidget, float angle);
void ldGaugeSetPointerImageType(ldGauge_t *pWidget,gaugePointerType_t pointerImgType,ldColor keyingOrMaskColor);

#define ldGaugeSetHidden          ldBaseSetHidden
#define ldGaugeMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_GAUGE_H_
