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

#ifndef __LD_TABLE_H__
#define __LD_TABLE_H__

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
#ifdef __LD_TABLE_IMPLEMENT__
#undef __LD_TABLE_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_TABLE_INHERIT__)
#undef __LD_TABLE_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct{
    bool isButton:1;
    bool isCheckable:1;
    bool isChecked:1;
    bool isStaticText:1;
    bool isSelectShow:1;
    bool isSelect:1;
    bool isEditable:1;
    bool isEditing:1;
    ldEditType_t editType:2;
    uint8_t *pText;
    arm_2d_tile_t *ptReleaseImgTile;
    arm_2d_tile_t *ptReleaseMaskTile;
    arm_2d_tile_t *ptPressImgTile;
    arm_2d_tile_t *ptPressMaskTile;
    arm_2d_font_t *ptFont;
    arm_2d_location_t tLocation;
    arm_2d_align_t tAlign;
    ldColor textColor;
    ldColor itemBgColor;
    ldColor releaseImgMaskColor;
    ldColor pressImgMaskColor;
    uint8_t textMax;
}ldTableItem_t;

typedef struct ldTable_t ldTable_t;

struct ldTable_t
{
    implement(ldBase_t);
//ARM_PRIVATE(
//    ld_scene_t *ptScene;
//)

    uint8_t rowCount;
    uint8_t columnCount;
    uint8_t itemSpace;//item 间隔
    int16_t *pColumnWidth;
    int16_t *pRowHeight;
    ldColor bgColor;
    ldColor selectColor;
    ldTableItem_t *ptItemInfo;
    int16_t scrollOffsetX;
    int16_t scrollOffsetY;
    uint8_t currentRow;
    uint8_t currentColumn;
    int64_t timer;
    uint16_t kbNameId;
    bool isBgTransparent:1;
    bool isAlignGrid:1;
};

ldTable_t* ldTable_init(ld_scene_t *ptScene, ldTable_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t rowCount, uint8_t columnCount, uint8_t itemSpace, arm_2d_font_t *ptFont);
void ldTable_depose( ldTable_t *ptWidget);
void ldTable_on_load( ldTable_t *ptWidget);
void ldTable_on_frame_start( ldTable_t *ptWidget);
void ldTable_show(ld_scene_t *pScene, ldTable_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldTableSetItemWidth(ldTable_t *ptWidget,uint8_t column,int16_t width);
void ldTableSetItemHeight(ldTable_t *ptWidget,uint8_t row,int16_t height);
void ldTableSetItemText(ldTable_t *ptWidget,uint8_t row,uint8_t column,uint8_t *pText,arm_2d_font_t* ptFont);
void ldTableSetItemStaticText(ldTable_t *ptWidget,uint8_t row,uint8_t column,uint8_t *pText,arm_2d_font_t* ptFont);
void ldTableSetItemColor(ldTable_t *ptWidget,uint8_t row,uint8_t column,ldColor textColor,ldColor bgColor);
void ldTableSetBgColor(ldTable_t *ptWidget,ldColor bgColor);
void ldTableSetItemAlign(ldTable_t *ptWidget,uint8_t row,uint8_t column,arm_2d_align_t tAlign);
void ldTableSetItemImage(ldTable_t *ptWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,arm_2d_tile_t* ptImgTile,arm_2d_tile_t *ptMaskTile,ldColor maskColor);
void ldTableSetItemButton(ldTable_t *ptWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,arm_2d_tile_t *ptReleaseImgTile,arm_2d_tile_t *ptReleaseMaskTile,ldColor releaseImgMaskColor,arm_2d_tile_t *ptPressImgTile,arm_2d_tile_t *ptPressMaskTile,ldColor pressImgMaskColor,bool isCheckable);
void ldTableSetKeyboard(ldTable_t* ptWidget,uint16_t kbNameId);
void ldTableSetEditable(ldTable_t* ptWidget,uint8_t row,uint8_t column,bool isEditable,uint8_t textMax);
void ldTableSetExcelType(ldTable_t *ptWidget,arm_2d_font_t* ptFont);
void ldTableSetAlignGrid(ldTable_t *ptWidget,bool isAlignGrid);

#define ldTableInit(nameId,parentNameId,x,y,width,height,rowCount,columnCount,itemSpace,ptFont) \
        ldTable_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,rowCount,columnCount,itemSpace,ptFont)

#define ldTableSetHidden                ldBaseSetHidden
#define ldTableMove                     ldBaseMove
#define ldTableSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
