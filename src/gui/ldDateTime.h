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

#ifndef __LD_DATE_TIME_H__
#define __LD_DATE_TIME_H__

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
#ifdef __LD_DATE_TIME_IMPLEMENT__
#undef __LD_DATE_TIME_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_DATE_TIME_INHERIT__)
#undef __LD_DATE_TIME_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

#define DATE_TIME_BUFFER_SIZE   32

typedef struct ldDateTime_t ldDateTime_t;

struct ldDateTime_t
{
    implement(ldBase_t);
ARM_PRIVATE(
    ld_scene_t *ptScene;
)
    arm_2d_align_t tAlign;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t year;
    const uint8_t *pFormatStr;
    uint8_t formatStrTemp[DATE_TIME_BUFFER_SIZE];
    ldColor bgColor;
    ldColor textColor;
    arm_2d_font_t *ptFont;
    bool isTransparent:1;
};

ldDateTime_t* ldDateTime_init(ld_scene_t *ptScene, ldDateTime_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont);
void ldDateTime_depose( ldDateTime_t *ptWidget);
void ldDateTime_on_load( ldDateTime_t *ptWidget);
void ldDateTime_on_frame_start( ldDateTime_t *ptWidget);
void ldDateTime_show(ld_scene_t *pScene, ldDateTime_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldDateTimeSetTransparent(ldDateTime_t* ptWidget,bool isTransparent);
void ldDateTimeSetFormat(ldDateTime_t* ptWidget,const uint8_t *pStr);
void ldDateTimeSetTextColor(ldDateTime_t* ptWidget,ldColor textColor);
void ldDateTimeSetAlign(ldDateTime_t *ptWidget,arm_2d_align_t tAlign);
void ldDateTimeSetBgColor(ldDateTime_t *ptWidget, ldColor bgColor);
void ldDateTimeSetDate(ldDateTime_t *ptWidget, uint16_t year, uint8_t month, uint8_t day);
void ldDateTimeSetTime(ldDateTime_t *ptWidget, uint8_t hour, uint8_t minute, uint8_t second);

#define ldDateTimeInit(nameId,parentNameId,x,y,width,height,ptFont) \
        ldDateTime_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptFont)

#define ldDateTimeSetHidden                ldBaseSetHidden
#define ldDateTimeMove                     ldBaseMove
#define ldDateTimeSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
