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

#ifndef _LD_LINEEDIT_H_
#define _LD_LINEEDIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isEditing:1;
    bool isCorner:1;
    bool blinkFlag:1;
    ldEditType_t editType:2;
    bool hasFloatPoint:1;
    ldFontDict_t *pFontDict;
    int64_t timer;
    uint16_t kbNameId;
    ldColor textColor;
    uint8_t *pText;
    uint8_t textLen;
    uint8_t textMax;

}ldLineEdit_t;

ldLineEdit_t* ldLineEditInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict, uint8_t textMax);
void ldLineEditFrameUpdate(ldLineEdit_t* pWidget);
void ldLineEditLoop(ldLineEdit_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldLineEditDel(ldLineEdit_t *pWidget);

void ldLineEditSetText(ldLineEdit_t* pWidget,uint8_t *pText);
void ldLineEditSetKeyboard(ldLineEdit_t* pWidget,uint16_t kbNameId);
void ldLineEditSetType(ldLineEdit_t* pWidget,ldEditType_t editType);

#define ldLineEditSetHidden          ldBaseSetHidden
#define ldLineEditMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_LINEEDIT_H_
