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

#define __LD_CALENDAR_IMPLEMENT__
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldCalendar.h"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wcast-qual"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wunused-const-variable"
#pragma clang diagnostic ignored "-Wmissing-declarations"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

const ldBaseWidgetFunc_t ldCalendarFunc = {
    .depose = (ldDeposeFunc_t)ldCalendar_depose,
    .load = (ldLoadFunc_t)ldCalendar_on_load,
    .frameStart = (ldFrameStartFunc_t)ldCalendar_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldCalendar_on_frame_complete,
    .show = (ldShowFunc_t)ldCalendar_show,
};

static inline int isLeap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 返回 y 年 m 月的天数
static int daysOfMonth(int year, int month)
{
    static const int t[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return t[month - 1] + (month == 2 && isLeap(year));
}

static void _getCalBuf(uint16_t year, uint8_t month, uint8_t *calBuf)
{
    int dim = daysOfMonth(year, month);                /* 本月天数 */
    int w1 = ldBaseGetWeek(year, month, 1);             /* 本月1日星期x，0=周日 */
    int pdim = daysOfMonth(year, month - (month > 1)); /* 上月天数 */

    int idx = 0;

    //上月
    for (int i = w1; i > 0; --i)
    {
        calBuf[idx++] = pdim - i + 1;
    }

    //本月
    for (int d = 1; d <= dim; ++d)
    {
        calBuf[idx++] = d | 0x80;
    }

    //下月
    for (int d = 1; idx < 42; ++d)
    {
        calBuf[idx++] = d;
    }
}

static const char *dayStr[32] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};

ldCalendar_t *ldCalendar_init(ld_scene_t *ptScene, ldCalendar_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont, uint16_t year, uint8_t month, uint8_t day)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldCalendar_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][calendar] id:%d", nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldCalendar_t));
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot, parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeCalendar;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldCalendarFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity = 255;

    ptWidget->bgColor = GLCD_COLOR_WHITE;
    ptWidget->textColor = GLCD_COLOR_BLACK;
    ptWidget->itemColor = __RGB(160, 210, 250);
    ptWidget->ptFont = ptFont;

    ptWidget->year = year - 2000;
    ptWidget->month = month;
    ptWidget->day = day;

    _getCalBuf(year, month, ptWidget->calBuf);

    LOG_INFO("[init][calendar] id:%d, size:%d", nameId, (int)sizeof(*ptWidget));
    return ptWidget;
}

void ldCalendar_depose(ld_scene_t *ptScene, ldCalendar_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if (ptWidget->use_as__ldBase_t.widgetType != widgetTypeCalendar)
    {
        return;
    }

    LOG_INFO("[depose][calendar] id:%d", ptWidget->use_as__ldBase_t.nameId);

    if (ptWidget->isStatic == false && ptWidget->dayNames != NULL)
    {
        for (int i = 0; i < 7; i++)
        {
            if (ptWidget->dayNames[i] != NULL)
            {
                ldFree(ptWidget->dayNames[i]);
            }
        }
        ldFree(ptWidget->dayNames);
    }

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t *)ptWidget);

    ldFree(ptWidget);
}

void ldCalendar_on_load(ld_scene_t *ptScene, ldCalendar_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
}

void ldCalendar_on_frame_start(ld_scene_t *ptScene, ldCalendar_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
}

void ldCalendar_on_frame_complete(ld_scene_t *ptScene, ldCalendar_t *ptWidget)
{
    assert(NULL != ptWidget);
}

void ldCalendar_show(ld_scene_t *ptScene, ldCalendar_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    arm_2d_region_t globalRegion;
    arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t *)ptWidget, &globalRegion, true);

    if (arm_2d_helper_pfb_is_region_active(ptTile, &globalRegion, true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if (ldBaseIsHidden((ldBase_t *)ptWidget))
            {
                break;
            }

            if (!ptWidget->isTransparent)
            {
                if (ptWidget->ptBgImgTile == NULL) // color
                {
                    if (ptWidget->use_as__ldBase_t.isCorner)
                    {
                        draw_round_corner_box(&tTarget,
                                              NULL,
                                              ptWidget->bgColor,
                                              ptWidget->use_as__ldBase_t.opacity,
                                              bIsNewFrame);
                    }
                    else
                    {
                        ldBaseColor(&tTarget,
                                    NULL,
                                    ptWidget->bgColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
                }
                else
                {
                    if (ptWidget->use_as__ldBase_t.isCorner)
                    {
                        draw_round_corner_image(ptWidget->ptBgImgTile,
                                                &tTarget,
                                                &ptWidget->ptBgImgTile->tRegion,
                                                bIsNewFrame,
                                                ptWidget->use_as__ldBase_t.opacity);
                    }
                    else
                    {
                        ldBaseImage(&tTarget,
                                    &ptWidget->ptBgImgTile->tRegion,
                                    ptWidget->ptBgImgTile,
                                    ptWidget->ptBgMaskTile,
                                    ptWidget->bgColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
                }
                arm_2d_op_wait_async(NULL);
            }

#define SPACE_SIZE 2

            uint8_t rowHeight;
            uint8_t headerHgitht;
            if (ptWidget->isHeader)
            {
                rowHeight = ((tTarget_canvas.tSize.iHeight) - (SPACE_SIZE * 9)) / 8;
                headerHgitht = rowHeight + SPACE_SIZE;
            }
            else
            {
                rowHeight = ((tTarget_canvas.tSize.iHeight) - (SPACE_SIZE * 8)) / 7;
                headerHgitht = 0;
            }

            arm_2d_region_t cellRegion = {
                .tLocation = {
                    .iX = SPACE_SIZE,
                    .iY = SPACE_SIZE},
                .tSize = {.iWidth = (tTarget_canvas.tSize.iWidth - (SPACE_SIZE * 8)) / 7, .iHeight = rowHeight}};

            // 月份
            if (ptWidget->isHeader)
            {
                cellRegion.tLocation.iY += headerHgitht;

                arm_2d_region_t headerRegion = {
                    .tLocation = {
                        .iX = SPACE_SIZE,
                        .iY = SPACE_SIZE},
                    .tSize = {.iWidth = tTarget_canvas.tSize.iWidth - (SPACE_SIZE * 2), .iHeight = rowHeight}};

                char tempBuf[32] = {0};

                char *addr;
                int ret;
                char strTemp[5];

                strcpy(tempBuf, (char *)ptWidget->headerNameFormat);

                addr = strstr(tempBuf, "yyyy");
                if (addr)
                {
                    ret = snprintf(strTemp, 5, "%04d", ptWidget->year + 2000);
                    if (ret > 0)
                    {
                        memcpy(addr, strTemp, 4);
                    }
                }
                addr = strstr(tempBuf, "mm");
                if (addr)
                {
                    ret = snprintf(strTemp, 3, "%02d", ptWidget->month);
                    if (ret > 0)
                    {
                        memcpy(addr, strTemp, 2);
                    }
                }
                addr = strstr(tempBuf, "dd");
                if (addr)
                {
                    ret = snprintf(strTemp, 3, "%02d", ptWidget->day);
                    if (ret > 0)
                    {
                        memcpy(addr, strTemp, 2);
                    }
                }

                ldBaseLabel(&tTarget,
                            &headerRegion,
                            (uint8_t*)tempBuf,
                            ptWidget->ptFont,
                            ARM_2D_ALIGN_CENTRE,
                            ptWidget->textColor,
                            ptWidget->use_as__ldBase_t.opacity);
            }

            // 星期
            for (uint8_t i = 0; i < 7; i++)
            {

                ldBaseLabel(&tTarget,
                            &cellRegion,
                            ptWidget->dayNames[i],
                            ptWidget->ptFont,
                            ARM_2D_ALIGN_CENTRE,
                            ptWidget->textColor,
                            ptWidget->use_as__ldBase_t.opacity);
                cellRegion.tLocation.iX += SPACE_SIZE + cellRegion.tSize.iWidth;
            }

            arm_2d_op_wait_async(NULL);

            // 日期
            cellRegion.tLocation.iX = SPACE_SIZE;
            cellRegion.tLocation.iY = headerHgitht + SPACE_SIZE + cellRegion.tSize.iHeight;

            for (uint8_t week = 0; week < 6; week++)
            {
                for (uint8_t day = 0; day < 7; day++)
                {
                    uint8_t dayNum = ptWidget->calBuf[week * 7 + day];

                    if (ptWidget->day == (dayNum - 0x80))
                    {
                        if (ptWidget->ptItemImgTile == NULL) // color
                        {
                            if (ptWidget->use_as__ldBase_t.isCorner)
                            {
                                draw_round_corner_box(&tTarget,
                                                      &cellRegion,
                                                      ptWidget->itemColor,
                                                      ptWidget->use_as__ldBase_t.opacity,
                                                      bIsNewFrame);
                            }
                            else
                            {
                                ldBaseColor(&tTarget,
                                            &cellRegion,
                                            ptWidget->itemColor,
                                            ptWidget->use_as__ldBase_t.opacity);
                            }
                        }
                        else
                        {
                            if (ptWidget->use_as__ldBase_t.isCorner)
                            {
                                draw_round_corner_image(ptWidget->ptItemImgTile,
                                                        &tTarget,
                                                        &cellRegion,
                                                        bIsNewFrame,
                                                        ptWidget->use_as__ldBase_t.opacity);
                            }
                            else
                            {
                                ldBaseImage(&tTarget,
                                            &cellRegion,
                                            ptWidget->ptItemImgTile,
                                            ptWidget->ptItemMaskTile,
                                            ptWidget->itemColor,
                                            ptWidget->use_as__ldBase_t.opacity);
                            }
                        }
                    }

                    ldColor textColor = ptWidget->textColor;
                    if (dayNum & 0x80)
                    {
                        dayNum -= 0x80;
                    }
                    else
                    {
                        textColor = GLCD_COLOR_LIGHT_GREY;
                    }

                    ldBaseLabel(&tTarget,
                                &cellRegion,
                                (uint8_t *)dayStr[dayNum],
                                ptWidget->ptFont,
                                ARM_2D_ALIGN_CENTRE,
                                textColor,
                                ptWidget->use_as__ldBase_t.opacity);

                    cellRegion.tLocation.iX += SPACE_SIZE + cellRegion.tSize.iWidth;
                }
                cellRegion.tLocation.iX = SPACE_SIZE;
                cellRegion.tLocation.iY += SPACE_SIZE + cellRegion.tSize.iHeight;
            }
            LD_BASE_WIDGET_SELECT;
            arm_2d_op_wait_async(NULL);
        }
    }
}

void ldCalendarSetDayNames(ldCalendar_t *ptWidget, uint8_t *names[7])
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    if (ptWidget->isStatic == false && ptWidget->dayNames != NULL)
    {
        for (int i = 0; i < 7; i++)
        {
            if (ptWidget->dayNames[i] != NULL)
            {
                ldFree(ptWidget->dayNames[i]);
            }
        }
        ldFree(ptWidget->dayNames);
    }

    uint8_t **pNames = (uint8_t **)ldCalloc(7, sizeof(uint8_t *));
    if (pNames == NULL)
    {
        LOG_ERROR("[set names failed][calendar] id:%d", ptWidget->use_as__ldBase_t.nameId);
        return;
    }

    for (int i = 0; i < 7; i++)
    {
        if (names[i] != NULL)
        {
            uint8_t len = strlen((const char *)names[i]) + 1;
            pNames[i] = (uint8_t *)ldMalloc(len);
            if (pNames[i] != NULL)
            {
                memcpy(pNames[i], names[i], len);
            }
            else
            {
                LOG_ERROR("[set names failed][calendar] id:%d, day:%d", ptWidget->use_as__ldBase_t.nameId, i);
            }
        }
        else
        {
            pNames[i] = NULL;
        }
    }

    ptWidget->dayNames = pNames;
    ptWidget->isStatic = false;
}

void ldCalendarSetStaticDayNames(ldCalendar_t *ptWidget, uint8_t *names[7])
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if (ptWidget->isStatic == false && ptWidget->dayNames != NULL)
    {
        for (int i = 0; i < 7; i++)
        {
            if (ptWidget->dayNames[i] != NULL)
            {
                ldFree(ptWidget->dayNames[i]);
            }
        }
        ldFree(ptWidget->dayNames);
    }
    ptWidget->dayNames = names;
    ptWidget->isStatic = true;
}

void ldCalendarSetHeader(ldCalendar_t *ptWidget, bool isEnable)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    ptWidget->isHeader = isEnable;
}

void ldCalendarSetHeaderFormat(ldCalendar_t *ptWidget, uint8_t *format)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL || format == NULL)
    {
        return;
    }
    ptWidget->headerNameFormat = format;
}

void ldCalendarSetDate(ldCalendar_t *ptWidget,uint16_t year,uint8_t month,uint8_t day)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    ptWidget->year = year - 2000;
    ptWidget->month = month;
    ptWidget->day = day;

    _getCalBuf(year, month, ptWidget->calBuf);
}

void ldCalendarGetDate(ldCalendar_t *ptWidget,uint16_t* year,uint8_t* month,uint8_t* day)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(year != NULL) {
        *year = ptWidget->year + 2000;
    }
    if(month != NULL) {
        *month = ptWidget->month;
    }
    if(day != NULL) {
        *day = ptWidget->day;
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
