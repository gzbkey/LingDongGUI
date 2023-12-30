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

#ifndef _LD_COMBOBOX_H_
#define _LD_COMBOBOX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isExpand:1;
    bool isCorner:1;
    uint32_t dropdownImgAddr;
    uint8_t dropdownImgWidth;
    uint8_t dropdownImgHeight;
    uint8_t itemMax;
    uint8_t itemCount;
    uint8_t itemSelect;
    uint8_t itemPreSelect;
    int16_t itemHeight;
    ldFontDict_t* pFontDict;
    void **ppItemStrGroup;
    ldColor charColor;

}ldComboBox_t;

ldComboBox_t* ldComboBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict, uint8_t itemMax);
void ldComboBoxFrameUpdate(ldComboBox_t* pWidget);
void ldComboBoxLoop(ldComboBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldComboBoxDel(ldComboBox_t *pWidget);

void ldComboBoxAddItem(ldComboBox_t* pWidget,uint8_t *pStr);
void ldComboBoxSetCorner(ldComboBox_t* pWidget,bool isCorner);

#define ldComboBoxSetHidden          ldBaseSetHidden
#define ldComboBoxMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_COMBOBOX_H_
