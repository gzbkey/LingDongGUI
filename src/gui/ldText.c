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

#define __LD_TEXT_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldText.h"

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

const ldBaseWidgetFunc_t ldTextFunc = {
    .depose = (ldDeposeFunc_t)ldText_depose,
    .load = (ldLoadFunc_t)ldText_on_load,
    .frameStart = (ldFrameStartFunc_t)ldText_on_frame_start,
    .show = (ldShowFunc_t)ldText_show,
};

static bool slotTextVerticalScroll(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldText_t *ptWidget = msg.ptSender;

    switch (msg.signal)
    {
    case SIGNAL_PRESS:
    {
        ptWidget->isRelease=false;
        ptWidget->_scrollOffset=ptWidget->scrollOffset;
        ptWidget->_isTopScroll=false;
        ptWidget->_isBottomScroll=false;
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        ptWidget->scrollOffset=ptWidget->_scrollOffset+(int16_t)GET_SIGNAL_OFFSET_Y(msg.value);
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    case SIGNAL_RELEASE:
    {
        ptWidget->isRelease=true;

        ptWidget->_scrollOffset=ptWidget->scrollOffset;

        if(ptWidget->scrollOffset>0)
        {
            ptWidget->_isTopScroll=true;
            ptWidget->_isBottomScroll=false;
        }

        if(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight>(ptWidget->strHeight+ptWidget->scrollOffset))
        {
            ptWidget->_scrollOffset=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-(ptWidget->strHeight+ptWidget->scrollOffset);
            ptWidget->_isTopScroll=false;
            ptWidget->_isBottomScroll=true;
        }

        if(ptWidget->strHeight<=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight)
        {
            ptWidget->_isTopScroll=true;
            ptWidget->_isBottomScroll=false;
        }

        do {
            static const arm_2d_helper_pi_slider_cfg_t tCFG = {
                .fProportion = 0.2f,
                .fIntegration = 0.1f,
                .nInterval = 10,
            };
            arm_2d_helper_pi_slider_init(&ptWidget->tPISlider, (arm_2d_helper_pi_slider_cfg_t *)&tCFG, 0);
        } while(0);
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    default:
        break;
    }

    return false;
}

ldText_t* ldText_init( ld_scene_t *ptScene,ldText_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,arm_2d_font_t *ptFont,bool isScroll)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldText_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][text] id:%d", nameId);
            return NULL;
        }
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeText;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldTextFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->bgColor=__RGB(255,255,255);
    ptWidget->ptFont=ptFont;
    ptWidget->isRelease=true;

    if(isScroll)
    {
        connect(ptWidget->use_as__ldBase_t.nameId,SIGNAL_PRESS,slotTextVerticalScroll);
        connect(ptWidget->use_as__ldBase_t.nameId,SIGNAL_HOLD_DOWN,slotTextVerticalScroll);
        connect(ptWidget->use_as__ldBase_t.nameId,SIGNAL_RELEASE,slotTextVerticalScroll);
    }

    LOG_INFO("[init][text] id:%d", nameId);
    return ptWidget;
}

void ldText_depose( ldText_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeText)
    {
        return;
    }

    LOG_INFO("[depose][text] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove(ptWidget->ptScene->ptNodeRoot,(arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget->pStr);
    ldFree(ptWidget);
}

void ldText_on_load( ldText_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldText_on_frame_start( ldText_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldText_show(ld_scene_t *ptScene, ldText_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

#if 0
    if (bIsNewFrame) {
        
    }
#endif
    if(arm_2d_helper_pfb_is_region_active(ptTile,&ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            if(ptWidget->isRelease)
            {
                int32_t iResult;
                bool isPiEnd;

                isPiEnd=arm_2d_helper_pi_slider(&ptWidget->tPISlider, ptWidget->_scrollOffset, &iResult);

                if(ptWidget->_isTopScroll)
                {
                    if(isPiEnd)
                    {
                        ptWidget->isRelease=false;
                        ptWidget->scrollOffset=0;
                    }
                    else
                    {
                        ptWidget->scrollOffset=ptWidget->_scrollOffset-iResult;
                        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
                    }
                }
                if(ptWidget->_isBottomScroll)
                {
                    if(isPiEnd)
                    {
                        ptWidget->isRelease=false;
                        ptWidget->scrollOffset=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->strHeight;
                    }
                    else
                    {
                        ptWidget->scrollOffset=(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->strHeight)-(ptWidget->_scrollOffset-iResult);
                        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
                    }
                }
            }

            if(!ptWidget->isTransparent)
            {
                if (ptWidget->ptImgTile==NULL)//color
                {
                    ldBaseColor(&tTarget,
                                NULL,
                                ptWidget->bgColor,
                                ptWidget->use_as__ldBase_t.opacity);
                }
                else
                {
                    ldBaseImage(&tTarget,
                                &ptWidget->ptImgTile->tRegion,
                                ptWidget->ptImgTile,
                                NULL,
                                0,
                                ptWidget->use_as__ldBase_t.opacity);
                }
                arm_2d_op_wait_async(NULL);
            }

            if(ptWidget->pStr!=NULL)
            {
                arm_2d_region_t textShowRegion=tTarget_canvas;
                textShowRegion.tLocation.iY+=ptWidget->scrollOffset;
                arm_lcd_text_set_target_framebuffer(&tTarget);
                arm_lcd_text_set_font(ptWidget->ptFont);
                arm_lcd_text_set_draw_region(&textShowRegion);
                arm_lcd_text_set_colour(ptWidget->textColor, GLCD_COLOR_WHITE);
                arm_lcd_text_set_opacity(ptWidget->use_as__ldBase_t.opacity);
                arm_lcd_printf("%s",ptWidget->pStr);
                arm_2d_op_wait_async(NULL);
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldTextSetTransparent(ldText_t* ptWidget,bool isTransparent)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isTransparent=isTransparent;
}

void ldTextSetText(ldText_t* ptWidget,uint8_t *pStr)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ldFree(ptWidget->pStr);
    ptWidget->pStr=ldCalloc(1,strlen((char*)pStr)+1);
    strcpy((char*)ptWidget->pStr,(char*)pStr);
}

void ldTextSetTextColor(ldText_t* ptWidget,ldColor charColor)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->textColor=charColor;
}

void ldTextSetBgImage(ldText_t *ptWidget, arm_2d_tile_t *ptImgTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptImgTile=ptImgTile;
    ptWidget->isTransparent=false;
}

void ldTextSetBgColor(ldText_t *ptWidget, ldColor bgColor)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->bgColor=bgColor;
    ptWidget->isTransparent=false;
    ptWidget->ptImgTile=NULL;
}

void ldTextScrollSeek(ldText_t *ptWidget,int16_t offset)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->scrollOffset=offset;
}

void ldTextScrollMove(ldText_t *ptWidget, int8_t moveValue)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->scrollOffset+=moveValue;
    if((moveValue>0)&&(ptWidget->scrollOffset<0))
    {
        if(ptWidget->scrollOffset<0)
        {
            ptWidget->scrollOffset=0;
        }
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
