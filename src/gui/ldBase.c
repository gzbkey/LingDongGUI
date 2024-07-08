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

arm_2d_control_node_t *ptNodeRoot = NULL;

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
    //    if (parent->ptChildList == NULL)
    //    {
    //        child->ptParent = parent;
    //        parent->ptChildList = child;
    //    }
    //    else
    //    {
    //        arm_2d_control_node_t *sibling = parent->ptChildList;
    //        child->ptParent = sibling;
    //        while (sibling->ptNext != NULL)
    //        {
    //            child->ptParent = sibling;
    //            sibling = sibling->ptNext;
    //        }
    //        sibling->ptNext = child;
    //    }
}

void ldBaseNodeRemove(arm_2d_control_node_t *ptNode)
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

void *ldBaseGetWidget(uint16_t nameId)
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

void ldBaseColor(arm_2d_tile_t *pTile, arm_2d_region_t *pRegion, ldColor color, uint8_t opacity)
{
    arm_2d_fill_colour_with_opacity(pTile, pRegion, (__arm_2d_color_t)color, opacity);
}

void ldBaseImage(arm_2d_tile_t *pTile, arm_2d_region_t *pRegion, arm_2d_tile_t *pImgTile, arm_2d_tile_t *pMaskTile, ldColor color, uint8_t opacity)
{
    if (pImgTile == NULL)
    {
        switch (pMaskTile->tInfo.tColourInfo.chScheme)
        {
        case ARM_2D_COLOUR_1BIT:
        {
            arm_2d_fill_colour_with_a1_mask_and_opacity(pTile,
                                                        pRegion,
                                                        pMaskTile,
                                                        (__arm_2d_color_t){color},
                                                        opacity);

            break;
        }
        case ARM_2D_COLOUR_MASK_A2:
        {
            arm_2d_fill_colour_with_a2_mask_and_opacity(pTile,
                                                        pRegion,
                                                        pMaskTile,
                                                        (__arm_2d_color_t){color},
                                                        opacity);
            break;
        }
        case ARM_2D_COLOUR_MASK_A4:
        {
            arm_2d_fill_colour_with_a4_mask_and_opacity(pTile,
                                                        pRegion,
                                                        pMaskTile,
                                                        (__arm_2d_color_t){color},
                                                        opacity);
            break;
        }
        case ARM_2D_COLOUR_MASK_A8:
        {
            arm_2d_fill_colour_with_mask_and_opacity(pTile,
                                                     pRegion,
                                                     pMaskTile,
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
        if (pMaskTile == NULL)
        {
            arm_2d_tile_copy_with_opacity(pImgTile,
                                          pTile,
                                          pRegion,
                                          opacity);
        }
        else
        {
            arm_2d_tile_copy_with_src_mask_only(pImgTile,
                                                pMaskTile,
                                                pTile,
                                                pRegion);
        }
    }
}
