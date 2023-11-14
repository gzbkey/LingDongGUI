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
 * @file    ldProgressBar.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   progress bar widget
 * @version 0.1
 * @date    2023-11-03
 */
#include "ldProgressBar.h"
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

static bool _progressBarDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        //del user object

        ldFree(((ldProgressBar_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldProgressBarDel(ldProgressBar_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeProgressBar)
    {
        return;
    }

    LOG_INFO("[progressBar] del,id:%d\n",pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _progressBarDel);
    }
}

ldProgressBar_t *ldProgressBarInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    ldProgressBar_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldProgressBar_t);
    if (pNewWidget != NULL)
    {
        pNewWidget->isParentHidden=false;
        
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeProgressBar;
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

        pNewWidget->bgAddr=LD_ADDR_NONE;
        pNewWidget->fgAddr=LD_ADDR_NONE;
        pNewWidget->frameAddr=LD_ADDR_NONE;
//        pNewWidget->maskAddr=LD_ADDR_NONE;
        pNewWidget->bgOffset=0;
        pNewWidget->fgOffset=0;
        pNewWidget->bgColor=GLCD_COLOR_WHITE;
        pNewWidget->fgColor=__RGB(0x94, 0xd2, 0x52);
        pNewWidget->frameColor=__RGB(0xa5, 0xc6, 0xef);
        pNewWidget->permille=0;

        pNewWidget->timer = arm_2d_helper_get_system_timestamp();

        LOG_INFO("[progressBar] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[progressBar] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

static void _progressBarColorShow(ldProgressBar_t *pWidget,arm_2d_tile_t *ptTarget)
{
    arm_2d_region_t tBarRegion = {
        .tLocation = {
           .iX = 0,
           .iY = 0,
        },
        .tSize = ((arm_2d_tile_t*) &pWidget->resource)->tRegion.tSize,//ptTarget->tRegion.tSize,
    };

    arm_2d_fill_colour( ptTarget,
                        &tBarRegion,
                        pWidget->frameColor);

    arm_2d_op_wait_async(NULL);

    tBarRegion.tSize.iHeight-=2;
    tBarRegion.tSize.iWidth-=2;
    tBarRegion.tLocation.iX += 1;
    tBarRegion.tLocation.iY += 1;

    arm_2d_fill_colour(ptTarget, &tBarRegion, pWidget->bgColor);

    arm_2d_op_wait_async(NULL);

    if (pWidget->permille > 0)
    {
        tBarRegion.tSize.iWidth = tBarRegion.tSize.iWidth * pWidget->permille / 1000;
        arm_2d_fill_colour( ptTarget,
                            &tBarRegion,
                            pWidget->fgColor
                            );
        arm_2d_op_wait_async(NULL);
    }
}

static void _progressBarImageShow(ldProgressBar_t *pWidget,arm_2d_tile_t *ptTarget,bool bIsNewFrame)
{
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tResTile=pWidget->resource;
    tResTile.bIsRoot=true;
    tResTile.bHasEnforcedColour=true;
    tResTile.tRegion.tLocation.iX=0;
    tResTile.tRegion.tLocation.iY=0;
#else
    arm_2d_vres_t tResTile=*((arm_2d_vres_t*)(&pWidget->resource));
    tResTile.tTile.bIsRoot=true;
    tResTile.tTile.bHasEnforcedColour=true;
    tResTile.tTile.bVirtualResource=true;
    tResTile.tTile.tRegion.tLocation.iX=0;
    tResTile.tTile.tRegion.tLocation.iY=0;
#endif

    arm_2d_region_t tBarRegion = {
        .tLocation = {
           .iX = 0,
           .iY = 0,
        },
        .tSize = ((arm_2d_tile_t*) &pWidget->resource)->tRegion.tSize,
    };

    do {

        //bg image
        ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth=pWidget->bgWidth;
        ((arm_2d_tile_t*)&tResTile)->pchBuffer=(uint8_t*)pWidget->bgAddr;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile.pTarget=pWidget->bgAddr;
#endif
        arm_2d_region_t tInnerRegion = {
            .tSize = {
                .iWidth = tBarRegion.tSize.iWidth + ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth,
                .iHeight = tBarRegion.tSize.iHeight,
            },
            .tLocation = {
                .iX = -pWidget->bgWidth + pWidget->bgOffset,
            },
        };
        arm_2d_tile_t tileInnerSlot;
        arm_2d_tile_generate_child(ptTarget, &tBarRegion, &tileInnerSlot, false);
        arm_2d_tile_fill_only( (arm_2d_tile_t*)&tResTile,
                            &tileInnerSlot,
                            &tInnerRegion);
        arm_2d_op_wait_async(NULL);
    } while(0);

    if (pWidget->permille > 0)
    {
        do {
            //fg image
            ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth=pWidget->fgWidth;
            ((arm_2d_tile_t*)&tResTile)->pchBuffer=(uint8_t*)pWidget->fgAddr;
#if USE_VIRTUAL_RESOURCE == 1
            tResTile.pTarget=pWidget->fgAddr;
#endif
            arm_2d_region_t tInnerRegion = {
                .tSize = {
                    .iWidth = tBarRegion.tSize.iWidth + ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth,
                    .iHeight = tBarRegion.tSize.iHeight,
                },
                .tLocation = {
                    .iX = -pWidget->fgWidth + pWidget->fgOffset,
                },
            };
            tBarRegion.tSize.iWidth = tBarRegion.tSize.iWidth * pWidget->permille / 1000;

            arm_2d_tile_t tileInnerSlot;
            arm_2d_tile_generate_child(ptTarget, &tBarRegion, &tileInnerSlot, false);
            arm_2d_tile_fill_only( (arm_2d_tile_t*)&tResTile,
                                &tileInnerSlot,
                                &tInnerRegion);
            arm_2d_op_wait_async(NULL);
        } while(0);
    }

    if(pWidget->frameAddr!=LD_ADDR_NONE)
    {
        do {
            //frame image png
            ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth=pWidget->frameWidth;
            ((arm_2d_tile_t*)&tResTile)->pchBuffer=(uint8_t*)pWidget->frameAddr;
#if USE_VIRTUAL_RESOURCE == 1
            tResTile.pTarget=pWidget->frameAddr;
#endif
            ldBaseImage(ptTarget,&tResTile,true,255);
            arm_2d_op_wait_async(NULL);
        } while (0);
    }

    //! update offset
    if (bIsNewFrame) {
        if (arm_2d_helper_is_time_out(PROGRESS_BAR_SPEED, &pWidget->timer))
        {
            if(pWidget->isBgMove)
            {
                pWidget->bgOffset++;
                if (pWidget->bgOffset >= pWidget->bgWidth)
                {
                    pWidget->bgOffset = 0;
                }
            }
            if(pWidget->isFgMove)
            {
                pWidget->fgOffset++;
                if (pWidget->fgOffset >= pWidget->fgWidth)
                {
                    pWidget->fgOffset = 0;
                }
            }
        }
    }
}

void ldProgressBarLoop(ldProgressBar_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
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
        if(pWidget->bgAddr==LD_ADDR_NONE&&pWidget->fgAddr==LD_ADDR_NONE)//color
        {
            _progressBarColorShow(pWidget,&tTarget);
        }
        else
        {
            _progressBarImageShow(pWidget,&tTarget,bIsNewFrame);
        }

    }
}

void ldProgressBarSetPercent(ldProgressBar_t *pWidget,float percent)
{
    if(pWidget==NULL)
    {
        return;
    }
    if(percent>=0)
    {
        if(percent>100)
        {
            percent=100;
        }
        pWidget->permille=percent*10;
    }
    else
    {
        if(percent<-100)
        {
            percent=-100;
        }
        pWidget->permille=1000+percent*10;
    }
}

void ldProgressBarSetBgImage(ldProgressBar_t *pWidget,uint32_t bgAddr,uint16_t bgWidth,bool isMove)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->bgAddr=bgAddr;
    pWidget->bgWidth=bgWidth;
    pWidget->isBgMove=isMove;
}

void ldProgressBarSetFgImage(ldProgressBar_t *pWidget,uint32_t fgAddr,uint16_t fgWidth,bool isMove)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->fgAddr=fgAddr;
    pWidget->fgWidth=fgWidth;
    pWidget->isFgMove=isMove;
}

void ldProgressBarSetFrameImage(ldProgressBar_t *pWidget,uint32_t frameAddr,uint16_t frameWidth)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->frameAddr=frameAddr;
    pWidget->frameWidth=frameWidth;
}

void ldProgressBarSetColor(ldProgressBar_t *pWidget,ldColor bgColor,ldColor fgColor,ldColor frameColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->bgAddr=LD_ADDR_NONE;
    pWidget->fgAddr=LD_ADDR_NONE;
    pWidget->bgColor=bgColor;
    pWidget->fgColor=fgColor;
    pWidget->frameColor=frameColor;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
