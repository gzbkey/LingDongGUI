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

#ifndef _LD_QRCODE_H_
#define _LD_QRCODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "qrcodegen.h"

typedef enum {
    eccLow = 0,
    eccMedium,
    eccQuartile,
    eccHigh,
}ldQRCodeEcc_t;

typedef struct {
    LD_COMMON_ATTRIBUTES;
    ldQRCodeEcc_t qrEcc;
    uint8_t qrMaxVersion;
    uint8_t qrZoom;
    ldColor bgColor;
    ldColor qrColor;
    uint8_t* qrText;
}ldQRCode_t;

ldQRCode_t *ldQRCodeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t* qrText, ldColor qrColor, ldColor bgColor, ldQRCodeEcc_t qrEcc, uint8_t qrMaxVersion, uint8_t qrZoom);
void ldQRCodeFrameStart(ldQRCode_t* pWidget);
void ldQRCodeLoop(ldQRCode_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldQRCodeDel(ldQRCode_t *pWidget);
void ldQRCodeSetText(ldQRCode_t *pWidget, uint8_t *pNewText);


#define ldQRCodeSetHidden          ldBaseSetHidden
#define ldQRCodeMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_QRCODE_H_
