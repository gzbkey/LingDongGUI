#include "ldButton.h"
#include "ldGui.h"

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

void ldButtonDel(ldButton_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeButton)
    {
        return;
    }

    LOG_INFO("[button] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _buttonDel);
    }
}

static bool slotButtonToggle(xConnectInfo_t info)
{
    ldButton_t *btn;

    btn=ldGetWidgetById(info.receiverId);

    if(info.signalType==SIGNAL_PRESS)
    {
        btn->isPressed=true;
    }
    if(info.signalType==SIGNAL_RELEASE)
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
        pNewWidget->releaseImgAddr=LD_ADDR_NONE;
        pNewWidget->pressImgAddr=LD_ADDR_NONE;
        pNewWidget->selectMaskAddr=LD_ADDR_NONE;
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

        xConnect(nameId,SIGNAL_PRESS,nameId,slotButtonToggle);
        xConnect(nameId,SIGNAL_RELEASE,nameId,slotButtonToggle);

        LOG_INFO("[button] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[button] init failed,id:%d\n",nameId);
    }
    return pNewWidget;
}

void ldButtonSetText(ldButton_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldBaseSetText(&pWidget->ptTextInfo,pStr);
}

void ldButtonSetColor(ldButton_t* pWidget,ldColor releaseColor,ldColor pressColor)
{
    pWidget->releaseColor=releaseColor;
    pWidget->pressColor=pressColor;
    pWidget->releaseImgAddr=LD_ADDR_NONE;
    pWidget->pressImgAddr=LD_ADDR_NONE;
}

void ldButtonSetImage(ldButton_t* pWidget,uint32_t releaseImgAddr,bool isReleaseMask,uint32_t pressImgAddr,bool isPressMask)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->releaseImgAddr=releaseImgAddr;
    pWidget->pressImgAddr=pressImgAddr;
    pWidget->isReleaseMask=isReleaseMask;
    pWidget->isPressMask=isPressMask;
}

void ldButtonSetSelectImage(ldButton_t* pWidget,uint32_t selectMaskAddr,ldColor selectColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->selectMaskAddr=selectMaskAddr;
    pWidget->selectColor=selectColor;
}

void ldButtonSetTextColor(ldButton_t* pWidget,ldColor charColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldBaseSetTextColor(&pWidget->ptTextInfo,charColor);
}

void ldButtonSetTransparent(ldButton_t* pWidget,bool isTransparent)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isTransparent=isTransparent;
}

void ldButtonSetHidden(ldButton_t* pWidget,bool isHidden)
{
    ldBaseSetHidden((ldCommon_t*) pWidget,isHidden);
}

void ldButtonSetRoundCorner(ldButton_t* pWidget,bool isCorner)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isCorner=isCorner;
}

void ldButtonSetSelect(ldButton_t* pWidget,bool isSelected)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isSelected=isSelected;
}

void ldButtonLoop(ldButton_t *pWidget,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    uint32_t btnColor;
    arm_2d_tile_t *ptResTile=(arm_2d_tile_t*)&pWidget->resource;

#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tempRes;
#else
    arm_2d_vres_t tempRes;
#endif

    if (pWidget == NULL)
    {
        return;
    }
    
    if((pWidget->isParentHidden)||(pWidget->isHidden)||(pWidget->isTransparent))
    {
        return;
    }
    
    arm_2d_container(ptParent,tTarget , &ptResTile->tRegion)
    {
        tTarget.tRegion.tLocation = ptResTile->tRegion.tLocation;
        
        if(!pWidget->isTransparent)
        {
            if ((pWidget->releaseImgAddr==LD_ADDR_NONE)&&(pWidget->pressImgAddr==LD_ADDR_NONE))//color
            {
                if(pWidget->isPressed)
                {
                    btnColor=pWidget->pressColor;
                }
                else
                {
                    btnColor=pWidget->releaseColor;
                }
                if(pWidget->isCorner)
                {
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize = tTarget.tRegion.tSize;
                    draw_round_corner_box(&tTarget,
                                                &((arm_2d_tile_t*)&tempRes)->tRegion,
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
                if(pWidget->isPressed)
                {
                    ptResTile->pchBuffer = (uint8_t *)pWidget->pressImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)ptResTile)->pTarget=pWidget->pressImgAddr;
#endif
                    if(!pWidget->isCorner)
                    {
                        ldBaseImage(&tTarget,ptResTile,pWidget->isPressMask,255);
                    }
                }
                else
                {
                    ptResTile->pchBuffer = (uint8_t *)pWidget->releaseImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)ptResTile)->pTarget=pWidget->releaseImgAddr;
#endif
                    if(!pWidget->isCorner)
                    {
                        ldBaseImage(&tTarget,ptResTile,pWidget->isReleaseMask,255);
                    }
                }
                if(pWidget->isCorner)
                {
#if USE_VIRTUAL_RESOURCE == 0
                    tempRes=*ptResTile;
#else
                    tempRes=*((arm_2d_vres_t*)ptResTile);
#endif
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;
                    draw_round_corner_image((arm_2d_tile_t*)&tempRes,
                                            &tTarget,
                                            &((arm_2d_tile_t*)&tempRes)->tRegion,
                                            bIsNewFrame);
                }
            }
            arm_2d_op_wait_async(NULL);

            if(pWidget->ptTextInfo!=NULL)
            {
                ldBaseShowText(tTarget,pWidget->ptTextInfo,0);
                arm_2d_op_wait_async(NULL);
            }

            if(pWidget->isSelected)
            {
                if (pWidget->selectMaskAddr==LD_ADDR_NONE)
                {
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize = tTarget.tRegion.tSize;

                    if(pWidget->isCorner)
                    {
                        draw_round_corner_border(&tTarget,&((arm_2d_tile_t*)&tempRes)->tRegion,pWidget->selectColor,
                                                 (arm_2d_border_opacity_t){SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY},
                                                 (arm_2d_corner_opacity_t){SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY,SELECT_COLOR_OPACITY});
                    }
                    else
                    {
                        arm_2d_draw_box(&tTarget,&((arm_2d_tile_t*)&tempRes)->tRegion,3,pWidget->selectColor,SELECT_COLOR_OPACITY);

                    }
                }
                else
                {
#if USE_VIRTUAL_RESOURCE == 0
                    tempRes=*ptResTile;
                    tempRes.tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A8;
                    tempRes.pchBuffer = (uint8_t *)pWidget->selectMaskAddr;
#else
                    tempRes=*((arm_2d_vres_t*)ptResTile);
                    ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A8;
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->selectMaskAddr;
                    tempRes.pTarget=pWidget->selectMaskAddr;
                    tempRes.Load = &__disp_adapter0_vres_asset_loader;
                    tempRes.Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
                    ldBaseMaskImage(&tTarget,(arm_2d_tile_t*)&tempRes,pWidget->selectColor,255);
                }
                arm_2d_op_wait_async(NULL);
            }
        }
    }
}


void ldButtonSetAlign(ldButton_t *pWidget,uint8_t align)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldBaseSetAlign(&pWidget->ptTextInfo,align);
}

void ldButtonSetFont(ldButton_t *pWidget,ldFontDict_t *pFontDict)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldBaseSetFont(&pWidget->ptTextInfo,pFontDict);
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
