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

#define __LD_SLIDER_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldSlider.h"

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

const ldBaseWidgetFunc_t ldSliderFunc = {
    .depose = (ldDeposeFunc_t)ldSlider_depose,
    .load = (ldLoadFunc_t)ldSlider_on_load,
#ifdef FRAME_START
    .frameStart = (ldFrameStartFunc_t)ldSlider_on_frame_start,
#endif
    .show = (ldShowFunc_t)ldSlider_show,
};

static bool slotSliderMove(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldSlider_t *ptWidget=msg.ptSender;

    arm_2d_location_t tClickLocal;
    tClickLocal.iX=(int16_t)GET_SIGNAL_VALUE_X(msg.value);
    tClickLocal.iY=(int16_t)GET_SIGNAL_VALUE_Y(msg.value);

    tClickLocal=ldBaseGetRelativeLocation(ptWidget,tClickLocal);

    uint8_t halfIndicWidth=ptWidget->indicWidth>>1;

    if(ptWidget->isHorizontal)
    {
        if(tClickLocal.iX<halfIndicWidth)
        {
            tClickLocal.iX=halfIndicWidth;
        }
        if(tClickLocal.iX>(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth - halfIndicWidth))
        {
            tClickLocal.iX=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth - halfIndicWidth;
        }

        tClickLocal.iX-=halfIndicWidth;

        ptWidget->permille = ((uint32_t)tClickLocal.iX * 1000) / (ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth - ptWidget->indicWidth);
    }
    else
    {
        if(tClickLocal.iY<halfIndicWidth)
        {
            tClickLocal.iY=halfIndicWidth;
        }
        if(tClickLocal.iY>(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight - halfIndicWidth))
        {
            tClickLocal.iY=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight - halfIndicWidth;
        }

        tClickLocal.iY-=halfIndicWidth;

        ptWidget->permille = 1000 - ((uint32_t)tClickLocal.iY * 1000) / (ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight - ptWidget->indicWidth);
    }
    ldMsgEmit(ptScene->ptMsgQueue,ptWidget,SIGNAL_VALUE_CHANGED,ptWidget->permille);

    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    return false;
}

ldSlider_t* ldSlider_init( ld_scene_t *ptScene,ldSlider_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldSlider_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][slider] id:%d", nameId);
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
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeSlider;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldSliderFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->bgColor=__RGB(0xe7, 0xea, 0xea);
    ptWidget->frameColor=__RGB(0xd6, 0xd6, 0xd6);
    ptWidget->indicColor=__RGB(0x00, 0x7a, 0xd9);
    ptWidget->indicWidth=10;
    ptWidget->slimSize=4;
    ptWidget->isHorizontal=true;

    ldMsgConnect(ptWidget, SIGNAL_PRESS, slotSliderMove);
    ldMsgConnect(ptWidget, SIGNAL_HOLD_DOWN, slotSliderMove);

    LOG_INFO("[init][slider] id:%d, size:%d", nameId,sizeof (*ptWidget));
    return ptWidget;
}

void ldSlider_depose( ldSlider_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeSlider)
    {
        return;
    }

    LOG_INFO("[depose][slider] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldSlider_on_load( ldSlider_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldSlider_on_frame_start( ldSlider_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldSlider_show(ld_scene_t *ptScene, ldSlider_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
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

    arm_2d_region_t globalRegion;
    arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t*)ptWidget,&globalRegion,true);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }


            arm_2d_region_t tIndicatorRegion;
            if((ptWidget->ptBgImgTile==NULL)&&(ptWidget->ptIndicImgTile==NULL))//color
            {
                arm_2d_region_t tFrameRegion;
                if(ptWidget->isHorizontal)
                {
                    tFrameRegion.tLocation.iX=0;
                    tFrameRegion.tLocation.iY=(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->slimSize)>>1;
                    tFrameRegion.tSize.iWidth=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth;
                    tFrameRegion.tSize.iHeight=ptWidget->slimSize;
                }
                else
                {
                    tFrameRegion.tLocation.iX=(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-ptWidget->slimSize)>>1;
                    tFrameRegion.tLocation.iY=0;
                    tFrameRegion.tSize.iWidth=ptWidget->slimSize;
                    tFrameRegion.tSize.iHeight=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
                }
                ldBaseColor(&tTarget, &tFrameRegion, ptWidget->bgColor,ptWidget->use_as__ldBase_t.opacity);
                arm_2d_draw_box(&tTarget,&tFrameRegion,1,ptWidget->frameColor,ptWidget->use_as__ldBase_t.opacity);

                if(ptWidget->isHorizontal)
                {
                    tIndicatorRegion.tLocation.iX = (ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth - ptWidget->indicWidth) * ptWidget->permille / 1000;
                    tIndicatorRegion.tLocation.iY = 0;
                    tIndicatorRegion.tSize.iWidth = ptWidget->indicWidth;
                    tIndicatorRegion.tSize.iHeight= ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
                }
                else
                {
                    tIndicatorRegion.tLocation.iX = 0;
                    tIndicatorRegion.tLocation.iY = (ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight - ptWidget->indicWidth) * (1000 - ptWidget->permille) / 1000;;
                    tIndicatorRegion.tSize.iWidth = ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth;
                    tIndicatorRegion.tSize.iHeight= ptWidget->indicWidth;
                }
                ldBaseColor(&tTarget,&tIndicatorRegion,ptWidget->indicColor,ptWidget->use_as__ldBase_t.opacity);
            }
            else
            {
                arm_2d_align_centre(tTarget_canvas, ptWidget->ptBgImgTile->tRegion.tSize)
                {
                    ldBaseImage(&tTarget,&__centre_region,ptWidget->ptBgImgTile,ptWidget->ptBgMaskTile,0,ptWidget->use_as__ldBase_t.opacity);
                }

                tIndicatorRegion.tSize = ptWidget->ptIndicImgTile->tRegion.tSize;
                if(ptWidget->isHorizontal)
                {
                    tIndicatorRegion.tLocation.iX = (ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth - ptWidget->indicWidth) * ptWidget->permille / 1000;
                    tIndicatorRegion.tLocation.iY = 0;
                }
                else
                {
                    tIndicatorRegion.tLocation.iX = 0;
                    tIndicatorRegion.tLocation.iY = (ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight - ptWidget->indicWidth) * (1000 - ptWidget->permille) / 1000;;
                }
                ldBaseImage(&tTarget,&tIndicatorRegion,ptWidget->ptIndicImgTile,ptWidget->ptIndicMaskTile,0,ptWidget->use_as__ldBase_t.opacity);
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldSliderSetPercent(ldSlider_t *ptWidget,float percent)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    if(percent>=0)
    {
        if(percent>100)
        {
            percent=100;
        }
        ptWidget->permille=percent*10;
    }
    else
    {
        if(percent<-100)
        {
            percent=-100;
        }
        ptWidget->permille=1000+percent*10;
    }
}

void ldSliderSetHorizontal(ldSlider_t *ptWidget,bool isHorizontal)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->isHorizontal=isHorizontal;
}

void ldSliderSetImage(ldSlider_t *ptWidget,arm_2d_tile_t *ptBgImgTile,arm_2d_tile_t *ptBgMaskTile,arm_2d_tile_t *ptIndicImgTile,arm_2d_tile_t *ptIndicMaskTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptBgImgTile=ptBgImgTile;
    ptWidget->ptBgMaskTile=ptBgMaskTile;
    ptWidget->ptIndicImgTile=ptIndicImgTile;
    ptWidget->ptIndicMaskTile=ptIndicMaskTile;
}

void ldSliderSetColor(ldSlider_t *ptWidget,ldColor bgColor,ldColor frameColor,ldColor indicColor)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->bgColor=bgColor;
    ptWidget->frameColor=frameColor;
    ptWidget->indicColor=indicColor;
    ptWidget->ptBgImgTile=NULL;
    ptWidget->ptBgMaskTile=NULL;
    ptWidget->ptIndicImgTile=NULL;
    ptWidget->ptIndicMaskTile=NULL;
}

void ldSliderSetIndicatorWidth(ldSlider_t *ptWidget,uint8_t indicWidth)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->indicWidth=indicWidth;
}

void ldSliderSetSlimSize(ldSlider_t *ptWidget,uint8_t slimSize)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->slimSize=slimSize;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
