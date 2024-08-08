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
        LOG_PRINT("  "); // 打印缩进
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

void ldBaseImageScale(arm_2d_tile_t *ptTile, arm_2d_region_t *ptRegion, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile,float scale,arm_2d_op_trans_msk_opa_t *ptOP,bool bIsNewFrame)
{
    arm_2d_location_t tCentre = {
                    .iX = ptImgTile->tRegion.tSize.iWidth >> 1,
                    .iY = ptImgTile->tRegion.tSize.iHeight >> 1,
                };

    if (ptMaskTile != NULL)
    {
        arm_2dp_tile_transform_with_src_mask_and_opacity(
                    ptOP,         //!< control block
                    ptImgTile,        //!< source tile
                    ptMaskTile,          //!< source mask
                    ptTile,             //!< target tile
                    ptRegion,               //!< target region
                    tCentre,            //!< pivot on source
                    0,           //!< rotation angle
                    scale,           //!< zoom scale
                    255         //!< opacity
                    );
    }
    else
    {
            arm_2dp_tile_transform_only((arm_2d_op_trans_t*)ptOP,
                                        ptImgTile,
                                        ptTile,
                                        ptRegion,
                                        tCentre,
                                        0,
                                        scale);
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

    if(str!=NULL)
    {
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

arm_2d_location_t ldBaseGetRelativeLocation(ldBase_t *ptWidget,arm_2d_location_t tLocation)
{
    arm_2d_control_node_t *ptRoot=&ptWidget->use_as__arm_2d_control_node_t;

    while(ptRoot!=NULL)
    {
        tLocation.iX-=((ldBase_t*)ptRoot)->use_as__arm_2d_control_node_t.tRegion.tLocation.iX;
        tLocation.iY-=((ldBase_t*)ptRoot)->use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
        ptRoot=ptRoot->ptParent;
    }
    return tLocation;
}

arm_2d_location_t ldBaseGetAbsoluteLocation(ldBase_t *ptWidget,arm_2d_location_t tLocation)
{
    arm_2d_control_node_t *ptNode=&ptWidget->use_as__arm_2d_control_node_t;

    while(ptNode!=NULL)
    {
        tLocation.iX+=((ldBase_t*)ptNode)->use_as__arm_2d_control_node_t.tRegion.tLocation.iX;
        tLocation.iY+=((ldBase_t*)ptNode)->use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
        ptNode=ptNode->ptParent;
    }
    return tLocation;
}

arm_2d_region_t ldBaseGetAbsoluteRegion(ldBase_t *ptWidget)
{
    arm_2d_region_t tRegion={
        .tLocation={0},
        .tSize=ptWidget->use_as__arm_2d_control_node_t.tRegion.tSize,
    };
    arm_2d_control_node_t *ptNode=&ptWidget->use_as__arm_2d_control_node_t;

    while(ptNode!=NULL)
    {
        tRegion.tLocation.iX+=((ldBase_t*)ptNode)->use_as__arm_2d_control_node_t.tRegion.tLocation.iX;
        tRegion.tLocation.iY+=((ldBase_t*)ptNode)->use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
        ptNode=ptNode->ptParent;
    }
    return tRegion;
}


void ldBaseDrawCircle(arm_2d_tile_t *pTile, int centerX, int centerY, int radius, ldColor color,uint8_t opacityMax, uint8_t opacityMin)
{
    int x, y;
    int opacity;

    if(radius==0)
    {
        opacityMin=opacityMax;
    }

    for (y = centerY - radius; y <= centerY + radius; y++)
    {
        for (x = centerX - radius; x <= centerX + radius; x++)
        {
            int distance = (x - centerX) * (x - centerX) + (y - centerY) * (y - centerY);
            int sqrRadius = radius * radius;

            if (distance <= sqrRadius) {
                if (distance == sqrRadius) {
                    opacity = opacityMin;
                } else {
                    double ratio = (double)distance / sqrRadius;
                    opacity = (int)(opacityMax - (opacityMax - opacityMin) * ratio);
                }

                arm_2d_location_t point={
                    .iX=x,
                    .iY=y,
                };
                arm_2d_draw_point(pTile,point, color,opacity);
            }
        }
    }
}

#define ipart_(X) ((int)(X))
#define round_(X) ((int)(((float)(X))+0.5f))
#define fpart_(X) (((float)(X))-(float)ipart_(X))
#define rfpart_(X) (1.0-fpart_(X))
#define swap_(a,b)    (a=(a)+(b),b=(a)-(b),a=(a)-(b))

static void _setPixelAlpha(arm_2d_tile_t *pTile,int x, int y, float brightness, ldColor color)
{
    arm_2d_location_t point={
        .iX=x,
        .iY=y,
    };
    arm_2d_draw_point(pTile,point, color,brightness*255);
}

/**
 * @brief   吴小林抗锯齿直线算法
 *
 * @param   pTile           目标控件指针
 * @param   x0              直线起始坐标x
 * @param   y0              直线起始坐标y
 * @param   x1              直线结束坐标x
 * @param   y1              直线结束坐标y
 * @param   color           直线颜色
 */
void ldBaseDrawLine0(arm_2d_tile_t *pTile,int16_t x0 , int16_t y0 , int16_t x1 , int16_t y1,ldColor color)
{
    int steep = abs(y1 - y0) > abs(x1 - x0) ;

    // swap the co-ordinates if slope > 1 or we
    // draw backwards
    if (steep)
    {
        swap_(x0, y0);
        swap_(x1, y1);
    }
    if (x0 > x1)
    {
        swap_(x0, x1);
        swap_(y0, y1);
    }

    //compute the slope
    float dx = x1-x0;
    float dy = y1-y0;
    float gradient = dy/dx;
    if (dx == 0.0)
        gradient = 1;

    int xpxl1 = x0;
    int xpxl2 = x1;
    float intersectY = y0;
    int  x;

    if (steep)
    {
        for (x = xpxl1; x <= xpxl2; x++)
        {
            _setPixelAlpha(pTile,ipart_(intersectY), x, rfpart_(intersectY),color);
            _setPixelAlpha(pTile,ipart_(intersectY) + 1, x,fpart_(intersectY),color);
            intersectY += gradient;
        }
    }
    else
    {
        for (x = xpxl1; x <= xpxl2; x++)
        {
            _setPixelAlpha(pTile,x, ipart_(intersectY),rfpart_(intersectY),color);
            _setPixelAlpha(pTile,x, ipart_(intersectY)+1,fpart_(intersectY),color);
            intersectY += gradient;
        }
    }
}

void ldBaseDrawLine(arm_2d_tile_t *pTile,int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t lineSize, ldColor color,uint8_t opacityMax, uint8_t opacityMin)
{
    if(lineSize<=1)
    {
        ldBaseDrawLine0(pTile,x0,y0,x1,y1,color);
    }
    else
    {
        uint16_t t;
        int32_t xerr=0,yerr=0,delta_x,delta_y,distance;
        int32_t incx,incy,xPos,yPos;

        delta_x=x1-x0; //计算坐标增量
        delta_y=y1-y0;
        xPos=x0;
        yPos=y0;
        if(delta_x>0)incx=1; //设置单步方向
        else if(delta_x==0)incx=0;//垂直线
        else {incx=-1;delta_x=-delta_x;}
        if(delta_y>0)incy=1;
        else if(delta_y==0)incy=0;//水平线
        else{incy=-1;delta_y=-delta_y;}
        if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
        else distance=delta_y;
        for(t=0;t<=distance+1;t++ )//画线输出
        {
            ldBaseDrawCircle(pTile,xPos,yPos,lineSize/2,color,opacityMax,opacityMin);
            xerr+=delta_x ;
            yerr+=delta_y ;
            if(xerr>distance)
            {
                xerr-=distance;
                xPos+=incx;
            }
            if(yerr>distance)
            {
                yerr-=distance;
                yPos+=incy;
            }
        }
    }
}

ldBase_t* ldBaseGetParent(ldBase_t* ptWidget)
{
    return ptWidget->use_as__arm_2d_control_node_t.ptParent;
}

void ldBaseBgMove(ld_scene_t *ptScene, int16_t bgWidth,int16_t bgHeight,int16_t offsetX,int16_t offsetY)
{
    ldBase_t *ptWidget= ptScene->ptNodeRoot;

    ldBaseMove(ptWidget,offsetX,offsetY);

    int16_t minX = MIN(0, offsetX);
    int16_t minY = MIN(0, offsetY);
    int16_t maxX = MAX(LD_CFG_SCEEN_WIDTH, offsetX + bgWidth);
    int16_t maxY = MAX(LD_CFG_SCEEN_HEIGHT, offsetX + bgHeight);

    ptWidget->use_as__arm_2d_control_node_t.tRegion.tSize.iWidth=maxX-minX;
    ptWidget->use_as__arm_2d_control_node_t.tRegion.tSize.iHeight=maxY-minY;

    extern void ldGuiUpdateScene(void);

    ldGuiUpdateScene();
}

arm_2d_control_node_t *ldBaseControlFindNodeWithLocation(arm_2d_control_node_t *ptRoot,arm_2d_location_t tLocation)
{
    arm_2d_control_node_t *ptNode = NULL;
    arm_2d_control_node_t *ptCandidate = NULL;
    arm_2d_control_node_t *ptTheLastContainer = NULL;

    /* tVisibleArea is an absolute region in the virtual screen */
    arm_2d_region_t tVisibleArea = {{0,0},{LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT}};

    ptNode = ptRoot;

    if (NULL == ptNode) {
        return NULL;
    }

    /* this must be the root node */
    assert(NULL == ptRoot->ptParent);

    /* get the root node region which is a relative region of the host tile
     * IMPORTANT: The region of the control root MUST be an absolute region
     */

    do {
        arm_2d_region_t tControlRegion = ptNode->tRegion;

        /* the node coordinate is an relative coordinate inside the container */
        tControlRegion.tLocation.iX += tVisibleArea.tLocation.iX;
        tControlRegion.tLocation.iY += tVisibleArea.tLocation.iY;

        bool bInsideControlRegion = false;

        /* we have to make sure the control's region is inside the visible area */
        if (arm_2d_region_intersect(&tControlRegion, &tVisibleArea, &tControlRegion)) {

            /* if it is inside the visible area, we check the touch point */
            bInsideControlRegion = arm_2d_is_point_inside_region(&tControlRegion, &tLocation);
        }

        if (bInsideControlRegion) {
            /* update candidate */
            ptCandidate = ptNode;
        }

        if (NULL != ptNode->ptNext) {
            /* There are more peers in the list, let's check them first */
            ptNode = ptNode->ptNext;
            continue;
        }

        /* When we reach here, we have already visited all peers in the list */
        if (NULL == ptCandidate) {
            /* the touch point hits nothing, let's end the search and
             * return NULL
             */
            break;
        }

        if (ptCandidate == ptTheLastContainer) {
            /* the only candidate is the container, i.e. the touch point hits
             * no controls in the container, let's return the container.
             */
            break;
        }

        if (NULL == ptCandidate->ptChildList) {
            /* the touch point hits a leaf, let's return the candidate */
            break;
        }

        /* When we reach here, the most recent candidate is a container, we
         * have to jump into it
         */
        ptTheLastContainer = ptCandidate;       /* update the last container */

        /* read the candidate region */
        tControlRegion = ptCandidate->tRegion;

        /* the candidate coordinate is an relative coordinate inside the container */
        tControlRegion.tLocation.iX += tVisibleArea.tLocation.iX;
        tControlRegion.tLocation.iY += tVisibleArea.tLocation.iY;

        /* update visible area */
        if (!arm_2d_region_intersect(&tControlRegion, &tVisibleArea, &tVisibleArea)) {
            /* this should not happen */
            assert(false);
        }

        /* search the child nodes */
        ptNode = ptCandidate->ptChildList;

    } while(true);

    return ptCandidate;
}

arm_2d_region_t ldBaseGetAlignRegion(arm_2d_region_t parentRegion,arm_2d_region_t childRegion,arm_2d_align_t tAlign)
{
    switch (tAlign)
    {
    case ARM_2D_ALIGN_LEFT:
    {
        childRegion.tLocation.iX=0;
        if(childRegion.tSize.iWidth>parentRegion.tSize.iWidth)
        {
            childRegion.tSize.iWidth=parentRegion.tSize.iWidth;
        }

        if(childRegion.tSize.iHeight<parentRegion.tSize.iHeight)
        {
            childRegion.tLocation.iY=(parentRegion.tSize.iHeight-childRegion.tSize.iHeight)>>1;
        }
        else
        {
            childRegion.tLocation.iY=0;
            childRegion.tSize.iHeight=parentRegion.tSize.iHeight;
        }
        break;
    }
    case ARM_2D_ALIGN_RIGHT:
    {
        if(childRegion.tSize.iWidth<parentRegion.tSize.iWidth)
        {
            childRegion.tLocation.iX=parentRegion.tSize.iWidth-childRegion.tSize.iWidth;
        }
        else
        {
            childRegion.tLocation.iX=0;
            childRegion.tSize.iWidth=parentRegion.tSize.iWidth;
        }

        if(childRegion.tSize.iHeight<parentRegion.tSize.iHeight)
        {
            childRegion.tLocation.iY=(parentRegion.tSize.iHeight-childRegion.tSize.iHeight)>>1;
        }
        else
        {
            childRegion.tLocation.iY=0;
            childRegion.tSize.iHeight=parentRegion.tSize.iHeight;
        }
        break;
    }
    case ARM_2D_ALIGN_TOP:
    {
        if(childRegion.tSize.iWidth<parentRegion.tSize.iWidth)
        {
            childRegion.tLocation.iX=(parentRegion.tSize.iWidth-childRegion.tSize.iWidth)>>1;
        }
        else
        {
            childRegion.tLocation.iX=0;
            childRegion.tSize.iWidth=parentRegion.tSize.iWidth;
        }

        childRegion.tLocation.iY=0;
        if(childRegion.tSize.iHeight>parentRegion.tSize.iHeight)
        {
            childRegion.tSize.iHeight=parentRegion.tSize.iHeight;
        }
        break;
    }
    case ARM_2D_ALIGN_BOTTOM:
    {
        if(childRegion.tSize.iWidth<parentRegion.tSize.iWidth)
        {
            childRegion.tLocation.iX=(parentRegion.tSize.iWidth-childRegion.tSize.iWidth)>>1;
        }
        else
        {
            childRegion.tLocation.iX=0;
            childRegion.tSize.iWidth=parentRegion.tSize.iWidth;
        }

        if(childRegion.tSize.iHeight<parentRegion.tSize.iHeight)
        {
            childRegion.tLocation.iY=parentRegion.tSize.iHeight-childRegion.tSize.iHeight;
        }
        else
        {
            childRegion.tLocation.iY=0;
            childRegion.tSize.iHeight=parentRegion.tSize.iHeight;
        }
        break;
    }
    case ARM_2D_ALIGN_CENTRE:
    {
        if(childRegion.tSize.iWidth<parentRegion.tSize.iWidth)
        {
            childRegion.tLocation.iX=(parentRegion.tSize.iWidth-childRegion.tSize.iWidth)>>1;
        }
        else
        {
            childRegion.tLocation.iX=0;
            childRegion.tSize.iWidth=parentRegion.tSize.iWidth;
        }

        if(childRegion.tSize.iHeight<parentRegion.tSize.iHeight)
        {
            childRegion.tLocation.iY=(parentRegion.tSize.iHeight-childRegion.tSize.iHeight)>>1;
        }
        else
        {
            childRegion.tLocation.iY=0;
            childRegion.tSize.iHeight=parentRegion.tSize.iHeight;
        }
        break;
    }
    default:
        break;
    }
    return childRegion;
}
