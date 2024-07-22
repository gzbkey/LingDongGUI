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

#define __ARM_2D_HELPER_CONTROL_INHERIT__
#include "ldBase.h"
#include "ldConfig.h"
#include <stdarg.h>
#if LD_MEM_MODE == MEM_MODE_TLFS
#include "tlsf.h"
#elif LD_MEM_MODE == MEM_MODE_FREERTOS_HEAP4
#include "freeRtosHeap4.h"
#elif LD_MEM_MODE == MEM_MODE_STDLIB
#include "stdlib.h"
#endif

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

#if LD_MEM_MODE == MEM_MODE_TLFS
static void *pTlsfMem = NULL;
__attribute__((aligned(8))) uint8_t ucHeap[LD_MEM_SIZE];
#elif LD_MEM_MODE == MEM_MODE_FREERTOS_HEAP4
__attribute__((aligned(8))) uint8_t ucHeap[LD_MEM_SIZE];
#endif

__WEAK void *ldMalloc(uint32_t size)
{
    void *p = NULL;
#if LD_MEM_MODE == MEM_MODE_TLFS
    if (pTlsfMem == NULL)
    {
        pTlsfMem = tlsf_create_with_pool((void *)ucHeap, sizeof(ucHeap));
    }
    p = tlsf_malloc(pTlsfMem, size);
#elif LD_MEM_MODE == MEM_MODE_FREERTOS_HEAP4
    p = pvPortMalloc(size);
#elif LD_MEM_MODE == MEM_MODE_STDLIB
    p = malloc(size);
#endif
    return p;
}

__WEAK void *ldCalloc(uint32_t num, uint32_t size)
{
    void *p = NULL;

    p = ldMalloc(num * size);

    if (p != NULL)
    {
        memset(p, 0, num * size);
    }
    return p;
}

__WEAK void ldFree(void *p)
{
    if (p == NULL)
    {
        return;
    }
#if LD_MEM_MODE == MEM_MODE_TLFS
    tlsf_free(pTlsfMem, p);
#elif LD_MEM_MODE == MEM_MODE_FREERTOS_HEAP4
    vPortFree(p);
#elif LD_MEM_MODE == MEM_MODE_STDLIB
    free(p);
#endif
    p=NULL;
}

__WEAK void *ldRealloc(void *ptr, uint32_t newSize)
{
    void *p = NULL;
#if LD_MEM_MODE == MEM_MODE_TLFS
    p = tlsf_realloc(pTlsfMem, ptr, newSize);
#elif LD_MEM_MODE == MEM_MODE_FREERTOS_HEAP4
    p = pvPortRealloc(ptr, newSize);
#elif LD_MEM_MODE == MEM_MODE_STDLIB
    return realloc(ptr, newSize);
#endif
    if (p != NULL)
    {
        memset(p, 0, newSize);
    }
    return p;
}

bool ldTimeOut(uint16_t ms, int64_t *pTimer, bool isReset)
{
    int64_t lPeriod;
    int64_t lTimestamp = arm_2d_helper_get_system_timestamp();

    if (1 == *pTimer)
    {
        return false;
    }

    lPeriod = arm_2d_helper_convert_ms_to_ticks(ms);
    if (0 == *pTimer)
    {
        *pTimer = lPeriod;
        *pTimer += lTimestamp;

        return false;
    }

    if (lTimestamp >= *pTimer)
    {
        if (isReset)
        {
            *pTimer = lPeriod + lTimestamp;
        }
        else
        {
            *pTimer = 1;
        }
        return true;
    }
    return false;
}

void ldBaseNodeAdd(arm_2d_control_node_t *parent, arm_2d_control_node_t *child)
{
    child->ptParent = parent;
    if (parent->ptChildList == NULL)
    {
        parent->ptChildList = child;
    }
    else
    {
        arm_2d_control_node_t *sibling = parent->ptChildList;
        while (sibling->ptNext != NULL)
        {
            sibling = sibling->ptNext;
        }
        sibling->ptNext = child;
    }
}

void ldBaseNodeRemove(arm_2d_control_node_t *ptNodeRoot,arm_2d_control_node_t *ptNode)
{
    arm_2d_control_node_t *ptNext=ptNode->ptNext;

    arm_ctrl_enum(ptNodeRoot, ptItem, PREORDER_TRAVERSAL)
    {
        if (ptItem->ptNext == ptNode)
        {
            ptItem->ptNext = ptNext;
            return;
        }

        if(ptItem->ptChildList == ptNode)
        {
            ptItem->ptChildList=ptNext;
            return;
        }
    }
}

#if (USE_LOG_LEVEL>=LOG_LEVEL_NONE)
void ldBaseNodeTreePrint(arm_2d_control_node_t *ptNodeRoot, int depth)
{
    for (int i = 0; i < depth; ++i)
    {
        LOG_NORMAL("  "); // 打印缩进
    }

    LOG_NORMAL("type:%02d,id:%02d",((ldBase_t*)ptNodeRoot)->widgetType,((ldBase_t*)ptNodeRoot)->nameId);

    arm_2d_control_node_t *child = ptNodeRoot->ptChildList;
    while (child != NULL)
    {
        ldBaseNodeTreePrint(child, depth + 1);
        child = child->ptNext;
    }
}
#endif

void *ldBaseGetWidget(arm_2d_control_node_t *ptNodeRoot,uint16_t nameId)
{
    arm_ctrl_enum(ptNodeRoot, ptItem, PREORDER_TRAVERSAL)
    {
        if (((ldBase_t *)ptItem)->nameId == nameId)
        {
            return ptItem;
        }
    }
    return NULL;
}

void ldBaseColor(arm_2d_tile_t *ptTile, arm_2d_region_t *ptRegion, ldColor color, uint8_t opacity)
{
    arm_2d_fill_colour_with_opacity(ptTile, ptRegion, (__arm_2d_color_t)color, opacity);
}

void ldBaseImage(arm_2d_tile_t *ptTile, arm_2d_region_t *ptRegion, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile, ldColor color, uint8_t opacity)
{
    if (ptImgTile == NULL)
    {
        switch (ptMaskTile->tInfo.tColourInfo.chScheme)
        {
        case ARM_2D_COLOUR_1BIT:
        {
            arm_2d_fill_colour_with_a1_mask_and_opacity(ptTile,
                                                        ptRegion,
                                                        ptMaskTile,
                                                        (__arm_2d_color_t){color},
                                                        opacity);

            break;
        }
        case ARM_2D_COLOUR_MASK_A2:
        {
            arm_2d_fill_colour_with_a2_mask_and_opacity(ptTile,
                                                        ptRegion,
                                                        ptMaskTile,
                                                        (__arm_2d_color_t){color},
                                                        opacity);
            break;
        }
        case ARM_2D_COLOUR_MASK_A4:
        {
            arm_2d_fill_colour_with_a4_mask_and_opacity(ptTile,
                                                        ptRegion,
                                                        ptMaskTile,
                                                        (__arm_2d_color_t){color},
                                                        opacity);
            break;
        }
        case ARM_2D_COLOUR_MASK_A8:
        {
            arm_2d_fill_colour_with_mask_and_opacity(ptTile,
                                                     ptRegion,
                                                     ptMaskTile,
                                                     (__arm_2d_color_t){color},
                                                     opacity);
            break;
        }
        default:
            break;
        }
    }
    else
    {
        if (ptMaskTile == NULL)
        {
            arm_2d_tile_copy_with_opacity(ptImgTile,
                                          ptTile,
                                          ptRegion,
                                          opacity);
        }
        else
        {
            arm_2d_tile_copy_with_src_mask_only(ptImgTile,
                                                ptMaskTile,
                                                ptTile,
                                                ptRegion);
        }
    }
}

void ldBaseLabel(arm_2d_tile_t *ptTile,arm_2d_region_t *ptRegion,uint8_t *pStr,arm_2d_font_t *ptFont,arm_2d_align_t tAlign,ldColor textColor,uint8_t opacity)
{
    arm_lcd_text_set_target_framebuffer(ptTile);
    arm_lcd_text_set_font(ptFont);
    arm_lcd_text_set_draw_region(ptRegion);
    arm_lcd_text_set_colour(textColor, GLCD_COLOR_WHITE);
    arm_lcd_text_set_opacity(opacity);
    arm_lcd_printf_label(tAlign,(char*)pStr);
}

void arm_lcd_text_puts(arm_2d_region_t* ptRegion,arm_2d_font_t *ptFont,char *str,uint8_t opacity)
{
    arm_2d_size_t tCharSize = ptFont->tCharSize;
    arm_2d_size_t tDrawRegionSize = ptRegion->tSize;

    arm_2d_location_t   tDrawOffset={0,0};
    arm_2d_char_descriptor_t tCharDescriptor;

    while(*str) {
        if (*str == '\r') {
            tDrawOffset.iX = 0;
        } else if (*str == '\n') {
            tDrawOffset.iX = 0;
            tDrawOffset.iY += tCharSize.iHeight;
        } else if (*str == '\t') {
            tDrawOffset.iX += tCharSize.iWidth * 4;
            tDrawOffset.iX -= tDrawOffset.iX % tCharSize.iWidth;

            if (tDrawOffset.iX >= tDrawRegionSize.iWidth) {
                tDrawOffset.iX = 0;
                tDrawOffset.iY += tCharSize.iHeight;

                if (tDrawOffset.iY >= tDrawRegionSize.iHeight) {
                    tDrawOffset.iY = 0;
                }
            }

        }else if (*str == '\b') {
            if (tDrawOffset.iX >= tCharSize.iWidth) {
                tDrawOffset.iX -= tCharSize.iWidth;
            } else {
                tDrawOffset.iX = 0;
            }
        } else {

            arm_2d_helper_get_char_descriptor(  ptFont,&tCharDescriptor,str);

            if (tDrawOffset.iX+tCharDescriptor.iAdvance >= tDrawRegionSize.iWidth)
            {
                tDrawOffset.iX = 0;
                tDrawOffset.iY += tCharSize.iHeight;
            }
            int16_t iX = tDrawOffset.iX + ptRegion->tLocation.iX;
            int16_t iY = tDrawOffset.iY + ptRegion->tLocation.iY;

            tDrawOffset.iX
                += lcd_draw_char(   iX, iY, (uint8_t **)&str, opacity);

            continue;
        }
        str++;
    }
}

arm_2d_size_t arm_lcd_text_get_box(char *str, arm_2d_font_t *ptFont)
{
    arm_2d_size_t tCharSize = ptFont->tCharSize;
    arm_2d_region_t tDrawBox = {
        .tSize.iHeight = tCharSize.iHeight,
    };
    arm_2d_char_descriptor_t tCharDescriptor;

    while(*str) {
        if (*str == '\r') {
            tDrawBox.tLocation.iX = 0;
        } else if (*str == '\n') {
            tDrawBox.tLocation.iX = 0;
            tDrawBox.tLocation.iY += tCharSize.iHeight;

            tDrawBox.tSize.iHeight += tCharSize.iHeight;
        } else if (*str == '\t') {
            tDrawBox.tLocation.iX += tCharSize.iWidth * 4;
            tDrawBox.tLocation.iX -= tDrawBox.tLocation.iX
                                   % tCharSize.iWidth;

            tDrawBox.tSize.iWidth = MAX(tDrawBox.tSize.iWidth, tDrawBox.tLocation.iX);

        }else if (*str == '\b') {
            if (tDrawBox.tLocation.iX >= tCharSize.iWidth) {
                tDrawBox.tLocation.iX -= tCharSize.iWidth;
            } else {
                tDrawBox.tLocation.iX = 0;
            }
        } else {

            int8_t chCodeLength = arm_2d_helper_get_utf8_byte_valid_length((uint8_t *)str);
            if (chCodeLength <= 0) {
                chCodeLength = 1;
            }

            arm_2d_helper_get_char_descriptor(  ptFont,&tCharDescriptor,str);

            tDrawBox.tLocation.iX += tCharDescriptor.iAdvance;
            tDrawBox.tSize.iWidth = MAX(tDrawBox.tSize.iWidth, tDrawBox.tLocation.iX);

            str += chCodeLength;
            continue;
        }

        str++;
    }

    return tDrawBox.tSize;
}

void ldBaseSetHidden(ldBase_t* ptWidget,bool isHidden)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->isDirtyRegionUpdate = true;
    ptWidget->isHidden=isHidden;
}

void ldBaseMove(ldBase_t* ptWidget,int16_t x,int16_t y)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->isDirtyRegionUpdate = true;

    ptWidget->tTempRegion=ptWidget->use_as__arm_2d_control_node_t.tRegion;
    ptWidget->use_as__arm_2d_control_node_t.tRegion.tLocation.iX=x;
    ptWidget->use_as__arm_2d_control_node_t.tRegion.tLocation.iY=y;

    arm_2d_region_get_minimal_enclosure(&ptWidget->tTempRegion,
                                        &ptWidget->use_as__arm_2d_control_node_t.tRegion,
                                        &ptWidget->tTempRegion);


}

void ldBaseSetOpacity(ldBase_t *ptWidget, uint8_t opacity)
{
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->isDirtyRegionUpdate = true;
    ptWidget->opacity=opacity;
}
