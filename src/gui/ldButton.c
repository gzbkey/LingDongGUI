#include "ldButton.h"


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
        if(((ldButton_t*)pTarget)->ptTextInfo!=NULL)
        {
            ldBaseTextDel(((ldButton_t*)pTarget)->ptTextInfo);
        }
        ldFree(((ldButton_t*)pTarget));
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

    LOG_DEBUG("[button] del,id:%d\n",widget->nameId);

    xDeleteConnect(widget->nameId);

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
    arm_2d_tile_t *tResTile;

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
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentType = ((ldCommon_t *)(parentInfo->info))->widgetType;
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isCheckable=false;
        pNewWidget->isChecked=false;
        pNewWidget->isPressed=false;
        pNewWidget->isSelected=false;
        pNewWidget->releaseColor = __RGB(217,225,244);
        pNewWidget->pressColor = __RGB(255,243,202);
        pNewWidget->selectColor = __RGB(255,0,0);
        pNewWidget->keyValue=0;
        pNewWidget->releaseImgAddr=0;
        pNewWidget->pressImgAddr=0;
        pNewWidget->selectMaskAddr=0;
        pNewWidget->isReleaseMask = false;
        pNewWidget->isPressMask = false;
        pNewWidget->isTransparent=false;
        pNewWidget->isHidden = false;
        pNewWidget->ptTextInfo = NULL;
        tResTile=(arm_2d_tile_t*)&pNewWidget->resource;
        tResTile->tRegion.tLocation.iX=x;
        tResTile->tRegion.tLocation.iY=y;
        tResTile->tRegion.tSize.iWidth=width;
        tResTile->tRegion.tSize.iHeight=height;
        tResTile->tInfo.bIsRoot = true;
        tResTile->tInfo.bHasEnforcedColour = true;
        tResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        tResTile->pchBuffer = 0;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        
        ((arm_2d_vres_t*)tResTile)->pTarget=0;
        ((arm_2d_vres_t*)tResTile)->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)tResTile)->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
        //动作
        xConnect(nameId,BTN_PRESS,nameId,slotButtonToggle);
        xConnect(nameId,BTN_RELEASE,nameId,slotButtonToggle);

        LOG_INFO("[button] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[button] init failed,id:%d\n",nameId);
    }
    return pNewWidget;
}

void ldButtonSetText(ldButton_t* widget,uint8_t *pStr)
{
    if(widget==NULL)
    {
        return;
    }
    ldBaseSetText(&widget->ptTextInfo,pStr);
}

void ldButtonSetColor(ldButton_t* widget,ldColor releaseColor,ldColor pressColor)
{
    widget->releaseColor=releaseColor;
    widget->pressColor=pressColor;
    widget->releaseImgAddr=0;
    widget->pressImgAddr=0;
}

void ldButtonSetImage(ldButton_t* widget,uint32_t releaseImgAddr,bool isReleaseMask,uint32_t pressImgAddr,bool isPressMask)
{
    if(widget==NULL)
    {
        return;
    }
    widget->releaseImgAddr=releaseImgAddr;
    widget->pressImgAddr=pressImgAddr;
    widget->isReleaseMask=isReleaseMask;
    widget->isPressMask=isPressMask;
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

void ldButtonSetTextColor(ldButton_t* widget,ldColor charColor)
{
    if(widget==NULL)
    {
        return;
    }
    ldBaseSetTextColor(&widget->ptTextInfo,charColor);
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
    ldBaseSetHidden((ldCommon_t*) widget,isHidden);
}

void ldButtonSetRoundCorner(ldButton_t* widget,bool isCorner)
{
    if(widget==NULL)
    {
        return;
    }
    widget->isCorner=isCorner;
}

void ldButtonSetSelect(ldButton_t* widget,bool isSelected)
{
    if(widget==NULL)
    {
        return;
    }
    widget->isSelected=isSelected;
}

void ldButtonLoop(ldButton_t *widget,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    uint32_t btnColor;
    arm_2d_tile_t *ptResTile=(arm_2d_tile_t*)&widget->resource;
    arm_2d_region_t tRegion;

    if (widget == NULL)
    {
        return;
    }
    
    if((widget->isParentHidden)||(widget->isHidden)||(widget->isTransparent))
    {
        return;
    }
    
    arm_2d_container(ptParent,tTarget , &ptResTile->tRegion)
    {
        tTarget.tRegion.tLocation = ptResTile->tRegion.tLocation;
        
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
                if(widget->isCorner)
                {
                    tRegion.tLocation.iX=0;
                    tRegion.tLocation.iY=0;
                    tRegion.tSize = tTarget.tRegion.tSize;
                    draw_round_corner_box(&tTarget,
                                                &tRegion,
                                                btnColor,
                                                255,
                                                bIsNewFrame);
                }
                else
                {
                    ldBaseColor(&tTarget,btnColor,255);
                }
            }
            else
            {
                if(widget->isPressed)
                {
                    ptResTile->pchBuffer = (uint8_t *)widget->pressImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)ptResTile)->pTarget=widget->pressImgAddr;
#endif
                    ldBaseImage(&tTarget,ptResTile,widget->isPressMask,255);
                }
                else
                {
                    ptResTile->pchBuffer = (uint8_t *)widget->releaseImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)ptResTile)->pTarget=widget->releaseImgAddr;
#endif
                    ldBaseImage(&tTarget,ptResTile,widget->isReleaseMask,255);
                }
            }
            arm_2d_op_wait_async(NULL);

            if(widget->ptTextInfo!=NULL)
            {
                ldBaseShowText(tTarget,widget->ptTextInfo,0);
                arm_2d_op_wait_async(NULL);
            }

            if(widget->isSelected)
            {
                if (widget->selectMaskAddr==0)
                {
                    tRegion.tLocation.iX=0;
                    tRegion.tLocation.iY=0;
                    tRegion.tSize = tTarget.tRegion.tSize;

                    if(widget->isCorner)
                    {
                        draw_round_corner_border(&tTarget,&tRegion,widget->selectColor,
                                                 (arm_2d_border_opacity_t){SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY},
                                                 (arm_2d_corner_opacity_t){SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY});
                    }
                    else
                    {
                        arm_2d_draw_box(&tTarget,&tRegion,3,widget->selectColor,SELECT_COLOR_OPACITY);

                    }
                }
                else
                {
#if USE_VIRTUAL_RESOURCE == 0
                    arm_2d_tile_t maskRes;
#else
                    arm_2d_vres_t maskRes;
#endif
                    (*((arm_2d_tile_t*)&maskRes))=*ptResTile;
                    (*((arm_2d_tile_t*)&maskRes)).tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A8;
                    ((arm_2d_tile_t*)&maskRes)->pchBuffer = (uint8_t *)widget->selectMaskAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    maskRes.pTarget=widget->selectMaskAddr;
                    maskRes.Load = &__disp_adapter0_vres_asset_loader;
                    maskRes.Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
                    ldBaseMaskImage(&tTarget,(arm_2d_tile_t*)&maskRes,widget->selectColor,255);
                }
                arm_2d_op_wait_async(NULL);
            }
        }
    }
}


void ldButtonSetAlign(ldButton_t *widget,uint8_t align)
{
    if(widget==NULL)
    {
        return;
    }
    ldBaseSetAlign(&widget->ptTextInfo,align);
}

void ldButtonSetFont(ldButton_t *widget,ldFontDict_t *pFontDict)
{
    if(widget==NULL)
    {
        return;
    }
    ldBaseSetFont(&widget->ptTextInfo,pFontDict);
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
