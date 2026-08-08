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

#ifndef __LD_CALENDAR_H__
#define __LD_CALENDAR_H__

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
#ifdef __LD_CALENDAR_IMPLEMENT__
#undef __LD_CALENDAR_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_CALENDAR_INHERIT__)
#undef __LD_CALENDAR_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldCalendar_t ldCalendar_t;

typedef enum ldCalendarDateFormat_t {
    YYYY_MM = 0,
    YYYY_MM_CN,
} ldCalendarDateFormat_t;

struct ldCalendar_t
{
    implement(ldBase_t);
    arm_2d_tile_t *ptBgImgTile;
    arm_2d_tile_t *ptBgMaskTile;
    arm_2d_tile_t *ptItemImgTile;
    arm_2d_tile_t *ptItemMaskTile;
    arm_2d_font_t *ptFont;
    ldColor bgColor;
    ldColor itemColor;
    ldColor textColor;
    uint8_t **dayNames;
    ldCalendarDateFormat_t dateFormatType;
    uint8_t calBuf[42];
    uint8_t year;
    uint8_t month;
    uint8_t day;
    bool isTransparent : 1;
    bool isHeader:1;
    bool isStatic:1;
};

ldCalendar_t* ldCalendar_init(ld_scene_t *ptScene, ldCalendar_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont, uint16_t year, uint8_t month, uint8_t day);
#define ldCalendarInit(nameId,parentNameId,x,y,width,height,ptFont,year,month,day) \
        ldCalendar_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,ptFont,year,month,day)
void ldCalendar_depose(ld_scene_t *ptScene, ldCalendar_t *ptWidget);
void ldCalendar_on_load(ld_scene_t *ptScene, ldCalendar_t *ptWidget);
void ldCalendar_on_frame_start(ld_scene_t *ptScene, ldCalendar_t *ptWidget);
void ldCalendar_on_frame_complete(ld_scene_t *ptScene, ldCalendar_t *ptWidget);
void ldCalendar_show(ld_scene_t *pScene, ldCalendar_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldCalendarSetDayNames(ldCalendar_t *ptWidget,uint8_t* names[7]);
void ldCalendarSetStaticDayNames(ldCalendar_t *ptWidget,uint8_t* names[7]);
void ldCalendarSetHeader(ldCalendar_t *ptWidget,bool isEnable);
void ldCalendarSetDateFormat(ldCalendar_t *ptWidget, ldCalendarDateFormat_t formatType);
void ldCalendarSetDate(ldCalendar_t *ptWidget,uint16_t year,uint8_t month,uint8_t day);

void ldCalendarGetDate(ldCalendar_t *ptWidget,uint16_t* year,uint8_t* month,uint8_t* day);


#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
