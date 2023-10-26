#include "ldScrollSelecter.h"
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

#define MOVE_SPEED_THRESHOLD_VALUE     (20)          //触摸移动速度超过此值，则产生惯性滑动效果
#define SPEED_2_OFFSET(speed)          (speed*3)     //通过速度值，生成惯性滑动距离

static bool _scrollSelecterDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        for(uint8_t i=0;i<((ldScrollSelecter_t *)pTarget)->itemCount;i++)
        {
            ldFree(((ldScrollSelecter_t *)pTarget)->ppItemStrGroup[i]);
        }
        ldFree(((ldScrollSelecter_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldScrollSelecterDel(ldScrollSelecter_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeScrollSelecter)
    {
        return;
    }

    LOG_INFO("[scrollSelecter] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _scrollSelecterDel);
    }
}

static uint8_t _ldScrollSelecterAutoItem(ldScrollSelecter_t *pWidget,int16_t offset)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    if(offset>=0)
    {
        return 0;
    }

    offset=-offset;
    int16_t temp1,temp2;

    temp1=offset%pResTile->tRegion.tSize.iHeight;
    temp2=offset/pResTile->tRegion.tSize.iHeight;
    if(temp1>=(pResTile->tRegion.tSize.iHeight/2))
    {
        temp2++;
    }

    if(temp2>=pWidget->itemCount)
    {
        temp2=pWidget->itemCount-1;
    }

    return temp2;
}

static int16_t _scrollOffset;

static bool slotScrollSelecterScroll(xConnectInfo_t info)
{
    ldScrollSelecter_t *selecter;

    selecter=ldGetWidgetById(info.receiverId);

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        selecter->isWaitMove=false;
        selecter->isAutoMove=false;
        _scrollOffset=selecter->scrollOffset;
        break;
    }
    case SIGNAL_TOUCH_HOLD_MOVE:
    {
        selecter->scrollOffset=_scrollOffset+(int16_t)(info.value&0xFFFF);
        break;
    }
    case SIGNAL_RELEASE:
    {
        if(!selecter->isAutoMove)
        {
            selecter->itemSelect=_ldScrollSelecterAutoItem(selecter,selecter->scrollOffset);
        }
        selecter->isWaitMove=true;
        break;
    }
    case SIGNAL_MOVE_SPEED:
    {
        int16_t ySpeed=(int16_t)(info.value&0xFFFF);

        if(ySpeed>MOVE_SPEED_THRESHOLD_VALUE)
        {
            selecter->itemSelect=_ldScrollSelecterAutoItem(selecter,_scrollOffset+SPEED_2_OFFSET(ySpeed));
            selecter->isAutoMove=true;
        }
        else
        {
            if(ySpeed<-MOVE_SPEED_THRESHOLD_VALUE)
            {
                selecter->itemSelect=_ldScrollSelecterAutoItem(selecter,_scrollOffset+SPEED_2_OFFSET(ySpeed));
                selecter->isAutoMove=true;
            }
        }
        break;
    }
    default:
        break;
    }

    return false;
}

ldScrollSelecter_t *ldScrollSelecterInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict, uint8_t itemMax)
{
    ldScrollSelecter_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    void **pNewStrGroup = NULL;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldScrollSelecter_t);
    pNewStrGroup=(void**)ldMalloc(sizeof (void*)*itemMax);

    if ((pNewWidget != NULL)&&(pNewStrGroup != NULL))
    {
        pNewWidget->isParentHidden=false;
        
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeScrollSelecter;
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isHidden = false;

        tResTile=(arm_2d_tile_t*)&pNewWidget->resource;
        tResTile->tRegion.tLocation.iX=x;
        tResTile->tRegion.tLocation.iY=y;
        tResTile->tRegion.tSize.iWidth=width;
        tResTile->tRegion.tSize.iHeight=height;
        tResTile->tInfo.bIsRoot = true;
        tResTile->tInfo.bHasEnforcedColour = true;
        tResTile->tInfo.tColourInfo.chScheme = ldBaseGetChScheme(pFontDict->maskType);
        tResTile->pchBuffer = (uint8_t*)LD_ADDR_NONE;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        ((arm_2d_vres_t*)tResTile)->pTarget = LD_ADDR_NONE;
        ((arm_2d_vres_t*)tResTile)->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)tResTile)->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif

        pNewWidget->itemMax=itemMax;
        pNewWidget->itemCount=0;
        pNewWidget->ppItemStrGroup=pNewStrGroup;

        for(uint8_t i=0;i<itemMax;i++)
        {
            pNewWidget->ppItemStrGroup[0]=NULL;
        }
#if USE_OPACITY == 1
        pNewWidget->opacity=255;
#endif
        pNewWidget->charColor=0;
        pNewWidget->pFontDict=pFontDict;
        pNewWidget->isTransparent=true;
        pNewWidget->bgColor=0;
        pNewWidget->moveOffset=1;
        pNewWidget->isAutoMove=false;
        pNewWidget->isWaitMove=false;

        xConnect(pNewWidget->nameId,SIGNAL_PRESS,pNewWidget->nameId,slotScrollSelecterScroll);
        xConnect(pNewWidget->nameId,SIGNAL_TOUCH_HOLD_MOVE,pNewWidget->nameId,slotScrollSelecterScroll);
        xConnect(pNewWidget->nameId,SIGNAL_RELEASE,pNewWidget->nameId,slotScrollSelecterScroll);
        xConnect(pNewWidget->nameId,SIGNAL_MOVE_SPEED,pNewWidget->nameId,slotScrollSelecterScroll);

        LOG_INFO("[scrollSelecter] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);
        ldFree(pNewStrGroup);

        LOG_ERROR("[scrollSelecter] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldScrollSelecterLoop(ldScrollSelecter_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        return;
    }

    if((pWidget->isWaitMove)&&(bIsNewFrame))
    {
        int16_t targetOffset=-(pWidget->itemSelect*((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight);

        if(pWidget->scrollOffset==targetOffset)
        {
            pWidget->isWaitMove=false;
        }
        else
        {
            if(pWidget->scrollOffset>targetOffset)
            {
                pWidget->scrollOffset-=pWidget->moveOffset;
                if(pWidget->scrollOffset<targetOffset)
                {
                    pWidget->scrollOffset=targetOffset;
                }
            }
            else
            {
                pWidget->scrollOffset+=pWidget->moveOffset;
                if(pWidget->scrollOffset>targetOffset)
                {
                    pWidget->scrollOffset=targetOffset;
                }
            }
        }
    }

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if(!pWidget->isTransparent)
        {
            if(pResTile->pchBuffer==(uint8_t*)LD_ADDR_NONE)
            {

#if USE_OPACITY == 1
            ldBaseColor(&tTarget,pWidget->bgColor,pWidget->opacity);
#else
            ldBaseColor(&tTarget,pWidget->bgColor,255);
#endif
            }
            else
            {
                pResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
#if USE_OPACITY == 1
            ldBaseImage(&tTarget,pResTile,false,pWidget->opacity);
#else
            ldBaseImage(&tTarget,pResTile,false,255);
#endif
            }
            arm_2d_op_wait_async(NULL);
        }

        pResTile->tInfo.tColourInfo.chScheme = ldBaseGetChScheme(pWidget->pFontDict->maskType);
        for(uint8_t strGroupCount=0;strGroupCount<pWidget->itemCount;strGroupCount++)
        {
            uint8_t *pStrGroup=pWidget->ppItemStrGroup[strGroupCount];
            int16_t offset=(strGroupCount*((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight)+pWidget->scrollOffset;
#if USE_OPACITY == 1
            ldBaseLineText(&tTarget,&pWidget->resource,pStrGroup,pWidget->pFontDict,pWidget->align,pWidget->charColor,offset,pWidget->opacity);
#else
            ldBaseLineText(&tTarget,&pWidget->resource,pStrGroup,pWidget->pFontDict,pWidget->align,pWidget->charColor,offset,255);
#endif
        }
        arm_2d_op_wait_async(NULL);
    }
}


void ldScrollSelecterAddItem(ldScrollSelecter_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }

    if(pWidget->itemCount<pWidget->itemMax)
    {
        if(pWidget->ppItemStrGroup[pWidget->itemCount]==NULL)
        {
            pWidget->ppItemStrGroup[pWidget->itemCount]=LD_MALLOC_STRING(pStr);
            strcpy(pWidget->ppItemStrGroup[pWidget->itemCount],(char*)pStr);
            pWidget->itemCount++;
        }
    }
}

void ldScrollSelecterSetTextColor(ldScrollSelecter_t* pWidget,ldColor charColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->charColor=charColor;
}

void ldScrollSelecterSetBgColor(ldScrollSelecter_t* pWidget,ldColor bgColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->bgColor=bgColor;
    ((arm_2d_tile_t*)&pWidget->resource)->pchBuffer = (uint8_t*)LD_ADDR_NONE;
#if USE_VIRTUAL_RESOURCE == 1
    ((arm_2d_vres_t*)&pWidget->resource)->pTarget = LD_ADDR_NONE;
#endif
    pWidget->isTransparent=false;
}

void ldScrollSelecterSetBgImage(ldScrollSelecter_t* pWidget,uint32_t imgAddr)
{
    if(pWidget==NULL)
    {
        return;
    }
    ((arm_2d_tile_t*)&pWidget->resource)->pchBuffer = (uint8_t*)imgAddr;
#if USE_VIRTUAL_RESOURCE == 1
    ((arm_2d_vres_t*)&pWidget->resource)->pTarget = imgAddr;
#endif
    pWidget->isTransparent=false;
}

void ldScrollSelecterSetTransparent(ldScrollSelecter_t* pWidget,bool isTransparent)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isTransparent=isTransparent;
}

void ldScrollSelecterSetOpacity(ldScrollSelecter_t *pWidget, uint8_t opacity)
{
    if (pWidget == NULL)
    {
        return;
    }
#if USE_OPACITY == 1
    pWidget->opacity=opacity;
#endif
}

//selecter的滑动速度
//最小值:1
//最大值:控件高度
void ldScrollSelecterSetSpeed(ldScrollSelecter_t *pWidget, uint8_t speed)
{
    if (pWidget == NULL)
    {
        return;
    }
    if(speed<1)
    {
        speed=1;
    }
    if(speed>((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight)
    {
        speed=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight;
    }
    pWidget->moveOffset=speed;
}

void ldScrollSelecterSetItem(ldScrollSelecter_t *pWidget, uint8_t itemNum)
{
    if (pWidget == NULL)
    {
        return;
    }
    if(itemNum>=pWidget->itemCount)
    {
        itemNum=pWidget->itemCount-1;
    }
    pWidget->itemSelect=itemNum;
    pWidget->isWaitMove=true;
}

void ldScrollSelecterSetAlign(ldScrollSelecter_t *pWidget,uint8_t align)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->align=align;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
