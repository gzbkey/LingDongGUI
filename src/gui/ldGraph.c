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
 * @file    graph.c
 * @author  
 * @brief   
 * @version 0.1
 * @date    
 */

#include "ldGraph.h"
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

//dot.png w:5 h:5 < A8 >
const static uint8_t graphDefalutDot_png[]={
    0x00, 0x63, 0x7F, 0x63, 0x00,
    0x63, 0x7F, 0xBF, 0x7F, 0x63,
    0x7F, 0xBF, 0xFF, 0xBF, 0x7F,
    0x63, 0x7F, 0xBF, 0x7F, 0x63,
    0x00, 0x63, 0x7F, 0x63, 0x00
};

static bool _graphDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldGraph_t * pWidget=(ldGraph_t *)pTarget;
        for(uint8_t i=0;i<pWidget->seriesCount;i++)
        {
            ldFree(pWidget->pSeries[i].pValueList);
        }
        ldFree(pWidget->pSeries);

        ldFree(((ldGraph_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldGraphDel(ldGraph_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeGraph)
    {
        return;
    }

    LOG_INFO("[graph] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _graphDel);
    }
}

ldGraph_t *ldGraphInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height ,uint8_t seriesMax)
{
    ldGraph_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    ldGraphSeries_t *pSeries = NULL;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldGraph_t);
    pSeries = ldMalloc(sizeof (ldGraphSeries_t)*seriesMax);
    if ((pNewWidget != NULL)&&(pSeries != NULL))
    {
        pNewWidget->isParentHidden=false;
        
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeGraph;
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

        pNewWidget->pointImgAddr=graphDefalutDot_png;
        pNewWidget->pointImgWidth=5;
        pNewWidget->isCorner=true;
        pNewWidget->isFrame=true;
        pNewWidget->frameSpace=10;
        pNewWidget->seriesMax=seriesMax;
        pNewWidget->seriesCount=0;
        pNewWidget->pSeries=pSeries;

        ldGraphSetAxis(pNewWidget,width-pNewWidget->frameSpace*2,height-pNewWidget->frameSpace*2,5);
        ldGraphSetGridOffset(pNewWidget,5);

        LOG_INFO("[graph] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pSeries);
        ldFree(pNewWidget);

        LOG_ERROR("[graph] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldGraphLoop(ldGraph_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
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
        // draw frame
        if(pWidget->isFrame)
        {
            if(pWidget->isCorner)
            {
                draw_round_corner_box(&tTarget,&tTarget_canvas,LD_COLOR_WHITE,255,bIsNewFrame);
                draw_round_corner_border(&tTarget,&tTarget_canvas,LD_COLOR_LIGHT_GREY,(arm_2d_border_opacity_t){255,255,255,255},(arm_2d_corner_opacity_t){255,255,255,255});
            }
            else
            {
                ldBaseColor(&tTarget,LD_COLOR_WHITE,255);
                arm_2d_draw_box(&tTarget,&tTarget_canvas,1,LD_COLOR_LIGHT_GREY,255);
            }
        }
        else
        {
            ldBaseColor(&tTarget,LD_COLOR_WHITE,255);
        }
        arm_2d_op_wait_async(NULL);

        // draw grid
        uint16_t xCount,yCount;
        arm_2d_location_t gridLocal;

        xCount=pWidget->xAxisMax/(pWidget->xScale*pWidget->gridOffset);
        yCount=pWidget->yAxisMax/(pWidget->yScale*pWidget->gridOffset*5);

        for(uint16_t j=0;j<=yCount;j++)
        {
            for(uint16_t i=0;i<=xCount;i++)
            {
                gridLocal.iX=pWidget->frameSpace+(pWidget->xScale*pWidget->gridOffset)*i;
                gridLocal.iY=pWidget->frameSpace+(pWidget->yScale*pWidget->gridOffset*5)*j;
                arm_2d_draw_point(&tTarget,gridLocal,LD_COLOR_GRAY,128);
            }
        }

        xCount=pWidget->xAxisMax/(pWidget->xScale*pWidget->gridOffset*5);
        yCount=pWidget->yAxisMax/(pWidget->yScale*pWidget->gridOffset);

        for(uint16_t j=0;j<=yCount;j++)
        {
            for(uint16_t i=0;i<=xCount;i++)
            {
                gridLocal.iX=pWidget->frameSpace+(pWidget->xScale*pWidget->gridOffset*5)*i;
                gridLocal.iY=pWidget->frameSpace+(pWidget->yScale*pWidget->gridOffset)*j;
                arm_2d_draw_point(&tTarget,gridLocal,LD_COLOR_GRAY,128);
            }
        }
        arm_2d_op_wait_async(NULL);


        if(pWidget->seriesCount>0)
        {

#if USE_VIRTUAL_RESOURCE == 0
            arm_2d_tile_t tempRes;
#else
            arm_2d_vres_t tempRes;
#endif
            tempRes=pWidget->resource;
            arm_2d_tile_t *pTempRes=(arm_2d_tile_t*)&tempRes;
            pTempRes->tRegion.tLocation.iX=0;
            pTempRes->tRegion.tLocation.iY=0;
            pTempRes->tRegion.tSize.iWidth=pWidget->pointImgWidth;
            pTempRes->tRegion.tSize.iHeight=pWidget->pointImgWidth;
            pTempRes->pchBuffer = (uint8_t *)pWidget->pointImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
            ((arm_2d_vres_t*)pTempRes)->pTarget=pWidget->pointImgAddr;
#endif
            pTempRes->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_MASK_A8;
            
            if(pWidget->pointImgAddr==graphDefalutDot_png)
            {
                pTempRes->bVirtualResource=false;
            }
            
            int16_t x=0,y=0,xPrev,yPrev;
            
            xCount=pWidget->xAxisMax/(pWidget->xAxisOffset*pWidget->xScale);

            for(uint16_t i=0;i<xCount;i++)
            {
                xPrev=x;
                x=pWidget->frameSpace+(pWidget->xAxisOffset*pWidget->xScale)*i-(pWidget->pointImgWidth/2);

                for(uint8_t k=0;k<pWidget->seriesCount;k++)
                {
                    if(i<pWidget->pSeries[k].valueCountMax)
                    {
                        y=tTarget_canvas.tSize.iHeight-pWidget->frameSpace-(pWidget->pointImgWidth/2)-(pWidget->pSeries[k].pValueList[i]*pWidget->yScale);

                        if(pWidget->pointImgAddr!=LD_ADDR_NONE)
                        {
                            pTempRes->tRegion.tLocation.iX=x;
                            pTempRes->tRegion.tLocation.iY=y;
                            ldBaseMaskImage(&tTarget,pTempRes,pWidget->pSeries[k].seriesColor,255);
                        }

                        if((i>0)&&(pWidget->pSeries[k].lineSize>0))
                        {
                            yPrev=tTarget_canvas.tSize.iHeight-pWidget->frameSpace-(pWidget->pointImgWidth/2)-(pWidget->pSeries[k].pValueList[i-1]*pWidget->yScale);
                            ldBaseDrawLine(&tTarget,xPrev+(pWidget->pointImgWidth/2),yPrev+pWidget->pointImgWidth/2,x+(pWidget->pointImgWidth/2),y+pWidget->pointImgWidth/2,pWidget->pSeries[k].lineSize,pWidget->pSeries[k].seriesColor,255,64);
                        }
                    }
                }
            }
            arm_2d_op_wait_async(NULL);
        }
        
#if LD_DEBUG == 1
        arm_2d_draw_box(&tTarget,&tBoxRegion,1,0,255);
#endif
        arm_2d_op_wait_async(NULL);
    }
}

void ldGraphSetPointImageMask(ldGraph_t *pWidget,uint32_t addr,int16_t width)
{
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->pointImgAddr=addr;
    pWidget->pointImgWidth=width;
    if(pWidget->frameSpace<width)
    {
        pWidget->frameSpace=width;
    }
}

int8_t ldGraphAddSeries(ldGraph_t *pWidget,ldColor seriesColor,uint8_t lineSize,uint16_t valueCountMax)
{
    uint16_t *pBuf;
    if (pWidget == NULL)
    {
        return -1;
    }
    if(pWidget->seriesCount<pWidget->seriesMax)
    {
        pBuf=ldMalloc(sizeof (uint16_t)*valueCountMax);
        if(pBuf!=NULL)
        {
            pWidget->pSeries[pWidget->seriesCount].pValueList=pBuf;
            pWidget->pSeries[pWidget->seriesCount].valueCountMax=valueCountMax;
            pWidget->pSeries[pWidget->seriesCount].seriesColor=seriesColor;
            pWidget->pSeries[pWidget->seriesCount].lineSize=lineSize;
            pWidget->seriesCount++;
            return (pWidget->seriesCount-1);
        }
    }
    return -1;
}

void ldGraphSetValue(ldGraph_t *pWidget,uint8_t seriesNum,uint16_t valueNum,uint16_t value)
{
    if (pWidget == NULL)
    {
        return ;
    }

    if(valueNum<pWidget->pSeries[seriesNum].valueCountMax)
    {
        pWidget->pSeries[seriesNum].pValueList[valueNum]=value;
    }
}

void ldGraphSetAxis(ldGraph_t *pWidget,uint16_t xAxis,uint16_t yAxis,uint16_t xAxisOffset)
{
    if (pWidget == NULL)
    {
        return ;
    }
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    float scale;
    scale=(float)(pResTile->tRegion.tSize.iWidth-pWidget->frameSpace*2)/xAxis;
    pWidget->xAxisMax=xAxis*scale;
    pWidget->xScale=scale;
    pWidget->xAxisOffset=xAxisOffset*scale;

    scale=(float)(pResTile->tRegion.tSize.iHeight-pWidget->frameSpace*2)/yAxis;
    pWidget->yAxisMax=yAxis*scale;
    pWidget->yScale=scale;
}

void ldGraphSetAxisOffset(ldGraph_t *pWidget,uint16_t xAxisOffset)
{
    if (pWidget == NULL)
    {
        return ;
    }
    pWidget->xAxisOffset=xAxisOffset;
}

void ldGraphSetFrameSpace(ldGraph_t *pWidget,uint8_t frameSpace,bool isCorner)
{
    if (pWidget == NULL)
    {
        return ;
    }

    pWidget->isFrame=(frameSpace>0)?true:false;

    pWidget->isCorner=isCorner;
    pWidget->frameSpace=frameSpace;
}

void ldGraphSetGridOffset(ldGraph_t *pWidget,uint8_t gridOffset)
{
    if (pWidget == NULL)
    {
        return ;
    }

    pWidget->gridOffset=gridOffset;
}

void ldGraphMoveAdd(ldGraph_t *pWidget,uint8_t seriesNum,uint16_t newValue)
{
    if (pWidget == NULL)
    {
        return ;
    }

    uint16_t *p=pWidget->pSeries[seriesNum].pValueList;

    for(int i =0;i < pWidget->pSeries[seriesNum].valueCountMax-1;i++)
    {
        p[i] = p[i+1];
    }
    p[pWidget->pSeries[seriesNum].valueCountMax-1] = newValue;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
