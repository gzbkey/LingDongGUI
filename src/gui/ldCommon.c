#include "ldCommon.h"
#include "xList.h"
#include <stdarg.h>
#include "ldConfig.h"
#include "ldWindow.h"
#include "ldImage.h"
#include "ldButton.h"
#include "ldText.h"
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
__attribute__((aligned (8))) static uint8_t tlsfMemBuf[LD_MEM_SIZE];
#endif
extern size_t xFreeBytesRemaining;
__WEAK void *ldMalloc(uint32_t size)
{
#if USE_TLSF == 1
    if(pTlsfMem==NULL)
    {
        pTlsfMem = tlsf_create_with_pool((void *)tlsfMemBuf, sizeof (tlsfMemBuf));
    }
    void* p=tlsf_malloc(pTlsfMem,size);
    if(p!=NULL)
    {
        memset(p,0,size);
    }
    return p;
#else
//    LOG_DEBUG("malloc:%d\n",size);
//    LOG_DEBUG("before :%llu\n",xFreeBytesRemaining);
    return pvPortMalloc(size);
//    void* p=malloc(size);
//    memset(p,0,size);
//    return p;
#endif
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
//    LOG_DEBUG("free\n");
//    LOG_DEBUG("remaining:%llu\n",xFreeBytesRemaining);
//    free(p);
#endif
}

__WEAK void *ldRealloc(void *ptr,uint32_t newSize)
{
#if USE_TLSF == 1
    return tlsf_realloc(pTlsfMem, ptr, newSize);
#else
    return realloc(ptr,newSize);
#endif
}

static bool ldGetInfoByName(xListNode *inList,xListNode ** out_info,uint16_t nameId)
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
                    if(ldGetInfoByName(pTempWidgetInfo->childList,out_info,nameId)==true)
                    {
                        return true;
                    }
                }

            }
        }
    }
    return false;
}


xListNode* ldGetWidgetInfoById(uint16_t nameId)
{
    xListNode * pListInfo;
    if(ldGetInfoByName(&ldWidgetLink,&pListInfo,nameId)==true)
    {
        return pListInfo;
    }
    return NULL;
}

void* ldGetWidgetById(uint16_t nameId)
{
    xListNode * pListInfo;
    if(ldGetInfoByName(&ldWidgetLink,&pListInfo,nameId)==true)
    {
        return pListInfo->info;
    }
    return NULL;
}

static bool ldGetInfoByPos(xListNode *inList,xListNode ** out_info,int16_t x,int16_t y)
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
                posT= ldGetGlobalPos(pTempWidgetInfo);
                widthT=(*(arm_2d_tile_t*)&(pTempWidgetInfo->resource)).tRegion.tSize.iWidth;
                heightT=(*(arm_2d_tile_t*)&(pTempWidgetInfo->resource)).tRegion.tSize.iHeight;
                    
                if(pTempWidgetInfo->childList==NULL)//无子控件
                {
                    if((x>posT.x)&&(x<(posT.x+widthT))&&(y>posT.y)&&(y<(posT.y+heightT)))
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
                            if(ldGetInfoByPos(pTempWidgetInfo->childList,&pListInfo2,x,y)==true)
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


xListNode* ldGetWidgetInfoByPos(int16_t x,int16_t y)
{
    xListNode * pListInfo;
    if(ldGetInfoByPos(&ldWidgetLink,&pListInfo,x,y)==true)
    {
        return pListInfo;
    }
    return NULL;
}

//获取全局坐标
ldPoint_t ldGetGlobalPos(ldCommon_t *widget)
{
    ldPoint_t pos={0,0},posParent;

    if(widget==NULL)
    {
        return pos;
    }

    pos.x=(*(arm_2d_tile_t*)(&widget->resource)).tRegion.tLocation.iX;
    pos.y=(*(arm_2d_tile_t*)(&widget->resource)).tRegion.tLocation.iY;

    posParent.x=0;
    posParent.y=0;

    if(widget->parentType!=widgetTypeNone)
    {
        if(widget->parentType!=widgetTypeWindow)
        {
        posParent=ldGetGlobalPos(widget->parentWidget);
    }
    else
    {
        posParent.x=(*(arm_2d_tile_t*)&(((ldCommon_t*)widget->parentWidget)->resource)).tRegion.tLocation.iX;
        posParent.y=(*(arm_2d_tile_t*)&(((ldCommon_t*)widget->parentWidget)->resource)).tRegion.tLocation.iY;
    }
	}
        
    pos.x+=posParent.x;
    pos.y+=posParent.y;

    return pos;
}


ARM_NONNULL(2)
bool ldTimeOut(uint16_t ms, int64_t *plTimer,bool isReset)
{
    int64_t lPeriod;
    int64_t lTimestamp = arm_2d_helper_get_system_timestamp();

    if(1==*plTimer)
    {
        return false;
    }
    
    lPeriod=arm_2d_helper_convert_ms_to_ticks(ms);
    if (0 == *plTimer) {
        *plTimer = lPeriod;
        *plTimer += lTimestamp;
        
        return false;
    }

    if (lTimestamp >= *plTimer)
    {
        if(isReset)
        {
            *plTimer = lPeriod + lTimestamp;
        }
        else
        {
            *plTimer =1;
        }
            return true;
    }
    return false;
}

void ldBaseColor(arm_2d_tile_t* ptTile,ldColor color,uint8_t opacity)
{
#if USE_OPACITY == 1
    arm_2d_fill_colour_with_opacity(ptTile, NULL,(__arm_2d_color_t)color, opacity);
#else
    arm_2d_fill_colour(ptTile, NULL, color);
#endif
}

void ldBaseImage(arm_2d_tile_t* ptTile,arm_2d_tile_t* ptResource,bool isWithMask,uint8_t opacity)
{
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t resource=*ptResource;
#else
    arm_2d_vres_t resource =*((arm_2d_vres_t*)ptResource);
#endif
    //root tile init
    (*(arm_2d_tile_t*)(&resource)).tRegion.tLocation.iX=0;
    (*(arm_2d_tile_t*)(&resource)).tRegion.tLocation.iY=0;

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
        arm_2d_tile_copy_with_src_mask_only((arm_2d_tile_t *)&resource,
                                            (arm_2d_tile_t *)&maskTile,
                                            ptTile,
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
                                          ptTile,
                                          NULL,
                                          opacity);

#else
            arm_2d_tile_copy_only((arm_2d_tile_t*)(&resource),
                                  ptTile,
                                  NULL);
#endif
            break;
        }
            
        default:
            break;
        }
    }
}

void ldBaseMaskImage(arm_2d_tile_t* ptTile,arm_2d_tile_t *ptResource,ldColor textColor,uint8_t opacity)
{
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t resource=*ptResource;
    arm_2d_tile_t *pRes;
#else
    arm_2d_vres_t resource =*((arm_2d_vres_t*)ptResource);
    arm_2d_vres_t *pRes;
#endif
    pRes=&resource;
    //root tile init
    ((arm_2d_tile_t*)pRes)->tRegion.tLocation.iX=0;
    ((arm_2d_tile_t*)pRes)->tRegion.tLocation.iY=0;

    switch (((arm_2d_tile_t*)pRes)->tInfo.tColourInfo.chScheme)
    {
    case ARM_2D_COLOUR_1BIT:
    {
#if USE_OPACITY == 1
        arm_2d_draw_pattern((arm_2d_tile_t*)pRes,
                            ptTile,
                            &((arm_2d_tile_t*)pRes)->tRegion,
                            ARM_2D_DRW_PATN_MODE_COPY,
                            textColor,
                            GLCD_COLOR_BLACK);
#else
        arm_2d_draw_pattern((arm_2d_tile_t*)pRes,
                            ptTile,
                            &(((arm_2d_tile_t*)pRes)->tRegion),
                            ARM_2D_DRW_PATN_MODE_COPY,
                            textColor,
                            GLCD_COLOR_BLACK);
#endif

        break;
    }
    case ARM_2D_COLOUR_MASK_A2:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_a2_mask_and_opacity(ptTile,
                                                    &((arm_2d_tile_t*)pRes)->tRegion,
                                                    (arm_2d_tile_t*)pRes,
                                                    (__arm_2d_color_t){textColor},
                                                    opacity);
#else
        arm_2d_fill_colour_with_a2_mask(ptTile,
                                        &(((arm_2d_tile_t*)pRes)->tRegion),
                                        (arm_2d_tile_t*)pRes,
                                        (__arm_2d_color_t){textColor});
#endif
        break;
    }
    case ARM_2D_COLOUR_MASK_A4:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_a4_mask_and_opacity(ptTile,
                                                    &((arm_2d_tile_t*)pRes)->tRegion,
                                                    (arm_2d_tile_t*)pRes,
                                                    (__arm_2d_color_t){textColor},
                                                    opacity);
#else
        arm_2d_fill_colour_with_a4_mask(ptTile,
                                        &(((arm_2d_tile_t*)pRes)->tRegion),
                                        (arm_2d_tile_t*)pRes,
                                        (__arm_2d_color_t){textColor});
#endif
        break;
    }
    case ARM_2D_COLOUR_MASK_A8:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_mask_and_opacity(ptTile,
                                                 &((arm_2d_tile_t*)pRes)->tRegion,
                                                 (arm_2d_tile_t*)pRes,
                                                 (__arm_2d_color_t){textColor},
                                                 opacity);
#else
        arm_2d_fill_colour_with_mask(ptTile,
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

void ldBaseTextDel(ldChar_t *charInfo)
{
    ldFree(charInfo->pStr);
    ldFree(charInfo);
}

ldChar_t * ldBaseCheckText(ldChar_t **charInfo)
{
    if(*charInfo==NULL)
    {
        *charInfo=ldMalloc(sizeof(ldChar_t));
        if(*charInfo==NULL)
        {
            return NULL;
        }
        (*charInfo)->pStr=NULL;
        (*(arm_2d_tile_t*)(&(*charInfo)->tFontTile)).tInfo.bIsRoot=true;
        (*(arm_2d_tile_t*)(&(*charInfo)->tFontTile)).tInfo.bHasEnforcedColour=true;
        (*(arm_2d_tile_t*)(&(*charInfo)->tFontTile)).tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A8;
#if USE_VIRTUAL_RESOURCE == 1
        (*(arm_2d_tile_t*)(&(*charInfo)->tFontTile)).bVirtualResource=true;
        (*charInfo)->tFontTile.pTarget=0;
        (*charInfo)->tFontTile.Load = &__disp_adapter0_vres_asset_loader;
        (*charInfo)->tFontTile.Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
    }
    return *charInfo;
}

void ldBaseSetFont(ldChar_t **pptCharInfo,ldFontDict_t* ptFontDict)
{
    if(ldBaseCheckText(pptCharInfo))
    {

    (*(arm_2d_tile_t*)(&(*pptCharInfo)->tFontTile)).pchBuffer=(uint8_t *)ptFontDict->pFontSrc;
#if USE_VIRTUAL_RESOURCE == 1
    (*pptCharInfo)->tFontTile.pTarget=ptFontDict->pFontSrc;
#endif
    (*pptCharInfo)->ptFontDict=ptFontDict;

    switch (ptFontDict->maskType)
    {
    case 1:
    {
        (*(arm_2d_tile_t*)(&(*pptCharInfo)->tFontTile)).tColourInfo.chScheme=ARM_2D_COLOUR_1BIT;
        break;
    }
    case 2:
    {
        (*(arm_2d_tile_t*)(&(*pptCharInfo)->tFontTile)).tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A2;
        break;
    }
    case 4:
    {
        (*(arm_2d_tile_t*)(&(*pptCharInfo)->tFontTile)).tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A4;
        break;
    }
    case 8:
    {
        (*(arm_2d_tile_t*)(&(*pptCharInfo)->tFontTile)).tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A8;
        break;
    }
    default:
    {
        (*(arm_2d_tile_t*)(&(*pptCharInfo)->tFontTile)).tColourInfo.chScheme=ARM_2D_COLOUR_1BIT;
    }
    }
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

uint8_t ldBaseGetCharInfo(ldFontDict_t *ptFontDict,uint8_t *charUtf8,int16_t *advWidth,int16_t *offsetX,int16_t *offsetY,int16_t *width,int16_t *height,uint32_t *imgAddr)
{
    uint32_t dictCount=0;
    uint8_t retBytes=0;
    ldFontInfo_t *pInfoList;
    dictCount=ptFontDict->count;
    pInfoList=ptFontDict->pInfoList;

    retBytes=_getUtf8Len(charUtf8[0]);

    *offsetX=0;
    *offsetY=0;
    *width=0;
    *height=0;
    *imgAddr=0;

    if(retBytes)
    {
        for(uint32_t i=0;i<dictCount;i++)
        {
            if(memcmp(charUtf8,pInfoList->utf8,retBytes)==0)
            {
                *advWidth=*(uint16_t*)pInfoList->advW;
                *offsetX=*(uint16_t*)pInfoList->offsetX;
                *offsetY=*(uint16_t*)pInfoList->offsetY;
                *width=*(uint16_t*)pInfoList->width;
                *height=*(uint16_t*)pInfoList->height;
                *imgAddr=*((uint32_t*)pInfoList->addr);
                break;
            }
            pInfoList++;
        }
    }
    return retBytes;
}

arm_2d_region_t ldBaseGetAlignRegion(arm_2d_tile_t *ptTile,arm_2d_size_t size,uint8_t align)
{
    arm_2d_region_t retRegion;

    arm_2d_canvas(ptTile,newCavas)
        {
            switch (align)
            {
            case (LD_ALIGN_TOP|LD_ALIGN_LEFT):
            {
                arm_2d_align_top_left(newCavas,size)
                {
                    retRegion=__top_left_region;
                }
                break;
            }
            case (LD_ALIGN_TOP|LD_ALIGN_CENTER):
            {
                arm_2d_align_top_centre(newCavas,size)
                {
                    retRegion=__top_centre_region;
                }
                break;
            }
            case (LD_ALIGN_TOP|LD_ALIGN_RIGHT):
            {
                arm_2d_align_top_right(newCavas,size)
                {
                    retRegion=__top_right_region;
                }
                break;
            }
            case LD_ALIGN_LEFT:
            {
                arm_2d_align_left(newCavas,size)
                {
                    retRegion=__left_region;
                }
                break;
            }
            case LD_ALIGN_CENTER:
            {
                arm_2d_align_centre(newCavas,size)
                {
                    retRegion=__centre_region;
                }
                break;
            }
            case LD_ALIGN_RIGHT:
            {
                arm_2d_align_right(newCavas,size)
                {
                    retRegion=__right_region;
                }
                break;
            }
            case (LD_ALIGN_BOTTOM|LD_ALIGN_LEFT):
            {
                arm_2d_align_bottom_left(newCavas,size)
                {
                    retRegion=__bottom_left_region;
                }
                break;
            }
            case (LD_ALIGN_BOTTOM|LD_ALIGN_CENTER):
            {
                arm_2d_align_bottom_centre(newCavas,size)
                {
                    retRegion=__bottom_centre_region;
                }
                break;
            }
            case (LD_ALIGN_BOTTOM|LD_ALIGN_RIGHT):
            {
                arm_2d_align_bottom_right(newCavas,size)
                {
                    retRegion=__bottom_right_region;
                }
                break;
            }
            default:
                break;
            }
        }
//    retRegion.tLocation.iX+=ptTile->tRegion.tLocation.iX;
//    retRegion.tLocation.iY+=ptTile->tRegion.tLocation.iY;
    return retRegion;
}

arm_2d_size_t ldBaseGetStringSize(ldChar_t *ptTextInfo,int16_t *bmpAscender,uint16_t frameWidth)
{
    arm_2d_size_t retSize;
    int16_t advWidth;
    int16_t offsetX;
    int16_t offsetY;
    int16_t width;
    int16_t height;
    uint32_t imgAddr;
    uint16_t len;

    int16_t h1_max=0;
    int16_t h2_min=0;
    int16_t lineWidth=0;
    int16_t sizeWidthMax=0;
    int16_t sizeHeightMax=ptTextInfo->ptFontDict->lineOffset;

    for(int i=0;i<ptTextInfo->strLen;)
    {
        len=ldBaseGetCharInfo(ptTextInfo->ptFontDict,&ptTextInfo->pStr[i],&advWidth,&offsetX,&offsetY,&width,&height,&imgAddr);
        if(width==0)
        {
        switch(ptTextInfo->pStr[i])
        {
        case '\r':
        {
            sizeWidthMax=MAX(sizeWidthMax,lineWidth);
            lineWidth=0;
            break;
        }
        case '\n':
        {
            sizeWidthMax=MAX(sizeWidthMax,lineWidth);
            lineWidth=0;
            sizeHeightMax+=ptTextInfo->ptFontDict->lineOffset;
            break;
        }
            default:
                break;
        }
        i+=len;
        continue;
        }

        i+=len;

        lineWidth+=advWidth;
        h1_max=MAX(h1_max,height+offsetY);
        h2_min=MIN(h2_min,offsetY);


        if(((lineWidth+advWidth)>frameWidth)&&(i<ptTextInfo->strLen))//自动换行
        {
            sizeWidthMax=MAX(sizeWidthMax,lineWidth);
            lineWidth=0;
            sizeHeightMax+=ptTextInfo->ptFontDict->lineOffset;
        }


    }

    if(sizeWidthMax==0)//单行
    {
        retSize.iWidth=lineWidth;
        retSize.iHeight=h1_max-h2_min;

        *bmpAscender=h1_max;
    }
    else
    {
        retSize.iWidth=sizeWidthMax;
        retSize.iHeight=sizeHeightMax;

        *bmpAscender=ptTextInfo->ptFontDict->lineOffset-ptTextInfo->ptFontDict->descender;
    }
    return retSize;
}




void ldBaseShowText(arm_2d_tile_t tTile,ldChar_t *ptTextInfo,int16_t scrollOffset)
{
    int16_t advWidth;
    int16_t width;
    int16_t height;
    uint32_t imgAddr;
    uint8_t len;
    int16_t offsetX=0,offsetY=0;
    int16_t textOffsetX=0;
    int16_t textOffsetY=ptTextInfo->ptFontDict->lineOffset+ptTextInfo->ptFontDict->descender+scrollOffset;
    int16_t bmpH1Max;

    arm_2d_size_t textSize;

    textSize= ldBaseGetStringSize(ptTextInfo,&bmpH1Max,tTile.tRegion.tSize.iWidth);

    arm_2d_region_t alignSize= ldBaseGetAlignRegion(&tTile,textSize,ptTextInfo->align);

    if(ptTextInfo->align==(LD_ALIGN_TOP|LD_ALIGN_LEFT))
    {
        alignSize.tSize.iHeight=tTile.tRegion.tSize.iHeight;
    }

#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t fontTile;
#else
    arm_2d_vres_t fontTile;
#endif

    arm_2d_container(&tTile,showTile,&alignSize)
    {
        for(int i=0;i<ptTextInfo->strLen;)
        {
            len=ldBaseGetCharInfo(ptTextInfo->ptFontDict,&ptTextInfo->pStr[i],&advWidth,&offsetX,&offsetY,&width,&height,&imgAddr);
            if(width==0)
            {
                switch(ptTextInfo->pStr[i])
                {
                case '\r':
                {
                    textOffsetX=0;
                    break;
                }
                case '\n':
                {
                    textOffsetX=0;
                    textOffsetY+=ptTextInfo->ptFontDict->lineOffset;
                    break;
                }
                default:
                    break;
                }
                i+=len;
                continue;
            }

#if USE_VIRTUAL_RESOURCE == 0
            fontTile = ptTextInfo->tFontTile;
            fontTile.tRegion.tSize.iWidth=width;
            fontTile.tRegion.tSize.iHeight=height;
            fontTile.pchBuffer+=imgAddr;
#else
            fontTile = *((arm_2d_vres_t*)&ptTextInfo->tFontTile);
            fontTile.tTile.tRegion.tSize.iWidth=width;
            fontTile.tTile.tRegion.tSize.iHeight=height;
            fontTile.tTile.pchBuffer=imgAddr;
            fontTile.pTarget=imgAddr;
#endif
            int16_t tempHeight;
            //偏移坐标
            if(textSize.iHeight>ptTextInfo->ptFontDict->lineOffset)//多行
            {
                if((textOffsetX+advWidth)>textSize.iWidth)//自动换行
                {
                    textOffsetX=0;
                    textOffsetY+=ptTextInfo->ptFontDict->lineOffset;
                }
                tempHeight=textOffsetY-(height+offsetY);
            }
            else
            {
                tempHeight=bmpH1Max-(height+offsetY);
            }

            arm_2d_tile_t charTile=impl_child_tile(showTile,textOffsetX+offsetX,tempHeight,width,height);

            ldBaseMaskImage(&charTile,(arm_2d_tile_t*)&fontTile,ptTextInfo->charColor,255);
            arm_2d_op_wait_async(NULL);

            textOffsetX+=advWidth;

            i+=len;
        }
    }
}

void ldBaseSetHidden(ldCommon_t* widget,bool isHidden)
{
    if(widget==NULL)
    {
        return;
    }
    widget->isHidden=isHidden;
}

void ldBaseSetText(ldChar_t **pptTextInfo,uint8_t *pStr)
{
    uint8_t newStrlen;
    newStrlen=strlen((char*)pStr);

    if(ldBaseCheckText(pptTextInfo))
    {
        if((newStrlen>(*pptTextInfo)->strLen)&&((*pptTextInfo)->pStr!=NULL))
        {
            ldFree((*pptTextInfo)->pStr);
            (*pptTextInfo)->pStr=NULL;
            (*pptTextInfo)->strLen=0;
        }

        if((*pptTextInfo)->pStr==NULL)
        {
            (*pptTextInfo)->pStr=ldMalloc(newStrlen);
            if((*pptTextInfo)->pStr==NULL)
            {
                return;
            }
            (*pptTextInfo)-> strLen=newStrlen;
        }

        strcpy((char*)(*pptTextInfo)->pStr,(char*)pStr);
    }
}

void ldBaseSetTextColor(ldChar_t **pptTextInfo,ldColor charColor)
{
    if(ldBaseCheckText(pptTextInfo))
    {
        (*pptTextInfo)->charColor=charColor;
    }
}

void ldBaseSetAlign(ldChar_t **pptTextInfo,uint8_t align)
{
    if(ldBaseCheckText(pptTextInfo))
    {
        (*pptTextInfo)->align=align;
    }
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
