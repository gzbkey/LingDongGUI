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
 * @file    ldButton.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   button widget
 * @version 0.1
 * @date    2023-11-03
 */
#include "ldButton.h"
#include "ldGui.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

static bool _buttonDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldFree(((ldButton_t*)pTarget)->pStr);
        ldFree(((ldButton_t*)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldButtonDel(ldButton_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeButton)
    {
        return;
    }

    LOG_INFO("[button] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _buttonDel);
    }
}

static bool slotButtonToggle(xConnectInfo_t info)
{
    ldButton_t *btn;

    btn=ldGetWidgetById(info.receiverId);

    if(info.signalType==SIGNAL_PRESS)
    {
        btn->isPressed=true;
    }
    if(info.signalType==SIGNAL_RELEASE)
    {
        btn->isPressed=false;
    }

    return false;
}


ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height)
{
    ldButton_t * pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList;
    arm_2d_tile_t *tResTile;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldButton_t);
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
        pNewWidget->widgetType = widgetTypeButton;
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isCheckable=false;
        pNewWidget->isChecked=false;
        pNewWidget->isPressed=false;
        pNewWidget->isSelected=false;
        pNewWidget->releaseColor = __RGB(217,225,244);
        pNewWidget->pressColor = __RGB(255,243,202);
        pNewWidget->selectColor = __RGB(255,0,0);
        pNewWidget->keyValue=0;
        pNewWidget->releaseImgAddr=LD_ADDR_NONE;
        pNewWidget->pressImgAddr=LD_ADDR_NONE;
        pNewWidget->selectMaskAddr=LD_ADDR_NONE;
        pNewWidget->isReleaseMask = false;
        pNewWidget->isPressMask = false;
        pNewWidget->isTransparent=false;
        pNewWidget->isHidden = false;
        pNewWidget->pStr = NULL;
        pNewWidget->pFontDict = NULL;
        pNewWidget->align = 0;
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

        xConnect(nameId,SIGNAL_PRESS,nameId,slotButtonToggle);
        xConnect(nameId,SIGNAL_RELEASE,nameId,slotButtonToggle);

        LOG_INFO("[button] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[button] init failed,id:%d\n",nameId);
    }
    return pNewWidget;
}

void ldButtonSetText(ldButton_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldFree(pWidget->pStr);
    pWidget->pStr=LD_MALLOC_STRING(pStr);
    strcpy((char*)pWidget->pStr,(char*)pStr);
}

void ldButtonSetColor(ldButton_t* pWidget,ldColor releaseColor,ldColor pressColor)
{
    pWidget->releaseColor=releaseColor;
    pWidget->pressColor=pressColor;
    pWidget->releaseImgAddr=LD_ADDR_NONE;
    pWidget->pressImgAddr=LD_ADDR_NONE;
}

void ldButtonSetImage(ldButton_t* pWidget,uint32_t releaseImgAddr,bool isReleaseMask,uint32_t pressImgAddr,bool isPressMask)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->releaseImgAddr=releaseImgAddr;
    pWidget->pressImgAddr=pressImgAddr;
    pWidget->isReleaseMask=isReleaseMask;
    pWidget->isPressMask=isPressMask;
}

void ldButtonSetSelectImage(ldButton_t* pWidget,uint32_t selectMaskAddr,ldColor selectColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->selectMaskAddr=selectMaskAddr;
    pWidget->selectColor=selectColor;
}

void ldButtonSetTextColor(ldButton_t* pWidget,ldColor charColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->charColor=charColor;
}

void ldButtonSetTransparent(ldButton_t* pWidget,bool isTransparent)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isTransparent=isTransparent;
}

void ldButtonSetRoundCorner(ldButton_t* pWidget,bool isCorner)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isCorner=isCorner;
}

void ldButtonSetSelect(ldButton_t* pWidget,bool isSelected)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isSelected=isSelected;
}

void ldButtonLoop(ldButton_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    uint32_t btnColor;
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tempRes;
#else
    arm_2d_vres_t tempRes;
#endif

    if (pWidget == NULL)
    {
        return;
    }
    
    if((pWidget->isParentHidden)||(pWidget->isHidden)||(pWidget->isTransparent))
    {
        return;
    }

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if(!pWidget->isTransparent)
        {
            if ((pWidget->releaseImgAddr==LD_ADDR_NONE)&&(pWidget->pressImgAddr==LD_ADDR_NONE))//color
            {
                if(pWidget->isPressed)
                {
                    btnColor=pWidget->pressColor;
                }
                else
                {
                    btnColor=pWidget->releaseColor;
                }
                if(pWidget->isCorner)
                {
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize = pResTile->tRegion.tSize;//tTarget.tRegion.tSize;
                    draw_round_corner_box(&tTarget,
                                                &((arm_2d_tile_t*)&tempRes)->tRegion,
                                                btnColor,
                                                255,
                                                bIsNewFrame);
                }
                else
                {
                    ldBaseColor(&tTarget,btnColor,255);
                }
            }
            else
            {
                if(pWidget->isPressed)
                {
                    pResTile->pchBuffer = (uint8_t *)pWidget->pressImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)pResTile)->pTarget=pWidget->pressImgAddr;
#endif
                    if(!pWidget->isCorner)
                    {
                        ldBaseImage(&tTarget,pResTile,pWidget->isPressMask,255);
                    }
                }
                else
                {
                    pResTile->pchBuffer = (uint8_t *)pWidget->releaseImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)pResTile)->pTarget=pWidget->releaseImgAddr;
#endif
                    if(!pWidget->isCorner)
                    {
                        ldBaseImage(&tTarget,pResTile,pWidget->isReleaseMask,255);
                    }
                }
                if(pWidget->isCorner)
                {
#if USE_VIRTUAL_RESOURCE == 0
                    tempRes=*pResTile;
#else
                    tempRes=*((arm_2d_vres_t*)pResTile);
#endif
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;
                    draw_round_corner_image((arm_2d_tile_t*)&tempRes,
                                            &tTarget,
                                            &((arm_2d_tile_t*)&tempRes)->tRegion,
                                            bIsNewFrame);
                }
            }
            arm_2d_op_wait_async(NULL);

            if(pWidget->pStr!=NULL)
            {
                ldBaseLineText(&tTarget,&pWidget->resource,pWidget->pStr,pWidget->pFontDict,pWidget->align,pWidget->charColor,0,255);
                arm_2d_op_wait_async(NULL);
            }

            if(pWidget->isSelected)
            {
                if (pWidget->selectMaskAddr==LD_ADDR_NONE)
                {
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize = pResTile->tRegion.tSize;//tTarget.tRegion.tSize;

                    if(pWidget->isCorner)
                    {
                        draw_round_corner_border(&tTarget,&((arm_2d_tile_t*)&tempRes)->tRegion,pWidget->selectColor,
                                                 (arm_2d_border_opacity_t){SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY},
                                                 (arm_2d_corner_opacity_t){SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY});
                    }
                    else
                    {
                        arm_2d_draw_box(&tTarget,&((arm_2d_tile_t*)&tempRes)->tRegion,3,pWidget->selectColor,SELECT_COLOR_OPACITY);
                    }
                }
                else
                {
#if USE_VIRTUAL_RESOURCE == 0
                    tempRes=*pResTile;
                    tempRes.tInfo.tColourInfo.chScheme=ldBaseGetChScheme(pWidget->pFontDict->maskType);
                    tempRes.pchBuffer = (uint8_t *)pWidget->selectMaskAddr;
#else
                    tempRes=*((arm_2d_vres_t*)pResTile);
                    ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme=ldBaseGetChScheme(pWidget->pFontDict->maskType);
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->selectMaskAddr;
                    tempRes.pTarget=pWidget->selectMaskAddr;
                    tempRes.Load = &__disp_adapter0_vres_asset_loader;
                    tempRes.Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
                    ldBaseMaskImage(&tTarget,(arm_2d_tile_t*)&tempRes,pWidget->selectColor,255);
                }
                arm_2d_op_wait_async(NULL);
            }
        }
    }
}


void ldButtonSetAlign(ldButton_t *pWidget,uint8_t align)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->align=align;
}

void ldButtonSetFont(ldButton_t *pWidget,ldFontDict_t *pFontDict)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->pFontDict=pFontDict;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
