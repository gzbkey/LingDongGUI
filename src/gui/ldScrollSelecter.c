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

#define __LD_SCROLL_SELECTER_IMPLEMENT__

#include "__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldScrollSelecter.h"

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

#define MOVE_SPEED_THRESHOLD_VALUE      (20)          //触摸移动速度超过此值，则产生惯性滑动效果
#define SPEED_2_OFFSET(speed)           (speed*3)     //通过速度值，生成惯性滑动距离

const ldBaseWidgetFunc_t ldScrollSelecterFunc = {
    .depose = (ldDeposeFunc_t)ldScrollSelecter_depose,
    .load = (ldLoadFunc_t)ldScrollSelecter_on_load,
    .frameStart = (ldFrameStartFunc_t)ldScrollSelecter_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldScrollSelecter_on_frame_complete,
    .show = (ldShowFunc_t)ldScrollSelecter_show,
};


static uint8_t _ldScrollSelecterAutoItem(ldScrollSelecter_t *ptWidget,int16_t offset)
{
    if(offset>=0)
    {
        return 0;
    }

    offset=-offset;
    int16_t temp1,temp2;

    uint16_t itemSpace=ptWidget->itemSpace+ptWidget->ptFont->tCharSize.iHeight;

    temp1=offset%itemSpace;
    temp2=offset/itemSpace;
    if(temp1>=(itemSpace/2))
    {
        temp2++;
    }

    if(temp2>=ptWidget->itemCount)
    {
        temp2=ptWidget->itemCount-1;
    }

    return temp2;
}

static int16_t _scrollOffset;

static bool slotScrollSelecterScroll(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldScrollSelecter_t *ptWidget=msg.ptSender;

    switch (msg.signal)
    {
    case SIGNAL_PRESS:
    {
        ptWidget->isWaitMove=false;
        ptWidget->isAutoMove=false;
        _scrollOffset=ptWidget->scrollOffset;
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        ptWidget->scrollOffset=_scrollOffset+(int16_t)GET_SIGNAL_OFFSET_Y(msg.value);
        break;
    }
    case SIGNAL_RELEASE:
    {
        int16_t ySpeed=(int16_t)GET_SIGNAL_SPEED_Y(msg.value);

        if((ySpeed>MOVE_SPEED_THRESHOLD_VALUE)||(ySpeed<-MOVE_SPEED_THRESHOLD_VALUE))
        {
            ptWidget->itemSelect=_ldScrollSelecterAutoItem(ptWidget,_scrollOffset+SPEED_2_OFFSET(ySpeed));
            ptWidget->isAutoMove=true;
        }
        if(!ptWidget->isAutoMove)
        {
            ptWidget->itemSelect=_ldScrollSelecterAutoItem(ptWidget,ptWidget->scrollOffset);
        }
        ptWidget->isWaitMove=true;
        break;
    }
    default:
        break;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    return false;
}

ldScrollSelecter_t* ldScrollSelecter_init( ld_scene_t *ptScene,ldScrollSelecter_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldScrollSelecter_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][scrollSelecter] id:%d", nameId);
            return NULL;
        }
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    height=height/3*3;//强制为3的倍数

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeScrollSelecter;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldScrollSelecterFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->ppItemStrGroup=NULL;
    ptWidget->bgColor=GLCD_COLOR_WHITE;
    ptWidget->charColor=GLCD_COLOR_BLACK;
    ptWidget->ptFont=ptFont;
    ptWidget->isTransparent=true;
    ptWidget->moveOffset=1;
    ptWidget->indicatorColor=GLCD_COLOR_DARK_GREY;

    ptWidget->itemSpace=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->ptFont->tCharSize.iHeight*3;

    if(ptWidget->itemSpace>0)
    {
        ptWidget->itemSpace=ptWidget->itemSpace>>1;
        ptWidget->isEdit=true;
        LOG_INFO("[init auto][scrollSelecter] edit mode");
    }
    else
    {
        ptWidget->isEdit=false;
        ptWidget->itemSpace=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->ptFont->tCharSize.iHeight;

        LOG_INFO("[init auto][scrollSelecter] view mode");
        if(ptWidget->itemSpace>0)
        {
            ptWidget->itemSpace=ptWidget->itemSpace>>1;
        }
        else
        {
            ptWidget->itemSpace=0;
        }
    }

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotScrollSelecterScroll);
    ldMsgConnect(ptWidget,SIGNAL_HOLD_DOWN,slotScrollSelecterScroll);
    ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotScrollSelecterScroll);

    LOG_INFO("[init][scrollSelecter] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    return ptWidget;
}

void ldScrollSelecter_depose(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeScrollSelecter)
    {
        return;
    }

    LOG_INFO("[depose][scrollSelecter] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
#if USE_VIRTUAL_RESOURCE == 1
    ldFree(ptWidget->ptImgTile);
    ldFree(ptWidget->ptMaskTile);
    ldFree(ptWidget->ptIndicatorTile);
    ldFree(ptWidget->ptFont);
#endif
    ldFree(ptWidget);
}

void ldScrollSelecter_on_load(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldScrollSelecter_on_frame_start(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldScrollSelecter_on_frame_complete(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget)
{
    assert(NULL != ptWidget);
}

void ldScrollSelecter_show(ld_scene_t *ptScene, ldScrollSelecter_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    if((ptWidget->isWaitMove)&&(bIsNewFrame))
    {
        int16_t targetOffset=(-ptWidget->itemSelect)*(ptWidget->itemSpace+ptWidget->ptFont->tCharSize.iHeight);

        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        if(ptWidget->scrollOffset==targetOffset)
        {
            ptWidget->isWaitMove=false;
        }
        else
        {
            if(ptWidget->scrollOffset>targetOffset)
            {
                ptWidget->scrollOffset-=ptWidget->moveOffset;
                if(ptWidget->scrollOffset<targetOffset)
                {
                    ptWidget->scrollOffset=targetOffset;
                }
            }
            else
            {
                ptWidget->scrollOffset+=ptWidget->moveOffset;
                if(ptWidget->scrollOffset>targetOffset)
                {
                    ptWidget->scrollOffset=targetOffset;
                }
            }
        }
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
                if(ptWidget->ptImgTile==NULL)
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
                                                NULL,
                                                bIsNewFrame,
                                                ptWidget->use_as__ldBase_t.opacity);
                    }
                    else
                    {
                        ldBaseImage(&tTarget,
                                    NULL,
                                    ptWidget->ptImgTile,
                                    ptWidget->ptMaskTile,
                                    ptWidget->bgColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
                }
                arm_2d_op_wait_async(NULL);
            }

            arm_2d_region_t itemsRegion={
                .tLocation={
                    .iX=0,
                    .iY=0,
                },
                .tSize={
                    .iWidth=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth,
                    .iHeight=ptWidget->ptFont->tCharSize.iHeight+ptWidget->itemSpace,
                }
            };

            for(uint8_t strGroupCount=0;strGroupCount<ptWidget->itemCount;strGroupCount++)
            {
                itemsRegion.tLocation.iY=strGroupCount*(ptWidget->ptFont->tCharSize.iHeight+ptWidget->itemSpace);
                if(ptWidget->isEdit)
                {
                    itemsRegion.tLocation.iY+=ptWidget->ptFont->tCharSize.iHeight+ptWidget->scrollOffset;
                }
                else
                {
                    itemsRegion.tLocation.iY-=ptWidget->itemSelect*(ptWidget->ptFont->tCharSize.iHeight+ptWidget->itemSpace);
                }

                ldBaseLabel(&tTarget,
                            &itemsRegion,
                            (uint8_t*)ptWidget->ppItemStrGroup[strGroupCount],
                            ptWidget->ptFont,
                            ARM_2D_ALIGN_BOTTOM,
                            ptWidget->charColor,
                            ptWidget->use_as__ldBase_t.opacity);
                arm_2d_op_wait_async(NULL);
            }

            if(ptWidget->isEdit)
            {
                if(ptWidget->ptIndicatorTile==NULL)
                {
#define LINE_HEIGHT   2
                    itemsRegion.tLocation.iY=ptWidget->ptFont->tCharSize.iHeight+((ptWidget->itemSpace+LINE_HEIGHT)>>1);
                    itemsRegion.tSize.iHeight=LINE_HEIGHT;
                    arm_2d_draw_box(&tTarget,&itemsRegion,1,ptWidget->indicatorColor,255);

                    itemsRegion.tLocation.iY+=ptWidget->ptFont->tCharSize.iHeight+ptWidget->itemSpace;
                    arm_2d_draw_box(&tTarget,&itemsRegion,1,ptWidget->indicatorColor,255);
                }
                else
                {
                    ldBaseImage(&tTarget,NULL,NULL,ptWidget->ptIndicatorTile,ptWidget->indicatorColor,ptWidget->use_as__ldBase_t.opacity);
                }
            }
            arm_2d_op_wait_async(NULL);

            LD_BASE_WIDGET_SELECT;
            arm_2d_op_wait_async(NULL);
        }
    }
}

void ldScrollSelecterSetItems(ldScrollSelecter_t* ptWidget,const uint8_t *pStrArray[],uint8_t arraySize)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ppItemStrGroup=pStrArray;
    ptWidget->itemCount=arraySize;
}

void ldScrollSelecterSetTextColor(ldScrollSelecter_t* ptWidget,ldColor charColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->charColor=charColor;
}

void ldScrollSelecterSetBackgroundColor(ldScrollSelecter_t* ptWidget,ldColor bgColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->bgColor=bgColor;
    ptWidget->ptImgTile=NULL;
    ptWidget->isTransparent=false;
}

void ldScrollSelecterSetBackgroundImage(ldScrollSelecter_t* ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptImgTile=ptImgTile;
    ptWidget->ptMaskTile=ptMaskTile;
    ptWidget->isTransparent=false;
}

void ldScrollSelecterSetTransparent(ldScrollSelecter_t* ptWidget,bool isTransparent)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isTransparent=isTransparent;
}

void ldScrollSelecterSetSpeed(ldScrollSelecter_t *ptWidget, uint8_t speed)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(speed<1)
    {
        speed=1;
    }
    if(speed>ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight)
    {
        speed=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
    }
    ptWidget->moveOffset=speed;
}

void ldScrollSelecterSelectItem(ldScrollSelecter_t *ptWidget, int8_t itemNum)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(itemNum<0)
    {
        itemNum=0;
    }
    if(itemNum>=ptWidget->itemCount)
    {
        itemNum=ptWidget->itemCount-1;
    }
    ptWidget->itemSelect=itemNum;
    if(ptWidget->isEdit)
    {
        ptWidget->isWaitMove=true;
    }
}

void ldScrollSelecterSelectText(ldScrollSelecter_t *ptWidget, uint8_t *text)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(text==NULL)
    {
        return;
    }
    for(uint8_t i=0;i<ptWidget->itemCount;i++)
    {
        if(strcmp((const char *)ptWidget->ppItemStrGroup[i], (const char *)text) == 0)
        {
            ptWidget->itemSelect=i;
            if(ptWidget->isEdit)
            {
                ptWidget->isWaitMove=true;
            }
            return;
        }
    }
}

uint8_t *ldScrollSelecterCurrentText(ldScrollSelecter_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return NULL;
    }
    return (uint8_t*)ptWidget->ppItemStrGroup[ptWidget->itemSelect];
}

void ldScrollSelecterSetEditMode(ldScrollSelecter_t *ptWidget, bool isEdit)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    if(ptWidget->isEdit==isEdit)
    {
        return;
    }

    int16_t h=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
    arm_2d_region_t region=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    if(isEdit) //3->1
    {
        region.tLocation.iY-=h;
        region.tSize.iHeight=h*3;
    }
    else //1->3
    {
        h=h/3;
        region.tLocation.iY+=h;
        region.tSize.iHeight=h;
    }

    ldBaseSetRegion((ldBase_t*)ptWidget,region);

    ptWidget->isEdit=isEdit;
    
    ptWidget->itemSpace=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->ptFont->tCharSize.iHeight*3;

    if(ptWidget->itemSpace>0)
    {
        ptWidget->itemSpace=ptWidget->itemSpace>>1;
    }
    else
    {
        ptWidget->itemSpace=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->ptFont->tCharSize.iHeight;
        if(ptWidget->itemSpace>0)
        {
            ptWidget->itemSpace=ptWidget->itemSpace>>1;
        }
        else
        {
            ptWidget->itemSpace=0;
        }
    }

    ptWidget->scrollOffset=(-ptWidget->itemSelect)*(ptWidget->itemSpace+ptWidget->ptFont->tCharSize.iHeight);
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
