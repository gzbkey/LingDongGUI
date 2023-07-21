#include "ldCommon.h"
#include "xList.h"
#include <stdarg.h>
#include "ldWindow.h"
#include "ldImage.h"
#include "ldButton.h"
#include "ldConfig.h"

#include "tlsf.h"

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
static uint8_t tlsfMemBuf[TLSF_MEM_SIZE];
#endif

__WEAK void *ldMalloc(uint32_t size)
{
#if USE_TLSF == 1
    if(pTlsfMem==NULL)
    {
        pTlsfMem = tlsf_create_with_pool((void *)tlsfMemBuf, TLSF_MEM_SIZE);
    }
    void* p=tlsf_malloc(pTlsfMem,size);
    memset(p,0,size);
    return p;

#else
    void* p=malloc(size);
    memset(p,0,size);
    return p;
#endif
}

__WEAK void ldFree(void *p)
{
#if USE_TLSF == 1
    tlsf_free(pTlsfMem, p);
#else
    free(p);
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
                widthT=pTempWidgetInfo->resource.tRegion.tSize.iWidth;
                heightT=pTempWidgetInfo->resource.tRegion.tSize.iHeight;
                    
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

    pos.x=widget->resource.tRegion.tLocation.iX;
    pos.y=widget->resource.tRegion.tLocation.iY;

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
        posParent.x=((ldCommon_t*)widget->parentWidget)->resource.tRegion.tLocation.iX;
        posParent.y=((ldCommon_t*)widget->parentWidget)->resource.tRegion.tLocation.iY;
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

/*
    widgetTypeNone,
    widgetTypeBackground,
    widgetTypeWindow,
    widgetTypeButton,
    widgetTypeImage,
    widgetTypeText,
    widgetTypeLineEdit,
    widgetTypeDashboardSimple,
    widgetTypeGraph,
    widgetTypeCheckBox,
    widgetTypeSlider,
    widgetTypeProgressBar,
    widgetTypeGauge,
    widgetTypeQRCode,
    widgetTypeDateTime,
    widgetTypeIconSlider,
    widgetTypeComboBox,
    widgetTypeIconRotateAround,
    widgetTypeNumber,
    widgetTypeStaticText,
    widgetTypeStartupImage,
    widgetTypeArc,
*/
void ldDelWidget(ldCommon_t *widget)
{
    switch(widget->widgetType)
    {
        case widgetTypeWindow:
        {
            ldWindowDel((ldWindow_t*)widget);
            break;
        }
        case widgetTypeImage:
        {
            ldImageDel((ldImage_t*)widget);
            break;
        }
        case widgetTypeButton:
        {
            ldButtonDel((ldButton_t*)widget);
            break;
        }
        default:
            break;
    }
}

void ldBaseColor(arm_2d_tile_t* ptTile,ldColor color,uint8_t opacity)
{
#if USE_OPACITY == 1
    arm_2d_fill_colour_with_opacity(ptTile, NULL,(__arm_2d_color_t)color, opacity);
#else
    arm_2d_fill_colour(ptTile, NULL, color);
#endif
}

void ldBaseImage(arm_2d_tile_t* ptTile,arm_2d_tile_t resource,bool isWithMask,uint8_t opacity)
{
    //root tile init
    resource.tRegion.tLocation.iX=0;
    resource.tRegion.tLocation.iY=0;
    if (isWithMask)
    {
        arm_2d_tile_t maskTile;
        maskTile = resource;
        maskTile.tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_8BIT;
        maskTile.pchBuffer += maskTile.tRegion.tSize.iWidth * maskTile.tRegion.tSize.iHeight * 2;

        arm_2d_tile_copy_with_src_mask_only((arm_2d_tile_t *)&resource,
                                            (arm_2d_tile_t *)&maskTile,
                                            ptTile,
                                            NULL);
    }
    else
    {
        switch ((resource).tInfo.tColourInfo.chScheme)
        {
        case ARM_2D_COLOUR_RGB565:
        case ARM_2D_COLOUR_CCCA8888:
        {
#if USE_OPACITY == 1
            arm_2d_tile_copy_with_opacity(&resource,
                                          ptTile,
                                          NULL,
                                          opacity);

#else
            arm_2d_tile_copy_only(&resource,
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

void ldBaseMaskImage(arm_2d_tile_t* ptTile,arm_2d_tile_t resource,ldColor textColor,uint8_t opacity)
{
    //root tile init
    resource.tRegion.tLocation.iX=0;
    resource.tRegion.tLocation.iY=0;

    switch (resource.tInfo.tColourInfo.chScheme)
    {
    case ARM_2D_COLOUR_1BIT:
    {
#if USE_OPACITY == 1
        arm_2d_draw_pattern(&resource,
                            ptTile,
                            &resource.tRegion,
                            ARM_2D_DRW_PATN_MODE_COPY,
                            textColor,
                            GLCD_COLOR_BLACK);
#else
        arm_2d_draw_pattern(&resource,
                            ptTile,
                            &resource.tRegion,
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
                                                    &resource.tRegion,
                                                    &resource,
                                                    (__arm_2d_color_t){textColor},
                                                    opacity);
#else
        arm_2d_fill_colour_with_a2_mask(ptTile,
                                        &resource.tRegion,
                                        &resource,
                                        (__arm_2d_color_t){textColor});
#endif
        break;
    }
    case ARM_2D_COLOUR_MASK_A4:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_a4_mask_and_opacity(ptTile,
                                                    &resource.tRegion,
                                                    &resource,
                                                    (__arm_2d_color_t){textColor},
                                                    opacity);
#else
        arm_2d_fill_colour_with_a4_mask(ptTile,
                                        &resource.tRegion,
                                        &resource,
                                        (__arm_2d_color_t){textColor});
#endif
        break;
    }
    case ARM_2D_COLOUR_MASK_A8:
    {
#if USE_OPACITY == 1
        arm_2d_fill_colour_with_mask_and_opacity(ptTile,
                                                 &resource.tRegion,
                                                 &resource,
                                                 (__arm_2d_color_t){textColor},
                                                 opacity);
#else
        arm_2d_fill_colour_with_mask(ptTile,
                                     &resource.tRegion,
                                     &resource,
                                     (__arm_2d_color_t){textColor});
#endif
        break;
    }
    default:
        break;
    }
}

void ldBaseSetTextInfo(arm_2d_tile_t* ptTile,ldChar_t *ptCharInfo,uint8_t opacity)
{
    arm_2d_canvas(ptTile,newCavas)
    {
        switch (ptCharInfo->align)
        {
        case (LD_ALIGN_TOP|LD_ALIGN_LEFT):
        {
            arm_2d_align_top_left(newCavas,arm_lcd_get_string_line_box((char*)ptCharInfo->pStr,ptCharInfo->ptFont))
            {
                arm_lcd_text_set_draw_region(&__top_left_region);
            }
            break;
        }
        case (LD_ALIGN_TOP|LD_ALIGN_CENTER):
        {
            arm_2d_align_top_centre(newCavas,arm_lcd_get_string_line_box((char*)ptCharInfo->pStr,ptCharInfo->ptFont))
            {
                arm_lcd_text_set_draw_region(&__top_centre_region);
            }
            break;
        }
        case (LD_ALIGN_TOP|LD_ALIGN_RIGHT):
        {
            arm_2d_align_top_right(newCavas,arm_lcd_get_string_line_box((char*)ptCharInfo->pStr,ptCharInfo->ptFont))
            {
                arm_lcd_text_set_draw_region(&__top_right_region);
            }
            break;
        }
        case LD_ALIGN_LEFT:
        {
            arm_2d_align_left(newCavas,arm_lcd_get_string_line_box((char*)ptCharInfo->pStr,ptCharInfo->ptFont))
            {
                arm_lcd_text_set_draw_region(&__left_region);
            }
            break;
        }
        case LD_ALIGN_CENTER:
        {
            arm_2d_align_centre(newCavas,arm_lcd_get_string_line_box((char*)ptCharInfo->pStr,ptCharInfo->ptFont))
            {
                arm_lcd_text_set_draw_region(&__centre_region);
            }
            break;
        }
        case LD_ALIGN_RIGHT:
        {
            arm_2d_align_right(newCavas,arm_lcd_get_string_line_box((char*)ptCharInfo->pStr,ptCharInfo->ptFont))
            {
                arm_lcd_text_set_draw_region(&__right_region);
            }
            break;
        }
        case (LD_ALIGN_BOTTOM|LD_ALIGN_LEFT):
        {
            arm_2d_align_bottom_left(newCavas,arm_lcd_get_string_line_box((char*)ptCharInfo->pStr,ptCharInfo->ptFont))
            {
                arm_lcd_text_set_draw_region(&__bottom_left_region);
            }
            break;
        }
        case (LD_ALIGN_BOTTOM|LD_ALIGN_CENTER):
        {
            arm_2d_align_bottom_centre(newCavas,arm_lcd_get_string_line_box((char*)ptCharInfo->pStr,ptCharInfo->ptFont))
            {
                arm_lcd_text_set_draw_region(&__bottom_centre_region);
            }
            break;
        }
        case (LD_ALIGN_BOTTOM|LD_ALIGN_RIGHT):
        {
            arm_2d_align_bottom_right(newCavas,arm_lcd_get_string_line_box((char*)ptCharInfo->pStr,ptCharInfo->ptFont))
            {
                arm_lcd_text_set_draw_region(&__bottom_right_region);
            }
            break;
        }
        default:
            break;
        }
            arm_lcd_text_set_target_framebuffer(ptTile);
            arm_lcd_text_set_font(ptCharInfo->ptFont);
            arm_lcd_text_set_colour(ptCharInfo->charColor, GLCD_COLOR_WHITE);
            arm_lcd_text_set_opacity(opacity);

            if(ptCharInfo->pStr!=NULL)
            {
                arm_lcd_puts((char*)ptCharInfo->pStr);
            }
    }
}

#if defined(__IS_COMPILER_IAR__) && __IS_COMPILER_IAR__
#define __va_list    va_list

#endif

int ldBaseSetText(const char *format, ...)
{
    int real_size;
    static char s_chBuffer[__LCD_PRINTF_CFG_TEXT_BUFFER_SIZE__ + 1];
    __va_list ap;
    va_start(ap, format);
        real_size = vsnprintf(s_chBuffer, sizeof(s_chBuffer)-1, format, ap);
    va_end(ap);
    real_size = MIN(sizeof(s_chBuffer)-1, real_size);
    s_chBuffer[real_size] = '\0';
    arm_lcd_puts(s_chBuffer);
    return real_size;
}

void ldBaseTextDel(ldChar_t *charInfo)
{
    ldFree(charInfo->ptFont);
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
    }
    return *charInfo;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
