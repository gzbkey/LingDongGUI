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
 * @file    QRCode.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   qr code widget
 * @version 0.1
 * @date    2023-11-14
 */
#include "ldQRCode.h"
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

static bool _QRCodeDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldFree(((ldQRCode_t *)pTarget)->qrText);
        ldFree(((ldQRCode_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldQRCodeDel(ldQRCode_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeQRCode)
    {
        return;
    }

    LOG_INFO("[qRCode] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _QRCodeDel);
    }
}

ldQRCode_t *ldQRCodeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t* qrText, ldColor qrColor, ldColor bgColor, uint8_t qrEcc, uint8_t qrMask, uint8_t qrMaxVersion, uint8_t qrZoom)
{
    ldQRCode_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    uint8_t *pText = NULL;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldQRCode_t);
    pText = LD_MALLOC_STRING(qrText);

    if ((pNewWidget != NULL)&&(pText!=NULL))
    {
        pNewWidget->isParentHidden=false;
        
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeQRCode;
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

        strcpy((char *)pText,(const char *)qrText);
        pNewWidget->qrText=pText;
        pNewWidget->qrEcc=qrEcc;
        pNewWidget->qrMask=qrMask;
        qrMaxVersion=(qrMaxVersion==0)?1:qrMaxVersion;
        pNewWidget->qrMaxVersion=qrMaxVersion;
        pNewWidget->qrZoom=qrZoom;
        pNewWidget->qrColor=qrColor;
        pNewWidget->bgColor=bgColor;

        LOG_INFO("[qRCode] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget->qrText);
        ldFree(pNewWidget);

        LOG_ERROR("[qRCode] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldQRCodeLoop(ldQRCode_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
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
        uint8_t *qr0;
        uint8_t *tempBuffer;
        uint32_t qrSize;
        bool ret;
        ldColor tempColor;

        qr0=ldMalloc(qrcodegen_BUFFER_LEN_FOR_VERSION(pWidget->qrMaxVersion));
        tempBuffer=ldMalloc(qrcodegen_BUFFER_LEN_FOR_VERSION(pWidget->qrMaxVersion));

        if((qr0!=NULL)&&(tempBuffer!=NULL))
        {
            ret = qrcodegen_encodeText((const char*)pWidget->qrText,tempBuffer, qr0, pWidget->qrEcc,pWidget->qrMaxVersion, pWidget->qrMaxVersion,pWidget->qrMask,true);

            if (ret)
            {
                qrSize = qrcodegen_getSize(qr0);

                for (uint32_t y = 0; y < qrSize; y++)
                {
                    for (uint32_t x = 0; x < qrSize; x++)
                    {
                        if(qrcodegen_getModule(qr0, x, y))
                        {
                            tempColor=pWidget->qrColor;
                        }
                        else
                        {
                            tempColor=pWidget->bgColor;
                        }

                        arm_2d_region_t tBoxRegion = {
                            .tLocation = {
                                .iX = x*pWidget->qrZoom,
                                .iY = y*pWidget->qrZoom,
                            },
                            .tSize = {
                                .iWidth=pWidget->qrZoom,
                                .iHeight=pWidget->qrZoom,
                            }
                        };
                        arm_2d_tile_t tChildTile;
                        arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tChildTile, false);

                        ldBaseColor(&tChildTile,tempColor,255);
                    }
                }
            }
        }

#if LD_DEBUG == 1
        arm_2d_draw_box(&tTarget,&tTarget_canvas,1,0,255);
#endif

        ldFree(qr0);
        ldFree(tempBuffer);

        arm_2d_op_wait_async(NULL);
    }
}

void ldQRCodeSetText(ldQRCode_t *pWidget, uint8_t *pNewText)
{
    uint8_t *pText;

    if (pWidget == NULL)
    {
        return;
    }
    ldFree(pWidget->qrText);
    pText=LD_MALLOC_STRING(pNewText);
    strcpy((char *)pText,(const char *)pNewText);
    pWidget->qrText=pText;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif