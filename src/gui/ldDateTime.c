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
 * @file    ldDateTime.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   date time widget
 * @version 0.1
 * @date    2023-11-03
 */
#include "ldDateTime.h"

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

static bool _dateTimeDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        //del user object

        ldFree(((ldDateTime_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldDateTimeDel(ldDateTime_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeDateTime)
    {
        return;
    }

    LOG_INFO("[dateTime] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _dateTimeDel);
    }
}

ldDateTime_t *ldDateTimeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t* pFontDict)
{
    ldDateTime_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldDateTime_t);
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
        pNewWidget->widgetType = widgetTypeDateTime;
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

        pNewWidget->pFontDict=pFontDict;
        pNewWidget->align=LD_ALIGN_CENTER;
        pNewWidget->isTransparent=true;
        pNewWidget->charColor=0;
        pNewWidget->year=2023;
        pNewWidget->month=1;
        pNewWidget->day=1;
        pNewWidget->hour=12;
        pNewWidget->minute=0;
        pNewWidget->second=0;
        strcpy((char*)pNewWidget->formatStr,"yyyy-mm-dd hh:nn:ss");
        pNewWidget->formatStrTemp[0]=0;

        LOG_INFO("[dateTime] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[dateTime] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldDateTimeLoop(ldDateTime_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
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

    if(bIsNewFrame)
    {
        char *addr;
        int ret;
        char strTemp[5];

        strcpy((char *)pWidget->formatStrTemp,(char *)pWidget->formatStr);

        addr=strstr((char *)pWidget->formatStrTemp,"yyyy");
        if(addr)
        {
            ret=snprintf(strTemp,5,"%04d",pWidget->year);
            if(ret > 0)
            {
                memcpy(addr,strTemp,4);
            }
        }
        addr=strstr((char *)pWidget->formatStrTemp,"mm");
        if(addr)
        {
            ret=snprintf(strTemp,3,"%02d",pWidget->month);
            if(ret > 0)
            {
                memcpy(addr,strTemp,2);
            }
        }
        addr=strstr((char *)pWidget->formatStrTemp,"dd");
        if(addr)
        {
            ret=snprintf(strTemp,3,"%02d",pWidget->day);
            if(ret > 0)
            {
                memcpy(addr,strTemp,2);
            }
        }
        addr=strstr((char *)pWidget->formatStrTemp,"hh");
        if(addr)
        {
            ret=snprintf(strTemp,3,"%02d",pWidget->hour);
            if(ret > 0)
            {
                memcpy(addr,strTemp,2);
            }
        }
        addr=strstr((char *)pWidget->formatStrTemp,"nn");
        if(addr)
        {
            ret=snprintf(strTemp,3,"%02d",pWidget->minute);
            if(ret > 0)
            {
                memcpy(addr,strTemp,2);
            }
        }
        addr=strstr((char *)pWidget->formatStrTemp,"ss");
        if(addr)
        {
            ret=snprintf(strTemp,3,"%02d",pWidget->second);
            if(ret > 0)
            {
                memcpy(addr,strTemp,2);
            }
        }
    }

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if(!pWidget->isTransparent)
        {
#if USE_OPACITY == 1
            ldBaseColor(&tTarget,pWidget->bgColor,pWidget->opacity);
#else
            ldBaseColor(&tTarget,pWidget->bgColor,255);
#endif
            arm_2d_op_wait_async(NULL);
        }

            pResTile->tInfo.tColourInfo.chScheme = ldBaseGetChScheme(pWidget->pFontDict->maskType);
#if USE_OPACITY == 1
            ldBaseLineText(&tTarget,&pWidget->resource,pWidget->formatStrTemp,pWidget->pFontDict,pWidget->align,pWidget->charColor,0,pWidget->opacity);
#else
            ldBaseLineText(&tTarget,&pWidget->resource,pWidget->formatStrTemp,pWidget->pFontDict,pWidget->align,pWidget->charColor,0,255);
#endif
            arm_2d_op_wait_async(NULL);
    }
}

void ldDateTimeSetTransparent(ldDateTime_t* pWidget,bool isTransparent)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isTransparent=isTransparent;
}

void ldDateTimeSetFormat(ldDateTime_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }

    strcpy((char*)pWidget->formatStr,(char*)pStr);
}

void ldDateTimeSetTextColor(ldDateTime_t* pWidget,ldColor charColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->charColor=charColor;
}

void ldDateTimeSetAlign(ldDateTime_t *pWidget,uint8_t align)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->align=align;
}

void ldDateTimeSetBgColor(ldDateTime_t *pWidget, ldColor bgColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->bgColor=bgColor;
    pWidget->isTransparent=false;
}

void ldDateTimeSetOpacity(ldDateTime_t *pWidget, uint8_t opacity)
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