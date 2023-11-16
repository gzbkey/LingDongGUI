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

#ifndef _LD_TABLE_H_
#define _LD_TABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct{
    bool isCheckable:1;
    bool isChecked:1;
    bool isStaticText:1;
    uint8_t *pText;
    ldColor textColor;
    ldColor itemBgColor;
    uint32_t releaseImgAddr;
    uint32_t pressImgAddr;
    arm_2d_region_t srcRegion;
    ldFontDict_t* pFontDict;
}ldTableItem_t;

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isBgTransparent:1;
    uint8_t rowCount;
    uint8_t columnCount;
    uint8_t itemSpace;//item 间隔
    int16_t *pColumnWidth;
    int16_t *pRowHeight;
    ldColor bgColor;
    ldTableItem_t *pItemInfo;
}ldTable_t;

ldTable_t *ldTableInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t rowCount, uint8_t columnCount, uint8_t itemSpace);
void ldTableLoop(ldTable_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldTableDel(ldTable_t *pWidget);
void ldTableSetItemWidth(ldTable_t *pWidget,uint8_t column,int16_t width);
void ldTableSetItemHeight(ldTable_t *pWidget,uint8_t row,int16_t height);

void ldTableSetItemText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);
void ldTableSetItemStaticText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);
void ldTableSetItemColor(ldTable_t *pWidget,uint8_t row,uint8_t column,ldColor textColor,ldColor bgColor);
void ldTableSetBgColor(ldTable_t *pWidget,ldColor bgColor);

#define ldTableSetHidden          ldBaseSetHidden
#define ldTableMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_TABLE_H_
