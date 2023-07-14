#include "ldButton.h"
#include "xBtnAction.h"
#include "xConnect.h"
#include "xLog.h"


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

static bool _buttonDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldFree(((ldButton_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldButtonDel(ldButton_t *widget)
{
    xListNode *listInfo;

    if (widget == NULL)
    {
        return;
    }

    if(widget->widgetType!=widgetTypeButton)
    {
        return;
    }

    // 查找父链表
    listInfo = ldGetWidgetInfoById(((ldCommon_t *)widget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, widget, _buttonDel);
    }
}

static bool slotButtonToggle(xConnectInfo_t info)
{
    ldButton_t *btn;

    btn=ldGetWidgetById(info.receiverId);

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


ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height)
{
    ldButton_t * pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList;

    parentInfo = ldGetWidgetInfoById(parentNameId);

    pNewWidget = LD_MALLOC_WIDGET_INFO(ldButton_t);
    
    
    if (pNewWidget != NULL)
    {
        pNewWidget->isParentHidden=false;
        
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
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

        xListInfoAdd(parentList, pNewWidget);
            
        pNewWidget->parentType = ((ldCommon_t *)(parentInfo->info))->widgetType;
        pNewWidget->parentWidget = parentInfo->info;
        
        pNewWidget->isCheckable=false;
        pNewWidget->isChecked=false;
        pNewWidget->isPressed=false;
        pNewWidget->isSelected=false;
            
        pNewWidget->releaseColor = __RGB(0,0,255);
        pNewWidget->pressColor = __RGB(255,255,0);
        pNewWidget->selectColor = __RGB(255,0,0);
        pNewWidget->charColor=0;
        pNewWidget->keyValue=0;
        pNewWidget->releaseImgAddr=0;
        pNewWidget->pressImgAddr=0;
        pNewWidget->selectMaskAddr=0;

        pNewWidget->resource.tInfo.bIsRoot = true;
        pNewWidget->resource.tInfo.bHasEnforcedColour = true;
        pNewWidget->resource.tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        pNewWidget->resource.pchBuffer = (uint8_t *)0;
#if USE_VIRTUAL_RESOURCE == 1
        pNewWidget->tReleaseImgRes.tInfo.bVirtualResource = true;
        
        ((arm_2d_vres_t*)(&pNewWidget->tReleaseImgRes))->pTarget=releaseImgAddr;
        ((arm_2d_vres_t*)(&pNewWidget->tReleaseImgRes))->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)(&pNewWidget->tReleaseImgRes))->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
        
        pNewWidget->isWithMask = false;
        pNewWidget->isTransparent=false;
        pNewWidget->isHidden = false;

        //动作
        xConnect(nameId,BTN_PRESS,nameId,slotButtonToggle);
        xConnect(nameId,BTN_RELEASE,nameId,slotButtonToggle);

        LOG_INFO("new button id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_INFO("create failed, button id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldButtonSetColor(ldButton_t* widget,ldColor releaseColor,ldColor pressColor)
{
    widget->releaseColor=releaseColor;
    widget->pressColor=pressColor;
    widget->releaseImgAddr=0;
    widget->pressImgAddr=0;
}

void ldButtonSetImage(ldButton_t* widget,uint32_t releaseImgAddr,uint32_t pressImgAddr,bool isWithMask)
{
    if(widget==NULL)
    {
        return;
    }
    widget->releaseImgAddr=releaseImgAddr;
    widget->pressImgAddr=pressImgAddr;
    widget->isWithMask=isWithMask;
}

void ldButtonSetSelectImage(ldButton_t* widget,uint32_t selectMaskAddr,ldColor selectColor)
{
    if(widget==NULL)
    {
        return;
    }
    widget->selectMaskAddr=selectMaskAddr;
    widget->selectColor=selectColor;
}

void ldButtonSetCharColor(ldButton_t* widget,ldColor charColor)
{
    if(widget==NULL)
    {
        return;
    }
    widget->charColor=charColor;
}

void ldButtonSetTransparent(ldButton_t* widget,bool isTransparent)
{
    if(widget==NULL)
    {
        return;
    }
    widget->isTransparent=isTransparent;
}

void ldButtonSetHidden(ldButton_t* widget,bool isHidden)
{
    if(widget==NULL)
    {
        return;
    }
    widget->isHidden=isHidden;
}

void ldButtonLoop(ldButton_t *widget,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    uint32_t btnColor;
    if (widget == NULL)
    {
        return;
    }
    
    if((widget->isParentHidden)||(widget->isHidden)||(widget->isTransparent))
    {
        return;
    }
    
    arm_2d_container(ptParent,tTarget , &widget->resource.tRegion)
    {
        tTarget.tRegion.tLocation = widget->resource.tRegion.tLocation;
        
        if(!widget->isTransparent)
        {
            if ((widget->releaseImgAddr==0)&&(widget->pressImgAddr==0))//color
            {
                if(widget->isPressed)
                {
                    btnColor=widget->pressColor;
                }
                else
                {
                    btnColor=widget->releaseColor;
                }

                ldBaseColor(&tTarget,btnColor,255);
            }
            else
            {
                if(widget->isPressed)
                {
                    widget->resource.pchBuffer = (uint8_t *)widget->pressImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)(&widget->tResource))->pTarget=widget->pressImgAddr;
#endif
                }
                else
                {
                    widget->resource.pchBuffer = (uint8_t *)widget->releaseImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)(&widget->tResource))->pTarget=widget->releaseImgAddr;
#endif
                }
                ldBaseImage(&tTarget,&widget->resource,widget->isWithMask,255);
            }
            if(widget->isSelected)
            {
                if ((widget->releaseImgAddr==0)&&(widget->pressImgAddr==0))//color
                {

                }
                else
                {
                    arm_2d_vres_t maskRes;
                    maskRes.tTile=widget->resource;
                    maskRes.tTile.tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A8;
                    ((arm_2d_tile_t*)&maskRes)->pchBuffer = (uint8_t *)widget->selectMaskAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    maskRes.pTarget=widget->selectMaskAddr;
                    maskRes.Load = &__disp_adapter0_vres_asset_loader;
                    maskRes.Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
                    ldBaseMaskImage(&tTarget,(arm_2d_tile_t*)&maskRes,widget->selectColor,255);
                }
            }
        }
    }
    arm_2d_op_wait_async(NULL);
}






#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

