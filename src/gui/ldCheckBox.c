#include "ldCheckBox.h"
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

#define CHECK_BOX_SIZE       14

//checkBoxDot.png w:6 h:6 < A8 >
const uint8_t checkBoxDot_png[]={
    0x00, 0x7F, 0xFF, 0xFF, 0x7F, 0x00,
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
    0x00, 0x7F, 0xFF, 0xFF, 0x7F, 0x00
};

//checkBoxDotSmall.png w:4 h:4 < A8 >
const uint8_t checkBoxDotSmall_png[]={
    0xC0, 0xFF, 0xFF, 0xC0,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xC0, 0xFF, 0xFF, 0xC0
};

//checked.png w:10 h:10 < A8 >
const uint8_t checked_png[]={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x08,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xC7, 0xFF, 0x78,
0x00, 0x40, 0x08, 0x00, 0x00, 0x38, 0xE7, 0xFF, 0xE7, 0x28,
0x9E, 0xFF, 0xCF, 0x10, 0x68, 0xF7, 0xFF, 0xC7, 0x18, 0x00,
0x3C, 0xDF, 0xFF, 0xEB, 0xFF, 0xFF, 0x97, 0x08, 0x00, 0x00,
0x00, 0x20, 0xE7, 0xFF, 0xFB, 0x68, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x2D, 0xC7, 0x57, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const arm_2d_tile_t checkBoxCircleMask = {
    .tRegion = {
        .tSize = {
            .iWidth = 6,
            .iHeight = 6,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_8BIT,
        },
    },
    .pchBuffer = (uint8_t *)checkBoxDot_png,
};

const arm_2d_tile_t checkBoxCircleSmallMask = {
    .tRegion = {
        .tSize = {
            .iWidth = 4,
            .iHeight = 4,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_8BIT,
        },
    },
    .pchBuffer = (uint8_t *)checkBoxDotSmall_png,
};

const arm_2d_tile_t checkBoxCheckedMask = {
    .tRegion = {
        .tSize = {
            .iWidth = 10,
            .iHeight = 10,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_8BIT,
        },
    },
    .pchBuffer = (uint8_t *)checked_png,
};

static bool _checkBoxDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        if(((ldCheckBox_t*)pTarget)->ptTextInfo!=NULL)
        {
            ldBaseTextDel(((ldCheckBox_t*)pTarget)->ptTextInfo);
        }
        ldFree(((ldCheckBox_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldCheckBoxDel(ldCheckBox_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeCheckBox)
    {
        return;
    }

    LOG_INFO("[checkBox] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _checkBoxDel);
    }
}

static bool slotCheckBoxToggle(xConnectInfo_t info)
{
    ldCheckBox_t *cb;
    cb=ldGetWidgetById(info.receiverId);
    if(info.signalType==SIGNAL_PRESS)
    {
        cb->isChecked=!cb->isChecked;
    }
    return false;
}

ldCheckBox_t *ldCheckBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    ldCheckBox_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldCheckBox_t);
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
        pNewWidget->widgetType = widgetTypeCheckBox;
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

        pNewWidget->isCorner=false;
        pNewWidget->isChecked=false;
        pNewWidget->bgColor=__RGB(255,255,255);
        pNewWidget->fgColor=__RGB(0,0,0);
        pNewWidget->textColor=__RGB(0,0,0);
        pNewWidget->checkedImgAddr=LD_ADDR_NONE;
        pNewWidget->uncheckedImgAddr=LD_ADDR_NONE;
        pNewWidget->ptTextInfo = NULL;
        pNewWidget->boxWidth=CHECK_BOX_SIZE;

        xConnect(nameId,SIGNAL_PRESS,nameId,slotCheckBoxToggle);

        LOG_INFO("[checkBox] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[checkBox] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldCheckBoxLoop(ldCheckBox_t *pWidget,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    arm_2d_tile_t *ptResTile=(arm_2d_tile_t*)&pWidget->resource;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        return;
    }

    arm_2d_container(ptParent,tTarget , &ptResTile->tRegion)
    {
        tTarget.tRegion.tLocation = ptResTile->tRegion.tLocation;

        if ((pWidget->checkedImgAddr==LD_ADDR_NONE)&&(pWidget->uncheckedImgAddr==LD_ADDR_NONE))//color
        {
            do{
                arm_2d_region_t tBoxRegion = {
                    .tLocation = {
                        .iX = 0,
                        .iY = (ptResTile->tRegion.tSize.iHeight-pWidget->boxWidth)/2,
                    },
                    .tSize = {
                        .iWidth=pWidget->boxWidth,
                        .iHeight=pWidget->boxWidth,
                    }
                };

                arm_2d_tile_t tChildTile;
                arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tChildTile, false);
                if(pWidget->isCorner)
                {
                    draw_round_corner_box(&tTarget,
                                          &tChildTile.tRegion,
                                          pWidget->fgColor,
                                          255,
                                          bIsNewFrame,
                                          &checkBoxCircleMask);

                    tChildTile.tRegion.tLocation.iX+=1;
                    tChildTile.tRegion.tLocation.iY+=1;
                    tChildTile.tRegion.tSize.iWidth-=2;
                    tChildTile.tRegion.tSize.iHeight-=2;
                    draw_round_corner_box(&tTarget,
                                          &tChildTile.tRegion,
                                          pWidget->bgColor,
                                          255,
                                          bIsNewFrame,
                                          &checkBoxCircleSmallMask);

                    tChildTile.tRegion.tLocation.iX+=1;
                    tChildTile.tRegion.tLocation.iY+=1;
                    tChildTile.tRegion.tSize.iWidth-=2;
                    tChildTile.tRegion.tSize.iHeight-=2;
                }
                else
                {
                    ldBaseColor(&tChildTile,pWidget->bgColor,255);
                    arm_2d_draw_box(&tTarget,&tChildTile.tRegion,1,pWidget->fgColor,255);

                    tChildTile.tRegion.tLocation.iX+=2;
                    tChildTile.tRegion.tLocation.iY+=2;
                    tChildTile.tRegion.tSize.iWidth-=4;
                    tChildTile.tRegion.tSize.iHeight-=4;
                }
                if(pWidget->isChecked)
                {
                    ldBaseMaskImage(&tChildTile,&checkBoxCheckedMask,pWidget->fgColor,255);
                }
            }while(0);
        }
        else
        {
            do{
#if USE_VIRTUAL_RESOURCE == 0
                arm_2d_tile_t srcTile=pWidget->resource;
#else
                arm_2d_vres_t srcTile=*(arm_2d_vres_t*)&pWidget->resource;
#endif
                ((arm_2d_tile_t*)&srcTile)->tRegion.tSize.iWidth=pWidget->boxWidth;
                ((arm_2d_tile_t*)&srcTile)->tRegion.tSize.iHeight=pWidget->boxWidth;

                //借用srcTile生成新tile
                ((arm_2d_tile_t*)&srcTile)->tRegion.tLocation.iX=0;
                ((arm_2d_tile_t*)&srcTile)->tRegion.tLocation.iY=(ptResTile->tRegion.tSize.iHeight-pWidget->boxWidth)/2;
                arm_2d_tile_t tChildTile;
                arm_2d_tile_generate_child(&tTarget, &((arm_2d_tile_t*)&srcTile)->tRegion, &tChildTile, false);

                if(pWidget->isChecked)
                {
                    ((arm_2d_tile_t*)&srcTile)->pchBuffer = (uint8_t *)pWidget->checkedImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)&srcTile)->pTarget=pWidget->checkedImgAddr;
#endif
                    ldBaseImage(&tChildTile,&srcTile,pWidget->isCheckedMask,255);
                }
                else
                {
                    ((arm_2d_tile_t*)&srcTile)->pchBuffer = (uint8_t *)pWidget->uncheckedImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)&srcTile)->pTarget=pWidget->uncheckedImgAddr;
#endif
                    ldBaseImage(&tChildTile,&srcTile,pWidget->isUncheckedMask,255);
                }
            }while(0);
        }
        arm_2d_op_wait_async(NULL);

        if(pWidget->ptTextInfo!=NULL)
        {
            //最后使用，不再生产中间变量，直接修改tTarget
            tTarget.tRegion.tLocation.iX+=pWidget->boxWidth+2;
            tTarget.tRegion.tSize.iWidth-=pWidget->boxWidth+2;

            ldBaseShowText(tTarget,pWidget->ptTextInfo,0);
            arm_2d_op_wait_async(NULL);
        }
    }
}

void ldCheckBoxSetHidden(ldCheckBox_t *pWidget,bool isHidden)
{
    ldBaseSetHidden((ldCommon_t*) pWidget,isHidden);
}

void ldCheckBoxSetColor(ldCheckBox_t* pWidget,ldColor bgColor,ldColor fgColor)
{
    pWidget->bgColor=bgColor;
    pWidget->fgColor=fgColor;
    pWidget->uncheckedImgAddr=LD_ADDR_NONE;
    pWidget->checkedImgAddr=LD_ADDR_NONE;
    pWidget->boxWidth=CHECK_BOX_SIZE;
}

void ldCheckBoxSetImage(ldCheckBox_t* pWidget,uint16_t boxWidth,uint32_t uncheckedImgAddr,bool isUncheckedMask,uint32_t checkedImgAddr,bool isCheckedMask)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->uncheckedImgAddr=uncheckedImgAddr;
    pWidget->checkedImgAddr=checkedImgAddr;
    pWidget->isUncheckedMask=isUncheckedMask;
    pWidget->isCheckedMask=isCheckedMask;
    pWidget->boxWidth=boxWidth;
}

void ldCheckBoxSetFont(ldCheckBox_t *pWidget,ldFontDict_t *pFontDict)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldBaseSetFont(&pWidget->ptTextInfo,pFontDict);
    pWidget->ptTextInfo->align=LD_ALIGN_LEFT;
}

void ldCheckBoxSetText(ldCheckBox_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldBaseSetText(&pWidget->ptTextInfo,pStr);
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
