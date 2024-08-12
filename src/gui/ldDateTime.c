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

#define __LD_DATE_TIME_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldDateTime.h"

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

const ldBaseWidgetFunc_t ldDateTimeFunc = {
    .depose = (ldDeposeFunc_t)ldDateTime_depose,
    .load = (ldLoadFunc_t)ldDateTime_on_load,
    .frameStart = (ldFrameStartFunc_t)ldDateTime_on_frame_start,
    .show = (ldShowFunc_t)ldDateTime_show,
};

static uint8_t defaultFormat[]="yyyy-mm-dd hh:nn:ss";

ldDateTime_t* ldDateTime_init(ld_scene_t *ptScene, ldDateTime_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldDateTime_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][dateTime] id:%d", nameId);
            return NULL;
        }
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

//    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeDateTime;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldDateTimeFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->ptFont=ptFont;
    ptWidget->tAlign=ARM_2D_ALIGN_CENTRE;
    ptWidget->isTransparent=true;
    ptWidget->year=2024;
    ptWidget->month=1;
    ptWidget->day=1;
    ptWidget->hour=12;
    ptWidget->minute=0;
    ptWidget->second=0;
    ptWidget->pFormatStr=defaultFormat;
    ptWidget->formatStrTemp[0]=0;

    LOG_INFO("[init][dateTime] id:%d, size:%llu", nameId,sizeof (*ptWidget));
    return ptWidget;
}

void ldDateTime_depose( ldDateTime_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeDateTime)
    {
        return;
    }

    LOG_INFO("[depose][dateTime] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldDateTime_on_load( ldDateTime_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldDateTime_on_frame_start( ldDateTime_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldDateTime_show(ld_scene_t *ptScene, ldDateTime_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    arm_2d_region_t globalRegion;
    arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t*)ptWidget,&globalRegion,false);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            if(bIsNewFrame)
            {
                char *addr;
                int ret;
                char strTemp[5];

                strcpy((char *)ptWidget->formatStrTemp,(char *)ptWidget->pFormatStr);

                addr=strstr((char *)ptWidget->formatStrTemp,"yyyy");
                if(addr)
                {
                    ret=snprintf(strTemp,5,"%04d",ptWidget->year);
                    if(ret > 0)
                    {
                        memcpy(addr,strTemp,4);
                    }
                }
                addr=strstr((char *)ptWidget->formatStrTemp,"mm");
                if(addr)
                {
                    ret=snprintf(strTemp,3,"%02d",ptWidget->month);
                    if(ret > 0)
                    {
                        memcpy(addr,strTemp,2);
                    }
                }
                addr=strstr((char *)ptWidget->formatStrTemp,"dd");
                if(addr)
                {
                    ret=snprintf(strTemp,3,"%02d",ptWidget->day);
                    if(ret > 0)
                    {
                        memcpy(addr,strTemp,2);
                    }
                }
                addr=strstr((char *)ptWidget->formatStrTemp,"hh");
                if(addr)
                {
                    ret=snprintf(strTemp,3,"%02d",ptWidget->hour);
                    if(ret > 0)
                    {
                        memcpy(addr,strTemp,2);
                    }
                }
                addr=strstr((char *)ptWidget->formatStrTemp,"nn");
                if(addr)
                {
                    ret=snprintf(strTemp,3,"%02d",ptWidget->minute);
                    if(ret > 0)
                    {
                        memcpy(addr,strTemp,2);
                    }
                }
                addr=strstr((char *)ptWidget->formatStrTemp,"ss");
                if(addr)
                {
                    ret=snprintf(strTemp,3,"%02d",ptWidget->second);
                    if(ret > 0)
                    {
                        memcpy(addr,strTemp,2);
                    }
                }
            }

            if(!ptWidget->isTransparent)
            {
                ldBaseColor(&tTarget,NULL,ptWidget->bgColor,ptWidget->use_as__ldBase_t.opacity);
                arm_2d_op_wait_async(NULL);
            }
            ldBaseLabel(&tTarget,&tTarget_canvas,ptWidget->formatStrTemp,ptWidget->ptFont,ptWidget->tAlign,ptWidget->textColor,ptWidget->use_as__ldBase_t.opacity);
            arm_2d_op_wait_async(NULL);
        }
    }
    arm_2d_op_wait_async(NULL);
}

void ldDateTimeSetTransparent(ldDateTime_t* ptWidget,bool isTransparent)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isTransparent=isTransparent;
}

void ldDateTimeSetFormat(ldDateTime_t* ptWidget,const uint8_t *pStr)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->pFormatStr=pStr;
}

void ldDateTimeSetTextColor(ldDateTime_t* ptWidget,ldColor textColor)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->textColor=textColor;
}

void ldDateTimeSetAlign(ldDateTime_t *ptWidget,arm_2d_align_t tAlign)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->tAlign=tAlign;
}

void ldDateTimeSetBgColor(ldDateTime_t *ptWidget, ldColor bgColor)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->bgColor=bgColor;
    ptWidget->isTransparent=false;
}

void ldDateTimeSetDate(ldDateTime_t *ptWidget, uint16_t year, uint8_t month, uint8_t day)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->year=year;
    ptWidget->month=month;
    ptWidget->day=day;
}

void ldDateTimeSetTime(ldDateTime_t *ptWidget, uint8_t hour, uint8_t minute, uint8_t second)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->hour=hour;
    ptWidget->minute=minute;
    ptWidget->second=second;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
