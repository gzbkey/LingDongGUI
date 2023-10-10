#include "ldScrollSelecter.h"

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

static bool _scrollSelecterDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        if(((ldScrollSelecter_t*)pTarget)->pTextInfo!=NULL)
        {
            ldBaseTextDel(((ldScrollSelecter_t*)pTarget)->pTextInfo);
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
        tResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_MASK_A8;
        tResTile->pchBuffer = 0;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        ((arm_2d_vres_t*)tResTile)->pTarget=0;
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
        pNewWidget->charColor=__RGB(0,255,0);
        pNewWidget->scrollOffset=-5;

        ldBaseSetFont(&pNewWidget->pTextInfo,pFontDict);


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
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tempRes;
#else
    arm_2d_vres_t tempRes;
#endif

    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        return;
    }

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        newRegion.tLocation.iX=0;
        newRegion.tLocation.iY=0;

        arm_2d_draw_box(&tTarget,&newRegion,1,0,255);

//        for(int i=0;i<pTextInfo->strLen;)
//        {
//            len=ldBaseGetCharInfo(pTextInfo->pFontDict,&pTextInfo->pStr[i],&advWidth,&offsetX,&offsetY,&width,&height,&imgAddr);
//            if(width==0)
//            {
//                switch(pTextInfo->pStr[i])
//                {
//                case '\r':
//                {
//                    textOffsetX=0;
//                    break;
//                }
//                case '\n':
//                {
//                    textOffsetX=0;
//                    textOffsetY+=pTextInfo->pFontDict->lineOffset;
//                    break;
//                }
//                default:
//                    break;
//                }
//                i+=len;
//                continue;
//            }

//#if USE_VIRTUAL_RESOURCE == 0
//            fontTile = pTextInfo->fontTile;
//            fontTile.tRegion.tSize.iWidth=width;
//            fontTile.tRegion.tSize.iHeight=height;
//            fontTile.pchBuffer+=imgAddr;
//#else
//            fontTile = *((arm_2d_vres_t*)&pTextInfo->fontTile);
//            fontTile.tTile.tRegion.tSize.iWidth=width;
//            fontTile.tTile.tRegion.tSize.iHeight=height;
//            fontTile.tTile.pchBuffer=imgAddr;
//            fontTile.pTarget=imgAddr;
//#endif
//            int16_t tempHeight;
//            //偏移坐标
//            if(textSize.iHeight>pTextInfo->pFontDict->lineOffset)//多行
//            {
//                if((textOffsetX+advWidth)>textSize.iWidth)//自动换行
//                {
//                    textOffsetX=0;
//                    textOffsetY+=pTextInfo->pFontDict->lineOffset;
//                }
//                tempHeight=textOffsetY-(height+offsetY);
//            }
//            else
//            {
//                tempHeight=bmpH1Max-(height+offsetY);
//            }

//            arm_2d_tile_t charTile=impl_child_tile(showTile,textOffsetX+offsetX,tempHeight,width,height);

//            ldBaseMaskImage(&charTile,(arm_2d_tile_t*)&fontTile,pTextInfo->charColor,opacity);
//            arm_2d_op_wait_async(NULL);

//            textOffsetX+=advWidth;

//            i+=len;
//        }

////        void ldBaseMaskImage(arm_2d_tile_t* pTile,arm_2d_tile_t *pResTile,ldColor textColor,uint8_t opacity)
//#if USE_VIRTUAL_RESOURCE == 0
//                    tempRes=*pResTile;
//                    tempRes.pchBuffer = (uint8_t *)pWidget->selectMaskAddr;
//#else
//                    tempRes=*((arm_2d_vres_t*)pResTile);
//                    ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->selectMaskAddr;
//                    tempRes.pTarget=pWidget->selectMaskAddr;
//                    tempRes.Load = &__disp_adapter0_vres_asset_loader;
//                    tempRes.Depose = &__disp_adapter0_vres_buffer_deposer;
//#endif

//#if USE_OPACITY == 1
//             ldBaseMaskImage(&tTarget,&tempRes,pWidget->charColor,pWidget.opacity);
//#else
//                    ldBaseMaskImage(&tTarget,&tempRes,pWidget->charColor,255);
//#endif


//        int16_t posY=tTarget.tRegion.tLocation.iY;
//        for(uint8_t i=0;i<pWidget->itemCount;i++)
//        {
//            newRegion.tLocation.iX=0;
//            newRegion.tLocation.iY=0;

////        tTarget.tRegion.tLocation.iY=posY+(pResTile->tRegion.tSize.iHeight*i);

////        LOG_DEBUG("ScrollSelecter\n");
//        ldBaseSetText(&pWidget->pTextInfo,pWidget->ppItemStrGroup[i]);

////        pWidget->pTextInfo->fontTile.tRegion.tLocation.iY=pWidget->scrollOffset+(pWidget->pTextInfo->fontTile.tRegion.tSize.iHeight*i);
////        LOG_DEBUG("====%d %s %ld \n",textInfo.strLen,textInfo.pStr,textInfo.pFontDict);
////        LOG_REGION("9999",tTarget.tRegion);
//        arm_2d_draw_box(&tTarget,&newRegion,1,0,255);
//#if USE_OPACITY == 1
//            ldBaseShowText(tTarget,newRegion,pWidget->pTextInfo,0,pWidget->opacity);
//#else
//            ldBaseShowText(tTarget,newRegion,pWidget->pTextInfo,pWidget->scrollOffset,255);
//#endif
//            arm_2d_op_wait_async(NULL);
//        }
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
            strcpy(pWidget->ppItemStrGroup[pWidget->itemCount],pStr);
//            LOG_DEBUG("===%s\n",pWidget->ppItemStrGroup[pWidget->itemCount]);
//            LOG_DEBUG("===%s\n",pStr);
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
    ldBaseSetTextColor(&pWidget->pTextInfo,charColor);
}

//void ldScrollSelecterSetText(ldScrollSelecter_t* pWidget,uint8_t *pStr)
//{
//    if(pWidget==NULL)
//    {
//        return;
//    }
//    ldBaseSetText(&pWidget->pTextInfo,pStr);
//}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
