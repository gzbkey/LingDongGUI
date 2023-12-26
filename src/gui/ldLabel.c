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
 * @file    ldLabel.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   label widget
 * @version 0.1
 * @date    2023-11-03
 */
#include "ldLabel.h"

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

static bool _labelDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldFree(((ldLabel_t*)pTarget)->pStr);
        ldFree(((ldLabel_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldLabelDel(ldLabel_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeLabel)
    {
        return;
    }

    LOG_INFO("[label] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _labelDel);
    }
}

/**
 * @brief   标签文本初始化
 * 
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @param   pFontDict       字体指针
 * @return  ldLabel_t*      新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
ldLabel_t *ldLabelInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict)
{
    ldLabel_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldLabel_t);
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
        pNewWidget->widgetType = widgetTypeLabel;
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
        pNewWidget->isTransparent=true;
        pNewWidget->pStr = NULL;
        pNewWidget->pFontDict = NULL;
        pNewWidget->align = 0;
        pNewWidget->charColor = 0;
        pNewWidget->pFontDict = pFontDict;
#if USE_OPACITY == 1
        pNewWidget->opacity=255;
#endif
        pNewWidget->dirtyRegionListItem.ptNext=NULL;
        pNewWidget->dirtyRegionListItem.tRegion = ldBaseGetGlobalRegion(pNewWidget,&((arm_2d_tile_t*)&pNewWidget->resource)->tRegion);
        pNewWidget->dirtyRegionListItem.bIgnore = false;
        pNewWidget->dirtyRegionListItem.bUpdated = true;
        pNewWidget->dirtyRegionState=waitChange;
        pNewWidget->dirtyRegionTemp=tResTile->tRegion;
        pNewWidget->isDirtyRegionAutoIgnore=true;

        LOG_INFO("[label] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[label] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldLabelFrameStart(ldLabel_t* pWidget)
{
    if(pWidget->dirtyRegionState==waitChange)
    {
        pWidget->dirtyRegionTemp=((arm_2d_tile_t*)&(pWidget->resource))->tRegion;
    }
    ldBaseDirtyRegionAutoUpdate((ldCommon_t*)pWidget,((arm_2d_tile_t*)&(pWidget->resource))->tRegion,pWidget->isDirtyRegionAutoIgnore);
}

void ldLabelLoop(ldLabel_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tempRes=*pResTile;
#else
    arm_2d_vres_t tempRes=*((arm_2d_vres_t*)pResTile);
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
        if(!pWidget->isTransparent)
        {
            if (pWidget->bgImgAddr==LD_ADDR_NONE)//color
            {
#if USE_OPACITY == 1
                ldBaseColor(&tTarget,pWidget->bgColor,pWidget->opacity);
#else
                ldBaseColor(&tTarget,pWidget->bgColor,255);
#endif
            }
            else
            {
                ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
                ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->bgImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                ((arm_2d_vres_t*)(&tempRes))->pTarget=pWidget->bgImgAddr;
#endif
#if USE_OPACITY == 1
                ldBaseImage(&tTarget,&tempRes,false,pWidget->opacity);
#else
                ldBaseImage(&tTarget,&tempRes,false,255);
#endif

            }
            arm_2d_op_wait_async(NULL);
        }

        if(pWidget->pStr!=NULL)
        {
            pResTile->tInfo.tColourInfo.chScheme = ldBaseGetChScheme(pWidget->pFontDict->maskType);
#if USE_OPACITY == 1
            ldBaseLineText(&tTarget,&pWidget->resource,pWidget->pStr,pWidget->pFontDict,pWidget->align,pWidget->charColor,0,pWidget->opacity);
#else
            ldBaseLineText(&tTarget,&pWidget->resource,pWidget->pStr,pWidget->pFontDict,pWidget->align,pWidget->charColor,0,255);
#endif
            arm_2d_op_wait_async(NULL);
        }
    }
}

/**
 * @brief   设置透明
 * 
 * @param   pWidget         目标控件指针
 * @param   isTransparent   true=透明 false=不透明
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldLabelSetTransparent(ldLabel_t* pWidget,bool isTransparent)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isTransparent=isTransparent;
}

/**
 * @brief   设置文本
 * 
 * @param   pWidget         目标控件指针
 * @param   pStr            文本指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldLabelSetText(ldLabel_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldFree(pWidget->pStr);
    pWidget->pStr=LD_MALLOC_STRING(pStr);
    strcpy((char*)pWidget->pStr,(char*)pStr);
    pWidget->dirtyRegionState=waitChange;
}

/**
 * @brief   设置文本颜色
 * 
 * @param   pWidget         目标控件指针
 * @param   charColor       文本颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldLabelSetTextColor(ldLabel_t* pWidget,ldColor charColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->charColor=charColor;
}

/**
 * @brief   文本的对齐方式
 * 
 * @param   pWidget         目标控件指针
 * @param   align           LD_ALIGN_CENTER
 *                          LD_ALIGN_TOP
 *                          LD_ALIGN_BOTTOM
 *                          LD_ALIGN_LEFT
 *                          LD_ALIGN_RIGHT
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldLabelSetAlign(ldLabel_t *pWidget,uint8_t align)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->align=align;
}

/**
 * @brief   设置背景图片
 * 
 * @param   pWidget         目标控件指针
 * @param   imageAddr       图片地址
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldLabelSetBgImage(ldLabel_t *pWidget, uint32_t imageAddr)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->bgImgAddr=imageAddr;
    pWidget->isTransparent=false;
}

/**
 * @brief   设置背景颜色
 * 
 * @param   pWidget         目标控件指针
 * @param   bgColor         背景颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldLabelSetBgColor(ldLabel_t *pWidget, ldColor bgColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->bgColor=bgColor;
    pWidget->isTransparent=false;
    pWidget->bgImgAddr=LD_ADDR_NONE;
}

/**
 * @brief   设置不透明度
 * 
 * @param   pWidget         目标控件指针
 * @param   opacity         0(透明)-255(不透明)
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldLabelSetOpacity(ldLabel_t *pWidget, uint8_t opacity)
{
    if (pWidget == NULL)
    {
        return;
    }
#if USE_OPACITY == 1
    pWidget->opacity=opacity;
#endif
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
