#include "ldButton.h"

//#include "global.h"
//#include "jfif_parser.h"

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

//static bool _windowDel(xListNode* pEachInfo,void* pTarget)
//{
//    ldDelWidget(pEachInfo->info);
//    return false;
//}

//void pWindowDel(ldWindow *widget)
//{
//    xListNode *listInfo;
//    
//    if(widget->widgetType!=widgetTypeWindow)
//    {
//        return;
//    }
//    listInfo=ldGetWidgetInfoById(widget->nameId);
//    
//    if(listInfo!=NULL)
//    {
//        xListInfoPrevTraverse(widget->childList,NULL,_windowDel);
//        xListFreeNode(widget->childList);
//        
//        pImageDel((ldImage *)widget);
//        
//    }
//}

ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height,ldColor releaseColor,ldColor pressColor,ldColor selectColor,uint32_t releaseImgAddr,uint32_t pressImgAddr,uint32_t selectMaskAddr,ldColor charColor,bool isWithMask,bool isTransparent,bool isHidden)
{
    ldButton_t * pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parent_link;
    uint32_t jpegAddr, jpegSize;
    

    parentInfo = ldGetWidgetInfoById(parentNameId);

    pNewWidget = LD_MALLOC_WIDGET_INFO(ldButton_t);
    
    
    if (pNewWidget != NULL)
    {
        pNewWidget->isParentHidden=false;
        
            parent_link = ((ldCommon *)parentInfo->info)->childList;
            if(((ldCommon *)parentInfo->info)->isHidden||((ldCommon *)parentInfo->info)->isParentHidden)
            {
                pNewWidget->isParentHidden=true;
            }

        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;

        pNewWidget->widgetType = widgetTypeButton;
        pNewWidget->geometry.x = x;
        pNewWidget->geometry.y = y;
        pNewWidget->geometry.width = width;
        pNewWidget->geometry.height = height;

        xListInfoAdd(parent_link, pNewWidget);
            
        pNewWidget->parentType = ((ldCommon *)(parentInfo->info))->widgetType;
        pNewWidget->parentWidget = parentInfo->info;
        

         pNewWidget->isCheckable=false;
            pNewWidget->isChecked=false;
            pNewWidget->isPressed=false;
            pNewWidget->isSelected=false;
            
        pNewWidget->releaseColor = releaseColor;
        pNewWidget->pressColor = pressColor;
        pNewWidget->selectColor = selectColor;
        pNewWidget->charColor=charColor;
        pNewWidget->keyValue=0;
        
            
        pNewWidget->tReleaseImgRes.tRegion.tSize.iWidth = width;
        pNewWidget->tReleaseImgRes.tRegion.tSize.iHeight = height;
        pNewWidget->tReleaseImgRes.tInfo.bIsRoot = true;
        pNewWidget->tReleaseImgRes.tInfo.bHasEnforcedColour = true;
        pNewWidget->tReleaseImgRes.tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        pNewWidget->tReleaseImgRes.pchBuffer = (uint8_t *)releaseImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
//        pNewWidget->tReleaseImgRes.pchBuffer=0;
        pNewWidget->tReleaseImgRes.tInfo.bVirtualResource = true;
        
        ((arm_2d_vres_t*)(&pNewWidget->tReleaseImgRes))->pTarget=releaseImgAddr;
        ((arm_2d_vres_t*)(&pNewWidget->tReleaseImgRes))->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)(&pNewWidget->tReleaseImgRes))->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif

        pNewWidget->tPressImgRes.tRegion.tSize.iWidth = width;
        pNewWidget->tPressImgRes.tRegion.tSize.iHeight = height;
        pNewWidget->tPressImgRes.tInfo.bIsRoot = true;
        pNewWidget->tPressImgRes.tInfo.bHasEnforcedColour = true;
        pNewWidget->tPressImgRes.tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        pNewWidget->tPressImgRes.pchBuffer = (uint8_t *)pressImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
//        pNewWidget->tPressImgRes.pchBuffer=0;
        pNewWidget->tPressImgRes.tInfo.bVirtualResource = true;
        
        ((arm_2d_vres_t*)(&pNewWidget->tPressImgRes))->pTarget=pressImgAddr;
        ((arm_2d_vres_t*)(&pNewWidget->tPressImgRes))->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)(&pNewWidget->tPressImgRes))->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif

        pNewWidget->tSelectMaskRes.tRegion.tSize.iWidth = width;
        pNewWidget->tSelectMaskRes.tRegion.tSize.iHeight = height;
        pNewWidget->tSelectMaskRes.tInfo.bIsRoot = true;
        pNewWidget->tSelectMaskRes.tInfo.bHasEnforcedColour = true;
        pNewWidget->tSelectMaskRes.tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        pNewWidget->tSelectMaskRes.pchBuffer = (uint8_t *)selectMaskAddr;
#if USE_VIRTUAL_RESOURCE == 1
//        pNewWidget->tSelectMaskRes.pchBuffer=0;
        pNewWidget->tSelectMaskRes.tInfo.bVirtualResource = true;
        
        ((arm_2d_vres_t*)(&pNewWidget->tSelectMaskRes))->pTarget=selectMaskAddr;
        ((arm_2d_vres_t*)(&pNewWidget->tSelectMaskRes))->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)(&pNewWidget->tSelectMaskRes))->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
        
        pNewWidget->isWithMask = isWithMask;
        pNewWidget->isTransparent=isTransparent;
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

void ldButtonLoop(ldButton_t *info,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    assert(NULL != ptParent);

    if (info == NULL)
    {
        return;
    }
    
    if((info->isParentHidden)||(info->isHidden)||(info->isTransparent))
    {
        return;
    }
    
    arm_2d_container(ptParent,tTarget , &info->tReleaseImgRes.tRegion)
    {
        tTarget.tRegion.tLocation.iX = info->geometry.x;
        tTarget.tRegion.tLocation.iY = info->geometry.y;
        
        if(!info->isTransparent)
        {
        if ((info->tReleaseImgRes.pchBuffer==0)&&(info->tPressImgRes.pchBuffer==0))//color
        {
            if(info->isSelected)
            {
                ldBaseColor(&tTarget,info->selectColor,255);
            }
            else
            {
                if(info->isPressed)
                {
                    ldBaseColor(&tTarget,info->pressColor,255);
                }
                else
                {
                    ldBaseColor(&tTarget,info->releaseColor,255);
                }
            }
        }
        else
        {
            if(info->isSelected)
            {
                ldBaseImage(&tTarget,&info->tSelectMaskRes,info->isWithMask,255);
            }
            else
            {
                if(info->isPressed)
                {
                    ldBaseImage(&tTarget,&info->tPressImgRes,info->isWithMask,255);
                }
                else
                {
                    ldBaseImage(&tTarget,&info->tReleaseImgRes,info->isWithMask,255);
                }
            }
        }
    }
    }
    arm_2d_op_wait_async(NULL);
}






#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

