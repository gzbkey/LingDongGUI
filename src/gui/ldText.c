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
 * @file    ldText.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   text widget
 * @version 0.1
 * @date    2023-11-03
 */
#include "ldText.h"
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

static int16_t _pressY,_scrollOffset;
static bool _isTopScroll=false,_isBottomScroll=false;

static bool _textDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        if(((ldText_t*)pTarget)->pTextInfo!=NULL)
        {
            ldBaseTextDel(((ldText_t*)pTarget)->pTextInfo);
        }
        ldTextSetScroll((ldText_t*)pTarget,false);
        ldFree(((ldText_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldTextDel(ldText_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeText)
    {
        return;
    }

    LOG_INFO("[text] del,id:%d\n",pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _textDel);
    }
}

ldText_t *ldTextInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict)
{
    ldText_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldText_t);
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
        pNewWidget->widgetType = widgetTypeText;
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isHidden = false;
        pNewWidget->bgImgAddr=LD_ADDR_NONE;

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

        pNewWidget->isTransparent=false;
        pNewWidget->bgColor=__RGB(255,255,255);
        ldBaseSetFont(&pNewWidget->pTextInfo,pFontDict);
        pNewWidget->scrollOffset=0;
        pNewWidget->isRelease=false;
#if USE_OPACITY == 1
        pNewWidget->opacity=255;
#endif
        LOG_INFO("[text] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[text] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldTextLoop(ldText_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
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

    if(pWidget->isRelease)
    {
        int32_t iResult;
        bool isPiEnd;

        isPiEnd=arm_2d_helper_pi_slider(&pWidget->tPISlider, _scrollOffset, &iResult);

        if(_isTopScroll)
        {
            if(isPiEnd)
            {
                pWidget->isRelease=false;
                pWidget->scrollOffset=0;
            }
            else
            {
                pWidget->scrollOffset=_scrollOffset-iResult;
            }
        }
        if(_isBottomScroll)
        {
            if(isPiEnd)
            {
                pWidget->isRelease=false;
                pWidget->scrollOffset=pResTile->tRegion.tSize.iHeight-pWidget->strHeight;
            }
            else
            {
                pWidget->scrollOffset=(pResTile->tRegion.tSize.iHeight-pWidget->strHeight)-(_scrollOffset-iResult);
            }
        }
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
        if(pWidget->pTextInfo!=NULL)
        {
#if USE_OPACITY == 1
            ldBaseShowText(tTarget,pResTile->tRegion,pWidget->pTextInfo,pWidget->scrollOffset,pWidget->opacity);
#else
            ldBaseShowText(tTarget,pResTile->tRegion,pWidget->pTextInfo,pWidget->scrollOffset,255);
#endif
            arm_2d_op_wait_async(NULL);
        }
    }
}

void ldTextSetTransparent(ldText_t* pWidget,bool isTransparent)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isTransparent=isTransparent;
}

void ldTextSetText(ldText_t* pWidget,uint8_t *pStr)
{
    int16_t bmpH1Max;
    arm_2d_size_t textSize;
    if(pWidget==NULL)
    {
        return;
    }
    ldBaseSetText(&pWidget->pTextInfo,pStr);
    textSize= ldBaseGetStringSize(pWidget->pTextInfo->pStr,pWidget->pTextInfo->pFontDict,&bmpH1Max,((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iWidth);
    pWidget->strHeight=textSize.iHeight;
}

void ldTextSetTextColor(ldText_t* pWidget,ldColor charColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldBaseSetTextColor(&pWidget->pTextInfo,charColor);
}

void ldTextSetAlign(ldText_t *pWidget,uint8_t align)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldBaseSetAlign(&pWidget->pTextInfo,align);
}

void ldTextScrollSeek(ldText_t *pWidget,int16_t offset)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->scrollOffset=offset;
}

void ldTextScrollMove(ldText_t *pWidget, int8_t moveValue)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->scrollOffset+=moveValue;
    if((moveValue>0)&&(pWidget->scrollOffset<0))
    {
        if(pWidget->scrollOffset<0)
        {
            pWidget->scrollOffset=0;
        }
    }
}


static bool slotTextVerticalScroll(xConnectInfo_t info)
{
    ldText_t *txt;

    txt=ldGetWidgetById(info.receiverId);



    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        txt->isRelease=false;
        _scrollOffset=txt->scrollOffset;
        _isTopScroll=false;
        _isBottomScroll=false;
        break;
    }
    case SIGNAL_TOUCH_HOLD_MOVE:
    {
        txt->scrollOffset=_scrollOffset+(int16_t)GET_SIGNAL_VALUE_Y(info.value);
        break;
    }
    case SIGNAL_RELEASE:
    {
        txt->isRelease=true;

        _scrollOffset=txt->scrollOffset;

        if(txt->scrollOffset>0)
        {
            _isTopScroll=true;
            _isBottomScroll=false;
        }

        if(((arm_2d_tile_t*)&txt->resource)->tRegion.tSize.iHeight>(txt->strHeight+txt->scrollOffset))
        {
            _scrollOffset=((arm_2d_tile_t*)&txt->resource)->tRegion.tSize.iHeight-(txt->strHeight+txt->scrollOffset);
            _isTopScroll=false;
            _isBottomScroll=true;
        }

        if(txt->strHeight<=((arm_2d_tile_t*)&txt->resource)->tRegion.tSize.iHeight)
        {
            _isTopScroll=true;
            _isBottomScroll=false;
        }

        do {
            static const arm_2d_helper_pi_slider_cfg_t tCFG = {
                .fProportion = 0.2f,
                .fIntegration = 0.1f,
                .nInterval = 10,
            };
            arm_2d_helper_pi_slider_init(&txt->tPISlider, (arm_2d_helper_pi_slider_cfg_t *)&tCFG, 0);
        } while(0);
        break;
    }
    default:
        break;
    }

    return false;
}

void ldTextSetScroll(ldText_t *pWidget,bool isEnable)
{
    if(pWidget==NULL)
    {
        return;
    }
    if(pWidget->isScroll!=isEnable)
    {
        pWidget->isScroll=isEnable;
        if(isEnable)
        {
            xConnect(pWidget->nameId,SIGNAL_PRESS,pWidget->nameId,slotTextVerticalScroll);
            xConnect(pWidget->nameId,SIGNAL_TOUCH_HOLD_MOVE,pWidget->nameId,slotTextVerticalScroll);
            xConnect(pWidget->nameId,SIGNAL_RELEASE,pWidget->nameId,slotTextVerticalScroll);
        }
        else
        {
            xDisconnect(pWidget->nameId,SIGNAL_PRESS,pWidget->nameId,slotTextVerticalScroll);
            xDisconnect(pWidget->nameId,SIGNAL_TOUCH_HOLD_MOVE,pWidget->nameId,slotTextVerticalScroll);
            xDisconnect(pWidget->nameId,SIGNAL_RELEASE,pWidget->nameId,slotTextVerticalScroll);
        }
    }
}

void ldTextSetBgImage(ldText_t *pWidget, uint32_t imageAddr)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->bgImgAddr=imageAddr;
    pWidget->isTransparent=false;
}

void ldTextSetBgColor(ldText_t *pWidget, ldColor bgColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->bgColor=bgColor;
    pWidget->isTransparent=false;
    pWidget->bgImgAddr=LD_ADDR_NONE;
}

void ldTextSetOpacity(ldText_t *pWidget, uint8_t opacity)
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
