#include "ldText.h"
#include "ldGui.h"

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

static bool _textDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        if(((ldText_t*)pTarget)->ptTextInfo!=NULL)
        {
            ldBaseTextDel(((ldText_t*)pTarget)->ptTextInfo);
        }
        ldTextSetScroll((ldText_t*)pTarget,false);
        ldFree(((ldText_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldTextDel(ldText_t *widget)
{
    xListNode *listInfo;

    if (widget == NULL)
    {
        return;
    }

    if(widget->widgetType!=widgetTypeText)
    {
        return;
    }

    LOG_DEBUG("[text] del,id:%d\n",widget->nameId);

    // 查找父链表
    listInfo = ldGetWidgetInfoById(((ldCommon_t *)widget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, widget, _textDel);
    }
}

ldText_t *ldTextInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict)
{
    ldText_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldText_t);
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
        pNewWidget->widgetType = widgetTypeText;
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentType = ((ldCommon_t *)(parentInfo->info))->widgetType;
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isHidden = false;

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

        pNewWidget->isTransparent=false;
        pNewWidget->bgColor=__RGB(255,255,255);
        ldBaseSetFont(&pNewWidget->ptTextInfo,pFontDict);
        pNewWidget->scrollOffset=0;

        LOG_INFO("[text] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[text] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldTextLoop(ldText_t *widget,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    arm_2d_tile_t *ptResTile=(arm_2d_tile_t*)&widget->resource;

    if (widget == NULL)
    {
        return;
    }

    if((widget->isParentHidden)||(widget->isHidden))
    {
        return;
    }

    arm_2d_container(ptParent,tTarget , &ptResTile->tRegion)
    {
        tTarget.tRegion.tLocation = ptResTile->tRegion.tLocation;

        if(!widget->isTransparent)
        {
            if (widget->bgImgAddr==0)//color
            {
                ldBaseColor(&tTarget,widget->bgColor,255);
            }
            else
            {
                ptResTile->pchBuffer = (uint8_t *)widget->bgImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                ((arm_2d_vres_t*)ptResTile)->pTarget=widget->releaseImgAddr;
#endif
                ldBaseImage(&tTarget,ptResTile,false,255);
            }
            arm_2d_op_wait_async(NULL);
        }
        if(widget->ptTextInfo!=NULL)
        {
            ldBaseShowText(tTarget,widget->ptTextInfo,widget->scrollOffset);
            arm_2d_op_wait_async(NULL);
        }
    }
}

void ldTextSetTransparent(ldText_t* widget,bool isTransparent)
{
    if(widget==NULL)
    {
        return;
    }
    widget->isTransparent=isTransparent;
}

void ldTextSetHidden(ldText_t *widget,bool isHidden)
{
    ldBaseSetHidden((ldCommon_t*) widget,isHidden);
}

void ldTextSetText(ldText_t* widget,uint8_t *pStr)
{
    if(widget==NULL)
    {
        return;
    }
    ldBaseSetText(&widget->ptTextInfo,pStr);
}

void ldTextSetTextColor(ldText_t* widget,ldColor charColor)
{
    if(widget==NULL)
    {
        return;
    }
    ldBaseSetTextColor(&widget->ptTextInfo,charColor);
}

void ldTextSetAlign(ldText_t *widget,uint8_t align)
{
    if(widget==NULL)
    {
        return;
    }
    ldBaseSetAlign(&widget->ptTextInfo,align);
}

void ldTextScrollSeek(ldText_t *widget,int16_t offset)
{
    if(widget==NULL)
    {
        return;
    }
    widget->scrollOffset=offset;
}

void ldTextScrollMove(ldText_t *widget, int8_t moveValue)
{
    if(widget==NULL)
    {
        return;
    }
    widget->scrollOffset+=moveValue;
    if((moveValue>0)&&(widget->scrollOffset<0))
    {
        if(widget->scrollOffset<0)
        {
            widget->scrollOffset=0;
        }
    }
}

static bool slotTextVerticalScroll(xConnectInfo_t info)
{
    ldText_t *txt;
    int16_t x;int16_t y;

    txt=ldGetWidgetById(info.receiverId);

    ldCfgTouchGetPoint(&x,&y);
    if(info.signalType==BTN_PRESS)
    {
        txt->touchMoveTemp=y;
    }
    else
    if(info.signalType==SIGNAL_TOUCH_HOLD_MOVE)
    {
        if((y-txt->touchMoveTemp)<0)
        {
            txt->scrollOffset-=1;
        }
        else
        {
            txt->scrollOffset+=1;
        }
    }

    return false;
}

void ldTextSetScroll(ldText_t *widget,bool isEnable)
{
    if(widget==NULL)
    {
        return;
    }
    if(widget->isScroll!=isEnable)
    {
        widget->isScroll=isEnable;
        if(isEnable)
        {
            xConnect(widget->nameId,BTN_PRESS,widget->nameId,slotTextVerticalScroll);
            xConnect(widget->nameId,SIGNAL_TOUCH_HOLD_MOVE,widget->nameId,slotTextVerticalScroll);
        }
        else
        {
            xDisconnect(widget->nameId,SIGNAL_TOUCH_HOLD_MOVE,widget->nameId,slotTextVerticalScroll);
        }
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
