#include "ldImage.h"

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

static bool _imageDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldFree(((ldImage_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldImageDel(ldImage_t *widget)
{
    xListNode *listInfo;

    if (widget == NULL)
    {
        return;
    }

    if((widget->widgetType!=widgetTypeImage)||(widget->widgetType!=widgetTypeWindow))
    {
        return;
    }

    // 查找父链表
    if ((ldCommon_t *)widget->parentType == widgetTypeNone)
    {
        listInfo = &ldWidgetLink;
    }
    else
    {
        listInfo = ldGetWidgetInfoById(((ldCommon_t *)widget->parentWidget)->nameId);
        listInfo = ((ldCommon_t *)listInfo->info)->childList;
    }

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, widget, _imageDel);
    }
}

ldImage_t *ldImageInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint32_t imageAddr, bool isWithMask)
{
    ldImage_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parent_link;
    arm_2d_tile_t *tResTile;

    parentInfo = ldGetWidgetInfoById(parentNameId);

    pNewWidget = LD_MALLOC_WIDGET_INFO(ldImage_t);
    
    if (pNewWidget != NULL)
    {
        pNewWidget->isParentHidden=false;
        
        if (parentInfo)
        {
            parent_link = ((ldCommon_t *)parentInfo->info)->childList;
            if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
            {
                pNewWidget->isParentHidden=true;
            }
        }
        else
        {
            parent_link = &ldWidgetLink;
        
        }
        
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;

        pNewWidget->widgetType = widgetTypeImage;

        xListInfoAdd(parent_link, pNewWidget);
        if (parent_link == &ldWidgetLink) // 自身为bg
        {
            pNewWidget->parentWidget = NULL;
            pNewWidget->parentType = widgetTypeNone;
        }
        else
        {
            pNewWidget->parentType = ((ldCommon_t *)(parentInfo->info))->widgetType;
            pNewWidget->parentWidget = parentInfo->info;
        }
        pNewWidget->bgColor = 0;
        pNewWidget->isColor = false;
        pNewWidget->isWithMask = isWithMask;
        pNewWidget->isHidden = false;
        pNewWidget->isTransparent=false;
#if USE_OPACITY == 1
        pNewWidget->opacity = 255;
#endif

        tResTile=(arm_2d_tile_t*)&pNewWidget->resource;

        tResTile->tRegion.tLocation.iX=x;
        tResTile->tRegion.tLocation.iY=y;
        tResTile->tRegion.tSize.iWidth=width;
        tResTile->tRegion.tSize.iHeight=height;

        tResTile->tInfo.bIsRoot = true;
        tResTile->tInfo.bHasEnforcedColour = true;
        tResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        tResTile->pchBuffer = (uint8_t *)imageAddr;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        
        ((arm_2d_vres_t*)tResTile)->pTarget=imageAddr;
        ((arm_2d_vres_t*)tResTile)->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)tResTile)->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
    }
    else
    {
        ldFree(pNewWidget);
    }

    return pNewWidget;
}

void ldImageSetBgColor(ldImage_t *widget,ldColor bgColor)
{
    if (widget == NULL)
    {
        return;
    }
    widget->isTransparent=false;
    widget->isColor=true;
    widget->bgColor=bgColor;
}

void ldImageSetHidden(ldImage_t *widget,bool isHidden)
{
    if (widget == NULL)
    {
        return;
    }
    widget->isHidden=isHidden;
}

void ldImageLoop(ldImage_t *widget, const arm_2d_tile_t *ptParent, bool bIsNewFrame)
{
#if USE_OPACITY == 1
#define IMG_OPACITY        widget->opacity
#else
#define IMG_OPACITY        255
#endif

    arm_2d_tile_t *tResTile=(arm_2d_tile_t*)&widget->resource;

    if (widget == NULL)
    {
        return;
    }
    
    if((widget->isParentHidden)||(widget->isHidden)||(widget->isTransparent))
    {
        return;
    }
    
    arm_2d_container(ptParent,tTarget , &tResTile->tRegion)
    {
        tTarget.tRegion.tLocation = tResTile->tRegion.tLocation;

        if (widget->isColor)
        {
            ldBaseColor(&tTarget,widget->bgColor,IMG_OPACITY);
        }
        else
        {
            ldBaseImage(&tTarget,(arm_2d_tile_t*)(&widget->resource),widget->isWithMask,IMG_OPACITY);
        }
    }
    arm_2d_op_wait_async(NULL);
}

void ldImageSetOpacity(ldImage_t *widget, uint8_t opacity)
{
    if (widget == NULL)
    {
        return;
    }
#if USE_OPACITY == 1
    widget->opacity=opacity;
#endif
}

void ldImageSetImage(ldImage_t *widget, uint32_t imageAddr, bool isWithMask)
{
    if (widget == NULL)
    {
        return;
    }
    widget->isTransparent=false;
    widget->isWithMask=isWithMask;
    if(((arm_2d_tile_t*)(&widget->resource))->tInfo.bVirtualResource)
    {
        ((arm_2d_vres_t*)(&widget->resource))->pTarget=imageAddr;
    }
    else
    {
        ((arm_2d_tile_t*)(&widget->resource))->pchBuffer = (uint8_t *)imageAddr;
    }
}

// grayBit :1 2 4 8
void ldImageSetGrayscale(ldImage_t *widget, uint8_t grayBit, ldColor writeColor)
{
    uint8_t colorType=0;
    if (widget == NULL)
    {
        return;
    }
    
    switch(grayBit)
    {
        case 1:
        {
            colorType=ARM_2D_COLOUR_1BIT;
            break;
        }
        case 2:
        {
            colorType=ARM_2D_COLOUR_MASK_A2;
            break;
        }
        case 4:
        {
            colorType=ARM_2D_COLOUR_MASK_A4;
            break;
        }
        case 8:
        {
            colorType=ARM_2D_COLOUR_MASK_A8;
            break;
        }
        default:
        {
            return ;
        }
    }
    widget->isTransparent=false;
    ((arm_2d_tile_t*)(&widget->resource))->tColourInfo.chScheme = colorType;
    widget->specialColor = writeColor;
}



#if defined(__clang__)
#pragma clang diagnostic pop
#endif
