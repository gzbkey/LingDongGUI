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

#define __LD_ICON_SLIDER_IMPLEMENT__

#include "__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldIconSlider.h"

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
#define IMG_FONT_SPACE                  (1)           //图片和文字的间隔

const ldBaseWidgetFunc_t ldIconSliderFunc = {
    .depose = (ldDeposeFunc_t)ldIconSlider_depose,
    .load = (ldLoadFunc_t)ldIconSlider_on_load,
#ifdef FRAME_START
    .frameStart = (ldFrameStartFunc_t)ldIconSlider_on_frame_start,
#endif
    .show = (ldShowFunc_t)ldIconSlider_show,
};


static uint8_t _ldIconSliderAutoSelectPage(ldIconSlider_t *ptWidget,int16_t offset)
{
    if(offset>=0)
    {
        return 0;
    }

    offset=-offset;
    int16_t temp1,temp2;

    int d;
    if(ptWidget->isHorizontalScroll)
    {
        d=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth;
    }
    else
    {
        d=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
    }

    temp1=offset%d;
    temp2=offset/d;
    if(temp1>=(d/2))
    {
        temp2++;
    }

    if(temp2>=ptWidget->pageMax)
    {
        temp2=ptWidget->pageMax-1;
    }

    return temp2;
}

static uint8_t _ldIconSliderAutoFirstItem(ldIconSlider_t *ptWidget,int16_t offset)
{
    if(offset>=0)
    {
        return 0;
    }

    offset=-offset;
    int16_t temp1,temp2;

    int d;
    int8_t iconFirstMax=0;
//    找到最后的first icon
    if(ptWidget->isHorizontalScroll)
    {
        d=ptWidget->iconSpace+ptWidget->iconWidth;
        iconFirstMax=ptWidget->iconCount-(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth/d);
        iconFirstMax/=ptWidget->rowCount;
    }
    else
    {
        d=ptWidget->iconSpace+ptWidget->iconWidth+ptWidget->ptFont->tCharSize.iHeight+IMG_FONT_SPACE;
        iconFirstMax=ptWidget->iconCount-(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight/d);
        iconFirstMax/=ptWidget->columnCount;
    }

    temp1=offset%d;
    temp2=offset/d;
    if(temp1>=(d/2))
    {
        temp2++;
    }

    if(temp2>=ptWidget->iconMax)
    {
        temp2=ptWidget->iconMax-1;
    }

    temp2=MIN(temp2,iconFirstMax);

    return temp2;
}

static int16_t _scrollOffset;

static bool slotIconSliderScroll(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldIconSlider_t *ptWidget=msg.ptSender;
    int16_t x,y;
    arm_2d_tile_t *pResTile;

    switch (msg.signal)
    {
    case SIGNAL_PRESS:
    {
        ptWidget->isWaitMove=false;
        ptWidget->isAutoMove=false;
        ptWidget->isHoldMove=false;
        _scrollOffset=ptWidget->scrollOffset;
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        if(ptWidget->isHorizontalScroll)
        {
            ptWidget->scrollOffset=_scrollOffset+(int16_t)GET_SIGNAL_OFFSET_X(msg.value);
        }
        else
        {
            ptWidget->scrollOffset=_scrollOffset+(int16_t)GET_SIGNAL_OFFSET_Y(msg.value);
        }
        ptWidget->isHoldMove=true;
        break;
    }
    case SIGNAL_RELEASE:
    {
        int16_t speed;
        if(ptWidget->isScrollEn)
        {
            if(ptWidget->isHorizontalScroll)
            {
                speed=(int16_t)GET_SIGNAL_SPEED_X(msg.value);
            }
            else
            {
                speed=(int16_t)GET_SIGNAL_SPEED_Y(msg.value);
            }

            if((speed>MOVE_SPEED_THRESHOLD_VALUE)||(speed<-MOVE_SPEED_THRESHOLD_VALUE))
            {
                if(ptWidget->pageMax==1)
                {
                    ptWidget->selectIconOrPage=_ldIconSliderAutoFirstItem(ptWidget,_scrollOffset+SPEED_2_OFFSET(speed));
                }
                else
                {
                    ptWidget->selectIconOrPage=_ldIconSliderAutoSelectPage(ptWidget,_scrollOffset+SPEED_2_OFFSET(speed));
                }
                ptWidget->isAutoMove=true;
            }
        }

        if(!ptWidget->isAutoMove)
        {
            if(ptWidget->isHoldMove)
            {
                if(ptWidget->pageMax==1)
                {
                    ptWidget->selectIconOrPage=_ldIconSliderAutoFirstItem(ptWidget,ptWidget->scrollOffset);
                }
                else
                {
                    ptWidget->selectIconOrPage=_ldIconSliderAutoSelectPage(ptWidget,ptWidget->scrollOffset);
                }
            }
            else
            {
                x=(int16_t)GET_SIGNAL_VALUE_X(msg.value);
                y=(int16_t)GET_SIGNAL_VALUE_Y(msg.value);

                uint16_t itemOffsetWidth,itemOffsetHeight;
                int32_t selectItem=0;

                uint8_t itemX=0;
                uint8_t itemY=0;

                arm_2d_location_t globalPos={0};
                globalPos=ldBaseGetAbsoluteLocation((ldBase_t*)ptWidget,globalPos);
                itemOffsetWidth=ptWidget->iconSpace+ptWidget->iconWidth;
                itemOffsetHeight=ptWidget->iconSpace+ptWidget->iconWidth+ptWidget->ptFont->tCharSize.iHeight+IMG_FONT_SPACE;


                itemX=((x-globalPos.iX))/itemOffsetWidth;
                itemY=((y-globalPos.iY))/itemOffsetHeight;

                if(ptWidget->pageMax==1)
                {
                    uint8_t itemOffsetX=0;
                    uint8_t itemOffsetY=0;
                    itemOffsetX=(-ptWidget->scrollOffset)/itemOffsetWidth;
                    itemOffsetY=(-ptWidget->scrollOffset)/itemOffsetHeight;

                    if(ptWidget->isHorizontalScroll)
                    {
                        if(ptWidget->rowCount==1)
                        {
                            (itemY==0)?(selectItem=itemX+itemOffsetX):(selectItem=(-1));
                        }
                    }
                    else
                    {
                        if(ptWidget->columnCount==1)
                        {
                            (itemX==0)?(selectItem=itemY+itemOffsetY):(selectItem=(-1));
                        }
                        else
                        {
                            selectItem=(itemY+itemOffsetY)*ptWidget->columnCount+itemX;
                        }
                    }
                }
                else
                {
                    uint8_t pageCount = _ldIconSliderAutoSelectPage(ptWidget,ptWidget->scrollOffset);

                    selectItem=pageCount*ptWidget->rowCount*ptWidget->columnCount;
                    selectItem+=itemY*ptWidget->columnCount+itemX;
                }

                if(selectItem>=ptWidget->iconCount)
                {
                    selectItem=(-1);
                }
                ldMsgEmit(ptScene->ptMsgQueue,ptWidget,SIGNAL_CLICKED_ITEM,selectItem);
                LOG_DEBUG("click item %d",selectItem);
            }
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

ldIconSlider_t* ldIconSlider_init( ld_scene_t *ptScene,ldIconSlider_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, int16_t iconWidth, uint8_t iconSpace, uint8_t columnCount, uint8_t rowCount, uint8_t pageMax, arm_2d_font_t* ptFont)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    ldIconInfo_t* ptIconInfoBuf = NULL;

    rowCount=MAX(rowCount,1);
    columnCount=MAX(columnCount,1);
    pageMax=MAX(pageMax,1);

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldIconSlider_t));
        ptIconInfoBuf = ldCalloc(1, sizeof (ldIconInfo_t)*(rowCount*columnCount*pageMax));
        if ((NULL == ptWidget)||(NULL == ptIconInfoBuf))
        {
            ldFree(ptWidget);
            ldFree(ptIconInfoBuf);
            LOG_ERROR("[init failed][iconSlider] id:%d", nameId);
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
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeIconSlider;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldIconSliderFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->iconMax=rowCount*columnCount*pageMax;
    ptWidget->rowCount=rowCount;
    ptWidget->columnCount=columnCount;
    ptWidget->pageMax=pageMax;
    ptWidget->ptIconInfoList=ptIconInfoBuf;
    ptWidget->iconSpace=iconSpace;
    ptWidget->iconWidth=iconWidth;
    ptWidget->iconCount=0;
    ptWidget->hasVerticalBorder=false;
    ptWidget->hasHorizontalBorder=false;
    ptWidget->isHorizontalScroll=true;
    ptWidget->scrollOffset=0;
    ptWidget->moveOffset=1;
    ptWidget->selectIconOrPage=0;
    ptWidget->ptFont=ptFont;
    ptWidget->charColor=0;
    if(rowCount==1)
    {
        if(columnCount==1)
        {
            ptWidget->hasVerticalBorder=false;
            ptWidget->hasHorizontalBorder=false;
        }
        else
        {
            ptWidget->hasVerticalBorder=true;
            ptWidget->hasHorizontalBorder=false;
        }
    }
    else
    {
        if(columnCount==1)
        {
            ptWidget->hasVerticalBorder=false;
            ptWidget->hasHorizontalBorder=true;
        }
        else
        {
            ptWidget->hasVerticalBorder=true;
            ptWidget->hasHorizontalBorder=true;
        }
    }

    ptWidget->isScrollEn=true;

    // 单行的情况
    if((ptWidget->pageMax==1)&&((ptWidget->rowCount==1)||(ptWidget->columnCount==1)))
    {
        if(ptWidget->rowCount<ptWidget->columnCount)
        {
            ptWidget->isHorizontalScroll=true;
            if(width>=ptWidget->iconMax*(ptWidget->iconWidth+ptWidget->iconSpace))
            {
                ptWidget->isScrollEn=false;
            }
        }
        else
        {
            ptWidget->isHorizontalScroll=false;

            if(height>=ptWidget->iconMax*(ptWidget->iconWidth+ptWidget->iconSpace+ptFont->tCharSize.iHeight+IMG_FONT_SPACE))
            {
                ptWidget->isScrollEn=false;
            }
        }
    }

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotIconSliderScroll);
    ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotIconSliderScroll);
    if(ptWidget->isScrollEn)
    {
        ldMsgConnect(ptWidget,SIGNAL_HOLD_DOWN,slotIconSliderScroll);
    }

    LOG_INFO("[init][iconSlider] id:%d, size:%d", nameId,sizeof (*ptWidget));
    return ptWidget;
}

void ldIconSlider_depose( ldIconSlider_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeIconSlider)
    {
        return;
    }

    LOG_INFO("[depose][iconSlider] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget->ptIconInfoList);
    ldFree(ptWidget);
}

void ldIconSlider_on_load( ldIconSlider_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldIconSlider_on_frame_start( ldIconSlider_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldIconSlider_show(ld_scene_t *ptScene, ldIconSlider_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    int16_t x,y,offsetX,offsetY;

    if((ptWidget->isWaitMove)&&(bIsNewFrame))
    {
        int16_t targetOffset=0;

        if(ptWidget->pageMax==1)
        {
            if(ptWidget->isHorizontalScroll)
            {
                targetOffset=-(ptWidget->selectIconOrPage*(ptWidget->iconSpace+ptWidget->iconWidth));
            }
            else
            {
                targetOffset=-(ptWidget->selectIconOrPage*(ptWidget->iconSpace+ptWidget->iconWidth+ptWidget->ptFont->tCharSize.iHeight+IMG_FONT_SPACE));
            }
        }
        else
        {
            if(ptWidget->isHorizontalScroll)
            {
                targetOffset=-(ptWidget->selectIconOrPage*ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth);
            }
            else
            {
                targetOffset=-(ptWidget->selectIconOrPage*ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight);
            }
        }


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
            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        }
    }

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

            uint8_t showCount=0;
            for(uint8_t pageCount=0;pageCount<ptWidget->pageMax;pageCount++)
            {
                offsetX=0;
                offsetY=0;
                if(ptWidget->hasVerticalBorder)
                {
                    offsetX=ptWidget->iconSpace;
                }
                if(ptWidget->hasHorizontalBorder)
                {
                    offsetY=ptWidget->iconSpace;
                }
                if(ptWidget->isHorizontalScroll)
                {
                    offsetX+=pageCount*ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth;
                    offsetX+=ptWidget->scrollOffset;
                }
                else
                {
                    offsetY+=pageCount*ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
                    offsetY+=ptWidget->scrollOffset;
                }

                y=0;
                for(uint8_t rowCount=0;rowCount<ptWidget->rowCount;rowCount++)
                {
                    x=0;
                    for(uint8_t columnCount=0;columnCount<ptWidget->columnCount;columnCount++)
                    {
                        if((showCount+1)>ptWidget->iconCount)
                        {
                            continue;
                        }

                        arm_2d_region_t imgRegion ={
                            .tLocation={
                                .iX=x+offsetX,
                                .iY=y+offsetY,
                            },
                            .tSize={
                                .iWidth=ptWidget->iconWidth,
                                .iHeight=ptWidget->iconWidth,
                            }
                        };

                        ldBaseImage(&tTarget,
                                    &imgRegion,
                                    ptWidget->ptIconInfoList[showCount].ptImgTile,
                                    ptWidget->ptIconInfoList[showCount].ptMaskTile,
                                    0,
                                    ptWidget->use_as__ldBase_t.opacity);

                        arm_2d_op_wait_async(NULL);

                        if((ptWidget->ptFont!=NULL)&&(ptWidget->ptIconInfoList[showCount].pName!=NULL))
                        {
                            arm_2d_region_t fontRegion ={
                                .tLocation={
                                    .iX=x+offsetX,
                                    .iY=y+offsetY+ptWidget->iconWidth+IMG_FONT_SPACE,
                                },
                                .tSize={
                                    .iWidth=ptWidget->iconWidth,
                                    .iHeight=ptWidget->ptFont->tCharSize.iHeight,
                                }
                            };
                            ldBaseLabel(&tTarget,
                                        &fontRegion,
                                        ptWidget->ptIconInfoList[showCount].pName,
                                        ptWidget->ptFont,
                                        ARM_2D_ALIGN_TOP,
                                        ptWidget->charColor,
                                        ptWidget->use_as__ldBase_t.opacity);
                            arm_2d_op_wait_async(NULL);
                        }

                        showCount++;
                        x+=ptWidget->iconWidth+ptWidget->iconSpace;
                    }
                    y+=ptWidget->iconWidth+ptWidget->iconSpace+ptWidget->ptFont->tCharSize.iHeight+IMG_FONT_SPACE;
                }
            }

        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldIconSliderAddIcon(ldIconSlider_t *ptWidget,arm_2d_tile_t *ptImgTile,arm_2d_tile_t *ptMaskTile,const uint8_t* pNameStr)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    if(ptWidget->iconCount<ptWidget->iconMax)
    {
        ptWidget->ptIconInfoList[ptWidget->iconCount].ptImgTile=ptImgTile;
        ptWidget->ptIconInfoList[ptWidget->iconCount].ptMaskTile=ptMaskTile;

        if((ptWidget->ptFont!=NULL)&&(pNameStr!=NULL))
        {
            ptWidget->ptIconInfoList[ptWidget->iconCount].pName=pNameStr;
        }
        ptWidget->iconCount++;
    }
}

void ldIconSliderSetHorizontalScroll(ldIconSlider_t *ptWidget,bool isHorizontal)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->isHorizontalScroll=isHorizontal;
}

void ldIconSliderSetSpeed(ldIconSlider_t *ptWidget, uint8_t speed)
{
    if (ptWidget == NULL)
    {
        return;
    }
    if(speed<1)
    {
        speed=1;
    }
    if(ptWidget->isHorizontalScroll)
    {
        if(speed>ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth)
        {
            speed=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth;
        }
    }
    else
    {
        if(speed>ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight)
        {
            speed=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
        }
    }

    ptWidget->moveOffset=speed;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
