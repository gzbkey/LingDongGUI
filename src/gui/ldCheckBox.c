/*
 * Copyright 2023-2024 Ou Jianbo (59935554@qq.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    ldCheckBox.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   复选框 + 单选功能，支持自定义图片和文字显示
 * @version 0.1
 * @date    2023-11-03
 */
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
        .tLocation = {
            .iX=0,
            .iY=0,
        },
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
        .tLocation = {
            .iX=0,
            .iY=0,
        },
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
        .tLocation = {
            .iX=0,
            .iY=0,
        },
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

struct {
    uint16_t nameId;
    uint8_t group;
}radioButtonValue;

static bool _checkBoxDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldFree(((ldCheckBox_t*)pTarget)->pStr);
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

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _checkBoxDel);
    }
}

static bool slotCheckBoxToggle(xConnectInfo_t info)
{
    ldCheckBox_t *cb;
    cb=ldBaseGetWidgetById(info.receiverId);
    if(info.signalType==SIGNAL_PRESS)
    {
        if(!cb->isRadioButton)
        {
            cb->isChecked=!cb->isChecked;
        }
        else
        {

            if(cb->isChecked==false)
            {
                cb->isChecked=true;
                radioButtonValue.group=cb->radioButtonGroup;
                radioButtonValue.nameId=cb->nameId;
            }
        }
    }
    return false;
}

/**
 * @brief   check box初始化函数
 * 
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @return  ldCheckBox_t*   新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-10
 */
ldCheckBox_t *ldCheckBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    ldCheckBox_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
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
        tResTile->pchBuffer = (uint8_t*)LD_ADDR_NONE;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        ((arm_2d_vres_t*)tResTile)->pTarget = LD_ADDR_NONE;
        ((arm_2d_vres_t*)tResTile)->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)tResTile)->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
        pNewWidget->isCorner=false;
        pNewWidget->isChecked=false;
        pNewWidget->bgColor=__RGB(255,255,255);
        pNewWidget->fgColor=__RGB(0,0,0);
        pNewWidget->charColor=__RGB(0,0,0);
        pNewWidget->checkedImgAddr=LD_ADDR_NONE;
        pNewWidget->uncheckedImgAddr=LD_ADDR_NONE;
        pNewWidget->pStr = NULL;
        pNewWidget->pFontDict = NULL;
        pNewWidget->align = 0;
        pNewWidget->boxWidth=CHECK_BOX_SIZE;
        pNewWidget->isRadioButton=false;
        pNewWidget->radioButtonGroup=0;
        pNewWidget->dirtyRegionListItem.ptNext=NULL;
        pNewWidget->dirtyRegionListItem.tRegion = ldBaseGetGlobalRegion(pNewWidget,&((arm_2d_tile_t*)&pNewWidget->resource)->tRegion);
        pNewWidget->dirtyRegionListItem.bIgnore = false;
        pNewWidget->dirtyRegionListItem.bUpdated = true;

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

/**
 * @brief   check box循环处理函数
 * 
 * @param   pWidget         目标控件指针
 * @param   pParentTile     父控件tile对象
 * @param   bIsNewFrame     新的一帧开始标志
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-10
 */
void ldCheckBoxLoop(ldCheckBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
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

    //自动清除radioButton选中状态
    if((pWidget->isChecked)&&(pWidget->isRadioButton))
    {
        if((pWidget->radioButtonGroup==radioButtonValue.group)&&(pWidget->nameId!=radioButtonValue.nameId))
        {
            pWidget->isChecked=false;
        }
    }

    ldBaseDirtyRegionAutoUpdate((ldCommon_t*)pWidget,&pResTile->tRegion,bIsNewFrame);
    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if ((pWidget->checkedImgAddr==LD_ADDR_NONE)&&(pWidget->uncheckedImgAddr==LD_ADDR_NONE))//color
        {
            do{
                arm_2d_region_t tBoxRegion = {
                    .tLocation = {
                        .iX = 0,
                        .iY = (pResTile->tRegion.tSize.iHeight-pWidget->boxWidth)/2,
                    },
                    .tSize = {
                        .iWidth=pWidget->boxWidth,
                        .iHeight=pWidget->boxWidth,
                    }
                };

                arm_2d_tile_t tChildTile;
                arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tChildTile, false);

                if((pWidget->isCorner)&&(!pWidget->isRadioButton))
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
                    if(pWidget->isRadioButton)
                    {
                        arm_2d_draw_box(&tTarget,&tChildTile.tRegion,5,pWidget->fgColor,255);
                    }
                    else
                    {
                        ldBaseMaskImage(&tChildTile,(arm_2d_tile_t*)&checkBoxCheckedMask,pWidget->fgColor,255);
                    }

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
                ((arm_2d_tile_t*)&srcTile)->tRegion.tLocation.iY=(pResTile->tRegion.tSize.iHeight-pWidget->boxWidth)/2;

                if(pWidget->isChecked)
                {
                    ((arm_2d_tile_t*)&srcTile)->pchBuffer = (uint8_t *)pWidget->checkedImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)&srcTile)->pTarget=pWidget->checkedImgAddr;
#endif
                    ldBaseImage(&tTarget,&srcTile,pWidget->isWithCheckedMask,255);
                }
                else
                {
                    ((arm_2d_tile_t*)&srcTile)->pchBuffer = (uint8_t *)pWidget->uncheckedImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)&srcTile)->pTarget=pWidget->uncheckedImgAddr;
#endif
                    ldBaseImage(&tTarget,&srcTile,pWidget->isWithUncheckedMask,255);
                }
            }while(0);
        }
        arm_2d_op_wait_async(NULL);

        if(pWidget->pStr!=NULL)
        {
            //最后使用，不再生产中间变量，直接修改tTarget
            tTarget.tRegion.tLocation.iX+=pWidget->boxWidth+2;
            tTarget.tRegion.tSize.iWidth-=pWidget->boxWidth+2;
            ldBaseLineText(&tTarget,&pWidget->resource,pWidget->pStr,pWidget->pFontDict,pWidget->align,pWidget->charColor,0,255);
            arm_2d_op_wait_async(NULL);
        }
    }
}

/**
 * @brief   设定颜色
 * 
 * @param   pWidget         目标控件指针
 * @param   bgColor         背景颜色
 * @param   fgColor         前景颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-10
 */
void ldCheckBoxSetColor(ldCheckBox_t* pWidget,ldColor bgColor,ldColor fgColor)
{
    pWidget->bgColor=bgColor;
    pWidget->fgColor=fgColor;
    pWidget->uncheckedImgAddr=LD_ADDR_NONE;
    pWidget->checkedImgAddr=LD_ADDR_NONE;
    pWidget->boxWidth=CHECK_BOX_SIZE;
}

/**
 * @brief   设定图片，只能设定方型图片
 * 
 * @param   pWidget          目标控件指针
 * @param   boxWidth         图片宽度
 * @param   uncheckedImgAddr 未选中的显示图片
 * @param   isUncheckedMask  未选中图片是否带透明度
 * @param   checkedImgAddr   选中的显示图片
 * @param   isCheckedMask    选中图片是否带透明度
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-10
 */
void ldCheckBoxSetImage(ldCheckBox_t* pWidget,uint16_t boxWidth,uint32_t uncheckedImgAddr,bool isUncheckedMask,uint32_t checkedImgAddr,bool isCheckedMask)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->uncheckedImgAddr=uncheckedImgAddr;
    pWidget->checkedImgAddr=checkedImgAddr;
    pWidget->isWithUncheckedMask=isUncheckedMask;
    pWidget->isWithCheckedMask=isCheckedMask;
    pWidget->boxWidth=boxWidth;
}

/**
 * @brief   设置字体
 * 
 * @param   pWidget         目标控件指针
 * @param   pFontDict       字体
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-10
 */
void ldCheckBoxSetFont(ldCheckBox_t *pWidget,ldFontDict_t *pFontDict)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->pFontDict=pFontDict;
    pWidget->align=LD_ALIGN_LEFT;
}

/**
 * @brief   设置显示文字
 * 
 * @param   pWidget         目标控件指针
 * @param   pStr            需要显示的字符串
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-10
 */
void ldCheckBoxSetText(ldCheckBox_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }
    ldFree(pWidget->pStr);
    pWidget->pStr=LD_MALLOC_STRING(pStr);
    strcpy((char*)pWidget->pStr,(char*)pStr);
}

/**
 * @brief   单选功能设定为同一组
 *          实现同一组的radio button自动单选
 * @param   pWidget         目标控件指针
 * @param   num             组号 0-255
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-10
 */
void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* pWidget,uint8_t num)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isRadioButton=true;
    pWidget->radioButtonGroup=num;
}

/**
 * @brief   实现圆角显示效果
 * 
 * @param   pWidget         目标控件指针
 * @param   isCorner        true=圆角 false=方角
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-10
 */
void ldCheckBoxSetCorner(ldCheckBox_t* pWidget,bool isCorner)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->isCorner=isCorner;
}

void ldCheckBoxSetCharColor(ldCheckBox_t* pWidget,ldColor charColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->charColor=charColor;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
