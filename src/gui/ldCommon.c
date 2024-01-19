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

/**
 * @file    ldCommon.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   通用函数文件
 * @version 0.1
 * @date    2023-11-03
 */
#include "ldCommon.h"
#include "xList.h"
#include <stdarg.h>
#include "ldConfig.h"
#include "ldWindow.h"
#include "ldImage.h"
#include "ldButton.h"
#include "ldText.h"
#include "ldGui.h"
#if USE_TLSF == 1
#include "tlsf.h"
#else
#include "freeRtosHeap4.h"
#endif

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

NEW_LIST(ldWidgetLink);

#if USE_TLSF == 1
static void * pTlsfMem=NULL;
#endif

__attribute__((aligned (8))) uint8_t ucHeap[LD_MEM_SIZE];

__WEAK void *ldCalloc(uint32_t size)
{
    void* p=NULL;
#if USE_TLSF == 1
    if(pTlsfMem==NULL)
    {
        pTlsfMem = tlsf_create_with_pool((void *)ucHeap, sizeof (ucHeap));
    }
    p=tlsf_malloc(pTlsfMem,size);
#else
     p=pvPortMalloc(size);
//    p=malloc(size);
#endif
    if(p!=NULL)
    {
        memset(p,0,size);
    }
    return p;
}

__WEAK void ldFree(void *p)
{
    if(p==NULL)
    {
        return;
    }
#if USE_TLSF == 1
    tlsf_free(pTlsfMem, p);
#else
    vPortFree(p);
//    free(p);
#endif
}

__WEAK void *ldRealloc(void *ptr,uint32_t newSize)
{
    void* p=NULL;
#if USE_TLSF == 1
    p = tlsf_realloc(pTlsfMem, ptr, newSize);
#else
    p = pvPortRealloc(ptr,newSize);
//    return realloc(ptr,newSize);
#endif
    if(p!=NULL)
    {
        memset(p,0,newSize);
    }
    return p;
}

static bool ldBaseGetInfoByName(xListNode *inList,xListNode ** out_info,uint16_t nameId)
{
    xListNode *tempPos,*safePos;
    ldCommon_t *pTempWidgetInfo;

    if(inList==NULL)
    {
        return false;
    }

    //判断链表是否为空
    if((inList->next!=inList)&&(inList->prev!=inList))
    {
        list_for_each_prev_safe(tempPos,safePos, inList)
        {
            if(tempPos->info!=NULL)
            {
                pTempWidgetInfo=(ldCommon_t*)(tempPos->info);
                if(nameId==pTempWidgetInfo->nameId)
                {
                    *out_info=tempPos;
                    return true;
                }
                else
                {
                    if(ldBaseGetInfoByName(pTempWidgetInfo->childList,out_info,nameId)==true)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


xListNode* ldBaseGetWidgetInfoById(uint16_t nameId)
{
    xListNode * pListInfo;
    if(ldBaseGetInfoByName(&ldWidgetLink,&pListInfo,nameId)==true)
    {
        return pListInfo;
    }
    return NULL;
}

void* ldBaseGetWidgetById(uint16_t nameId)
{
    xListNode * pListInfo;
    if(ldBaseGetInfoByName(&ldWidgetLink,&pListInfo,nameId)==true)
    {
        return pListInfo->info;
    }
    return NULL;
}

static bool ldBaseGetInfoByPos(xListNode *inList,xListNode ** out_info,int16_t x,int16_t y)
{
    xListNode *tempPos,*safePos;
    xListNode *pListInfo2;
    ldCommon_t *pTempWidgetInfo;
    ldPoint_t posT;
    int16_t widthT,heightT;

    if(inList==NULL)
    {
        return false;
    }

    //判断链表是否为空
    if((inList->next!=inList)&&(inList->prev!=inList))
    {
        list_for_each_prev_safe(tempPos,safePos, inList)
        {
            if(tempPos->info!=NULL)
            {
                pTempWidgetInfo=(ldCommon_t*)(tempPos->info);

                //获取全局坐标，用绝对值坐标进行比较
                posT= ldBaseGetGlobalPos(pTempWidgetInfo);
                widthT=(*(arm_2d_tile_t*)&(pTempWidgetInfo->resource)).tRegion.tSize.iWidth;
                heightT=(*(arm_2d_tile_t*)&(pTempWidgetInfo->resource)).tRegion.tSize.iHeight;

                if(pTempWidgetInfo->childList==NULL)//无子控件
                {
                    if((x>posT.x)&&(x<(posT.x+widthT))&&(y>posT.y)&&(y<(posT.y+heightT))&&(!pTempWidgetInfo->isHidden))
                    {
                       *out_info= tempPos;//返回控件widget
                       return true;
                    }
                }
                else//有子控件
                {
                    if(pTempWidgetInfo->isHidden==false)//无隐藏
                    {
                        //判断在窗体范围内
                        if((x>posT.x)&&(x<(posT.x+widthT))&&(y>posT.y)&&(y<(posT.y+heightT)))
                        {
                            if(ldBaseGetInfoByPos(pTempWidgetInfo->childList,&pListInfo2,x,y)==true)
                            {
                                *out_info= pListInfo2;
                                return true;
                            }
                            else
                            {
                                *out_info=tempPos;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}


xListNode* ldBaseGetWidgetInfoByPos(int16_t x,int16_t y)
{
    xListNode * pListInfo;
    if(ldBaseGetInfoByPos(&ldWidgetLink,&pListInfo,x,y)==true)
    {
        return pListInfo;
    }
    return NULL;
}

//获取全局坐标
ldPoint_t ldBaseGetGlobalPos(ldCommon_t *pWidget)
{
    ldPoint_t pos={0,0},posParent={0,0};

    if(pWidget==NULL)
    {
        return pos;
    }

    pos.x=(*(arm_2d_tile_t*)(&pWidget->resource)).tRegion.tLocation.iX;
    pos.y=(*(arm_2d_tile_t*)(&pWidget->resource)).tRegion.tLocation.iY;

    if(pWidget->parentWidget!=NULL)
    {
        if(((ldCommon_t*)pWidget->parentWidget)->widgetType!=widgetTypeBackground)
        {
            posParent=ldBaseGetGlobalPos(pWidget->parentWidget);
        }
        else
        {
            posParent.x=(*(arm_2d_tile_t*)&(((ldCommon_t*)pWidget->parentWidget)->resource)).tRegion.tLocation.iX;
            posParent.y=(*(arm_2d_tile_t*)&(((ldCommon_t*)pWidget->parentWidget)->resource)).tRegion.tLocation.iY;
        }
    }

    pos.x+=posParent.x;
    pos.y+=posParent.y;

    return pos;
}


ARM_NONNULL(2)
bool ldTimeOut(uint16_t ms, int64_t *pTimer,bool isReset)
{
    int64_t lPeriod;
    int64_t lTimestamp = arm_2d_helper_get_system_timestamp();

    if(1==*pTimer)
    {
        return false;
    }

    lPeriod=arm_2d_helper_convert_ms_to_ticks(ms);
    if (0 == *pTimer) {
        *pTimer = lPeriod;
        *pTimer += lTimestamp;

        return false;
    }

    if (lTimestamp >= *pTimer)
    {
        if(isReset)
        {
            *pTimer = lPeriod + lTimestamp;
        }
        else
        {
            *pTimer =1;
        }
            return true;
    }
    return false;
}

void ldBaseColor(arm_2d_tile_t* pTile,ldColor color,uint8_t opacity)
{
#if USE_OPACITY == 1
    arm_2d_fill_colour_with_opacity(pTile, NULL,(__arm_2d_color_t)color, opacity);
#else
    arm_2d_fill_colour(pTile, NULL, color);
#endif
}

void ldBaseImage(arm_2d_tile_t* pTile,arm_2d_tile_t* pResTile,bool isWithMask,uint8_t opacity)
{
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t resource=*pResTile;
#else
    arm_2d_vres_t resource =*((arm_2d_vres_t*)pResTile);
#endif

    if (isWithMask)
    {
#if USE_VIRTUAL_RESOURCE == 0
        arm_2d_tile_t maskTile = resource;
#else
        arm_2d_vres_t maskTile = *((arm_2d_vres_t*)&resource);
#endif
        (*(arm_2d_tile_t*)(&maskTile)).tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_8BIT;
#if LD_CFG_COLOR_DEPTH == 8
        (*(arm_2d_tile_t*)(&maskTile)).pchBuffer += (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iWidth * (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iHeight;
#endif
#if LD_CFG_COLOR_DEPTH == 16
        (*(arm_2d_tile_t*)(&maskTile)).pchBuffer += (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iWidth * (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iHeight * 2;
#endif
#if LD_CFG_COLOR_DEPTH == 32
        (*(arm_2d_tile_t*)(&maskTile)).pchBuffer += (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iWidth * (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iHeight * 4;
#endif

#if USE_VIRTUAL_RESOURCE == 1
        maskTile.pTarget=(uintptr_t)maskTile.tTile.pchBuffer;
#endif
        arm_2d_tile_copy_with_src_mask_only((arm_2d_tile_t *)&resource,
                                            (arm_2d_tile_t *)&maskTile,
                                            pTile,
                                            NULL);
    }
    else
    {
        switch ( (*(arm_2d_tile_t*)(&resource)).tInfo.tColourInfo.chScheme)
        {
        case ARM_2D_COLOUR_RGB565:
        case ARM_2D_COLOUR_CCCA8888:
        {
#if USE_OPACITY == 1
            arm_2d_tile_copy_with_opacity((arm_2d_tile_t*)(&resource),
                                          pTile,
                                          NULL,
                                          opacity);

#else
            arm_2d_tile_copy_only((arm_2d_tile_t*)(&resource),
                                  pTile,
                                  NULL);
#endif
            break;
        }

        default:
            break;
        }
    }
}

void ldBaseImageScale(arm_2d_tile_t* pTile,arm_2d_tile_t* pResTile,bool isWithMask,float scale,arm_2d_op_trans_msk_opa_t *pOP,bool bIsNewFrame)
{
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t resource=*pResTile;
#else
    arm_2d_vres_t resource =*((arm_2d_vres_t*)pResTile);
#endif
    arm_2d_location_t tCentre = {
                    .iX = (*(arm_2d_tile_t*)(&resource)).tRegion.tSize.iWidth >> 1,
                    .iY = (*(arm_2d_tile_t*)(&resource)).tRegion.tSize.iHeight >> 1,
                };

    if (isWithMask)
    {
#if USE_VIRTUAL_RESOURCE == 0
        arm_2d_tile_t maskTile = resource;
#else
        arm_2d_vres_t maskTile = *((arm_2d_vres_t*)&resource);
#endif
        (*(arm_2d_tile_t*)(&maskTile)).tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_8BIT;
        (*(arm_2d_tile_t*)(&maskTile)).pchBuffer += (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iWidth * (*(arm_2d_tile_t*)(&maskTile)).tRegion.tSize.iHeight * 2;
#if USE_VIRTUAL_RESOURCE == 1
        maskTile.pTarget=(uintptr_t)maskTile.tTile.pchBuffer;
#endif
        arm_2dp_tile_transform_with_src_mask_and_opacity(
                    pOP,         //!< control block
                    (arm_2d_tile_t*)(&resource),        //!< source tile
                    (arm_2d_tile_t *)&maskTile,          //!< source mask
                    pTile,             //!< target tile
                    NULL,               //!< target region
                    tCentre,            //!< pivot on source
                    0,           //!< rotation angle
                    scale,           //!< zoom scale
                    255         //!< opacity
                    );
    }
    else
    {
        switch ( (*(arm_2d_tile_t*)(&resource)).tInfo.tColourInfo.chScheme)
        {
        case ARM_2D_COLOUR_RGB565:
        case ARM_2D_COLOUR_CCCA8888:
        {
            arm_2dp_tile_transform_only((arm_2d_op_trans_t*)pOP,
                                        (arm_2d_tile_t*)(&resource),
                                        pTile,
                                        NULL,
                                        tCentre,
                                        0,
                                        scale);
            break;
        }

        default:
            break;
        }
    }
}

void ldBaseMaskImage(arm_2d_tile_t* pTile,arm_2d_tile_t* pResTile,ldColor textColor,uint8_t opacity)
{
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t resource=*pResTile;
    arm_2d_tile_t *pRes;
#else
    arm_2d_vres_t resource =*((arm_2d_vres_t*)pResTile);
    arm_2d_vres_t *pRes;
#endif
    pRes=&resource;

    switch (((arm_2d_tile_t*)pRes)->tInfo.tColourInfo.chScheme)
    {
    case ARM_2D_COLOUR_1BIT:
    {
#if USE_OPACITY == 1
        arm_2d_draw_pattern((arm_2d_tile_t*)pRes,
                            pTile,
                            &((arm_2d_tile_t*)pRes)->tRegion,
                            ARM_2D_DRW_PATN_MODE_COPY,
                            textColor,
                            LD_COLOR_BLACK);
#else
        arm_2d_draw_pattern((arm_2d_tile_t*)pRes,
                            pTile,
                            &(((arm_2d_tile_t*)pRes)->tRegion),
                            ARM_2D_DRW_PATN_MODE_COPY,
                            textColor,
                            LD_COLOR_BLACK);
#endif

        break;
    }
    case ARM_2D_COLOUR_MASK_A2:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_a2_mask_and_opacity(pTile,
                                                    &((arm_2d_tile_t*)pRes)->tRegion,
                                                    (arm_2d_tile_t*)pRes,
                                                    (__arm_2d_color_t){textColor},
                                                    opacity);
#else
        arm_2d_fill_colour_with_a2_mask(pTile,
                                        &(((arm_2d_tile_t*)pRes)->tRegion),
                                        (arm_2d_tile_t*)pRes,
                                        (__arm_2d_color_t){textColor});
#endif
        break;
    }
    case ARM_2D_COLOUR_MASK_A4:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_a4_mask_and_opacity(pTile,
                                                    &((arm_2d_tile_t*)pRes)->tRegion,
                                                    (arm_2d_tile_t*)pRes,
                                                    (__arm_2d_color_t){textColor},
                                                    opacity);
#else
        arm_2d_fill_colour_with_a4_mask(pTile,
                                        &(((arm_2d_tile_t*)pRes)->tRegion),
                                        (arm_2d_tile_t*)pRes,
                                        (__arm_2d_color_t){textColor});
#endif
        break;
    }
    case ARM_2D_COLOUR_MASK_A8:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_mask_and_opacity(pTile,
                                                 &((arm_2d_tile_t*)pRes)->tRegion,
                                                 (arm_2d_tile_t*)pRes,
                                                 (__arm_2d_color_t){textColor},
                                                 opacity);
#else
        arm_2d_fill_colour_with_mask(pTile,
                                     &(((arm_2d_tile_t*)pRes)->tRegion),
                                     (arm_2d_tile_t*)pRes,
                                     (__arm_2d_color_t){textColor});
#endif
        break;
    }
    default:
        break;
    }
}

void ldBaseTextDel(ldChar_t *pCharInfo)
{
    ldFree(pCharInfo->pStr);
    ldFree(pCharInfo);
}

ldChar_t * ldBaseCheckText(ldChar_t **ppCharInfo)
{
    if(*ppCharInfo==NULL)
    {
        *ppCharInfo=ldCalloc(sizeof(ldChar_t));
        if(*ppCharInfo==NULL)
        {
            return NULL;
        }
        (*ppCharInfo)->pStr=NULL;
        (*ppCharInfo)->align=LD_ALIGN_CENTER;
        (*(arm_2d_tile_t*)(&(*ppCharInfo)->fontTile)).tInfo.bIsRoot=true;
        (*(arm_2d_tile_t*)(&(*ppCharInfo)->fontTile)).tInfo.bHasEnforcedColour=true;
        (*(arm_2d_tile_t*)(&(*ppCharInfo)->fontTile)).tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A8;
#if USE_VIRTUAL_RESOURCE == 1
        (*(arm_2d_tile_t*)(&(*ppCharInfo)->fontTile)).bVirtualResource=true;
        (*ppCharInfo)->fontTile.pTarget=0;
        (*ppCharInfo)->fontTile.Load = &__disp_adapter0_vres_asset_loader;
        (*ppCharInfo)->fontTile.Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
    }
    return *ppCharInfo;
}

uint8_t ldBaseGetChScheme(uint8_t maskType)
{
    switch (maskType)
    {
    case 1:
    {
        return ARM_2D_COLOUR_1BIT;
    }
    case 2:
    {
        return ARM_2D_COLOUR_MASK_A2;
    }
    case 4:
    {
        return ARM_2D_COLOUR_MASK_A4;
    }
    case 8:
    {
        return ARM_2D_COLOUR_MASK_A8;
    }
    default:
    {
        return ARM_2D_COLOUR_1BIT;
    }
    }
}

void ldBaseSetFont(ldChar_t **ppCharInfo, ldFontDict_t* pFontDictAddr)
{
    if(ldBaseCheckText(ppCharInfo))
    {
        (*(arm_2d_tile_t*)(&(*ppCharInfo)->fontTile)).pchBuffer=(uint8_t *)pFontDictAddr->pFontSrc;
#if USE_VIRTUAL_RESOURCE == 1
        (*ppCharInfo)->fontTile.pTarget=pFontDictAddr->pFontSrc;
#endif
        (*ppCharInfo)->pFontDict=pFontDictAddr;
        (*(arm_2d_tile_t*)(&(*ppCharInfo)->fontTile)).tColourInfo.chScheme=ldBaseGetChScheme(pFontDictAddr->maskType);
    }
}

static uint8_t _getUtf8Len(uint8_t dat)
{
    uint8_t retBytes=0;

    if ((dat & 0x80) == 0) {
        retBytes = 1;
    } else if ((dat & 0x20) == 0) {
        retBytes = 2;
    } else if ((dat & 0x10) == 0) {
        retBytes = 3;
    } else if ((dat & 0x8) == 0) {
        retBytes = 4;
    } else if ((dat & 0x4) == 0) {
        retBytes = 5;
    } else if ((dat & 0x2) == 0) {
        retBytes = 6;
    } else {
        retBytes = 0;
    }
    return retBytes;
}

uint8_t ldBaseGetCharInfo(ldFontDict_t *pFontDict, uint8_t *pCharUtf8, int16_t *pAdvWidth, int16_t *pOffsetX, int16_t *pOffsetY, int16_t *pWidth, int16_t *pHeight, uint32_t *pImgAddr)
{
    uint32_t dictCount=0;
    uint8_t retBytes=0;
    ldFontInfo_t *pInfoList;
    dictCount=pFontDict->count;
    pInfoList=(ldFontInfo_t*)pFontDict->pInfoList;
    retBytes=_getUtf8Len(pCharUtf8[0]);
    *pOffsetX=0;
    *pOffsetY=0;
    *pWidth=0;
    *pHeight=0;
    *pImgAddr=0;

    if(retBytes)
    {
        for(uint32_t i=0;i<dictCount;i++)
        {
            if(memcmp(pCharUtf8,pInfoList->utf8,retBytes)==0)
            {
                *pAdvWidth=*(uint16_t*)pInfoList->advW;
                *pOffsetX=*(uint16_t*)pInfoList->offsetX;
                *pOffsetY=*(uint16_t*)pInfoList->offsetY;
                *pWidth=*(uint16_t*)pInfoList->width;
                *pHeight=*(uint16_t*)pInfoList->height;
                *pImgAddr=*((uint32_t*)pInfoList->addr);
                break;
            }
            pInfoList++;
        }
    }
    return retBytes;
}

arm_2d_region_t ldBaseAutoAlign(arm_2d_region_t *pRegion,arm_2d_size_t *pShowSize,uint8_t align)
{
    arm_2d_region_t retRegion;
    switch (align)
    {
    case (LD_ALIGN_TOP|LD_ALIGN_LEFT):
    {
        arm_2d_align_top_left(*pRegion,*pShowSize)
        {
            retRegion=__top_left_region;
        }
        break;
    }
    case (LD_ALIGN_TOP|LD_ALIGN_CENTER):
    {
        arm_2d_align_top_centre(*pRegion,*pShowSize)
        {
            retRegion=__top_centre_region;
        }
        break;
    }
    case (LD_ALIGN_TOP|LD_ALIGN_RIGHT):
    {
        arm_2d_align_top_right(*pRegion,*pShowSize)
        {
            retRegion=__top_right_region;
        }
        break;
    }
    case LD_ALIGN_LEFT:
    {
        arm_2d_align_left(*pRegion,*pShowSize)
        {
            retRegion=__left_region;
        }
        break;
    }
    case LD_ALIGN_CENTER:
    {
        arm_2d_align_centre(*pRegion,*pShowSize)
        {
            retRegion=__centre_region;
        }
        break;
    }
    case LD_ALIGN_RIGHT:
    {
        arm_2d_align_right(*pRegion,*pShowSize)
        {
            retRegion=__right_region;
        }
        break;
    }
    case (LD_ALIGN_BOTTOM|LD_ALIGN_LEFT):
    {
        arm_2d_align_bottom_left(*pRegion,*pShowSize)
        {
            retRegion=__bottom_left_region;
        }
        break;
    }
    case (LD_ALIGN_BOTTOM|LD_ALIGN_CENTER):
    {
        arm_2d_align_bottom_centre(*pRegion,*pShowSize)
        {
            retRegion=__bottom_centre_region;
        }
        break;
    }
    case (LD_ALIGN_BOTTOM|LD_ALIGN_RIGHT):
    {
        arm_2d_align_bottom_right(*pRegion,*pShowSize)
        {
            retRegion=__bottom_right_region;
        }
        break;
    }
    default:
        break;
    }

    retRegion.tLocation.iX-=pRegion->tLocation.iX;
    retRegion.tLocation.iY-=pRegion->tLocation.iY;

    return retRegion;
}

arm_2d_size_t ldBaseGetStringSize(uint8_t* pStr,ldFontDict_t *pFontDict,int16_t *pRetBmpAscender, uint16_t frameWidth)
{
    arm_2d_size_t retSize;
    int16_t advWidth;
    int16_t offsetX;
    int16_t offsetY;
    int16_t width;
    int16_t height;
    uint32_t imgAddr;
    uint16_t len;
    int16_t lineWidth=0;
    int16_t sizeWidthMax=0;
    int16_t sizeHeightMax=pFontDict->lineOffset;
    uint8_t strLen=strlen((char*)pStr);

    for(int i=0;i<strLen;)
    {
        len=ldBaseGetCharInfo(pFontDict,&pStr[i],&advWidth,&offsetX,&offsetY,&width,&height,&imgAddr);
        if((width==0)&&(pStr[i]!=' '))
        {
            switch(pStr[i])
            {
            case '\r':
            {
                sizeWidthMax=MAX(sizeWidthMax,lineWidth);
                lineWidth=0;
                i+=len;
                continue;
                break;
            }
            case '\n':
            {
                sizeWidthMax=MAX(sizeWidthMax,lineWidth);
                lineWidth=0;
                sizeHeightMax+=pFontDict->lineOffset;
                i+=len;
                continue;
                break;
            }
            default:
                break;
            }
        }

        i+=len;

        lineWidth+=advWidth;
        if(((lineWidth+advWidth)>frameWidth)&&(i<strLen))//自动换行
        {
            sizeWidthMax=MAX(sizeWidthMax,lineWidth);
            lineWidth=0;
            sizeHeightMax+=pFontDict->lineOffset;
        }
    }

    if(sizeWidthMax==0)//单行
    {
        retSize.iWidth=lineWidth;
        retSize.iHeight=pFontDict->lineStrHeight;//h1_max-h2_min;

        *pRetBmpAscender=pFontDict->lineStrAscender;//h1_max;
    }
    else
    {
        retSize.iWidth=sizeWidthMax;
        retSize.iHeight=sizeHeightMax;

        *pRetBmpAscender=pFontDict->lineOffset-pFontDict->descender;
    }

    return retSize;
}

void ldBaseShowText(arm_2d_tile_t target,arm_2d_region_t region,ldChar_t *pTextInfo,int16_t scrollOffset,uint8_t opacity)
{
    int16_t advWidth;
    int16_t width;
    int16_t height;
    uint32_t imgAddr;
    uint8_t len;
    int16_t offsetX=0,offsetY=0;
    int16_t textOffsetX=0;
    int16_t textOffsetY=pTextInfo->pFontDict->lineOffset+pTextInfo->pFontDict->descender+scrollOffset;
    int16_t bmpH1Max;
    arm_2d_size_t textSize;

    textSize= ldBaseGetStringSize(pTextInfo->pStr,pTextInfo->pFontDict,&bmpH1Max,region.tSize.iWidth);

    arm_2d_region_t alignSize= ldBaseAutoAlign(&target.tRegion,&textSize,pTextInfo->align);

    if(pTextInfo->align==(LD_ALIGN_TOP|LD_ALIGN_LEFT))
    {
        alignSize.tSize.iHeight=target.tRegion.tSize.iHeight;
    }

#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t fontTile;
#else
    arm_2d_vres_t fontTile;
#endif

    arm_2d_container(&target,showTile,&alignSize)
    {
        for(int i=0;i<pTextInfo->strLen;)
        {
            len=ldBaseGetCharInfo(pTextInfo->pFontDict,&pTextInfo->pStr[i],&advWidth,&offsetX,&offsetY,&width,&height,&imgAddr);
            if((width==0)&&(pTextInfo->pStr[i]!=' '))
            {
                switch(pTextInfo->pStr[i])
                {
                case '\r':
                {
                    textOffsetX=0;
                    break;
                }
                case '\n':
                {
                    textOffsetX=0;
                    textOffsetY+=pTextInfo->pFontDict->lineOffset;
                    break;
                }
                default:
                    break;
                }
                i+=len;
                continue;
            }

#if USE_VIRTUAL_RESOURCE == 0
            fontTile = pTextInfo->fontTile;
            fontTile.tRegion.tSize.iWidth=width;
            fontTile.tRegion.tSize.iHeight=height;
            fontTile.pchBuffer+=imgAddr;
#else
            fontTile = *((arm_2d_vres_t*)&pTextInfo->fontTile);
            fontTile.tTile.tRegion.tSize.iWidth=width;
            fontTile.tTile.tRegion.tSize.iHeight=height;
            fontTile.tTile.pchBuffer=imgAddr;
            fontTile.pTarget=imgAddr;
#endif
            int16_t tempHeight;
            //偏移坐标
            if(textSize.iHeight>pTextInfo->pFontDict->lineOffset)//多行
            {
                if((textOffsetX+advWidth)>textSize.iWidth)//自动换行
                {
                    textOffsetX=0;
                    textOffsetY+=pTextInfo->pFontDict->lineOffset;
                }
                tempHeight=textOffsetY-(height+offsetY);
            }
            else
            {
                tempHeight=bmpH1Max-(height+offsetY);
            }

            arm_2d_tile_t charTile=impl_child_tile(showTile,textOffsetX+offsetX,tempHeight,width,height);

            ldBaseMaskImage(&charTile,(arm_2d_tile_t*)&fontTile,pTextInfo->charColor,opacity);
            arm_2d_op_wait_async(NULL);

            textOffsetX+=advWidth;

            i+=len;
        }
    }
}

void ldBaseCharacter(arm_2d_tile_t* pParentTile,arm_2d_region_t* pShowRegion,arm_2d_tile_t *pResTile,ldColor textColor,uint8_t opacity)
{
#if USE_OPACITY == 0
    ARM_2D_UNUSED(opacity);
#endif
#if LD_DEBUG == 1
    arm_2d_draw_box(pParentTile,pShowRegion,1,textColor,255);
#endif
    switch (pResTile->tInfo.tColourInfo.chScheme)
    {
    case ARM_2D_COLOUR_1BIT:
    {
#if USE_OPACITY == 1
        arm_2d_draw_pattern(pResTile,
                            pParentTile,
                            pShowRegion,
                            ARM_2D_DRW_PATN_MODE_COPY,
                            textColor,
                            LD_COLOR_BLACK);
#else
        arm_2d_draw_pattern(pResTile,
                            pParentTile,
                            pShowRegion,
                            ARM_2D_DRW_PATN_MODE_COPY,
                            textColor,
                            LD_COLOR_BLACK);
#endif

        break;
    }
    case ARM_2D_COLOUR_MASK_A2:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_a2_mask_and_opacity(pParentTile,
                                                    pShowRegion,
                                                    pResTile,
                                                    (__arm_2d_color_t){textColor},
                                                    opacity);
#else
        arm_2d_fill_colour_with_a2_mask(pParentTile,
                                        pShowRegion,
                                        pResTile,
                                        (__arm_2d_color_t){textColor});
#endif
        break;
    }
    case ARM_2D_COLOUR_MASK_A4:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_a4_mask_and_opacity(pParentTile,
                                                    pShowRegion,
                                                    pResTile,
                                                    (__arm_2d_color_t){textColor},
                                                    opacity);
#else
        arm_2d_fill_colour_with_a4_mask(pParentTile,
                                        pShowRegion,
                                        pResTile,
                                        (__arm_2d_color_t){textColor});
#endif
        break;
    }
    case ARM_2D_COLOUR_MASK_A8:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_mask_and_opacity(pParentTile,
                                                 pShowRegion,
                                                 pResTile,
                                                 (__arm_2d_color_t){textColor},
                                                 opacity);
#else
        arm_2d_fill_colour_with_mask(pParentTile,
                                     pShowRegion,
                                     pResTile,
                                     (__arm_2d_color_t){textColor});
#endif
        break;
    }
    default:
        break;
    }
}

arm_2d_region_t ldBaseLineText(arm_2d_tile_t *pTile,arm_2d_tile_t *pResTileTemplate,uint8_t *pStr,ldFontDict_t* pFontDict,uint8_t align,ldColor textColor,int16_t scrollOffset,uint8_t opacity)
{
    int16_t advWidth;
    int16_t width;
    int16_t height;
    uint32_t imgAddr;
    uint8_t len;
    int16_t offsetX=0,offsetY=0;
    int16_t textOffsetX=0;
    int16_t bmpH1Max;

    arm_2d_region_t alignSize={0};
    arm_2d_size_t textSize={0};

    if(align==LD_ALIGN_LEFT_AUTO)
    {
        if(textSize.iWidth>pTile->tRegion.tSize.iWidth)
        {
            align=LD_ALIGN_RIGHT;
        }
        else
        {
            align=LD_ALIGN_LEFT;
        }
    }

    if(pStr==NULL)
    {
        textSize.iHeight=pFontDict->lineStrHeight;
        alignSize= ldBaseAutoAlign(&pTile->tRegion,&textSize,align);
        return alignSize;
    }

    uint8_t strLen=strlen((char*)pStr);

    textSize= ldBaseGetStringSize(pStr,pFontDict,&bmpH1Max,0xFFFF);

    alignSize= ldBaseAutoAlign(&pTile->tRegion,&textSize,align);

#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t resTile = *pResTileTemplate;
#else
    arm_2d_vres_t resTile = *((arm_2d_vres_t*)pResTileTemplate);
#endif

    textOffsetX=0;

    for(int i=0;i<strLen;)
    {
        len=ldBaseGetCharInfo(pFontDict,&pStr[i],&advWidth,&offsetX,&offsetY,&width,&height,&imgAddr);

        if((width==0)&&(pStr[i]!=' '))
        {
            break;
        }

#if USE_VIRTUAL_RESOURCE == 0

        resTile.tRegion.tSize.iWidth=width;
        resTile.tRegion.tSize.iHeight=height;
        resTile.pchBuffer=(uint8_t*)(pFontDict->pFontSrc+imgAddr);
        resTile.tInfo.tColourInfo.chScheme = ldBaseGetChScheme(pFontDict->maskType);
#else
        ((arm_2d_tile_t*)&resTile)->tRegion.tSize.iWidth=width;
        ((arm_2d_tile_t*)&resTile)->tRegion.tSize.iHeight=height;
        ((arm_2d_tile_t*)&resTile)->pchBuffer=imgAddr;
        ((arm_2d_tile_t*)&resTile)->tInfo.tColourInfo.chScheme = ldBaseGetChScheme(pFontDict->maskType);
        resTile.pTarget=imgAddr;
#endif
        int16_t tempHeight;


        tempHeight=bmpH1Max-(height+offsetY);
        ((arm_2d_tile_t*)&resTile)->tRegion.tLocation.iX=0;
        ((arm_2d_tile_t*)&resTile)->tRegion.tLocation.iY=0;

        arm_2d_region_t showRegion;
        showRegion.tLocation.iX=alignSize.tLocation.iX+textOffsetX+offsetX;
        showRegion.tLocation.iY=alignSize.tLocation.iY+tempHeight+scrollOffset;
        showRegion.tSize=((arm_2d_tile_t*)&resTile)->tRegion.tSize;

        ldBaseCharacter(pTile,&showRegion,&resTile,textColor,opacity);

        arm_2d_op_wait_async(NULL);

        textOffsetX+=advWidth;

        i+=len;
    }

    return alignSize;
}

void ldBaseSetHidden(ldCommon_t* pWidget,bool isHidden)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->isHidden=isHidden;
}

void ldBaseSetText(ldChar_t **ppTextInfo,uint8_t *pStr)
{
    uint8_t newStrlen;
    newStrlen=strlen((char*)pStr)+1;

    if(ldBaseCheckText(ppTextInfo))
    {
        if((newStrlen>(*ppTextInfo)->strLen)&&((*ppTextInfo)->pStr!=NULL))
        {
            ldFree((*ppTextInfo)->pStr);
            (*ppTextInfo)->pStr=NULL;
            (*ppTextInfo)->strLen=0;
        }

        if((*ppTextInfo)->pStr==NULL)
        {
            (*ppTextInfo)->pStr=ldCalloc(newStrlen);
            if((*ppTextInfo)->pStr==NULL)
            {
                return;
            }
            (*ppTextInfo)-> strLen=newStrlen-1;
        }
//        memset((char*)(*ppTextInfo)->pStr,0,newStrlen);
        strcpy((char*)(*ppTextInfo)->pStr,(char*)pStr);
    }
}

void ldBaseSetTextColor(ldChar_t **ppTextInfo,ldColor charColor)
{
    if(ldBaseCheckText(ppTextInfo))
    {
        (*ppTextInfo)->charColor=charColor;
    }
}

void ldBaseSetAlign(ldChar_t **ppTextInfo,uint8_t align)
{
    if(ldBaseCheckText(ppTextInfo))
    {
        (*ppTextInfo)->align=align;
    }
}

arm_2d_region_t ldBaseGetGlobalRegion(ldCommon_t *pWidget, arm_2d_region_t *pTargetRegion)
{
    arm_2d_region_t parentRegion={{0,0},{0,0}},outRegion;
    ldPoint_t globalPos;
    bool flag;

    if(pWidget->parentWidget==NULL)
    {
        parentRegion.tSize.iWidth=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iWidth;
        parentRegion.tSize.iHeight=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight;
        globalPos.x=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iX;
        globalPos.y=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iY;
    }
    else
    {
        parentRegion.tSize.iWidth=((arm_2d_tile_t*)&((ldCommon_t*)pWidget->parentWidget)->resource)->tRegion.tSize.iWidth;
        parentRegion.tSize.iHeight=((arm_2d_tile_t*)&((ldCommon_t*)pWidget->parentWidget)->resource)->tRegion.tSize.iHeight;
        globalPos=ldBaseGetGlobalPos((ldCommon_t *)pWidget->parentWidget);
    }
    flag=arm_2d_region_intersect(&parentRegion,pTargetRegion,&outRegion);
    if(flag==false)
    {
        return (arm_2d_region_t){{0,0},{0,0}};
    }
    outRegion.tLocation.iX=outRegion.tLocation.iX+globalPos.x;
    outRegion.tLocation.iY=outRegion.tLocation.iY+globalPos.y;

    return outRegion;
}

void ldBaseMove(ldCommon_t* pWidget,int16_t x,int16_t y)
{
    if (pWidget == NULL)
    {
        return;
    }

    pWidget->dirtyRegionState=waitChange;

    ((arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iX=x;
    ((arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iY=y;
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
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-04
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

void ldBaseAddDirtyRegion(arm_2d_region_list_item_t *pItemDirtyRegionList,arm_2d_region_list_item_t ** ppRootDirtyRegionList)
{
    arm_2d_region_list_item_t **ppTempDirty;

    ppTempDirty=ppRootDirtyRegionList;
    while(*ppTempDirty!=NULL)
    {
        ppTempDirty=&(*ppTempDirty)->ptNext;
    }
    *ppTempDirty=pItemDirtyRegionList;
}

// pNewRegion和pWidget坐标都是相对父控件来计算
void ldBaseDirtyRegionAutoUpdate(ldCommon_t* pWidget,arm_2d_region_t newRegion,bool isAutoIgnore)
{
        switch (pWidget->dirtyRegionState)
        {
        case waitChange://扩大到新范围
        {
            arm_2d_region_t tempRegion;

            arm_2d_region_get_minimal_enclosure(&newRegion,&pWidget->dirtyRegionTemp,&tempRegion);
            pWidget->dirtyRegionListItem.tRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&tempRegion);
            pWidget->dirtyRegionListItem.bIgnore=false;
            pWidget->dirtyRegionListItem.bUpdated=true;
            pWidget->dirtyRegionTemp=newRegion;
            pWidget->dirtyRegionState=waitUpdate;
            break;
        }
        case waitUpdate://缩小到新范围
        {
            pWidget->dirtyRegionListItem.tRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pWidget->dirtyRegionTemp);
            pWidget->dirtyRegionListItem.bIgnore=false;
            pWidget->dirtyRegionListItem.bUpdated=true;
            pWidget->dirtyRegionState=none;

            if(isAutoIgnore&&(pWidget->dirtyRegionListItem.bIgnore==false))
            {
                pWidget->dirtyRegionListItem.bIgnore=true;
            }
            break;
        }
        default:
            break;
        }
}

static void ldGuiUpdateDirtyRegion(xListNode* pLink)
{
    xListNode *temp_pos,*safePos;

    list_for_each_safe(temp_pos,safePos, pLink)
    {
        if(temp_pos->info!=NULL)
        {
            ((ldCommon_t *)temp_pos->info)->dirtyRegionState=waitChange;

            if(((ldCommon_t *)temp_pos->info)->childList!=NULL)
            {
                ldGuiUpdateDirtyRegion(((ldCommon_t *)temp_pos->info)->childList);
            }
        }
    }
}

void ldBaseBgMove(int16_t x,int16_t y)
{
    ldCommon_t *pWidget=ldBaseGetWidgetById(0);
    ldBaseMove(pWidget,x,y);

    //只考虑左移和上移
    ((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iWidth=LD_CFG_SCEEN_WIDTH-x;
    ((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight=LD_CFG_SCEEN_HEIGHT-y;

    ldGuiUpdateDirtyRegion(&ldWidgetLink);

    pWidget->dirtyRegionState=waitChange;
    pWidget->isDirtyRegionAutoIgnore=true;
}


arm_2d_region_t ldLayoutHorizontal(arm_2d_region_t *pWidgetRegion,arm_2d_region_t *pBufferRegion,int16_t width,int16_t height,int16_t leftSpace,int16_t rightSpace,int16_t topSpace,int16_t bottomSpace)
{
    arm_2d_region_t retRegion;
    //换行判断
    if((pBufferRegion->tLocation.iX+leftSpace+width)>pWidgetRegion->tSize.iWidth)
    {
        pBufferRegion->tLocation.iX=0;
        pBufferRegion->tLocation.iY+=pBufferRegion->tSize.iHeight;
        pBufferRegion->tSize.iHeight=0;
        pBufferRegion->tSize.iWidth=0;
    }
    retRegion=*pBufferRegion;
    retRegion.tLocation.iX+=leftSpace;
    retRegion.tLocation.iY+=topSpace;
    retRegion.tSize.iWidth=width;
    retRegion.tSize.iHeight=height;
    //高度最大值判断
    if(pBufferRegion->tSize.iHeight<(topSpace+height+bottomSpace))
    {
        pBufferRegion->tSize.iHeight=topSpace+height+bottomSpace;
    }
    //准备下一次使用的起始坐标
    pBufferRegion->tLocation.iX+=leftSpace+width+rightSpace;

    return retRegion;
}

arm_2d_region_t ldLayoutVertical(arm_2d_region_t *pWidgetRegion,arm_2d_region_t *pBufferRegion,int16_t width,int16_t height,int16_t leftSpace,int16_t rightSpace,int16_t topSpace,int16_t bottomSpace)
{
    arm_2d_region_t retRegion;
    //换列判断
    if((pBufferRegion->tLocation.iY+topSpace+height)>pWidgetRegion->tSize.iHeight)
    {
        pBufferRegion->tLocation.iX+=pBufferRegion->tSize.iWidth;
        pBufferRegion->tLocation.iY=0;
        pBufferRegion->tSize.iHeight=0;
        pBufferRegion->tSize.iWidth=0;
    }
    retRegion=*pBufferRegion;
    retRegion.tLocation.iX+=leftSpace;
    retRegion.tLocation.iY+=topSpace;
    retRegion.tSize.iWidth=width;
    retRegion.tSize.iHeight=height;
    //宽度最大值判断
    if(pBufferRegion->tSize.iWidth<(leftSpace+width+rightSpace))
    {
        pBufferRegion->tSize.iWidth=leftSpace+width+rightSpace;
    }
    //准备下一次使用的起始坐标
    pBufferRegion->tLocation.iY+=topSpace+height+bottomSpace;
    return retRegion;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
