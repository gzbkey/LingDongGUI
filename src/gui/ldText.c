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

#define __LD_TEXT_IMPLEMENT__

#include "__common.h"
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

#define SLIDER_W           10
#define SLIDER_H           20

const ldBaseWidgetFunc_t ldTextFunc = {
    .depose = (ldDeposeFunc_t)ldText_depose,
    .load = (ldLoadFunc_t)ldText_on_load,
    .frameStart = (ldFrameStartFunc_t)ldText_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldText_on_frame_complete,
    .show = (ldShowFunc_t)ldText_show,
};

static bool slotTextVerticalScroll(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldText_t *ptWidget = msg.ptSender;

    switch (msg.signal)
    {
    case SIGNAL_PRESS:
    {
        ptWidget->isMoveReset=false;
        ptWidget->_scrollOffset=ptWidget->scrollOffset;
        ptWidget->_isTopScroll=false;
        ptWidget->_isBottomScroll=false;
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        text_box_update(&ptWidget->tTextPanel);
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        ptWidget->scrollOffset=ptWidget->_scrollOffset+(int16_t)GET_SIGNAL_OFFSET_Y(msg.value);
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

        //计算滑块位置
        int strMove=ptWidget->strHeight-ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
        if(strMove>0)
        {
            int percent256=(ptWidget->scrollOffset<<8)/strMove;

            int32_t trackLen = ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight - SLIDER_H;

            ptWidget->sliderPos = (-percent256 * trackLen) >> 8;

            if(ptWidget->sliderPos<0)
            {
                ptWidget->sliderPos=0;
            }
            if(ptWidget->sliderPos>trackLen)
            {
                ptWidget->sliderPos=trackLen;
            }
        }
        else
        {
            ptWidget->sliderPos=-1;
        }

        break;
    }
    case SIGNAL_RELEASE:
    {
        ptWidget->sliderPos=-1;
        ptWidget->isMoveReset=true;
        ptWidget->_scrollOffset=ptWidget->scrollOffset;

        if(ptWidget->scrollOffset>0)
        {
            ptWidget->_isTopScroll=true;
            ptWidget->_isBottomScroll=false;
        }
        else if(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight>=ptWidget->strHeight)
        {
            ptWidget->_isTopScroll=true;
            ptWidget->_isBottomScroll=false;
        }
        else if(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight>=(ptWidget->strHeight+ptWidget->scrollOffset))
        {
            ptWidget->_scrollOffset=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-(ptWidget->strHeight+ptWidget->scrollOffset);
            ptWidget->_isTopScroll=false;
            ptWidget->_isBottomScroll=true;
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

ldText_t* ldText_init(ld_scene_t *ptScene,ldText_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,arm_2d_font_t *ptFont,text_box_line_alignment_t align,bool isScroll)
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
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->bgColor=__RGB(255,255,255);
    ptWidget->ptFont=ptFont;
    ptWidget->strHeight=-1;
    ptWidget->sliderPos=-1;

    text_box_cfg_t tCFG = {
        .ptFont = ptFont,
        .tStreamIO = {
            .ptIO       = &TEXT_BOX_IO_C_STRING_READER,
            .pTarget    = (uintptr_t)&ptWidget->tStringReader,
        },
        .u2LineAlign = align,
        //.fScale = 1.0f,
        //.chSpaceBetweenParagraph = 20,

        .ptScene = (arm_2d_scene_t *)ptScene,
        .bUseDirtyRegions = false,
    };

    text_box_init(&ptWidget->tTextPanel, &tCFG);
    text_box_set_start_line(&ptWidget->tTextPanel, 0);
    text_box_set_scrolling_position(&ptWidget->tTextPanel, 0);

    if(isScroll)
    {
        ldMsgConnect(ptWidget,SIGNAL_PRESS,slotTextVerticalScroll);
        ldMsgConnect(ptWidget,SIGNAL_HOLD_DOWN,slotTextVerticalScroll);
        ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotTextVerticalScroll);
    }

    LOG_INFO("[init][text] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    return ptWidget;
}

void ldText_depose(ld_scene_t *ptScene, ldText_t *ptWidget)
{
    assert(NULL != ptWidget);
    ARM_2D_UNUSED(ptScene);

    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeText)
    {
        return;
    }

    LOG_INFO("[depose][text] id:%d", ptWidget->use_as__ldBase_t.nameId);

    text_box_depose(&ptWidget->tTextPanel);
    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
    if(!ptWidget->_isStatic)
    {
        ldFree(ptWidget->pStr);
    }
#if USE_VIRTUAL_RESOURCE == 1
    ldFree(ptWidget->ptImgTile);
    ldFree(ptWidget->ptMaskTile);
    ldFree(ptWidget->ptFont);
#endif
    ldFree(ptWidget);
}

void ldText_on_load(ld_scene_t *ptScene, ldText_t *ptWidget)
{
    assert(NULL != ptWidget);
    ARM_2D_UNUSED(ptScene);

    text_box_on_load(&ptWidget->tTextPanel);
}

void ldText_on_frame_start(ld_scene_t *ptScene, ldText_t *ptWidget)
{
    assert(NULL != ptWidget);
    ARM_2D_UNUSED(ptScene);

    if(ptWidget->isMoveReset)
    {
        int16_t iResult;
        bool isPiEnd;

        if(ptWidget->_isTopScroll)
        {
            isPiEnd=arm_2d_helper_pi_slider(&ptWidget->tPISlider, ptWidget->_scrollOffset, &iResult);
            if(isPiEnd)
            {
                ptWidget->isMoveReset=false;
                ptWidget->scrollOffset=0;
            }
            else
            {
                ptWidget->scrollOffset=ptWidget->_scrollOffset-iResult;
                ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
            }
        }
        else if(ptWidget->_isBottomScroll)
        {
            isPiEnd=arm_2d_helper_pi_slider(&ptWidget->tPISlider, ptWidget->_scrollOffset, &iResult);
            if(isPiEnd)
            {
                ptWidget->isMoveReset=false;
                ptWidget->scrollOffset=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->strHeight;
            }
            else
            {
                ptWidget->scrollOffset=(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->strHeight)-(ptWidget->_scrollOffset-iResult);
                ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
            }
        }
        else
        {
            ptWidget->isMoveReset=false;
        }
    }

    text_box_set_scrolling_position(&ptWidget->tTextPanel, -ptWidget->scrollOffset);

    text_box_on_frame_start(&ptWidget->tTextPanel);
}

void ldText_on_frame_complete(ld_scene_t *ptScene, ldText_t *ptWidget)
{
    assert(NULL != ptWidget);
    ARM_2D_UNUSED(ptScene);

    text_box_on_frame_complete(&ptWidget->tTextPanel);
}

void ldText_show(ld_scene_t *ptScene, ldText_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    ARM_2D_UNUSED(ptScene);

    if(ptWidget == NULL)
    {
        return;
    }

    arm_2d_region_t globalRegion;
    arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t*)ptWidget,&globalRegion,true);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ldBaseIsHidden((ldBase_t*)ptWidget))
            {
                break;
            }

            if(!ptWidget->isTransparent)
            {
                if (ptWidget->ptImgTile==NULL)//color
                {
                    if(ptWidget->use_as__ldBase_t.isCorner)
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
                    if(ptWidget->use_as__ldBase_t.isCorner)
                    {
                        draw_round_corner_image(ptWidget->ptImgTile,
                                                &tTarget,
                                                &ptWidget->ptImgTile->tRegion,
                                                bIsNewFrame,
                                                ptWidget->use_as__ldBase_t.opacity);
                    }
                    else
                    {
                        ldBaseImage(&tTarget,
                                    &ptWidget->ptImgTile->tRegion,
                                    ptWidget->ptImgTile,
                                    ptWidget->ptMaskTile,
                                    ptWidget->bgColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
                }
                arm_2d_op_wait_async(NULL);
            }

            if(ptWidget->pStr!=NULL)
            {
                arm_lcd_text_set_char_spacing(1);
                arm_lcd_text_set_line_spacing(4);
                text_box_show(  &ptWidget->tTextPanel,
                                &tTarget,
                                NULL,
                                (__arm_2d_color_t) {ptWidget->textColor},
                                ptWidget->use_as__ldBase_t.opacity,
                                bIsNewFrame);

                if(ptWidget->strHeight==-1)
                {
                    ptWidget->strHeight=text_box_get_line_height(&ptWidget->tTextPanel)*text_box_get_line_count(&ptWidget->tTextPanel,0);
                }
                arm_2d_op_wait_async(NULL);
            }

            if(ptWidget->sliderPos>=0)
            {
                arm_2d_region_t sliderBgRegion={
                    .tLocation={
                        .iX=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-SLIDER_W,
                        .iY=0,
                    },
                    .tSize={
                        .iWidth=SLIDER_W,
                        .iHeight=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight,
                    }
                };

                ldBaseColor(&tTarget,
                            &sliderBgRegion,
                            __RGB(220,220,220),
                            ptWidget->use_as__ldBase_t.opacity);

                arm_2d_region_t sliderRegion={
                    .tLocation={
                        .iX=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-SLIDER_W+2,
                        .iY=ptWidget->sliderPos,
                    },
                    .tSize={
                        .iWidth=SLIDER_W-4,
                        .iHeight=SLIDER_H,
                    }
                };
                ldBaseColor(&tTarget,
                            &sliderRegion,
                            __RGB(120,120,120),
                            ptWidget->use_as__ldBase_t.opacity);
            }

            LD_BASE_WIDGET_SELECT;
            arm_2d_op_wait_async(NULL);
        }
    }
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
    if(ptWidget == NULL || pStr == NULL)
    {
        return;
    }
    int len = strlen((char*)pStr);
    if(len == 0)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    if(ptWidget->_isStatic==false)
    {
        ldFree(ptWidget->pStr);
    }
    ptWidget->_isStatic=false;
    ptWidget->pStr=ldCalloc(1,len+1);
    if(ptWidget->pStr!=NULL)
    {
        strcpy((char*)ptWidget->pStr,(char*)pStr);
    }
    ptWidget->scrollOffset=0;
    text_box_c_str_reader_init( &ptWidget->tStringReader,
                                (char*)ptWidget->pStr,
                                strlen((char*)ptWidget->pStr));
}

void ldTextSetStaticText(ldText_t* ptWidget,const uint8_t *pStr)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    if(ptWidget->_isStatic==false)
    {
        ldFree(ptWidget->pStr);
    }
    ptWidget->_isStatic=true;
    ptWidget->pStr=(uint8_t*)pStr;
    ptWidget->scrollOffset=0;
    text_box_c_str_reader_init( &ptWidget->tStringReader,
                                (char*)ptWidget->pStr,
                                strlen((char*)ptWidget->pStr));
}

void ldTextSetTextColor(ldText_t* ptWidget,ldColor textColor)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->textColor=textColor;
}

void ldTextSetBackgroundImage(ldText_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptImgTile=ptImgTile;
    ptWidget->ptMaskTile=ptMaskTile;
    ptWidget->isTransparent=false;
}

void ldTextSetBackgroundColor(ldText_t *ptWidget, ldColor bgColor)
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
