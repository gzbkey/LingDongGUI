#include "ldProgressBar.h"

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

static bool _progressBarDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        //del user object

        ldFree(((ldProgressBar_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldProgressBarDel(ldProgressBar_t *widget)
{
    xListNode *listInfo;

    if (widget == NULL)
    {
        return;
    }

    if(widget->widgetType!=widgetTypeProgressBar)
    {
        return;
    }

    LOG_DEBUG("[progressBar] del,id:%d\n",widget->nameId);

    // 查找父链表
    listInfo = ldGetWidgetInfoById(((ldCommon_t *)widget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, widget, _progressBarDel);
    }
}

ldProgressBar_t *ldProgressBarInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    ldProgressBar_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldProgressBar_t);
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
        pNewWidget->widgetType = widgetTypeProgressBar;
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

        pNewWidget->bgAddr=LD_ADDR_NONE;
        pNewWidget->fgAddr=LD_ADDR_NONE;
        pNewWidget->frameAddr=LD_ADDR_NONE;
//        pNewWidget->maskAddr=LD_ADDR_NONE;
        pNewWidget->bgOffset=0;
        pNewWidget->fgOffset=0;
        pNewWidget->bgColor=GLCD_COLOR_WHITE;
        pNewWidget->fgColor=__RGB(0x94, 0xd2, 0x52);
        pNewWidget->frameColor=__RGB(0xa5, 0xc6, 0xef);
        pNewWidget->permille=0;

        pNewWidget->timer = arm_2d_helper_get_system_timestamp();

        LOG_INFO("[progressBar] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[progressBar] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

static void _progressBarColorShow(arm_2d_tile_t *ptTarget,uint16_t iProgress,ldColor bgColor,ldColor fgColor,ldColor frameColor)
{
    arm_2d_region_t tBarRegion = {
        .tLocation = {
           .iX = 0,
           .iY = 0,
        },
        .tSize = ptTarget->tRegion.tSize,
    };

    arm_2d_fill_colour( ptTarget,
                        &tBarRegion,
                        frameColor);

    arm_2d_op_wait_async(NULL);

    tBarRegion.tSize.iHeight-=2;
    tBarRegion.tSize.iWidth-=2;
    tBarRegion.tLocation.iX += 1;
    tBarRegion.tLocation.iY += 1;

    arm_2d_fill_colour(ptTarget, &tBarRegion, bgColor);

    arm_2d_op_wait_async(NULL);

    if (iProgress > 0)
    {
        tBarRegion.tSize.iWidth = tBarRegion.tSize.iWidth * iProgress / 1000;
        arm_2d_fill_colour( ptTarget,
                            &tBarRegion,
                            fgColor
                            );
        arm_2d_op_wait_async(NULL);
    }
}

static void _progressBarImageShow(ldProgressBar_t *widget,arm_2d_tile_t *ptTarget,bool bIsNewFrame)
{
#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tResTile=widget->resource;
    tResTile.bIsRoot=true;
    tResTile.bHasEnforcedColour=true;
    tResTile.tRegion.tLocation.iX=0;
    tResTile.tRegion.tLocation.iY=0;
#else
    arm_2d_vres_t tResTile=*((arm_2d_vres_t*)(&widget->resource));
    tResTile.tTile.bIsRoot=true;
    tResTile.tTile.bHasEnforcedColour=true;
    tResTile.tTile.bVirtualResource=true;
    tResTile.tTile.tRegion.tLocation.iX=0;
    tResTile.tTile.tRegion.tLocation.iY=0;
#endif

    arm_2d_region_t tBarRegion = {
        .tLocation = {
           .iX = 0,
           .iY = 0,
        },
        .tSize = ptTarget->tRegion.tSize,
    };

    do {

        //bg image
        ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth=widget->bgWidth;
        ((arm_2d_tile_t*)&tResTile)->pchBuffer=(uint8_t*)widget->bgAddr;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile.pTarget=widget->bgAddr;
#endif
        arm_2d_region_t tInnerRegion = {
            .tSize = {
                .iWidth = tBarRegion.tSize.iWidth + ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth,
                .iHeight = tBarRegion.tSize.iHeight,
            },
            .tLocation = {
                .iX = -widget->bgWidth + widget->bgOffset,
            },
        };
        arm_2d_tile_t tileInnerSlot;
        arm_2d_tile_generate_child(ptTarget, &tBarRegion, &tileInnerSlot, false);
        arm_2d_tile_fill_only( (arm_2d_tile_t*)&tResTile,
                            &tileInnerSlot,
                            &tInnerRegion);
        arm_2d_op_wait_async(NULL);
    } while(0);

    if (widget->permille > 0)
    {
        do {
            //fg image
            ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth=widget->fgWidth;
            ((arm_2d_tile_t*)&tResTile)->pchBuffer=(uint8_t*)widget->fgAddr;
#if USE_VIRTUAL_RESOURCE == 1
            tResTile.pTarget=widget->fgAddr;
#endif
            arm_2d_region_t tInnerRegion = {
                .tSize = {
                    .iWidth = tBarRegion.tSize.iWidth + ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth,
                    .iHeight = tBarRegion.tSize.iHeight,
                },
                .tLocation = {
                    .iX = -widget->fgWidth + widget->fgOffset,
                },
            };
            tBarRegion.tSize.iWidth = tBarRegion.tSize.iWidth * widget->permille / 1000;

            arm_2d_tile_t tileInnerSlot;
            arm_2d_tile_generate_child(ptTarget, &tBarRegion, &tileInnerSlot, false);
            arm_2d_tile_fill_only( (arm_2d_tile_t*)&tResTile,
                                &tileInnerSlot,
                                &tInnerRegion);
            arm_2d_op_wait_async(NULL);
        } while(0);
    }

    if(widget->frameAddr!=LD_ADDR_NONE)
    {
        do {
            //frame image png
            ((arm_2d_tile_t*)&tResTile)->tRegion.tSize.iWidth=ptTarget->tRegion.tSize.iWidth;
            ((arm_2d_tile_t*)&tResTile)->pchBuffer=(uint8_t*)widget->frameAddr;
#if USE_VIRTUAL_RESOURCE == 1
            tResTile.pTarget=widget->frameAddr;
#endif
            ldBaseImage(ptTarget,&tResTile,true,255);
            arm_2d_op_wait_async(NULL);
        } while (0);
    }

    //! update offset
    if (bIsNewFrame) {
        if (arm_2d_helper_is_time_out(PROGRESS_BAR_SPEED, &widget->timer))
        {
            if(widget->isBgMove)
            {
                widget->bgOffset++;
                if (widget->bgOffset >= widget->bgWidth)
                {
                    widget->bgOffset = 0;
                }
            }
            if(widget->isFgMove)
            {
                widget->fgOffset++;
                if (widget->fgOffset >= widget->fgWidth)
                {
                    widget->fgOffset = 0;
                }
            }
        }
    }
}

void ldProgressBarLoop(ldProgressBar_t *widget,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
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

        if(widget->bgAddr==LD_ADDR_NONE&&widget->fgAddr==LD_ADDR_NONE)//color
        {
            _progressBarColorShow(&tTarget,widget->permille,widget->bgColor,widget->fgColor,widget->frameColor);
        }
        else
        {
            _progressBarImageShow(widget,&tTarget,bIsNewFrame);
        }

    }
}

void ldProgressBarSetHidden(ldProgressBar_t *widget,bool isHidden)
{
    ldBaseSetHidden((ldCommon_t*) widget,isHidden);
}

void ldProgressBarSetPercent(ldProgressBar_t *widget,float percent)
{
    if(widget==NULL)
    {
        return;
    }
    if(percent>=0)
    {
        if(percent>100)
        {
            percent=100;
        }
        widget->permille=percent*10;
    }
    else
    {
        if(percent<-100)
        {
            percent=-100;
        }
        widget->permille=1000+percent*10;
    }
}

void ldProgressBarSetBgImage(ldProgressBar_t *widget,uint32_t bgAddr,uint16_t bgWidth,bool isMove)
{
    if(widget==NULL)
    {
        return;
    }
    widget->bgAddr=bgAddr;
    widget->bgWidth=bgWidth;
    widget->isBgMove=isMove;
}

void ldProgressBarSetFgImage(ldProgressBar_t *widget,uint32_t fgAddr,uint16_t fgWidth,bool isMove)
{
    if(widget==NULL)
    {
        return;
    }
    widget->fgAddr=fgAddr;
    widget->fgWidth=fgWidth;
    widget->isFgMove=isMove;
}

void ldProgressBarSetFrameImage(ldProgressBar_t *widget,uint32_t frameAddr)
{
    if(widget==NULL)
    {
        return;
    }
    widget->frameAddr=frameAddr;
}

void ldProgressBarSetColor(ldProgressBar_t *widget,ldColor bgColor,ldColor fgColor,ldColor frameColor)
{
    if(widget==NULL)
    {
        return;
    }
    widget->bgAddr=LD_ADDR_NONE;
    widget->fgAddr=LD_ADDR_NONE;
    widget->bgColor=bgColor;
    widget->fgColor=fgColor;
    widget->frameColor=frameColor;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
