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

#define __LD_MESSAGE_BOX_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldMessageBox.h"

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

#define LD_MSG_BOX_SPACE                2

const ldBaseWidgetFunc_t ldMessageBoxFunc = {
    .depose = (ldDeposeFunc_t)ldMessageBox_depose,
    .load = (ldLoadFunc_t)ldMessageBox_on_load,
    .frameStart = (ldFrameStartFunc_t)ldMessageBox_on_frame_start,
    .show = (ldShowFunc_t)ldMessageBox_show,
};

static bool slotMsgBoxToggle(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldMessageBox_t *ptWidget= msg.ptSender;
    arm_2d_location_t pos;
    arm_2d_region_t btnRegion=ptWidget->btnRegion;
    int8_t clickNum=-1;

    arm_2d_location_t tLocation={0};
    tLocation=ldBaseGetAbsoluteLocation(ptWidget,tLocation);
    pos.iX=(int16_t)GET_SIGNAL_VALUE_X(msg.value)-tLocation.iX;
    pos.iY=(int16_t)GET_SIGNAL_VALUE_Y(msg.value)-tLocation.iY;

    for(uint8_t i=0;i<ptWidget->btnCount;i++)
    {
        btnRegion.tLocation.iX+=LD_MSG_BOX_SPACE;
        if(arm_2d_is_point_inside_region(&btnRegion,&pos))
        {
            clickNum=i;
            break;
        }
        btnRegion.tLocation.iX+=btnRegion.tSize.iWidth+LD_MSG_BOX_SPACE;
    }

    switch (msg.signal)
    {
    case SIGNAL_PRESS:
    {
        if(clickNum>=0)
        {
            SETBIT(ptWidget->isBtnPressed,clickNum);
        }
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    case SIGNAL_RELEASE:
    {
        if(clickNum>=0)
        {
            CLRBIT(ptWidget->isBtnPressed,clickNum);
            ptWidget->clickNum=clickNum;
            if(ptWidget->ptFunc!=NULL)
            {
                ptWidget->ptFunc(ptWidget);
            }
            ptWidget->clickNum=-1;
            ptWidget->use_as__ldBase_t.isHidden=true;
            ldBaseSetDeleteLater((ldBase_t*)ptWidget);
//            ptWidget->use_as__ldBase_t.ptGuiFunc->depose(ptWidget);
        }
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

        break;
    }
    default:
        break;
    }

    return false;
}

ldMessageBox_t* ldMessageBox_init( ld_scene_t *ptScene,ldMessageBox_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldMessageBox_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][messageBox] id:%d", nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldMessageBox_t));
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = 0;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = 0;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeMessageBox;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldMessageBoxFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    if(x==(-1)&&y==(-1))
    {
        arm_2d_region_t tScreen = {
            .tSize = {
                __DISP0_CFG_SCEEN_WIDTH__,
                __DISP0_CFG_SCEEN_HEIGHT__
            },
        };
        arm_2d_align_centre(tScreen, width, height)
        {
            ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion=__centre_region;
        }
    }
    else
    {
        ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
        ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    }
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->padding.top=10;
    ptWidget->padding.bottom=10;
    ptWidget->padding.left=10;
    ptWidget->padding.right=10;
    ptWidget->isCorner=true;
    ptWidget->bgColor=GLCD_COLOR_WHITE;
    ptWidget->ptFont=ptFont;
    ptWidget->titleHeight=(height-ptWidget->padding.top-ptWidget->padding.bottom)/5;
    ptWidget->msgHeight=ptWidget->titleHeight*3;
    ptWidget->btnCount=3;
    ptWidget->releaseColor = __RGB(217, 225, 244);
    ptWidget->pressColor = __RGB(255, 243, 202);
    ptWidget->btnRegion.tLocation.iX=ptWidget->padding.left;
    ptWidget->btnRegion.tLocation.iY=ptWidget->padding.top+ptWidget->titleHeight+ptWidget->msgHeight;
    ptWidget->btnRegion.tSize.iWidth=(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-ptWidget->padding.left-ptWidget->padding.right)/ptWidget->btnCount-LD_MSG_BOX_SPACE*2;
    ptWidget->btnRegion.tSize.iHeight=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->padding.top-ptWidget->padding.bottom-ptWidget->titleHeight-ptWidget->msgHeight;
    ptWidget->clickNum=-1;

    ldMsgConnect(ptWidget, SIGNAL_PRESS, slotMsgBoxToggle);
    ldMsgConnect(ptWidget, SIGNAL_RELEASE, slotMsgBoxToggle);

    LOG_INFO("[init][messageBox] id:%d, size:%llu", nameId,sizeof (*ptWidget));
    return ptWidget;
}

void ldMessageBox_depose( ldMessageBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeMessageBox)
    {
        return;
    }

    LOG_INFO("[depose][messageBox] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldMessageBox_on_load( ldMessageBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
}

void ldMessageBox_on_frame_start( ldMessageBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
}

void ldMessageBox_show(ld_scene_t *ptScene, ldMessageBox_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
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

            //bg
            if(ptWidget->isCorner)
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
            arm_2d_op_wait_async(NULL);

            //title
            do {
                arm_2d_region_t region={
                    .tLocation={
                        .iX=ptWidget->padding.left,
                        .iY=ptWidget->padding.top,
                    },
                    .tSize={
                        .iWidth=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-ptWidget->padding.left-ptWidget->padding.right,
                        .iHeight=ptWidget->titleHeight,
                    },
                };

                ldBaseLabel(&tTarget,&region,ptWidget->pTitleStr,ptWidget->ptFont,ARM_2D_ALIGN_LEFT,ptWidget->titleStrColor,ptWidget->use_as__ldBase_t.opacity);
                arm_2d_op_wait_async(NULL);
            } while (false);

            //msg
            do {
                arm_2d_region_t region={
                    .tLocation={
                        .iX=ptWidget->padding.left,
                        .iY=ptWidget->padding.top+ptWidget->titleHeight,
                    },
                    .tSize={
                        .iWidth=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-ptWidget->padding.left-ptWidget->padding.right,
                        .iHeight=ptWidget->msgHeight,
                    },
                };

                ldBaseLabel(&tTarget,&region,ptWidget->pMsgStr,ptWidget->ptFont,ARM_2D_ALIGN_CENTRE,ptWidget->msgStrColor,ptWidget->use_as__ldBase_t.opacity);
                arm_2d_op_wait_async(NULL);
            } while (false);

            //btn
            do {

                arm_2d_region_t btnRegion=ptWidget->btnRegion;

                for(uint8_t i=0;i<ptWidget->btnCount;i++)
                {
                    btnRegion.tLocation.iX+=LD_MSG_BOX_SPACE;

                    ldColor color;
                    if(GETBIT(ptWidget->isBtnPressed,i))
                    {
                        color=ptWidget->pressColor;
                    }
                    else
                    {
                        color=ptWidget->releaseColor;
                    }

                    if(ptWidget->isCorner)
                    {
                        draw_round_corner_box(&tTarget,
                                              &btnRegion,
                                              color,
                                              ptWidget->use_as__ldBase_t.opacity,
                                              bIsNewFrame);
                    }
                    else
                    {
                        ldBaseColor(&tTarget,
                                    &btnRegion,
                                    color,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }

                    if(ptWidget->ppBtnStrGroup!=NULL)
                    {
                        ldBaseLabel(&tTarget,&btnRegion,ptWidget->ppBtnStrGroup[i],ptWidget->ptFont,ARM_2D_ALIGN_CENTRE,ptWidget->msgStrColor,ptWidget->use_as__ldBase_t.opacity);
                    }

                    btnRegion.tLocation.iX+=btnRegion.tSize.iWidth+LD_MSG_BOX_SPACE;
                }

                arm_2d_op_wait_async(NULL);
            } while (false);
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldMessageBoxSetTitle(ldMessageBox_t* ptWidget,const uint8_t *pStr)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->pTitleStr=pStr;
}

void ldMessageBoxSetMsg(ldMessageBox_t* ptWidget,const uint8_t *pStr)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->pMsgStr=pStr;
}

void ldMessageBoxSetBtn(ldMessageBox_t* ptWidget,const uint8_t *pStrArray[],uint8_t arraySize)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->ppBtnStrGroup=pStrArray;
    if(arraySize==0)
    {
        arraySize=1;
    }
    if(arraySize>3)
    {
        arraySize=3;
    }
    ptWidget->btnCount=arraySize;
    ptWidget->btnRegion.tLocation.iX=ptWidget->padding.left;
    ptWidget->btnRegion.tLocation.iY=ptWidget->padding.top+ptWidget->titleHeight+ptWidget->msgHeight;
    ptWidget->btnRegion.tSize.iWidth=(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-ptWidget->padding.left-ptWidget->padding.right)/ptWidget->btnCount-LD_MSG_BOX_SPACE*2;
    ptWidget->btnRegion.tSize.iHeight=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->padding.top-ptWidget->padding.bottom-ptWidget->titleHeight-ptWidget->msgHeight;
}

void ldMessageBoxSetCallback(ldMessageBox_t* ptWidget,ldMsgBoxFunc_t ptFunc)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->ptFunc=ptFunc;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
