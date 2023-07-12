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

void pImageDel(ldImage_t *widget)
{
    xListNode *listInfo;

    //    if(widget->widgetType!=widgetTypeImage)
    //    {
    //        return;
    //    }

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

ldImage_t *ldImageInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, bool isColor, ldColor bgColor, uint32_t imageAddr, bool isWithMask, bool isHidden)
{
    ldImage_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parent_link;
    uint32_t jpegAddr, jpegSize;

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
        pNewWidget->resource.tRegion.tLocation.iX=x;
        pNewWidget->resource.tRegion.tLocation.iY=y;
        pNewWidget->resource.tRegion.tSize.iWidth=width;
        pNewWidget->resource.tRegion.tSize.iHeight=height;

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

//        pNewWidget->resource.tRegion.tSize.iWidth = width;
//        pNewWidget->resource.tRegion.tSize.iHeight = height;
        pNewWidget->resource.tInfo.bIsRoot = true;
        pNewWidget->resource.tInfo.bHasEnforcedColour = true;
        pNewWidget->resource.tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        pNewWidget->resource.pchBuffer = (uint8_t *)imageAddr;
#if USE_VIRTUAL_RESOURCE == 1
        pNewWidget->resource.pchBuffer=0;
        pNewWidget->resource.tInfo.bVirtualResource = true;
        
        ((arm_2d_vres_t*)(&pNewWidget->resource))->pTarget=imageAddr;
        ((arm_2d_vres_t*)(&pNewWidget->resource))->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)(&pNewWidget->resource))->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
        
//        pNewWidget->isHwDec = isHwDec;
        pNewWidget->bgColor = bgColor;
        pNewWidget->isColor = isColor;
        pNewWidget->isWithMask = isWithMask;
        pNewWidget->isHidden = isHidden;
#if USE_IMAGE_OPACITY == 1
        pNewWidget->opacity = 255;
#endif
    }
    else
    {
        ldFree(pNewWidget);
    }

    return pNewWidget;
}

//jpeg硬解,更新resource地址
__WEAK
void ldImageHwDec(ldImage_t *info)
{
    
}

void ldImageLoop(ldImage_t *info, const arm_2d_tile_t *ptParent, bool bIsNewFrame)
{
#if USE_IMAGE_OPACITY == 1
#define IMG_OPACITY        info->opacity
#else
#define IMG_OPACITY        255
#endif

    assert(NULL != ptParent);

    if (info == NULL)
    {
        return;
    }
    
    if((info->isParentHidden)||(info->isHidden)||(info->isTransparent))
    {
        return;
    }
    
    arm_2d_container(ptParent,tTarget , &info->resource.tRegion)
    {
        tTarget.tRegion.tLocation = info->resource.tRegion.tLocation;

        if (info->isColor)
        {
            ldBaseColor(&tTarget,info->bgColor,IMG_OPACITY);
        }
        else
        {
            ldBaseImage(&tTarget,&info->resource,info->isWithMask,IMG_OPACITY);
        }
    }
    arm_2d_op_wait_async(NULL);
}

void pImageSetOpacity(ldImage_t *info, uint8_t opacity)
{
    if (info == NULL)
    {
        return;
    }
#if USE_IMAGE_OPACITY == 1
    info->opacity=opacity;
#endif
}

void nImageSetOpacity(uint16_t nameId, uint8_t opacity)
{
    xListNode *pListInfo;
    pListInfo = ldGetWidgetInfoById(nameId);
    pImageSetOpacity(pListInfo->info, opacity);
}

void pImageSetAddr(ldImage_t *info, uint32_t addr)
{
    if (info == NULL)
    {
        return;
    }
    if(info->resource.tInfo.bVirtualResource)
    {
        ((arm_2d_vres_t*)(&info->resource))->pTarget=addr;
    }
    else
    {
        info->resource.pchBuffer = (uint8_t *)addr;
    }
}

void nImageSetAddr(uint16_t nameId, uint32_t addr)
{
    xListNode *pListInfo;
    pListInfo = ldGetWidgetInfoById(nameId);
    pImageSetAddr(pListInfo->info, addr);
}

// grayBit :1 2 4 8
void pImageSetGrayscale(ldImage_t *info, uint8_t grayBit, ldColor writeColor)
{
    uint8_t colorType=0;
    if (info == NULL)
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
    
    info->resource.tColourInfo.chScheme = colorType;
    info->specialColor = writeColor;
}

// grayBit :1 2 4 8
void nImageSetGrayscale(uint16_t nameId, uint8_t grayBit, ldColor writeColor)
{
    xListNode *pListInfo;
    pListInfo = ldGetWidgetInfoById(nameId);
    pImageSetGrayscale(pListInfo->info, grayBit, writeColor);
}



#if defined(__clang__)
#pragma clang diagnostic pop
#endif
