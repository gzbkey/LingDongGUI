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
 * @brief   table widget
 * @version 0.1
 * @date    2023-11-14
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

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _tableDel);
    }
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
ldTable_t *ldTableInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t rowCount, uint8_t columnCount, uint8_t itemSpace)
{
    ldTable_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    int16_t *widthBuf;
    int16_t *heightBuf;
    ldTableItem_t *pItemInfoBuf;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldTable_t);
    widthBuf = ldMalloc(sizeof (int16_t)*columnCount);
    heightBuf = ldMalloc(sizeof (int16_t)*rowCount);
    pItemInfoBuf = ldMalloc(sizeof (ldTableItem_t)*columnCount*rowCount);

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
        }

        pNewWidget->isBgTransparent=false;
        pNewWidget->bgColor=0;

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

/**
 * @brief   表格显示处理
 * 
 * @param   pWidget         目标控件指针
 * @param   pParentTile     父控件tile对象
 * @param   bIsNewFrame     新的一帧开始标志
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-16
 */
void ldTableLoop(ldTable_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_region_t tBoxRegion;
    arm_2d_tile_t tChildTile;

    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

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
        if(!pWidget->isBgTransparent)
        {
            ldBaseColor(&tTarget,pWidget->bgColor,255);
        }

        tBoxRegion.tLocation.iY=0;
        for(uint8_t y=0;y<pWidget->rowCount;y++)
        {
            if(y>0)
            {
                tBoxRegion.tLocation.iY+=pWidget->pRowHeight[y-1];
            }
            tBoxRegion.tLocation.iY+=pWidget->itemSpace;

            tBoxRegion.tLocation.iX=0;
            for(uint8_t x=0;x<pWidget->columnCount;x++)
            {
                if(x>0)
                {
                    tBoxRegion.tLocation.iX+=pWidget->pColumnWidth[x-1];
                }
                tBoxRegion.tLocation.iX+=pWidget->itemSpace;

                tBoxRegion.tSize.iWidth=pWidget->pColumnWidth[x];
                tBoxRegion.tSize.iHeight=pWidget->pRowHeight[y];
                arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tChildTile, false);

                ldTableItem_t *item= &pWidget->pItemInfo[y*pWidget->columnCount+x];
                ldBaseColor(&tChildTile,item->itemBgColor,255);
                if(item->pText!=NULL)
                {
                    LOG_DEBUG("%d,%d,%s\n",x,y,item->pText);
                    ldBaseLineText(&tChildTile,pResTile,item->pText,item->pFontDict,0,item->textColor,0,255);
                }

#if LD_DEBUG == 1
                arm_2d_draw_box(&tTarget,&tChildTile.tRegion,1,0,255);
#endif
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
        pWidget->pColumnWidth[row]=height;
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
            item->pText=LD_MALLOC_STRING(pText);
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

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
