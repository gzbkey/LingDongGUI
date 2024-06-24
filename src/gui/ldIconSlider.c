/*
 * Copyright 2023-2024 Ou Jianbo (59935554@qq.com)
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

/**
 * @file    ldIconSlider.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   icon slider widget
 * @signal  SIGNAL_CLICKED_ITEM
 */
#include "ldIconSlider.h"
#include "ldGui.h"

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

#define MOVE_SPEED_THRESHOLD_VALUE     (20)          //触摸移动速度超过此值，则产生惯性滑动效果
#define SPEED_2_OFFSET(speed)          (speed*3)     //通过速度值，生成惯性滑动距离
#define IMG_FONT_SPACE                 (1)           //图片和文字的间隔

void ldIconSliderDel(ldIconSlider_t *pWidget);
void ldIconSliderFrameUpdate(ldIconSlider_t* pWidget);
void ldIconSliderLoop(arm_2d_scene_t *pScene,ldIconSlider_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
const ldGuiCommonFunc_t ldIconSliderCommonFunc={
    (ldDelFunc_t)ldIconSliderDel,
    (ldLoopFunc_t)ldIconSliderLoop,
    (ldUpdateFunc_t)ldIconSliderFrameUpdate,
};

static bool _iconSliderDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        if(((ldIconSlider_t *)pTarget)->pFontDict!=NULL)
        {
            for(int i=0;i<((ldIconSlider_t *)pTarget)->iconCount;i++)
            {
                ldFree(((ldIconSlider_t *)pTarget)->pIconInfoList[i].pName);
            }
        }
        ldFree(((ldIconSlider_t *)pTarget)->pIconInfoList);

        ldFree(((ldIconSlider_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldIconSliderDel(ldIconSlider_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeIconSlider)
    {
        return;
    }

    LOG_INFO("[iconSlider] del,id:%d",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _iconSliderDel);
    }
}

static uint8_t _ldIconSliderAutoSelectPage(ldIconSlider_t *pWidget,int16_t offset)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    if(offset>=0)
    {
        return 0;
    }

    offset=-offset;
    int16_t temp1,temp2;

    int d;
    if(pWidget->isHorizontalScroll)
    {
        d=pResTile->tRegion.tSize.iWidth;
    }
    else
    {
        d=pResTile->tRegion.tSize.iHeight;
    }

    temp1=offset%d;
    temp2=offset/d;
    if(temp1>=(d/2))
    {
        temp2++;
    }

    if(temp2>=pWidget->pageMax)
    {
        temp2=pWidget->pageMax-1;
    }

    return temp2;
}

static uint16_t _getStrHeight(ldFontDict_t* pFontDict)
{
    uint16_t strHeight=0;
    if(pFontDict!=NULL)
    {
        strHeight=pFontDict->lineStrHeight;
    }
    return strHeight;
}

static uint8_t _ldIconSliderAutoFirstItem(ldIconSlider_t *pWidget,int16_t offset)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    if(offset>=0)
    {
        return 0;
    }

    offset=-offset;
    int16_t temp1,temp2;

    int d;
    int8_t iconFirstMax=0;
//    找到最后的first icon
    if(pWidget->isHorizontalScroll)
    {
        d=pWidget->iconSpace+pWidget->iconWidth;
        iconFirstMax=pWidget->iconCount-(pResTile->tRegion.tSize.iWidth/d);
        iconFirstMax/=pWidget->rowCount;
    }
    else
    {
        d=pWidget->iconSpace+pWidget->iconWidth+_getStrHeight(pWidget->pFontDict)+IMG_FONT_SPACE;
        iconFirstMax=pWidget->iconCount-(pResTile->tRegion.tSize.iHeight/d);
        iconFirstMax/=pWidget->columnCount;
    }

    temp1=offset%d;
    temp2=offset/d;
    if(temp1>=(d/2))
    {
        temp2++;
    }

    if(temp2>=pWidget->iconMax)
    {
        temp2=pWidget->iconMax-1;
    }

    temp2=MIN(temp2,iconFirstMax);

    return temp2;
}

static int16_t _scrollOffset;

static bool slotIconSliderScroll(xConnectInfo_t info)
{
    ldIconSlider_t *pWidget;
    int16_t x,y;
    arm_2d_tile_t *pResTile;

    pWidget=ldBaseGetWidgetById(info.receiverId);

    pResTile=(arm_2d_tile_t*)&pWidget->resource;

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        pWidget->isWaitMove=false;
        pWidget->isAutoMove=false;
        pWidget->isHoldMove=false;
        _scrollOffset=pWidget->scrollOffset;
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        if(pWidget->isHorizontalScroll)
        {
            pWidget->scrollOffset=_scrollOffset+(int16_t)GET_SIGNAL_OFFSET_X(info.value);
        }
        else
        {
            pWidget->scrollOffset=_scrollOffset+(int16_t)GET_SIGNAL_OFFSET_Y(info.value);
        }
        pWidget->isHoldMove=true;
        break;
    }
    case SIGNAL_RELEASE:
    {
        int16_t speed;
        if(pWidget->isScrollEn)
        {
            if(pWidget->isHorizontalScroll)
            {
                speed=(int16_t)GET_SIGNAL_SPEED_X(info.value);
            }
            else
            {
                speed=(int16_t)GET_SIGNAL_SPEED_Y(info.value);
            }

            if((speed>MOVE_SPEED_THRESHOLD_VALUE)||(speed<-MOVE_SPEED_THRESHOLD_VALUE))
            {
                if(pWidget->pageMax==1)
                {
                    pWidget->selectIconOrPage=_ldIconSliderAutoFirstItem(pWidget,_scrollOffset+SPEED_2_OFFSET(speed));
                }
                else
                {
                    pWidget->selectIconOrPage=_ldIconSliderAutoSelectPage(pWidget,_scrollOffset+SPEED_2_OFFSET(speed));
                }
                pWidget->isAutoMove=true;
            }
        }

        if(!pWidget->isAutoMove)
        {
            if(pWidget->isHoldMove)
            {
                if(pWidget->pageMax==1)
                {
                    pWidget->selectIconOrPage=_ldIconSliderAutoFirstItem(pWidget,pWidget->scrollOffset);
                }
                else
                {
                    pWidget->selectIconOrPage=_ldIconSliderAutoSelectPage(pWidget,pWidget->scrollOffset);
                }
            }
            else
            {
                x=(int16_t)GET_SIGNAL_VALUE_X(info.value);
                y=(int16_t)GET_SIGNAL_VALUE_Y(info.value);

                uint16_t itemOffsetWidth,itemOffsetHeight;
                int32_t selectItem=0;

                uint8_t itemX=0;
                uint8_t itemY=0;


                ldPoint_t globalPos=ldBaseGetGlobalPos((ldCommon_t *)pWidget);
                itemOffsetWidth=pWidget->iconSpace+pWidget->iconWidth;
                itemOffsetHeight=pWidget->iconSpace+pWidget->iconWidth+_getStrHeight(pWidget->pFontDict)+IMG_FONT_SPACE;


                itemX=((x-globalPos.x))/itemOffsetWidth;
                itemY=((y-globalPos.y))/itemOffsetHeight;

                if(pWidget->pageMax==1)
                {
                    uint8_t itemOffsetX=0;
                    uint8_t itemOffsetY=0;
                    itemOffsetX=(-pWidget->scrollOffset)/itemOffsetWidth;
                    itemOffsetY=(-pWidget->scrollOffset)/itemOffsetHeight;

                    if(pWidget->isHorizontalScroll)
                    {
                        if(pWidget->rowCount==1)
                        {
                            (itemY==0)?(selectItem=itemX+itemOffsetX):(selectItem=(-1));
                        }
                    }
                    else
                    {
                        if(pWidget->columnCount==1)
                        {
                            (itemX==0)?(selectItem=itemY+itemOffsetY):(selectItem=(-1));
                        }
                        else
                        {
                            selectItem=(itemY+itemOffsetY)*pWidget->columnCount+itemX;
                        }
                    }
                }
                else
                {
                    uint8_t pageCount = _ldIconSliderAutoSelectPage(pWidget,pWidget->scrollOffset);

                    selectItem=pageCount*pWidget->rowCount*pWidget->columnCount;
                    selectItem+=itemY*pWidget->columnCount+itemX;
                }

                if(selectItem>=pWidget->iconCount)
                {
                    selectItem=(-1);
                }
                xEmit(pWidget->nameId,SIGNAL_CLICKED_ITEM,selectItem);
                LOG_DEBUG("icon slider click item %d",selectItem);
            }
        }
        pWidget->isWaitMove=true;
        break;
    }
    default:
        break;
    }
    pWidget->dirtyRegionState=waitChange;
    return false;
}

/**
 * @brief   图标滑动控件初始化
 * 
 * @param   pScene          场景指针
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @param   iconWidth       图标宽度
 * @param   iconSpace       图标间距
 * @param   columnCount     列数
 * @param   rowCount        行数
 * @param   pageMax         页数
 * @param   pFontDict       字体指针
 * @return  ldIconSlider_t* 新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
ldIconSlider_t* ldIconSliderInit(arm_2d_scene_t *pScene,uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, int16_t iconWidth, uint8_t iconSpace, uint8_t columnCount, uint8_t rowCount, uint8_t pageMax, ldFontDict_t* pFontDict)
{
    ldIconSlider_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    ldIconInfo_t* pIconInfoBuf = NULL;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
    pNewWidget = LD_CALLOC_WIDGET_INFO(ldIconSlider_t);
    rowCount=MAX(rowCount,1);
    columnCount=MAX(columnCount,1);
    pageMax=MAX(pageMax,1);
    pIconInfoBuf = ldCalloc(sizeof (ldIconInfo_t)*(rowCount*columnCount*pageMax));
    if ((pNewWidget != NULL)&&(pIconInfoBuf != NULL))
    {
        pNewWidget->isParentHidden=false;
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeIconSlider;
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isHidden = false;
        tResTile=(arm_2d_tile_t*)&pNewWidget->resource;
        tResTile->tRegion.tLocation.iX=x;
        tResTile->tRegion.tLocation.iY=y;
        tResTile->tRegion.tSize.iWidth=width;
        tResTile->tRegion.tSize.iHeight=height;
        tResTile->tInfo.bIsRoot = true;
        tResTile->tInfo.bHasEnforcedColour = true;
        tResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        tResTile->pchBuffer = (uint8_t*)LD_ADDR_NONE;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        ((arm_2d_vres_t*)tResTile)->pTarget = LD_ADDR_NONE;
        ((arm_2d_vres_t*)tResTile)->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)tResTile)->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
        pNewWidget->iconMax=rowCount*columnCount*pageMax;
        pNewWidget->rowCount=rowCount;
        pNewWidget->columnCount=columnCount;
        pNewWidget->pageMax=pageMax;
        pNewWidget->pIconInfoList=pIconInfoBuf;
        pNewWidget->iconSpace=iconSpace;
        pNewWidget->iconWidth=iconWidth;
        pNewWidget->iconCount=0;
        pNewWidget->hasVerticalBorder=false;
        pNewWidget->hasHorizontalBorder=false;
        pNewWidget->isHorizontalScroll=true;
        pNewWidget->scrollOffset=0;
        pNewWidget->moveOffset=1;
        pNewWidget->selectIconOrPage=0;
        pNewWidget->pFontDict=pFontDict;
        pNewWidget->charColor=0;
        if(rowCount==1)
        {
            if(columnCount==1)
            {
                pNewWidget->hasVerticalBorder=false;
                pNewWidget->hasHorizontalBorder=false;
            }
            else
            {
                pNewWidget->hasVerticalBorder=true;
                pNewWidget->hasHorizontalBorder=false;
            }
        }
        else
        {
            if(columnCount==1)
            {
                pNewWidget->hasVerticalBorder=false;
                pNewWidget->hasHorizontalBorder=true;
            }
            else
            {
                pNewWidget->hasVerticalBorder=true;
                pNewWidget->hasHorizontalBorder=true;
            }
        }

        pNewWidget->isScrollEn=true;

        // 单行的情况
        if((pNewWidget->pageMax==1)&&((pNewWidget->rowCount==1)||(pNewWidget->columnCount==1)))
        {
            if(pNewWidget->rowCount<pNewWidget->columnCount)
            {
                pNewWidget->isHorizontalScroll=true;
                if(width>=pNewWidget->iconMax*(pNewWidget->iconWidth+pNewWidget->iconSpace))
                {
                    pNewWidget->isScrollEn=false;
                }
            }
            else
            {
                pNewWidget->isHorizontalScroll=false;

                if(height>=pNewWidget->iconMax*(pNewWidget->iconWidth+pNewWidget->iconSpace+_getStrHeight(pFontDict)+IMG_FONT_SPACE))
                {
                    pNewWidget->isScrollEn=false;
                }
            }
        }
        pNewWidget->pFunc=&ldIconSliderCommonFunc;
        pNewWidget->dirtyRegionState=waitChange;

        arm_2d_scene_player_dynamic_dirty_region_init(&pNewWidget->dirtyRegionListItem,pScene);

        xConnect(pNewWidget->nameId,SIGNAL_PRESS,pNewWidget->nameId,slotIconSliderScroll);
        xConnect(pNewWidget->nameId,SIGNAL_RELEASE,pNewWidget->nameId,slotIconSliderScroll);
        if(pNewWidget->isScrollEn)
        {
            xConnect(pNewWidget->nameId,SIGNAL_HOLD_DOWN,pNewWidget->nameId,slotIconSliderScroll);
        }

        LOG_INFO("[iconSlider] init,id:%d",nameId);
    }
    else
    {
        ldFree(pNewWidget);
        ldFree(pIconInfoBuf);

        LOG_ERROR("[iconSlider] init failed,id:%d",nameId);
    }

    return pNewWidget;
}

void ldIconSliderFrameUpdate(ldIconSlider_t* pWidget)
{
    pWidget->dirtyRegionState=ldBaseUpdateDirtyRegionState(pWidget->dirtyRegionState);
    arm_2d_dynamic_dirty_region_on_frame_start(&pWidget->dirtyRegionListItem,waitChange);
}

void ldIconSliderLoop(arm_2d_scene_t *pScene,ldIconSlider_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;
    int16_t x,y,offsetX,offsetY;

#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tempRes = *pResTile;
#else
    arm_2d_vres_t tempRes = *((arm_2d_vres_t*)pResTile);
#endif
    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        return;
    }

    if((pWidget->isWaitMove)&&(bIsNewFrame))
    {
        int16_t targetOffset=0;

        if(pWidget->pageMax==1)
        {
            if(pWidget->isHorizontalScroll)
            {
                targetOffset=-(pWidget->selectIconOrPage*(pWidget->iconSpace+pWidget->iconWidth));
            }
            else
            {
                targetOffset=-(pWidget->selectIconOrPage*(pWidget->iconSpace+pWidget->iconWidth+_getStrHeight(pWidget->pFontDict)+IMG_FONT_SPACE));
            }
        }
        else
        {
            if(pWidget->isHorizontalScroll)
            {
                targetOffset=-(pWidget->selectIconOrPage*((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iWidth);
            }
            else
            {
                targetOffset=-(pWidget->selectIconOrPage*((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight);
            }
        }


        if(pWidget->scrollOffset==targetOffset)
        {
            pWidget->isWaitMove=false;
            pWidget->dirtyRegionState=waitEnd;
        }
        else
        {
            if(pWidget->scrollOffset>targetOffset)
            {
                pWidget->scrollOffset-=pWidget->moveOffset;
                if(pWidget->scrollOffset<targetOffset)
                {
                    pWidget->scrollOffset=targetOffset;
                }
            }
            else
            {
                pWidget->scrollOffset+=pWidget->moveOffset;
                if(pWidget->scrollOffset>targetOffset)
                {
                    pWidget->scrollOffset=targetOffset;
                }
            }
            pWidget->dirtyRegionState=waitChange;
        }
    }

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
#if LD_DEBUG == 1
        arm_2d_draw_box(&tTarget,&tTarget_canvas,1,0,255);
#endif
        if(ldBaseDirtyRegionUpdate((ldCommon_t*)pWidget,&tTarget_canvas,&pWidget->dirtyRegionListItem,pWidget->dirtyRegionState))
        {
            pWidget->dirtyRegionState=waitEnd;
        }

        if(pWidget->dirtyRegionState<waitRefresh)
        {
            return;
        }

        uint8_t showCount=0;
        for(uint8_t pageCount=0;pageCount<pWidget->pageMax;pageCount++)
        {
            offsetX=0;
            offsetY=0;
            if(pWidget->hasVerticalBorder)
            {
                offsetX=pWidget->iconSpace;
            }
            if(pWidget->hasHorizontalBorder)
            {
                offsetY=pWidget->iconSpace;
            }
            if(pWidget->isHorizontalScroll)
            {
                offsetX+=pageCount*pResTile->tRegion.tSize.iWidth;
                offsetX+=pWidget->scrollOffset;
            }
            else
            {
                offsetY+=pageCount*pResTile->tRegion.tSize.iHeight;
                offsetY+=pWidget->scrollOffset;
            }

            y=0;
            for(uint8_t rowCount=0;rowCount<pWidget->rowCount;rowCount++)
            {
                x=0;
                for(uint8_t columnCount=0;columnCount<pWidget->columnCount;columnCount++)
                {
                    if((showCount+1)>pWidget->iconCount)
                    {
                        continue;
                    }

                    arm_2d_tile_t imgPosTile = impl_child_tile(tTarget,x+offsetX,y+offsetY,pWidget->iconWidth,pWidget->iconWidth);
                    ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iWidth=pWidget->iconWidth;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iHeight=pWidget->iconWidth;
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer=(uint8_t *)pWidget->pIconInfoList[showCount].imgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    tempRes.pTarget=pWidget->pIconInfoList[showCount].imgAddr;
#endif
                    ldBaseImage(&imgPosTile,(arm_2d_tile_t*)&tempRes,pWidget->pIconInfoList[showCount].isWithMask,255);
                    arm_2d_op_wait_async(NULL);

                    if((pWidget->pFontDict!=NULL)&&(pWidget->pIconInfoList[showCount].pName!=NULL))
                    {
                        arm_2d_tile_t fontPosTile = impl_child_tile(tTarget,x+offsetX,y+offsetY+pWidget->iconWidth+IMG_FONT_SPACE,pWidget->iconWidth,pWidget->pFontDict->lineStrHeight);
                        ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme = ldBaseGetChScheme(pWidget->pFontDict->maskType);
                        ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iWidth=fontPosTile.tRegion.tSize.iWidth;
                        ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iHeight=pWidget->pFontDict->lineStrHeight;
                        ldBaseLineText(&fontPosTile,&tempRes,pWidget->pIconInfoList[showCount].pName,pWidget->pFontDict,LD_ALIGN_CENTER,pWidget->charColor,0,255);
                        arm_2d_op_wait_async(NULL);
                    }

                    showCount++;
                    x+=pWidget->iconWidth+pWidget->iconSpace;
                }
                y+=pWidget->iconWidth+pWidget->iconSpace+_getStrHeight(pWidget->pFontDict)+IMG_FONT_SPACE;
            }
        }
    }
}

/**
 * @brief   添加图标
 * 
 * @param   pWidget         目标控件指针
 * @param   imageAddr       图片地址
 * @param   isWithMask      图片带mask
 * @param   pNameStr        图标名称字符串指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldIconSliderAddIcon(ldIconSlider_t *pWidget,uintptr_t imageAddr,bool isWithMask,uint8_t* pNameStr)
{
    if(pWidget==NULL)
    {
        return;
    }

    if(pWidget->iconCount<pWidget->iconMax)
    {
        pWidget->pIconInfoList[pWidget->iconCount].imgAddr=imageAddr;
        pWidget->pIconInfoList[pWidget->iconCount].isWithMask=isWithMask;

        if((pWidget->pFontDict!=NULL)&&(pNameStr!=NULL))
        {
            pWidget->pIconInfoList[pWidget->iconCount].pName=LD_CALLOC_STRING(pNameStr);
            if(pWidget->pIconInfoList[pWidget->iconCount].pName!=NULL)
            {
                strcpy((char*)pWidget->pIconInfoList[pWidget->iconCount].pName,(char*)pNameStr);
            }
        }

        pWidget->iconCount++;
    }

}

/**
 * @brief   滑动方向
 * 
 * @param   pWidget         目标控件指针
 * @param   isHorizontal    true=水平滑动 false=纵向滑动
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldIconSliderSetHorizontalScroll(ldIconSlider_t *pWidget,bool isHorizontal)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isHorizontalScroll=isHorizontal;
}

/**
 * @brief   设置移动速度
 * 
 * @param   pWidget         目标控件指针
 * @param   speed           速度值，最小值:1,最大值：控件宽度或者高度
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldIconSliderSetSpeed(ldIconSlider_t *pWidget, uint8_t speed)
{
    if (pWidget == NULL)
    {
        return;
    }
    if(speed<1)
    {
        speed=1;
    }
    if(pWidget->isHorizontalScroll)
    {
        if(speed>((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iWidth)
        {
            speed=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iWidth;
        }
    }
    else
    {
        if(speed>((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight)
        {
            speed=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight;
        }
    }

    pWidget->moveOffset=speed;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
