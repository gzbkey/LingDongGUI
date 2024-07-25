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

#ifndef __LD_QR_CODE_H__
#define __LD_QR_CODE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-declarations"
#pragma clang diagnostic ignored "-Wmicrosoft-anon-tag"
#pragma clang diagnostic ignored "-Wpadded"
#endif



/* OOC header, please DO NOT modify  */
#ifdef __LD_QR_CODE_IMPLEMENT__
#undef __LD_QR_CODE_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_QR_CODE_INHERIT__)
#undef __LD_QR_CODE_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"
#include "qrcodegen.h"

#define QR_ECC_7                        0
#define QR_ECC_15                       1
#define QR_ECC_25                       2
#define QR_ECC_30                       3

typedef struct ldQRCode_t ldQRCode_t;

struct ldQRCode_t
{
    implement(ldBase_t);
    ARM_PRIVATE(
            ld_scene_t *ptScene;
    )
    uint8_t qrEcc;
    uint8_t qrMaxVersion;
    uint8_t qrZoom;
    ldColor bgColor;
    ldColor qrColor;
    uint8_t* pStr;
};

ldQRCode_t* ldQRCode_init(ld_scene_t *ptScene, ldQRCode_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t* pStr, ldColor qrColor, ldColor bgColor, uint8_t qrEcc, uint8_t qrMaxVersion, uint8_t qrZoom);
void ldQRCode_depose( ldQRCode_t *ptWidget);
void ldQRCode_on_load( ldQRCode_t *ptWidget);
void ldQRCode_on_frame_start( ldQRCode_t *ptWidget);
void ldQRCode_show(ld_scene_t *pScene, ldQRCode_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldQRCodeSetText(ldQRCode_t *ptWidget, uint8_t *pStr);

#define ldQRCodeInit(nameId,parentNameId,x,y,width,height,pStr,qrColor,bgColor,qrEcc,qrMaxVersion,qrZoom) \
        ldQRCode_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,pStr,qrColor,bgColor,qrEcc,qrMaxVersion,qrZoom)

#define ldQRCodeSetHidden                ldBaseSetHidden
#define ldQRCodeMove                     ldBaseMove
#define ldQRCodeSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
