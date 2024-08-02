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

#define __LD_TABLE_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldGui.h"
#include "ldTable.h"

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

const ldBaseWidgetFunc_t ldTableFunc = {
    .depose = (ldDeposeFunc_t)ldTable_depose,
    .load = (ldLoadFunc_t)ldTable_on_load,
    .frameStart = (ldFrameStartFunc_t)ldTable_on_frame_start,
    .show = (ldShowFunc_t)ldTable_show,
};


void _ldTableSelectItem(ldTable_t *ptWidget,ldTableItem_t *item)
{
    for(uint8_t y=0;y<ptWidget->rowCount;y++)
    {
        for(uint8_t x=0;x<ptWidget->columnCount;x++)
        {
            if(&ptWidget->ptItemInfo[y*ptWidget->columnCount+x]==item)
            {
                item->isSelect=true;
                ptWidget->currentRow=y;
                ptWidget->currentColumn=x;
            }
            else
            {
                ptWidget->ptItemInfo[y*ptWidget->columnCount+x].isSelect=false;
            }
        }
    }
}

uint8_t ldTableCurrentColumn(ldTable_t *ptWidget)
{
    return ptWidget->currentColumn;
}

uint8_t ldTableCurrentRow(ldTable_t *ptWidget)
{
    return ptWidget->currentRow;
}

ldTableItem_t *ldTableItem(ldTable_t *ptWidget,uint8_t row, uint8_t column)
{
    return &ptWidget->ptItemInfo[row*ptWidget->columnCount+column];
}

ldTableItem_t *ldTableCurrentItem(ldTable_t *ptWidget)
{
    return ldTableItem(ptWidget,ptWidget->currentRow,ptWidget->currentColumn);
}

arm_2d_location_t _ldTableGetItemPos(ldTable_t *ptWidget,uint8_t row, uint8_t column)
{
    arm_2d_location_t offset={0,0};

    for(uint8_t x=0;x<ptWidget->columnCount;x++)
    {
        if(x<column)
        {
            offset.iX+=ptWidget->itemSpace;
            offset.iX+=ptWidget->pColumnWidth[x];
        }
    }
    offset.iX+=ptWidget->itemSpace+ptWidget->scrollOffsetX;

    for(uint8_t y=0;y<ptWidget->rowCount;y++)
    {
        if(y<row)
        {
            offset.iY+=ptWidget->itemSpace;
            offset.iY+=ptWidget->pRowHeight[y];
        }
    }
    offset.iY+=ptWidget->itemSpace+ptWidget->scrollOffsetY;
    return offset;
}

arm_2d_region_t _ldTableGetItemRegion(ldTable_t *ptWidget,uint8_t row, uint8_t column)
{
    arm_2d_location_t targetItemPos=_ldTableGetItemPos(ptWidget,row,column);
    arm_2d_region_t itemRegion;
    itemRegion.tLocation.iX=targetItemPos.iX;
    itemRegion.tLocation.iY=targetItemPos.iY;
    itemRegion.tSize.iWidth=ptWidget->pColumnWidth[column];
    itemRegion.tSize.iHeight=ptWidget->pRowHeight[row];
    return itemRegion;
}

arm_2d_region_t _ldTableGetItemCellGlobalRegion(ldTable_t *ptWidget,uint8_t row, uint8_t column)
{
    ldTableItem_t *targetItem=ldTableItem(ptWidget,row,column);

    arm_2d_location_t targetItemPos=_ldTableGetItemPos(ptWidget,row,column);

    int16_t targetItemWidth=ptWidget->pColumnWidth[column];

    int16_t targetItemHeight=ptWidget->pRowHeight[row];

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

    arm_2d_region_t outRegion,tempRegion={0};
    tempRegion.tLocation=targetItem->tLocation;
    if(targetItem->ptReleaseImgTile!=NULL)
    {
        tempRegion.tSize=targetItem->ptReleaseImgTile->tRegion.tSize;
    }
    else if(targetItem->ptReleaseMaskTile!=NULL)
    {
        tempRegion.tSize=targetItem->ptReleaseMaskTile->tRegion.tSize;
    }
    arm_2d_region_intersect(&itemRegion,&tempRegion,&outRegion);

    outRegion.tLocation.iX=targetItem->tLocation.iX+targetItemPos.iX+ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX;
    outRegion.tLocation.iY=targetItem->tLocation.iY+targetItemPos.iY+ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;

    return outRegion;
}

ldTableItem_t *ldTableItemAt(ldTable_t *ptWidget,int16_t x,int16_t y)
{
    uint8_t itemX,itemY;

//    x-=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX;
//    y-=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;

    itemX=0;
    itemY=0;
    int16_t tempOffset=ptWidget->scrollOffsetX-x;
    for(uint8_t i=0;i<ptWidget->columnCount;i++)
    {
        if(tempOffset<-ptWidget->pColumnWidth[i])
        {
            tempOffset+=ptWidget->pColumnWidth[i];
            tempOffset+=ptWidget->itemSpace;
            itemX++;
        }
        else
        {
            break;
        }
    }

    tempOffset=ptWidget->scrollOffsetY-y;
    for(uint8_t i=0;i<ptWidget->rowCount;i++)
    {
        if(tempOffset<-ptWidget->pRowHeight[i])
        {
            tempOffset+=ptWidget->pRowHeight[i];
            tempOffset+=ptWidget->itemSpace;
            itemY++;
        }
        else
        {
            break;
        }
    }

    if(itemX>=ptWidget->columnCount)
    {
        itemX=ptWidget->columnCount-1;
    }
    if(itemY>=ptWidget->rowCount)
    {
        itemY=ptWidget->rowCount-1;
    }

    return &ptWidget->ptItemInfo[itemY*ptWidget->columnCount+itemX];
}

static int16_t _scrollOffsetX,_scrollOffsetY;
static int16_t _totalItemWidth,_totalItemHeight;
static bool _isStopMove;
static bool slotTableProcess(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldTable_t *ptWidget;
    int16_t x,y;
    int16_t offsetX,offsetY;
    arm_2d_tile_t *pResTile;
    ldTableItem_t *currentItem;
    int64_t tempTimer;
//    ldCommonKB_t *kb;

    ptWidget=msg.ptSender;;


    switch (msg.signal)
    {
    case SIGNAL_PRESS:
    {
        arm_2d_location_t tLocation={0};
        tLocation=ldBaseGetAbsoluteLocation(ptWidget,tLocation);
//        ldPoint_t parentPos=ldBaseGetGlobalPos(ptWidget->parentWidget);
        x=(int16_t)GET_SIGNAL_VALUE_X(msg.value)-tLocation.iX;
        y=(int16_t)GET_SIGNAL_VALUE_Y(msg.value)-tLocation.iY;

        currentItem=ldTableItemAt(ptWidget,x,y);

        _ldTableSelectItem(ptWidget,currentItem);

        _scrollOffsetX=ptWidget->scrollOffsetX;
        _scrollOffsetY=ptWidget->scrollOffsetY;

        _totalItemWidth=0;
        for(uint8_t i=0;i<ptWidget->columnCount;i++)
        {
            _totalItemWidth+=ptWidget->itemSpace;
            _totalItemWidth+=ptWidget->pColumnWidth[i];
        }
        _totalItemWidth+=ptWidget->itemSpace;

        _totalItemHeight=0;
        for(uint8_t i=0;i<ptWidget->rowCount;i++)
        {
            _totalItemHeight+=ptWidget->itemSpace;
            _totalItemHeight+=ptWidget->pRowHeight[i];
        }

        if(currentItem->isButton)
        {
            arm_2d_region_t cellRegion= _ldTableGetItemCellGlobalRegion(ptWidget,ptWidget->currentRow,ptWidget->currentColumn);

            if((x>=cellRegion.tLocation.iX)&&(y>=cellRegion.tLocation.iY)&&
                    (x<(cellRegion.tLocation.iX+cellRegion.tSize.iWidth))&&
                    (y<(cellRegion.tLocation.iY+cellRegion.tSize.iHeight)))
            {
                _isStopMove=true;
                currentItem->isChecked=!currentItem->isChecked;
            }
        }

//        if(currentItem->isEditable)
//        {
//            tempTimer=arm_2d_helper_convert_ticks_to_ms(arm_2d_helper_get_system_timestamp());

//            if((tempTimer-ptWidget->timer)<500)
//            {
//                currentItem->isEditing=true;
//                if(ptWidget->kbNameId)
//                {
//                    kb=ldBaseGetWidgetById(ptWidget->kbNameId);
//                    if(kb!=NULL)
//                    {
//                        kb->editType=currentItem->editType;
//                        kb->ppStr=&currentItem->pText;
//                        kb->strMax=currentItem->textMax;
//                        kb->editorId=ptWidget->nameId;
//                        cursorBlinkFlag=true;
//                        cursorBlinkCount=0;
//                        ldBaseSetHidden((ldCommon_t*)kb,false);

//                        arm_2d_region_t itemRegion= _ldTableGetItemRegion(ptWidget,ptWidget->currentRow,ptWidget->currentColumn);

//                        if((itemRegion.tLocation.iY+itemRegion.tSize.iHeight+pResTile->tRegion.tLocation.iY)>(LD_CFG_SCEEN_HEIGHT/2))
//                        {
//                            ldBaseMove((ldCommon_t*)kb,0,LD_CFG_SCEEN_HEIGHT/2);
//                            ldBaseBgMove(LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT,0,-(LD_CFG_SCEEN_HEIGHT/2));
//                        }
//                        else
//                        {
//                            ldBaseMove((ldCommon_t*)kb,0,0);
//                        }
//                    }
//                }
//            }
//            ptWidget->timer=tempTimer;
//        }
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        if(_isStopMove)
        {
            break;
        }
        offsetX=(int16_t)GET_SIGNAL_OFFSET_X(msg.value);
        offsetY=(int16_t)GET_SIGNAL_OFFSET_Y(msg.value);


        offsetX=abs(offsetX);
        offsetY=abs(offsetY);

        if(offsetX>=offsetY)
        {
            ptWidget->scrollOffsetX=_scrollOffsetX+(int16_t)GET_SIGNAL_OFFSET_X(msg.value);
            if(ptWidget->scrollOffsetX>0)
            {
                ptWidget->scrollOffsetX=0;
            }
            else
            {
                if((_totalItemWidth+ptWidget->scrollOffsetX)<ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth)
                {
                    ptWidget->scrollOffsetX=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-_totalItemWidth;
                }

                if(_totalItemWidth<ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth)
                {
                    ptWidget->scrollOffsetX=0;
                }
            }
        }
        else
        {
            ptWidget->scrollOffsetY=_scrollOffsetY+(int16_t)GET_SIGNAL_OFFSET_Y(msg.value);
            if(ptWidget->scrollOffsetY>0)
            {
                ptWidget->scrollOffsetY=0;
            }
            else
            {
                if((_totalItemHeight+ptWidget->scrollOffsetY)<ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight)
                {
                    ptWidget->scrollOffsetY=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-_totalItemHeight;
                }

                if(_totalItemHeight<ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight)
                {
                    ptWidget->scrollOffsetY=0;
                }
            }
        }
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    case SIGNAL_RELEASE:
    {
        currentItem=ldTableCurrentItem(ptWidget);
        _isStopMove=false;
        if((!currentItem->isCheckable)&&(currentItem->isButton))
        {
            currentItem->isChecked=false;
            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        }

        break;
    }
    default:
        break;
    }

    return false;
}

static bool slotEditEnd(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldTableItem_t *currentItem=ldTableCurrentItem(msg.ptSender);
    currentItem->isEditing=false;
    return false;
}

ldTable_t* ldTable_init( ld_scene_t *ptScene,ldTable_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t rowCount, uint8_t columnCount, uint8_t itemSpace, arm_2d_font_t* ptFont)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    int16_t *widthBuf;
    int16_t *heightBuf;
    ldTableItem_t *ptItemInfoBuf;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldTable_t));
        widthBuf = ldCalloc(1,sizeof (int16_t)*columnCount);
        heightBuf = ldCalloc(1,sizeof (int16_t)*rowCount);
        ptItemInfoBuf = ldCalloc(1,sizeof (ldTableItem_t)*columnCount*rowCount);
        if (NULL == ptWidget)
        {
            ldFree(ptWidget);
            ldFree(widthBuf);
            ldFree(heightBuf);
            ldFree(ptItemInfoBuf);
            LOG_ERROR("[init failed][table] id:%d", nameId);
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
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeTable;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldTableFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->rowCount= rowCount;
    ptWidget->columnCount=columnCount;
    ptWidget->itemSpace=itemSpace;
    ptWidget->pColumnWidth=widthBuf;
    ptWidget->pRowHeight=heightBuf;
    ptWidget->ptItemInfo=ptItemInfoBuf;
    memset(ptItemInfoBuf,0,sizeof (ldTableItem_t)*columnCount*rowCount);
    ptWidget->isBgTransparent=false;
    ptWidget->bgColor=__RGB(245, 245, 245);
    ptWidget->selectColor=__RGB(65,143,31);
    int16_t w=(width-itemSpace)/columnCount-itemSpace;
    for(uint8_t i=0;i<columnCount;i++)
    {
        ptWidget->pColumnWidth[i]=w;
    }
    int16_t h=(height-itemSpace)/rowCount-itemSpace;
    for(uint8_t i=0;i<rowCount;i++)
    {
        ptWidget->pRowHeight[i]=h;
    }
    for(uint16_t i=0;i<(columnCount*rowCount);i++)
    {
        ptWidget->ptItemInfo[i].textColor=0;
        ptWidget->ptItemInfo[i].itemBgColor=__RGB( 255, 255, 255);
        ptWidget->ptItemInfo[i].isStaticText=false;
        ptWidget->ptItemInfo[i].pText=NULL;
        ptWidget->ptItemInfo[i].tAlign=ARM_2D_ALIGN_CENTRE;
        ptWidget->ptItemInfo[i].isButton=false;
        ptWidget->ptItemInfo[i].isCheckable=false;
        ptWidget->ptItemInfo[i].isChecked=false;
        ptWidget->ptItemInfo[i].isSelectShow=true;
        ptWidget->ptItemInfo[i].isSelect=false;
        ptWidget->ptItemInfo[i].editType=typeString;
        ptWidget->ptItemInfo[i].textMax=0;
        ptWidget->ptItemInfo[i].ptFont=ptFont;
        ptWidget->ptItemInfo[i].isEditable=true;
    }
    ptWidget->timer=arm_2d_helper_convert_ticks_to_ms(arm_2d_helper_get_system_timestamp());

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotTableProcess);
    ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotTableProcess);
    ldMsgConnect(ptWidget,SIGNAL_HOLD_DOWN,slotTableProcess);
    ldMsgConnect(ptWidget,SIGNAL_EDITING_FINISHED,slotEditEnd);

    LOG_INFO("[init][table] id:%d", nameId);
    return ptWidget;
}

void ldTable_depose( ldTable_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeTable)
    {
        return;
    }

    LOG_INFO("[depose][table] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove(ptWidget->ptScene->ptNodeRoot,(arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget->pColumnWidth);
    ldFree(ptWidget->pRowHeight);
    ldFree(ptWidget->ptItemInfo);
    ldFree(ptWidget);
}

void ldTable_on_load( ldTable_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldTable_on_frame_start( ldTable_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldTable_show(ld_scene_t *ptScene, ldTable_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    arm_2d_region_t tBoxRegion;
    arm_2d_tile_t tItemTile,tImgTile;

#if 0
    if (bIsNewFrame) {
        
    }
#endif
    arm_2d_region_t globalRegion=ldBaseGetAbsoluteRegion(ptWidget);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            if(!ptWidget->isBgTransparent)
            {
                ldBaseColor(&tTarget,NULL,ptWidget->bgColor,255);
            }

            tBoxRegion.tLocation.iY=ptWidget->scrollOffsetY;
            for(uint8_t y=0;y<ptWidget->rowCount;y++)
            {
                if(y>0)
                {
                    tBoxRegion.tLocation.iY+=ptWidget->pRowHeight[y-1];
                }
                tBoxRegion.tLocation.iY+=ptWidget->itemSpace;

                tBoxRegion.tLocation.iX=ptWidget->scrollOffsetX;
                for(uint8_t x=0;x<ptWidget->columnCount;x++)
                {
                    if(x>0)
                    {
                        tBoxRegion.tLocation.iX+=ptWidget->pColumnWidth[x-1];
                    }
                    tBoxRegion.tLocation.iX+=ptWidget->itemSpace;
                    tBoxRegion.tSize.iWidth=ptWidget->pColumnWidth[x];
                    tBoxRegion.tSize.iHeight=ptWidget->pRowHeight[y];

                    arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tItemTile, false);
                    ldTableItem_t *item= &ptWidget->ptItemInfo[y*ptWidget->columnCount+x];
                    ldBaseColor(&tItemTile,NULL,item->itemBgColor,ptWidget->use_as__ldBase_t.opacity);

                    if(((item->pText!=NULL)||(item->isEditing))&&(!item->isStaticText))
                    {
                        //预留光标位置
                        tItemTile.tRegion.tSize.iWidth-=CURSOR_WIDTH;

                        ldBaseLabel(&tItemTile,
                                    &((arm_2d_region_t){0,
                                                        0,
                                                        tItemTile.tRegion.tSize.iWidth,
                                                        tItemTile.tRegion.tSize.iHeight}),
                                    item->pText,
                                    item->ptFont,
                                    item->tAlign,
                                    item->textColor,
                                    ptWidget->use_as__ldBase_t.opacity);

                        if((cursorBlinkCount>CURSOR_BLINK_TIMEOUT)&&bIsNewFrame&&item->isEditing)
                        {
                            cursorBlinkCount=0;
                            cursorBlinkFlag=!cursorBlinkFlag;
                        }

//                        if(item->isEditing)
//                        {
//                            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
//                            if(cursorBlinkFlag)
//                            {
//                                arm_2d_region_t itemRegion= _ldTableGetItemRegion(ptWidget,ptWidget->currentRow,ptWidget->currentColumn);
//                                itemRegion.tLocation.iX+=showRegion.tLocation.iX+showRegion.tSize.iWidth+2;
//                                itemRegion.tLocation.iY+=showRegion.tLocation.iY;
//                                itemRegion.tSize.iWidth=CURSOR_WIDTH;
//                                itemRegion.tSize.iHeight=item->pFontDict->lineStrHeight;
//                                arm_2d_draw_box(&tTarget,&itemRegion,1,0,255);
//                            }
//                        }

                        tItemTile.tRegion.tSize.iWidth+=CURSOR_WIDTH;

                    }
                    else
                    {
                        if((item->pText!=NULL)&&(item->isStaticText))
                        {
                            ldBaseLabel(&tItemTile,
                                        &((arm_2d_region_t){0,
                                                            0,
                                                            tItemTile.tRegion.tSize.iWidth,
                                                            tItemTile.tRegion.tSize.iHeight}),
                                        item->pText,
                                        item->ptFont,
                                        item->tAlign,
                                        item->textColor,
                                        ptWidget->use_as__ldBase_t.opacity);
                        }
                    }

                    if((item->ptReleaseImgTile!=NULL)||(item->ptReleaseMaskTile!=NULL))
                    {
                        if((!item->isButton)||((item->isButton)&&(item->isChecked==false)))
                        {
                            arm_2d_region_t tempRegion={
                                .tLocation=item->tLocation,
                                .tSize=item->ptReleaseImgTile->tRegion.tSize,
                            };
                            ldBaseImage(&tImgTile,&tempRegion,item->ptReleaseImgTile,item->ptReleaseMaskTile,item->releaseImgMaskColor,ptWidget->use_as__ldBase_t.opacity);
                        }
                    }

                    if((item->ptPressImgTile!=NULL)||(item->ptPressMaskTile!=NULL))
                    {
                        if((item->isButton)&&(item->isChecked==true))
                        {
                            arm_2d_region_t tempRegion={
                                .tLocation=item->tLocation,
                                .tSize=item->ptPressImgTile->tRegion.tSize,
                            };
                            ldBaseImage(&tImgTile,&tempRegion,item->ptPressImgTile,item->ptPressMaskTile,item->pressImgMaskColor,ptWidget->use_as__ldBase_t.opacity);
                        }
                    }

                    if((item->isSelect)&&(item->isSelectShow))
                    {
                        arm_2d_draw_box(&tTarget,&tItemTile.tRegion,2,ptWidget->selectColor,255);
                    }

#if LD_DEBUG == 1
                    arm_2d_draw_box(&tTarget,&tItemTile.tRegion,1,0,255);
#endif
                    arm_2d_op_wait_async(NULL);
                }
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldTableSetItemWidth(ldTable_t *ptWidget,uint8_t column,int16_t width)
{
    if (ptWidget == NULL)
    {
        return;
    }
    if(column<ptWidget->columnCount)
    {
        ptWidget->pColumnWidth[column]=width;
    }
}

void ldTableSetItemHeight(ldTable_t *ptWidget,uint8_t row,int16_t height)
{
    if (ptWidget == NULL)
    {
        return;
    }
    if(row<ptWidget->rowCount)
    {
        ptWidget->pRowHeight[row]=height;
    }
}

void ldTableSetItemText(ldTable_t *ptWidget,uint8_t row,uint8_t column,uint8_t *pText,arm_2d_font_t* ptFont)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    if((row<ptWidget->rowCount)&&(column<ptWidget->columnCount))
    {
        ldTableItem_t *item= &ptWidget->ptItemInfo[row*ptWidget->columnCount+column];
        if(item->isStaticText==false)
        {
            ldFree(item->pText);
            item->pText=ldCalloc(1,strlen((char*)pText)+1);
            if(item->pText!=NULL)
            {
                strcpy((char*)item->pText,(char*)pText);
            }
            item->ptFont=ptFont;
        }
    }
}

void ldTableSetItemStaticText(ldTable_t *ptWidget,uint8_t row,uint8_t column,uint8_t *pText,arm_2d_font_t* ptFont)
{
    if (ptWidget == NULL)
    {
        return;
    }

    if((row<ptWidget->rowCount)&&(column<ptWidget->columnCount))
    {
        ldTableItem_t *item= &ptWidget->ptItemInfo[row*ptWidget->columnCount+column];
        if(item->isStaticText==false)
        {
            ldFree(item->pText);
        }
        item->isStaticText=true;
        item->pText=pText;
        item->ptFont=ptFont;
        item->isEditable=false;
    }
}

void ldTableSetItemColor(ldTable_t *ptWidget,uint8_t row,uint8_t column,ldColor textColor,ldColor bgColor)
{
    if (ptWidget == NULL)
    {
        return;
    }
    if((row<ptWidget->rowCount)&&(column<ptWidget->columnCount))
    {
        ldTableItem_t *item= &ptWidget->ptItemInfo[row*ptWidget->columnCount+column];
        item->textColor=textColor;
        item->itemBgColor=bgColor;
    }
}

void ldTableSetBgColor(ldTable_t *ptWidget,ldColor bgColor)
{
    if (ptWidget == NULL)
    {
        return;
    }
    ptWidget->bgColor=bgColor;
}

void ldTableSetItemAlign(ldTable_t *ptWidget,uint8_t row,uint8_t column,arm_2d_align_t tAlign)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    if((row<ptWidget->rowCount)&&(column<ptWidget->columnCount))
    {
        ldTableItem_t *item= &ptWidget->ptItemInfo[row*ptWidget->columnCount+column];
        item->tAlign=tAlign;
    }
}

void ldTableSetItemImage(ldTable_t *ptWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,arm_2d_tile_t* ptImgTile,arm_2d_tile_t *ptMaskTile,ldColor maskColor)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    if((row<ptWidget->rowCount)&&(column<ptWidget->columnCount))
    {
        ldTableItem_t *item= &ptWidget->ptItemInfo[row*ptWidget->columnCount+column];
        item->tLocation.iX=x;
        item->tLocation.iY=y;
        item->ptPressImgTile=ptImgTile;
        item->ptPressMaskTile=ptMaskTile;
        item->releaseImgMaskColor=maskColor;
        item->ptReleaseImgTile=NULL;
        item->ptReleaseMaskTile=NULL;
        item->isButton=false;
        item->isSelectShow=false;
        item->isEditable=false;
    }
}

void ldTableSetItemButton(ldTable_t *ptWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,arm_2d_tile_t *ptReleaseImgTile,arm_2d_tile_t *ptReleaseMaskTile,ldColor releaseImgMaskColor,arm_2d_tile_t *ptPressImgTile,arm_2d_tile_t *ptPressMaskTile,ldColor pressImgMaskColor,bool isCheckable)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    if((row<ptWidget->rowCount)&&(column<ptWidget->columnCount))
    {
        ldTableItem_t *item= &ptWidget->ptItemInfo[row*ptWidget->columnCount+column];
        item->tLocation.iX=x;
        item->tLocation.iY=y;
        item->ptReleaseImgTile=ptReleaseImgTile;
        item->ptReleaseMaskTile=ptReleaseMaskTile;
        item->releaseImgMaskColor=releaseImgMaskColor;
        item->ptPressImgTile=ptPressImgTile;
        item->ptPressMaskTile=ptPressMaskTile;
        item->pressImgMaskColor=pressImgMaskColor;
        item->isButton=true;
        item->isCheckable=isCheckable;
        item->isSelectShow=false;
        item->isEditable=false;
    }
}

void ldTableSetKeyboard(ldTable_t* ptWidget,uint16_t kbNameId)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->kbNameId=kbNameId;
}

void ldTableSetEditable(ldTable_t* ptWidget,uint8_t row,uint8_t column,bool isEditable)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->ptItemInfo[row*ptWidget->columnCount+column].isEditable=isEditable;
}

void ldTableSetExcelType(ldTable_t *ptWidget,arm_2d_font_t* ptFont)
{
    uint8_t i;
    uint8_t strBuf[2]={0};
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    ldTableSetEditable(ptWidget,0,0,false);

    ldTableSetBgColor(ptWidget,__RGB(219,219,219));
    strBuf[0]='1';
    for(i=0;i<ptWidget->rowCount;i++)
    {
        if(i==0)
        {
            ldTableSetItemHeight(ptWidget,0,22);
        }
        else
        {
            ldTableSetItemHeight(ptWidget,i,18);
            if(ptFont!=NULL)
            {
                ldTableSetItemText(ptWidget,i,0,strBuf,ptFont);
                ldTableSetEditable(ptWidget,i,0,false);
                strBuf[0]++;
            }
        }
        ldTableSetItemColor(ptWidget,i,0,GLCD_COLOR_BLACK,__RGB(238,238,238));
    }
    strBuf[0]='A';
    for(i=1;i<ptWidget->columnCount;i++)
    {
        if(i==0)
        {
            ldTableSetItemWidth(ptWidget,0,35);
        }
        else
        {
            ldTableSetItemWidth(ptWidget,i,71);
            if(ptFont!=NULL)
            {
                ldTableSetItemText(ptWidget,0,i,strBuf,ptFont);
                ldTableSetEditable(ptWidget,0,i,false);
                strBuf[0]++;
            }
        }
        ldTableSetItemColor(ptWidget,0,i,GLCD_COLOR_BLACK,__RGB(238,238,238));
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
