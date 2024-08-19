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

#define __LD_GRAPH_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldGraph.h"

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

const arm_2d_tile_t c_tile_graphDefalutDot_Mask = {
    .tRegion = {
        .tSize = {
            .iWidth = 5,
            .iHeight = 5,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_8BIT,
        },
    },
    .pchBuffer = (uint8_t *)graphDefalutDot_png,
};

const ldBaseWidgetFunc_t ldGraphFunc = {
    .depose = (ldDeposeFunc_t)ldGraph_depose,
    .load = (ldLoadFunc_t)ldGraph_on_load,
    .frameStart = (ldFrameStartFunc_t)ldGraph_on_frame_start,
    .show = (ldShowFunc_t)ldGraph_show,
};

ldGraph_t* ldGraph_init( ld_scene_t *ptScene,ldGraph_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height ,uint8_t seriesMax)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    ldGraphSeries_t *pSeries = NULL;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldGraph_t));
        pSeries = ldCalloc(1,sizeof (ldGraphSeries_t)*seriesMax);
        if ((NULL == ptWidget)&&(NULL == pSeries))
        {
            ldFree(ptWidget);
            ldFree(pSeries);
            LOG_ERROR("[init failed][graph] id:%d", nameId);
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
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeGraph;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldGraphFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->ptPointMaskTile=&c_tile_graphDefalutDot_Mask;
    ptWidget->isCorner=true;
    ptWidget->isFrame=true;
    ptWidget->frameSpace=10;
    ptWidget->seriesMax=seriesMax;
    ptWidget->seriesCount=0;
    ptWidget->pSeries=pSeries;

    ldGraphSetAxis(ptWidget,width-ptWidget->frameSpace*2,height-ptWidget->frameSpace*2,5);
    ldGraphSetGridOffset(ptWidget,5);

    LOG_INFO("[init][graph] id:%d, size:%llu", nameId,sizeof (*ptWidget));
    return ptWidget;
}

void ldGraph_depose( ldGraph_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeGraph)
    {
        return;
    }

    LOG_INFO("[depose][graph] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget->pSeries);
    ldFree(ptWidget);
}

void ldGraph_on_load( ldGraph_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldGraph_on_frame_start( ldGraph_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldGraph_show(ld_scene_t *ptScene, ldGraph_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
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

            // draw frame
            if(ptWidget->isFrame)
            {
                if(ptWidget->isCorner)
                {
                    draw_round_corner_box(&tTarget,&tTarget_canvas,GLCD_COLOR_WHITE,255,bIsNewFrame);
                    draw_round_corner_border(&tTarget,&tTarget_canvas,GLCD_COLOR_LIGHT_GREY,(arm_2d_border_opacity_t){255,255,255,255},(arm_2d_corner_opacity_t){255,255,255,255});
                }
                else
                {
                    ldBaseColor(&tTarget,NULL,GLCD_COLOR_WHITE,255);
                    arm_2d_draw_box(&tTarget,&tTarget_canvas,1,GLCD_COLOR_LIGHT_GREY,255);
                }
            }
            else
            {
                ldBaseColor(&tTarget,NULL,GLCD_COLOR_WHITE,255);
            }
            arm_2d_op_wait_async(NULL);

            // draw grid
            uint16_t xCount,yCount;
            arm_2d_location_t gridLocal;

            xCount=ptWidget->xAxisMax/(ptWidget->xScale*ptWidget->gridOffset);
            yCount=ptWidget->yAxisMax/(ptWidget->yScale*ptWidget->gridOffset*5);

            for(uint16_t j=0;j<=yCount;j++)
            {
                for(uint16_t i=0;i<=xCount;i++)
                {
                    gridLocal.iX=ptWidget->frameSpace+(ptWidget->xScale*ptWidget->gridOffset)*i;
                    gridLocal.iY=ptWidget->frameSpace+(ptWidget->yScale*ptWidget->gridOffset*5)*j;
                    arm_2d_draw_point(&tTarget,gridLocal,GLCD_COLOR_LIGHT_GREY,128);
                }
            }

            xCount=ptWidget->xAxisMax/(ptWidget->xScale*ptWidget->gridOffset*5);
            yCount=ptWidget->yAxisMax/(ptWidget->yScale*ptWidget->gridOffset);

            for(uint16_t j=0;j<=yCount;j++)
            {
                for(uint16_t i=0;i<=xCount;i++)
                {
                    gridLocal.iX=ptWidget->frameSpace+(ptWidget->xScale*ptWidget->gridOffset*5)*i;
                    gridLocal.iY=ptWidget->frameSpace+(ptWidget->yScale*ptWidget->gridOffset)*j;
                    arm_2d_draw_point(&tTarget,gridLocal,GLCD_COLOR_LIGHT_GREY,128);
                }
            }
            arm_2d_op_wait_async(NULL);


            if(ptWidget->seriesCount>0)
            {
                int16_t x=0,y=0,xPrev,yPrev;

                xCount=ptWidget->xAxisMax/(ptWidget->xAxisOffset*ptWidget->xScale);

                for(uint16_t i=0;i<xCount;i++)
                {
                    xPrev=x;
                    x=ptWidget->frameSpace+(ptWidget->xAxisOffset*ptWidget->xScale)*i-(ptWidget->ptPointMaskTile->tRegion.tSize.iWidth/2);

                    for(uint8_t k=0;k<ptWidget->seriesCount;k++)
                    {
                        if(i<ptWidget->pSeries[k].valueCountMax)
                        {
                            y=tTarget_canvas.tSize.iHeight-ptWidget->frameSpace-(ptWidget->ptPointMaskTile->tRegion.tSize.iWidth/2)-(ptWidget->pSeries[k].pValueList[i]*ptWidget->yScale);

                            ldBaseImage(&tTarget,
                                        &((arm_2d_region_t){x,
                                                            y,
                                                            ptWidget->ptPointMaskTile->tRegion.tSize.iWidth,
                                                            ptWidget->ptPointMaskTile->tRegion.tSize.iHeight}),
                                        NULL,
                                        ptWidget->ptPointMaskTile,
                                        ptWidget->pSeries[k].seriesColor,
                                        ptWidget->use_as__ldBase_t.opacity);

                            if((i>0)&&(ptWidget->pSeries[k].lineSize>0))
                            {
                                yPrev=tTarget_canvas.tSize.iHeight-ptWidget->frameSpace-(ptWidget->ptPointMaskTile->tRegion.tSize.iWidth/2)-(ptWidget->pSeries[k].pValueList[i-1]*ptWidget->yScale);
                                ldBaseDrawLine(&tTarget,
                                               xPrev+(ptWidget->ptPointMaskTile->tRegion.tSize.iWidth/2),
                                               yPrev+ptWidget->ptPointMaskTile->tRegion.tSize.iWidth/2,
                                               x+(ptWidget->ptPointMaskTile->tRegion.tSize.iWidth/2),
                                               y+ptWidget->ptPointMaskTile->tRegion.tSize.iWidth/2,
                                               ptWidget->pSeries[k].lineSize,
                                               ptWidget->pSeries[k].seriesColor,
                                               ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity>>1);
                            }
                        }
                    }
                }
                arm_2d_op_wait_async(NULL);
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldGraphSetPointImageMask(ldGraph_t *ptWidget, arm_2d_tile_t *ptPointMaskTile)
{
    if (ptWidget == NULL)
    {
        return;
    }
    ptWidget->ptPointMaskTile=ptPointMaskTile;
    if(ptWidget->frameSpace<ptPointMaskTile->tRegion.tSize.iWidth)
    {
        ptWidget->frameSpace=ptPointMaskTile->tRegion.tSize.iWidth;
    }
}

int8_t ldGraphAddSeries(ldGraph_t *ptWidget,ldColor seriesColor,uint8_t lineSize,uint16_t valueCountMax)
{
    uint16_t *pBuf;
    if (ptWidget == NULL)
    {
        return -1;
    }
    if(ptWidget->seriesCount<ptWidget->seriesMax)
    {
        pBuf=ldCalloc(1,sizeof (uint16_t)*valueCountMax);
        if(pBuf!=NULL)
        {
            ptWidget->pSeries[ptWidget->seriesCount].pValueList=pBuf;
            ptWidget->pSeries[ptWidget->seriesCount].valueCountMax=valueCountMax;
            ptWidget->pSeries[ptWidget->seriesCount].seriesColor=seriesColor;
            ptWidget->pSeries[ptWidget->seriesCount].lineSize=lineSize;
            ptWidget->seriesCount++;
            return (ptWidget->seriesCount-1);
        }
    }
    return -1;
}

void ldGraphSetValue(ldGraph_t *ptWidget,uint8_t seriesNum,uint16_t valueNum,uint16_t value)
{
    if (ptWidget == NULL)
    {
        return ;
    }

    if(valueNum<ptWidget->pSeries[seriesNum].valueCountMax)
    {
        ptWidget->pSeries[seriesNum].pValueList[valueNum]=value;
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    }
}

void ldGraphSetAxis(ldGraph_t *ptWidget,uint16_t xAxis,uint16_t yAxis,uint16_t xAxisOffset)
{
    if (ptWidget == NULL)
    {
        return ;
    }

    float scale;
    scale=(float)(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-ptWidget->frameSpace*2)/xAxis;
    ptWidget->xAxisMax=xAxis*scale;
    ptWidget->xScale=scale;
    ptWidget->xAxisOffset=xAxisOffset*scale;

    scale=(float)(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-ptWidget->frameSpace*2)/yAxis;
    ptWidget->yAxisMax=yAxis*scale;
    ptWidget->yScale=scale;
}

void ldGraphSetAxisOffset(ldGraph_t *ptWidget,uint16_t xAxisOffset)
{
    if (ptWidget == NULL)
    {
        return ;
    }
    ptWidget->xAxisOffset=xAxisOffset;
}

void ldGraphSetFrameSpace(ldGraph_t *ptWidget,uint8_t frameSpace,bool isCorner)
{
    if (ptWidget == NULL)
    {
        return ;
    }

    ptWidget->isFrame=(frameSpace>0)?true:false;

    ptWidget->isCorner=isCorner;
    ptWidget->frameSpace=frameSpace;
}

void ldGraphSetGridOffset(ldGraph_t *ptWidget,uint8_t gridOffset)
{
    if (ptWidget == NULL)
    {
        return ;
    }

    ptWidget->gridOffset=gridOffset;
}

void ldGraphMoveAdd(ldGraph_t *ptWidget,uint8_t seriesNum,uint16_t newValue)
{
    if (ptWidget == NULL)
    {
        return ;
    }

    uint16_t *p=ptWidget->pSeries[seriesNum].pValueList;

    for(int i =0;i < ptWidget->pSeries[seriesNum].valueCountMax-1;i++)
    {
        p[i] = p[i+1];
    }
    p[ptWidget->pSeries[seriesNum].valueCountMax-1] = newValue;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
