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

#define __LD_LIST_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldList.h"

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

#define MOVE_SPEED_VALUE                (3)

const ldBaseWidgetFunc_t ldListFunc = {
    .depose = (ldDeposeFunc_t)ldList_depose,
    .load = (ldLoadFunc_t)ldList_on_load,
    .frameStart = (ldFrameStartFunc_t)ldList_on_frame_start,
    .show = (ldShowFunc_t)ldList_show,
};

static bool slotListScroll(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldList_t *ptWidget = msg.ptSender;
    switch (msg.signal)
    {
    case SIGNAL_PRESS:
    {
        ptWidget->isMoveReset=false;


        ptWidget->_offset=ptWidget->offset;
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        ptWidget->offset=ptWidget->_offset+(int16_t)GET_SIGNAL_OFFSET_Y(msg.value);
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        ptWidget->isHoldMove=true;
        break;
    }
    case SIGNAL_RELEASE:
    {
        if(ptWidget->isHoldMove)
        {
            ptWidget->isHoldMove=false;
            ptWidget->isMoveReset=true;

            ptWidget->_offset= ldBaseAutoVerticalGridAlign(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion,
                                                           ptWidget->offset,
                                                           ptWidget->itemCount,
                                                           ptWidget->itemHeight,
                                                           0);
        }
        else
        {

            ptWidget->clickItemPos.iX=0;
            ptWidget->clickItemPos.iY=0;
            ptWidget->clickItemPos=ldBaseGetAbsoluteLocation((ldBase_t*)ptWidget,ptWidget->clickItemPos);
            ptWidget->clickItemPos.iX=(int16_t)GET_SIGNAL_VALUE_X(msg.value)-ptWidget->clickItemPos.iX;
            ptWidget->clickItemPos.iY=(int16_t)GET_SIGNAL_VALUE_Y(msg.value)-ptWidget->clickItemPos.iY;

            ptWidget->selectItem=((-ptWidget->offset)+ptWidget->clickItemPos.iY)/ptWidget->itemHeight;

            ptWidget->clickItemPos.iY%=ptWidget->itemHeight;
        }
        break;
    }
    default:
        break;
    }
}

ldList_t* ldList_init( ld_scene_t *ptScene,ldList_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldList_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][list] id:%d", nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldList_t));
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeList;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldListFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->itemHeight=30;
    ptWidget->itemCount=10;

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotListScroll);
    ldMsgConnect(ptWidget,SIGNAL_HOLD_DOWN,slotListScroll);
    ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotListScroll);

    LOG_INFO("[init][list] id:%d, size:%llu", nameId,sizeof (*ptWidget));
    return ptWidget;
}

void ldList_depose( ldList_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeList)
    {
        return;
    }

    LOG_INFO("[depose][list] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldList_on_load( ldList_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
}

void ldList_on_frame_start( ldList_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
}

void ldList_show(ld_scene_t *ptScene, ldList_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    if (bIsNewFrame) {
        if(ptWidget->isMoveReset)
        {
            if(ptWidget->_offset>ptWidget->offset)
            {
                ptWidget->offset+=MOVE_SPEED_VALUE;
                if(ptWidget->_offset<ptWidget->offset)
                {
                    ptWidget->offset=ptWidget->_offset;
                }
            }
            else
            {
                if(ptWidget->_offset<ptWidget->offset)
                {
                    ptWidget->offset-=MOVE_SPEED_VALUE;
                    if(ptWidget->_offset>ptWidget->offset)
                    {
                        ptWidget->offset=ptWidget->_offset;
                    }
                }
                else
                {
                    ptWidget->isMoveReset=false;
                }
            }
            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        }
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

            arm_2d_region_t itemRegion={
                .tSize={
                    .iWidth=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth,
                    .iHeight=ptWidget->itemHeight,
                },
            };

            if(ptWidget->ptItemFunc!=NULL)
            {
                arm_2d_location_t clickPos={-1,-1};
                for(uint8_t i=0;i<ptWidget->itemCount;i++)
                {
                    itemRegion.tLocation.iY=i*ptWidget->itemHeight+ptWidget->offset;

                    arm_2d_container(&tTarget,tItemTile , &itemRegion)
                    {
                        if((ptWidget->selectItem==i)&&(ptWidget->clickItemPos.iX!=-1)&&(ptWidget->clickItemPos.iY!=-1))
                        {
                            clickPos=ptWidget->clickItemPos;
                            ptWidget->clickItemPos.iX=-1;
                            ptWidget->clickItemPos.iY=-1;
                        }
                        ptWidget->ptItemFunc(ptWidget,i,&tItemTile,&tItemTile_canvas,clickPos,bIsNewFrame);
                        clickPos.iX=-1;
                        clickPos.iY=-1;
                    }
                }
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldListSetItemFunc(ldList_t *ptWidget,ldListItemFunc_t ptItemFunc)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->ptItemFunc=ptItemFunc;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
