#include "ldCommon.h"
#include "xList.h"
#include "ldWindow.h"
#include "ldImage.h"
#include <stdarg.h>

NEW_LIST(ldWidgetLink);

__WEAK void *ldMalloc(uint32_t size)
{
    return malloc(size);
}

__WEAK void ldFree(void *p)
{
    free(p);
}

__WEAK void *ldRealloc(void *ptr,uint32_t newSize)
{
    return realloc(ptr,newSize);
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
            pWindowDel((ldWindow_t*)widget);
            break;
        }
        case widgetTypeImage:
        {
            pImageDel((ldImage_t*)widget);
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

void ldBaseImage(arm_2d_tile_t* ptTile,arm_2d_tile_t *resource,bool isWithMask,uint8_t opacity)
{
    if (isWithMask)
        {
            arm_2d_tile_t srcTile, maskTile;

            srcTile = *resource;
            srcTile.tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
            srcTile.pchBuffer += srcTile.tRegion.tSize.iWidth * srcTile.tRegion.tSize.iHeight;

            maskTile = *resource;
            maskTile.tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_8BIT;

            arm_2d_tile_copy_with_src_mask_only((arm_2d_tile_t *)&srcTile,
                                                (arm_2d_tile_t *)&maskTile,
                                                ptTile,
                                                NULL);
        }
        else
        {
            arm_2d_region_t tDrawRegion =
                {
                    .tLocation =
                        {
                            .iX = 0,
                            .iY = 0,
                        },
                    .tSize = (*resource).tRegion.tSize,
                };

            switch ((*resource).tInfo.tColourInfo.chScheme)
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
                arm_2d_tile_copy_only(resource,
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

void ldBaseTextImage(arm_2d_tile_t* ptTile,arm_2d_tile_t resource,ldColor textColor,uint8_t opacity)
{
            arm_2d_region_t tDrawRegion =
                {
                    .tLocation =
                        {
                            .iX = 0,
                            .iY = 0,
                        },
                    .tSize = resource.tRegion.tSize,
                };

            switch (resource.tInfo.tColourInfo.chScheme)
            {
            case ARM_2D_COLOUR_1BIT:
            {
#if USE_OPACITY == 1
                arm_2d_draw_pattern(&resource,
                                    ptTile,
                                    &tDrawRegion,
                                    ARM_2D_DRW_PATN_MODE_COPY,
                                    textColor,
                                    GLCD_COLOR_BLACK);
#else
                arm_2d_draw_pattern(&resource,
                                    ptTile,
                                    &tDrawRegion,
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
                                                            &tDrawRegion,
                                                            &resource,
                                                            (__arm_2d_color_t){textColor},
                                                            opacity);
#else
                arm_2d_fill_colour_with_a2_mask(ptTile,
                                                &tDrawRegion,
                                               &resource,
                                               (__arm_2d_color_t){textColor});
#endif
                break;
            }
            case ARM_2D_COLOUR_MASK_A4:
            {
#if USE_OPACITY == 1
                arm_2d_fill_colour_with_a4_mask_and_opacity(ptTile,
                                                            &tDrawRegion,
                                                            &resource,
                                                            (__arm_2d_color_t){textColor},
                                                            opacity);
#else
                arm_2d_fill_colour_with_a4_mask(ptTile,
                                                &tDrawRegion,
                                               &resource,
                                               (__arm_2d_color_t){textColor});
#endif
                break;
            }
            case ARM_2D_COLOUR_MASK_A8:
            {
#if USE_OPACITY == 1
                arm_2d_fill_colour_with_mask_and_opacity(ptTile,
                                                         &tDrawRegion,
                                                         (arm_2d_tile_t *)&info->resource,
                                                         (__arm_2d_color_t){textColor},
                                                         opacity);
#else
                arm_2d_fill_colour_with_mask(ptTile,
                                             &tDrawRegion,
                                            &resource,
                                            (__arm_2d_color_t){textColor});
#endif
                break;
            }
            
            
            default:
                break;
            }
}

void ldBaseSetTextInfo(arm_2d_tile_t* ptTile,const arm_2d_font_t *ptFont,ldColor textColor,uint8_t opacity)
{
    
    arm_lcd_text_set_draw_region(&ptTile->tRegion);
    arm_lcd_text_set_font(ptFont);
    arm_lcd_text_set_colour(textColor, GLCD_COLOR_WHITE);
    arm_lcd_text_set_opacity(opacity);

    arm_lcd_text_location(0,0);
        
    arm_lcd_puts("123");
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

//ldBaseSetTextInfo(&tTarget,&ARM_2D_FONT_6x8.use_as__arm_2d_font_t,GLCD_COLOR_RED,64);
//        ldBaseSetText("123");

