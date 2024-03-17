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
 * @file    ldTable.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   表格控件
 */
#include "ldTable.h"
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

void ldTableDel(ldTable_t *pWidget);
void ldTableFrameUpdate(ldTable_t* pWidget);
void ldTableLoop(ldTable_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
const ldGuiCommonFunc_t ldTableCommonFunc={
    (ldDelFunc_t)ldTableDel,
    (ldLoopFunc_t)ldTableLoop,
    (ldUpdateFunc_t)ldTableFrameUpdate,
};

static bool _tableDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldTable_t * target=pTarget;
        int count=target->columnCount*target->rowCount;
        for(int i=0;i<count;i++)
        {
            if(target->pItemInfo[i].isStaticText==false)
            {
                ldFree(target->pItemInfo[i].pText);
            }
        }
        ldFree(target->pItemInfo);
        ldFree(target->pRowHeight);
        ldFree(target->pColumnWidth);

        ldFree(target);
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldTableDel(ldTable_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeTable)
    {
        return;
    }

    LOG_INFO("[table] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _tableDel);
    }
}

void _ldTableSelectItem(ldTable_t *pWidget,ldTableItem_t *item)
{
    for(uint8_t y=0;y<pWidget->rowCount;y++)
    {
        for(uint8_t x=0;x<pWidget->columnCount;x++)
        {
            if(&pWidget->pItemInfo[y*pWidget->columnCount+x]==item)
            {
                item->isSelect=true;
                pWidget->currentRow=y;
                pWidget->currentColumn=x;
            }
            else
            {
                pWidget->pItemInfo[y*pWidget->columnCount+x].isSelect=false;
            }
        }
    }
}

/**
 * @brief   获取选中列的编号
 * 
 * @param   pWidget         目标控件指针
 * @return  uint8_t         列号
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-21
 */
uint8_t ldTableCurrentColumn(ldTable_t *pWidget)
{
    return pWidget->currentColumn;
}

/**
 * @brief   获取选中行的编号
 * 
 * @param   pWidget         目标控件指针
 * @return  uint8_t         行号
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-21
 */
uint8_t ldTableCurrentRow(ldTable_t *pWidget)
{
    return pWidget->currentRow;
}

/**
 * @brief   获取选中行的项目
 * 
 * @param   pWidget         目标控件指针
 * @return  ldTableItem_t*  返回目标item指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-21
 */
ldTableItem_t *ldTableCurrentItem(ldTable_t *pWidget)
{
    return ldTableItem(pWidget,pWidget->currentRow,pWidget->currentColumn);
}


ldPoint_t _ldTableGetItemPos(ldTable_t *pWidget,uint8_t row, uint8_t column)
{
    ldPoint_t offset={0,0};

    for(uint8_t x=0;x<pWidget->columnCount;x++)
    {
        if(x<column)
        {
            offset.x+=pWidget->itemSpace;
            offset.x+=pWidget->pColumnWidth[x];
        }
    }
    offset.x+=pWidget->itemSpace+pWidget->scrollOffsetX;

    for(uint8_t y=0;y<pWidget->rowCount;y++)
    {
        if(y<row)
        {
            offset.y+=pWidget->itemSpace;
            offset.y+=pWidget->pRowHeight[y];
        }
    }
    offset.y+=pWidget->itemSpace+pWidget->scrollOffsetY;
    return offset;
}

arm_2d_region_t _ldTableGetItemRegion(ldTable_t *pWidget,uint8_t row, uint8_t column)
{
    ldPoint_t targetItemPos=_ldTableGetItemPos(pWidget,row,column);
    arm_2d_region_t itemRegion;
    itemRegion.tLocation.iX=targetItemPos.x;
    itemRegion.tLocation.iY=targetItemPos.y;
    itemRegion.tSize.iWidth=pWidget->pColumnWidth[column];
    itemRegion.tSize.iHeight=pWidget->pRowHeight[row];
    return itemRegion;
}

arm_2d_region_t _ldTableGetItemCellGlobalRegion(ldTable_t *pWidget,uint8_t row, uint8_t column)
{
    ldTableItem_t *targetItem=ldTableItem(pWidget,row,column);

    ldPoint_t targetItemPos=_ldTableGetItemPos(pWidget,row,column);

    int16_t targetItemWidth=pWidget->pColumnWidth[column];

    int16_t targetItemHeight=pWidget->pRowHeight[row];

    arm_2d_region_t itemRegion={
        .tLocation={
            .iX=0,
            .iY=0,
        },
        .tSize={
            .iWidth=targetItemWidth,
            .iHeight=targetItemHeight,
        },
    };

    arm_2d_region_t outRegion;

    arm_2d_region_intersect(&itemRegion,&targetItem->imgRegion,&outRegion);

    outRegion.tLocation.iX=targetItem->imgRegion.tLocation.iX+targetItemPos.x+((arm_2d_tile_t *)(arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iX;
    outRegion.tLocation.iY=targetItem->imgRegion.tLocation.iY+targetItemPos.y+((arm_2d_tile_t *)(arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iY;

    return outRegion;
}

/**
 * @brief   获取指定的项目
 * 
 * @param   pWidget         目标控件指针
 * @param   row             行号
 * @param   column          列号
 * @return  ldTableItem_t*  返回目标item指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-21
 */
ldTableItem_t *ldTableItem(ldTable_t *pWidget,uint8_t row, uint8_t column)
{
    return &pWidget->pItemInfo[row*pWidget->columnCount+column];
}

/**
 * @brief   获取指定坐标的项目
 * 
 * @param   pWidget         目标控件指针
 * @param   x               全局坐标x轴
 * @param   y               全局坐标y轴
 * @return  ldTableItem_t*  返回目标item指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-21
 */
ldTableItem_t *ldTableItemAt(ldTable_t *pWidget,int16_t x,int16_t y)
{
    uint8_t itemX,itemY;
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;
    x-=pResTile->tRegion.tLocation.iX;
    y-=pResTile->tRegion.tLocation.iY;

    itemX=0;
    itemY=0;
    int16_t tempOffset=pWidget->scrollOffsetX-x;
    for(uint8_t i=0;i<pWidget->columnCount;i++)
    {
        if(tempOffset<-pWidget->pColumnWidth[i])
        {
            tempOffset+=pWidget->pColumnWidth[i];
            tempOffset+=pWidget->itemSpace;
            itemX++;
        }
        else
        {
            break;
        }
    }

    tempOffset=pWidget->scrollOffsetY-y;
    for(uint8_t i=0;i<pWidget->rowCount;i++)
    {
        if(tempOffset<-pWidget->pRowHeight[i])
        {
            tempOffset+=pWidget->pRowHeight[i];
            tempOffset+=pWidget->itemSpace;
            itemY++;
        }
        else
        {
            break;
        }
    }

    if(itemX>=pWidget->columnCount)
    {
        itemX=pWidget->columnCount-1;
    }
    if(itemY>=pWidget->rowCount)
    {
        itemY=pWidget->rowCount-1;
    }

    return &pWidget->pItemInfo[itemY*pWidget->columnCount+itemX];
}

static int16_t _scrollOffsetX,_scrollOffsetY;
static int16_t _totalItemWidth,_totalItemHeight;
static bool _isStopMove;
static bool slotTableProcess(xConnectInfo_t info)
{
    ldTable_t *pWidget;
    int16_t x,y;
    int16_t offsetX,offsetY;
    arm_2d_tile_t *pResTile;
    ldTableItem_t *currentItem;
    int64_t tempTimer;
    ldCommonKB_t *kb;

    pWidget=ldBaseGetWidgetById(info.receiverId);

    pResTile=(arm_2d_tile_t*)&pWidget->resource;

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        ldPoint_t parentPos=ldBaseGetGlobalPos(pWidget->parentWidget);
        x=(int16_t)GET_SIGNAL_VALUE_X(info.value)-parentPos.x;
        y=(int16_t)GET_SIGNAL_VALUE_Y(info.value)-parentPos.y;

        currentItem=ldTableItemAt(pWidget,x,y);

        _ldTableSelectItem(pWidget,currentItem);

        _scrollOffsetX=pWidget->scrollOffsetX;
        _scrollOffsetY=pWidget->scrollOffsetY;

        _totalItemWidth=0;
        for(uint8_t i=0;i<pWidget->columnCount;i++)
        {
            _totalItemWidth+=pWidget->itemSpace;
            _totalItemWidth+=pWidget->pColumnWidth[i];
        }
        _totalItemWidth+=pWidget->itemSpace;

        _totalItemHeight=0;
        for(uint8_t i=0;i<pWidget->rowCount;i++)
        {
            _totalItemHeight+=pWidget->itemSpace;
            _totalItemHeight+=pWidget->pRowHeight[i];
        }

        if(currentItem->isButton)
        {
            arm_2d_region_t cellRegion= _ldTableGetItemCellGlobalRegion(pWidget,pWidget->currentRow,pWidget->currentColumn);

            if((x>=cellRegion.tLocation.iX)&&(y>=cellRegion.tLocation.iY)&&
                    (x<(cellRegion.tLocation.iX+cellRegion.tSize.iWidth))&&
                    (y<(cellRegion.tLocation.iY+cellRegion.tSize.iHeight)))
            {
                _isStopMove=true;
                currentItem->isChecked=!currentItem->isChecked;
            }
        }

        if(currentItem->isEditable)
        {
            tempTimer=arm_2d_helper_convert_ticks_to_ms(arm_2d_helper_get_system_timestamp());

            if((tempTimer-pWidget->timer)<500)
            {
                currentItem->isEditing=true;
                if(pWidget->kbNameId)
                {
                    kb=ldBaseGetWidgetById(pWidget->kbNameId);
                    if(kb!=NULL)
                    {
                        kb->editType=currentItem->editType;
                        kb->ppStr=&currentItem->pText;
                        kb->strMax=currentItem->textMax;
                        kb->editorId=pWidget->nameId;
                        cursorBlinkFlag=true;
                        cursorBlinkCount=0;
                        ldBaseSetHidden((ldCommon_t*)kb,false);

                        arm_2d_region_t itemRegion= _ldTableGetItemRegion(pWidget,pWidget->currentRow,pWidget->currentColumn);

                        if((itemRegion.tLocation.iY+itemRegion.tSize.iHeight+pResTile->tRegion.tLocation.iY)>(LD_CFG_SCEEN_HEIGHT/2))
                        {
                            ldBaseMove((ldCommon_t*)kb,0,LD_CFG_SCEEN_HEIGHT/2);
                            ldBaseBgMove(LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT,0,-(LD_CFG_SCEEN_HEIGHT/2));
                        }
                        else
                        {
                            ldBaseMove((ldCommon_t*)kb,0,0);
                        }
                    }
                }
            }
            pWidget->timer=tempTimer;
        }
        pWidget->dirtyRegionState=waitChange;
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        if(_isStopMove)
        {
            break;
        }
        offsetX=(int16_t)GET_SIGNAL_OFFSET_X(info.value);
        offsetY=(int16_t)GET_SIGNAL_OFFSET_Y(info.value);


        offsetX=abs(offsetX);
        offsetY=abs(offsetY);

        if(offsetX>=offsetY)
        {
            pWidget->scrollOffsetX=_scrollOffsetX+(int16_t)GET_SIGNAL_OFFSET_X(info.value);
            if(pWidget->scrollOffsetX>0)
            {
                pWidget->scrollOffsetX=0;
            }
            else
            {
                if((_totalItemWidth+pWidget->scrollOffsetX)<((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iWidth)
                {
                    pWidget->scrollOffsetX=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iWidth-_totalItemWidth;
                }

                if(_totalItemWidth<((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iWidth)
                {
                    pWidget->scrollOffsetX=0;
                }
            }
        }
        else
        {
            pWidget->scrollOffsetY=_scrollOffsetY+(int16_t)GET_SIGNAL_OFFSET_Y(info.value);
            if(pWidget->scrollOffsetY>0)
            {
                pWidget->scrollOffsetY=0;
            }
            else
            {
                if((_totalItemHeight+pWidget->scrollOffsetY)<((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight)
                {
                    pWidget->scrollOffsetY=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight-_totalItemHeight;
                }

                if(_totalItemHeight<((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight)
                {
                    pWidget->scrollOffsetY=0;
                }
            }
        }
        pWidget->dirtyRegionState=waitChange;
        break;
    }
    case SIGNAL_RELEASE:
    {
        currentItem=ldTableCurrentItem(pWidget);
        _isStopMove=false;
        if((!currentItem->isCheckable)&&(currentItem->isButton))
        {
            currentItem->isChecked=false;
            pWidget->dirtyRegionState=waitChange;
        }

        break;
    }
    default:
        break;
    }

    return false;
}

static bool slotEditEnd(xConnectInfo_t info)
{
    ldTable_t *pWidget=ldBaseGetWidgetById(info.receiverId);
    ldTableItem_t *currentItem=ldTableCurrentItem(pWidget);
    currentItem->isEditing=false;
    return false;
}

/**
 * @brief   表格初始化
 * 
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @param   rowCount        行数
 * @param   columnCount     列数
 * @param   itemSpace       间隔
 * @return  ldTable_t*      新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-16
 */
ldTable_t *ldTableInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t rowCount, uint8_t columnCount, uint8_t itemSpace, ldFontDict_t* pFontDict)
{
    ldTable_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    int16_t *widthBuf;
    int16_t *heightBuf;
    ldTableItem_t *pItemInfoBuf;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
    pNewWidget = LD_CALLOC_WIDGET_INFO(ldTable_t);
    widthBuf = ldCalloc(sizeof (int16_t)*columnCount);
    heightBuf = ldCalloc(sizeof (int16_t)*rowCount);
    pItemInfoBuf = ldCalloc(sizeof (ldTableItem_t)*columnCount*rowCount);

    if ((pNewWidget != NULL)&&(widthBuf!=NULL)&&(heightBuf!=NULL)&&(pItemInfoBuf!=NULL))
    {
        pNewWidget->isParentHidden=false;
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeTable;
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
        pNewWidget->rowCount= rowCount;
        pNewWidget->columnCount=columnCount;
        pNewWidget->itemSpace=itemSpace;
        pNewWidget->pColumnWidth=widthBuf;
        pNewWidget->pRowHeight=heightBuf;
        pNewWidget->pItemInfo=pItemInfoBuf;
        memset(pItemInfoBuf,0,sizeof (ldTableItem_t)*columnCount*rowCount);
        pNewWidget->isBgTransparent=false;
        pNewWidget->bgColor=LD_COLOR_WHITE_SMOKE;
        pNewWidget->selectColor=__RGB(65,143,31);
        int16_t w=(width-itemSpace)/columnCount-itemSpace;
        for(uint8_t i=0;i<columnCount;i++)
        {
            pNewWidget->pColumnWidth[i]=w;
        }
        int16_t h=(height-itemSpace)/rowCount-itemSpace;
        for(uint8_t i=0;i<rowCount;i++)
        {
            pNewWidget->pRowHeight[i]=h;
        }
        for(uint16_t i=0;i<(columnCount*rowCount);i++)
        {
            pNewWidget->pItemInfo[i].textColor=0;
            pNewWidget->pItemInfo[i].itemBgColor=__RGB( 255, 255, 255);
            pNewWidget->pItemInfo[i].isStaticText=false;
            pNewWidget->pItemInfo[i].pText=NULL;
            pNewWidget->pItemInfo[i].align=LD_ALIGN_CENTER;
            pNewWidget->pItemInfo[i].releaseImgAddr=LD_ADDR_NONE;
            pNewWidget->pItemInfo[i].isReleaseImgMask=false;
            pNewWidget->pItemInfo[i].isReleaseImgWithMask=false;
            pNewWidget->pItemInfo[i].pressImgAddr=LD_ADDR_NONE;
            pNewWidget->pItemInfo[i].isButton=false;
            pNewWidget->pItemInfo[i].isCheckable=false;
            pNewWidget->pItemInfo[i].isChecked=false;
            pNewWidget->pItemInfo[i].isSelectShow=true;
            pNewWidget->pItemInfo[i].isSelect=false;
            pNewWidget->pItemInfo[i].editType=typeString;
            pNewWidget->pItemInfo[i].textMax=0;
            pNewWidget->pItemInfo[i].pFontDict=pFontDict;
            pNewWidget->pItemInfo[i].isEditable=true;
        }
        pNewWidget->dirtyRegionListItem.tRegion = ldBaseGetGlobalRegion((ldCommon_t *)pNewWidget,&((arm_2d_tile_t*)&pNewWidget->resource)->tRegion);
        pNewWidget->pFunc=&ldTableCommonFunc;
        pNewWidget->timer=arm_2d_helper_convert_ticks_to_ms(arm_2d_helper_get_system_timestamp());
        pNewWidget->kbNameId=0;

        arm_2d_user_dynamic_dirty_region_init(&pNewWidget->dirtyRegionListItem,NULL);

        xConnect(pNewWidget->nameId,SIGNAL_PRESS,pNewWidget->nameId,slotTableProcess);
        xConnect(pNewWidget->nameId,SIGNAL_RELEASE,pNewWidget->nameId,slotTableProcess);
        xConnect(pNewWidget->nameId,SIGNAL_HOLD_DOWN,pNewWidget->nameId,slotTableProcess);
        xConnect(0,SIGNAL_EDITING_FINISHED,nameId,slotEditEnd);

        LOG_INFO("[table] init,id:%d\n",nameId);

    }
    else
    {
        ldFree(pItemInfoBuf);
        ldFree(widthBuf);
        ldFree(heightBuf);
        ldFree(pNewWidget);

        LOG_ERROR("[table] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldTableFrameUpdate(ldTable_t* pWidget)
{
    arm_2d_user_dynamic_dirty_region_on_frame_start(&pWidget->dirtyRegionListItem,waitChange);
}

void ldTableLoop(ldTable_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_region_t tBoxRegion;
    arm_2d_tile_t tItemTile,tImgTile;

    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

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

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if(ldBaseDirtyRegionUpdate(&tTarget,&tTarget_canvas,&pWidget->dirtyRegionListItem,pWidget->dirtyRegionState))
        {
            pWidget->dirtyRegionState=none;
        }

        if(!pWidget->isBgTransparent)
        {
            ldBaseColor(&tTarget,pWidget->bgColor,255);
        }

        tBoxRegion.tLocation.iY=pWidget->scrollOffsetY;
        for(uint8_t y=0;y<pWidget->rowCount;y++)
        {
            if(y>0)
            {
                tBoxRegion.tLocation.iY+=pWidget->pRowHeight[y-1];
            }
            tBoxRegion.tLocation.iY+=pWidget->itemSpace;

            tBoxRegion.tLocation.iX=pWidget->scrollOffsetX;
            for(uint8_t x=0;x<pWidget->columnCount;x++)
            {
                if(x>0)
                {
                    tBoxRegion.tLocation.iX+=pWidget->pColumnWidth[x-1];
                }
                tBoxRegion.tLocation.iX+=pWidget->itemSpace;
                tBoxRegion.tSize.iWidth=pWidget->pColumnWidth[x];
                tBoxRegion.tSize.iHeight=pWidget->pRowHeight[y];

                arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tItemTile, false);
                ldTableItem_t *item= &pWidget->pItemInfo[y*pWidget->columnCount+x];
                ldBaseColor(&tItemTile,item->itemBgColor,255);

                if(((item->pText!=NULL)||(item->isEditing))&&(!item->isStaticText))
                {
                    //预留光标位置
                    tItemTile.tRegion.tSize.iWidth-=CURSOR_WIDTH;


                    arm_2d_region_t showRegion=ldBaseLineText(&tItemTile,pResTile,item->pText,item->pFontDict,item->align,item->textColor,0,255);

                    if((cursorBlinkCount>CURSOR_BLINK_TIMEOUT)&&bIsNewFrame&&item->isEditing)
                    {
                        cursorBlinkCount=0;
                        cursorBlinkFlag=!cursorBlinkFlag;
                    }

                    if(item->isEditing)
                    {
                        pWidget->dirtyRegionState=waitChange;
                        if(cursorBlinkFlag)
                        {
                            arm_2d_region_t itemRegion= _ldTableGetItemRegion(pWidget,pWidget->currentRow,pWidget->currentColumn);
                            itemRegion.tLocation.iX+=showRegion.tLocation.iX+showRegion.tSize.iWidth+2;
                            itemRegion.tLocation.iY+=showRegion.tLocation.iY;
                            itemRegion.tSize.iWidth=CURSOR_WIDTH;
                            itemRegion.tSize.iHeight=item->pFontDict->lineStrHeight;
                            arm_2d_draw_box(&tTarget,&itemRegion,1,0,255);
                        }
                    }

                    tItemTile.tRegion.tSize.iWidth+=CURSOR_WIDTH;

                }
                else
                {
                    if((item->pText!=NULL)&&(item->isStaticText))
                    {
                        ldBaseLineText(&tItemTile,pResTile,item->pText,item->pFontDict,item->align,item->textColor,0,255);
                    }
                }

                if((item->releaseImgAddr!=LD_ADDR_NONE)||(item->pressImgAddr!=LD_ADDR_NONE))
                {
                    ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize=item->imgRegion.tSize;

                    arm_2d_tile_generate_child(&tItemTile, &item->imgRegion, &tImgTile, false);

                    if(item->releaseImgAddr!=LD_ADDR_NONE)
                    {
                        if((!item->isButton)||((item->isButton)&&(item->isChecked==false)))
                        {
                            ((arm_2d_tile_t*)&tempRes)->pchBuffer=(uint8_t *)item->releaseImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                            tempRes.pTarget=item->releaseImgAddr;
#endif
                            if(item->isReleaseImgMask)
                            {
                                ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_MASK_A8;
                                ldBaseMaskImage(&tImgTile,(arm_2d_tile_t*)&tempRes,item->releaseImgMaskColor,255);
                            }
                            else
                            {
                                ldBaseImage(&tImgTile,(arm_2d_tile_t*)&tempRes,item->isReleaseImgWithMask,255);
                            }

                        }
                    }

                    if(item->pressImgAddr!=LD_ADDR_NONE)
                    {
                        if((item->isButton)&&(item->isChecked==true))
                        {
                            ((arm_2d_tile_t*)&tempRes)->pchBuffer=(uint8_t *)item->pressImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                            tempRes.pTarget=item->pressImgAddr;
#endif
                            ldBaseImage(&tImgTile,(arm_2d_tile_t*)&tempRes,false,255);
                        }
                    }
                }

                if((item->isSelect)&&(item->isSelectShow))
                {
                    arm_2d_draw_box(&tTarget,&tItemTile.tRegion,2,pWidget->selectColor,255);
                }


#if LD_DEBUG == 1
                arm_2d_draw_box(&tTarget,&tItemTile.tRegion,1,0,255);
#endif
                arm_2d_op_wait_async(NULL);
            }
        }
#if LD_DEBUG == 1
        arm_2d_draw_box(&tTarget,&tTarget_canvas,1,0,255);
#endif
        arm_2d_op_wait_async(NULL);
    }
}

/**
 * @brief   设置指定列的宽度
 * 
 * @param   pWidget         目标控件指针
 * @param   column          列
 * @param   width           宽度
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-16
 */
void ldTableSetItemWidth(ldTable_t *pWidget,uint8_t column,int16_t width)
{
    if (pWidget == NULL)
    {
        return;
    }
    if(column<pWidget->columnCount)
    {
        pWidget->pColumnWidth[column]=width;
    }
}

/**
 * @brief   设置指定行的高度
 * 
 * @param   pWidget         目标控件指针
 * @param   row             行
 * @param   height          高度
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-16
 */
void ldTableSetItemHeight(ldTable_t *pWidget,uint8_t row,int16_t height)
{
    if (pWidget == NULL)
    {
        return;
    }
    if(row<pWidget->rowCount)
    {
        pWidget->pRowHeight[row]=height;
    }
}

/**
 * @brief   设置项目的文本
 * 
 * @param   pWidget         目标控件指针
 * @param   row             行
 * @param   column          列
 * @param   pText           文本内容
 * @param   pFontDict       字体
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-16
 */
void ldTableSetItemText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict)
{
    if (pWidget == NULL)
    {
        return;
    }
    if((row<pWidget->rowCount)&&(column<pWidget->columnCount))
    {
        ldTableItem_t *item= &pWidget->pItemInfo[row*pWidget->columnCount+column];
        if(item->isStaticText==false)
        {
            ldFree(item->pText);
            item->pText=LD_CALLOC_STRING(pText);
            strcpy((char*)item->pText,(char*)pText);
            item->pFontDict=pFontDict;
        }
    }
}

/**
 * @brief   设置项目的静态文本，不可变更的文本内容，不消耗内存
 * 
 * @param   pWidget         目标控件指针
 * @param   row             行
 * @param   column          列
 * @param   pText           文本内容
 * @param   pFontDict       字体
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-16
 */
void ldTableSetItemStaticText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict)
{
    if (pWidget == NULL)
    {
        return;
    }

    if((row<pWidget->rowCount)&&(column<pWidget->columnCount))
    {
        ldTableItem_t *item= &pWidget->pItemInfo[row*pWidget->columnCount+column];
        if(item->isStaticText==false)
        {
            ldFree(item->pText);
        }
        item->isStaticText=true;
        item->pText=pText;
        item->pFontDict=pFontDict;
        item->isEditable=false;
    }
}

/**
 * @brief   设置项目颜色
 * 
 * @param   pWidget         目标控件指针
 * @param   row             行
 * @param   column          列
 * @param   textColor       文字颜色
 * @param   bgColor         文字背景颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-16
 */
void ldTableSetItemColor(ldTable_t *pWidget,uint8_t row,uint8_t column,ldColor textColor,ldColor bgColor)
{
    if (pWidget == NULL)
    {
        return;
    }
    if((row<pWidget->rowCount)&&(column<pWidget->columnCount))
    {
        ldTableItem_t *item= &pWidget->pItemInfo[row*pWidget->columnCount+column];
        item->textColor=textColor;
        item->itemBgColor=bgColor;
    }
}

/**
 * @brief   设置表格底色
 * 
 * @param   pWidget         目标控件指针
 * @param   bgColor         底色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-16
 */
void ldTableSetBgColor(ldTable_t *pWidget,ldColor bgColor)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->bgColor=bgColor;
}

/**
 * @brief   设置指定项目文本对齐方式
 * 
 * @param   pWidget         目标控件指针
 * @param   row             行号
 * @param   column          列号
 * @param   align           对齐方式
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-21
 */
void ldTableSetItemAlign(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t align)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    if((row<pWidget->rowCount)&&(column<pWidget->columnCount))
    {
        ldTableItem_t *item= &pWidget->pItemInfo[row*pWidget->columnCount+column];
        item->align=align;
    }
}

/**
 * @brief   设置项目图片
 * 
 * @param   pWidget         目标控件指针
 * @param   row             行号
 * @param   column          列号
 * @param   x               图片在项目中的x轴坐标
 * @param   y               图片在项目中的y轴坐标
 * @param   width           图片宽度
 * @param   height          图片高度
 * @param   imgAddr         图片地址
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-21
 */
void ldTableSetItemImage(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uintptr_t imgAddr,bool isWithMask,ldColor maskColor,bool isMask)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    if((row<pWidget->rowCount)&&(column<pWidget->columnCount))
    {
        ldTableItem_t *item= &pWidget->pItemInfo[row*pWidget->columnCount+column];
        item->imgRegion.tLocation.iX=x;
        item->imgRegion.tLocation.iY=y;
        item->imgRegion.tSize.iWidth=width;
        item->imgRegion.tSize.iHeight=height;
        item->releaseImgAddr=imgAddr;
        item->isReleaseImgWithMask=isWithMask;
        item->isReleaseImgMask=isMask;
        item->releaseImgMaskColor=maskColor;
        item->pressImgAddr=LD_ADDR_NONE;
        item->isButton=false;
        item->isSelectShow=false;
        item->isEditable=false;
    }
}

/**
 * @brief   设置项目按键图片
 * 
 * @param   pWidget         目标控件指针
 * @param   row             行号
 * @param   column          列号
 * @param   x               图片在项目中的x轴坐标
 * @param   y               图片在项目中的y轴坐标
 * @param   width           图片宽度
 * @param   height          图片高度
 * @param   releaseImgAddr  释放的图片地址
 * @param   pressImgAddr    按下的图片地址
 * @param   isCheckable     是否为开关型按键
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-21
 */
void ldTableSetItemButton(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uintptr_t releaseImgAddr,uintptr_t pressImgAddr,bool isCheckable)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    if((row<pWidget->rowCount)&&(column<pWidget->columnCount))
    {
        ldTableItem_t *item= &pWidget->pItemInfo[row*pWidget->columnCount+column];
        item->imgRegion.tLocation.iX=x;
        item->imgRegion.tLocation.iY=y;
        item->imgRegion.tSize.iWidth=width;
        item->imgRegion.tSize.iHeight=height;
        item->releaseImgAddr=releaseImgAddr;
        item->pressImgAddr=pressImgAddr;
        item->isButton=true;
        item->isCheckable=isCheckable;
        item->isSelectShow=false;
        item->isEditable=false;
    }
}

/**
 * @brief   excel风格
 *
 * @param   pWidget         目标控件指针
 * @param   pFontDict       字体指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2024-01-17
 */
void ldTableSetExcelType(ldTable_t *pWidget,ldFontDict_t* pFontDict)
{
    uint8_t i;
    uint8_t strBuf[2]={0};
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;

    ldTableSetBgColor(pWidget,__RGB(219,219,219));
    strBuf[0]='1';
    for(i=0;i<pWidget->rowCount;i++)
    {
        if(i==0)
        {
            ldTableSetItemHeight(pWidget,0,22);
        }
        else
        {
            ldTableSetItemHeight(pWidget,i,18);
            if(pFontDict!=NULL)
            {
                ldTableSetItemText(pWidget,i,0,strBuf,pFontDict);
                strBuf[0]++;
            }
        }
        ldTableSetItemColor(pWidget,i,0,LD_COLOR_BLACK,__RGB(238,238,238));
    }
    strBuf[0]='A';
    for(i=1;i<pWidget->columnCount;i++)
    {
        if(i==0)
        {
            ldTableSetItemWidth(pWidget,0,35);
        }
        else
        {
            ldTableSetItemWidth(pWidget,i,71);
            if(pFontDict!=NULL)
            {
                ldTableSetItemText(pWidget,0,i,strBuf,pFontDict);
                strBuf[0]++;
            }
        }
        ldTableSetItemColor(pWidget,0,i,LD_COLOR_BLACK,__RGB(238,238,238));
    }
}

/**
 * @brief   关联键盘
 *
 * @param   pWidget         目标控件指针
 * @param   kbNameId        目标键盘id
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2024-01-17
 */
void ldTableSetKeyboard(ldTable_t* pWidget,uint16_t kbNameId)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->kbNameId=kbNameId;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
