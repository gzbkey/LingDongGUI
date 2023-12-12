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

        LOG_INFO("[label] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[label] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldLabelLoop(ldLabel_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
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

    ldBaseDirtyRegionAutoUpdate((ldCommon_t*)pWidget,&pResTile->tRegion,bIsNewFrame);
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
                pResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
                pResTile->pchBuffer = (uint8_t *)pWidget->bgImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                ((arm_2d_vres_t*)pResTile)->pTarget=pWidget->bgImgAddr;
#endif
#if USE_OPACITY == 1
                ldBaseImage(&tTarget,pResTile,false,pWidget->opacity);
#else
                ldBaseImage(&tTarget,pResTile,false,255);
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

void ldLabelSetTransparent(ldLabel_t* pWidget,bool isTransparent)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isTransparent=isTransparent;
}

void ldLabelSetText(ldLabel_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldFree(pWidget->pStr);
    pWidget->pStr=LD_MALLOC_STRING(pStr);
    strcpy((char*)pWidget->pStr,(char*)pStr);
}

void ldLabelSetTextColor(ldLabel_t* pWidget,ldColor charColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->charColor=charColor;
}

void ldLabelSetAlign(ldLabel_t *pWidget,uint8_t align)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->align=align;
}

void ldLabelSetBgImage(ldLabel_t *pWidget, uint32_t imageAddr)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->bgImgAddr=imageAddr;
    pWidget->isTransparent=false;
}

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
