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

#define __LD_QR_CODE_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldQRCode.h"

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

const ldBaseWidgetFunc_t ldQRCodeFunc = {
    .depose = (ldDeposeFunc_t)ldQRCode_depose,
    .load = (ldLoadFunc_t)ldQRCode_on_load,
    .frameStart = (ldFrameStartFunc_t)ldQRCode_on_frame_start,
    .show = (ldShowFunc_t)ldQRCode_show,
};

ldQRCode_t* ldQRCode_init(ld_scene_t *ptScene, ldQRCode_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t* pStr, ldColor qrColor, ldColor bgColor, uint8_t qrEcc, uint8_t qrMaxVersion, uint8_t qrZoom)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    uint8_t *ptText = NULL;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldQRCode_t));
        ptText = ldCalloc(1,strlen((char*)pStr)+1);
        if ((NULL == ptWidget) || (NULL == ptText))
        {
            ldFree(ptWidget);
            ldFree(ptText);
            LOG_ERROR("[init failed][QRCode] id:%d", nameId);
            return NULL;
        }
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeQRCode;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldQRCodeFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    strcpy((char*)ptText,(char*)pStr);
    ptWidget->pStr=ptText;
    ptWidget->qrEcc=qrEcc;
    qrMaxVersion=(qrMaxVersion==0)?1:qrMaxVersion;
    ptWidget->qrMaxVersion=qrMaxVersion;
    ptWidget->qrZoom=qrZoom;
    ptWidget->qrColor=qrColor;
    ptWidget->bgColor=bgColor;

    LOG_INFO("[init][QRCode] id:%d", nameId);
    return ptWidget;
}

void ldQRCode_depose( ldQRCode_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeQRCode)
    {
        return;
    }

    LOG_INFO("[depose][QRCode] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove(ptWidget->ptScene->ptNodeRoot,(arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldQRCode_on_load( ldQRCode_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldQRCode_on_frame_start( ldQRCode_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldQRCode_show(ld_scene_t *ptScene, ldQRCode_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
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
    if(arm_2d_helper_pfb_is_region_active(ptTile,&ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            uint8_t *qr0;
            uint8_t *tempBuffer;
            uint32_t qrSize;
            bool ret;
            ldColor tempColor;

            qr0=ldCalloc(1,qrcodegen_BUFFER_LEN_FOR_VERSION(ptWidget->qrMaxVersion));
            tempBuffer=ldCalloc(1,qrcodegen_BUFFER_LEN_FOR_VERSION(ptWidget->qrMaxVersion));

            if((qr0!=NULL)&&(tempBuffer!=NULL))
            {
                ret = qrcodegen_encodeText((const char*)ptWidget->pStr,tempBuffer, qr0, (enum qrcodegen_Ecc)ptWidget->qrEcc,ptWidget->qrMaxVersion, ptWidget->qrMaxVersion,qrcodegen_Mask_AUTO,true);

                if (ret)
                {
                    qrSize = qrcodegen_getSize(qr0);

                    for (uint32_t y = 0; y < qrSize; y++)
                    {
                        for (uint32_t x = 0; x < qrSize; x++)
                        {
                            if(qrcodegen_getModule(qr0, x, y))
                            {
                                tempColor=ptWidget->qrColor;
                            }
                            else
                            {
                                tempColor=ptWidget->bgColor;
                            }
                            arm_2d_region_t tBoxRegion = {
                                .tLocation = {
                                    .iX = x*ptWidget->qrZoom,
                                    .iY = y*ptWidget->qrZoom,
                                },
                                .tSize = {
                                    .iWidth=ptWidget->qrZoom,
                                    .iHeight=ptWidget->qrZoom,
                                }
                            };
                            arm_2d_tile_t tChildTile;
                            arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tChildTile, false);
                            ldBaseColor(&tChildTile,NULL,tempColor,ptWidget->use_as__ldBase_t.opacity);
                        }
                    }
                }
            }
            ldFree(qr0);
            ldFree(tempBuffer);
        }
    }
    arm_2d_op_wait_async(NULL);
}

void ldQRCodeSetText(ldQRCode_t *ptWidget, uint8_t *pStr)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ldFree(ptWidget->pStr);
    ptWidget->pStr=ldCalloc(1,strlen((char*)pStr)+1);
    if(ptWidget->pStr!=NULL)
    {
        strcpy((char*)ptWidget->pStr,(char*)pStr);
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
