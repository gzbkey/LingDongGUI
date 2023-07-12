#include "ldButton.h"
#include "xBtnAction.h"
#include "xConnect.h"



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

static bool slotButtonToggle(xConnectInfo_t info)
{
    ldButton_t *btn;

    btn=ldGetWidgetInfoById(info.receiverId)->info;

    if(info.signalType==BTN_PRESS)
    {
        btn->isPressed=true;
    }
    if(info.signalType==BTN_RELEASE)
    {
        btn->isPressed=false;
    }

    return false;
}


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
        
            parent_link = ((ldCommon_t *)parentInfo->info)->childList;
            if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
            {
                pNewWidget->isParentHidden=true;
            }

        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;

        pNewWidget->widgetType = widgetTypeButton;
        pNewWidget->resource.tRegion.tLocation.iX=x;
        pNewWidget->resource.tRegion.tLocation.iY=y;
        pNewWidget->resource.tRegion.tSize.iWidth=width;
        pNewWidget->resource.tRegion.tSize.iHeight=height;

        xListInfoAdd(parent_link, pNewWidget);
            
        pNewWidget->parentType = ((ldCommon_t *)(parentInfo->info))->widgetType;
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
        pNewWidget->releaseImgAddr=releaseImgAddr;
        pNewWidget->pressImgAddr=pressImgAddr;
        pNewWidget->selectMaskAddr=selectMaskAddr;

        pNewWidget->resource.tInfo.bIsRoot = true;
        pNewWidget->resource.tInfo.bHasEnforcedColour = true;
        pNewWidget->resource.tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        pNewWidget->resource.pchBuffer = (uint8_t *)releaseImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
        pNewWidget->tReleaseImgRes.tInfo.bVirtualResource = true;
        
        ((arm_2d_vres_t*)(&pNewWidget->tReleaseImgRes))->pTarget=releaseImgAddr;
        ((arm_2d_vres_t*)(&pNewWidget->tReleaseImgRes))->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)(&pNewWidget->tReleaseImgRes))->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
        
        pNewWidget->isWithMask = isWithMask;
        pNewWidget->isTransparent=isTransparent;
        pNewWidget->isHidden = isHidden;
#if USE_IMAGE_OPACITY == 1
        pNewWidget->opacity = 255;
#endif
        //¶¯×÷
        xConnect(nameId,BTN_PRESS,nameId,slotButtonToggle);
        xConnect(nameId,BTN_RELEASE,nameId,slotButtonToggle);

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
    
    arm_2d_container(ptParent,tTarget , &info->resource.tRegion)
    {
        tTarget.tRegion.tLocation = info->resource.tRegion.tLocation;
        
        if(!info->isTransparent)
        {
        if ((info->releaseImgAddr==0)&&(info->pressImgAddr==0))//color
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
//                info->tResource.pchBuffer = (uint8_t *)info->selectMaskAddr;
//#if USE_VIRTUAL_RESOURCE == 1
//                ((arm_2d_vres_t*)(&info->tResource))->pTarget=info->selectMaskAddr;
//#endif
//                ldBaseImage(&tTarget,&info->tResource,info->isWithMask,255);
            }
            else
            {
                if(info->isPressed)
                {
                    info->resource.pchBuffer = (uint8_t *)info->pressImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)(&info->tResource))->pTarget=info->pressImgAddr;
#endif
                }
                else
                {
                    info->resource.pchBuffer = (uint8_t *)info->releaseImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)(&info->tResource))->pTarget=info->releaseImgAddr;
#endif
                }
                ldBaseImage(&tTarget,&info->resource,info->isWithMask,255);
            }
        }
    }
    }
    arm_2d_op_wait_async(NULL);
}






#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

