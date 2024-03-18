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
 * @file    ldComboBox.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   下拉框控件
 * @signal  SIGNAL_CLICKED_ITEM
 */

#include "ldComboBox.h"
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

#define COMBO_BOX_TEXT_LEFT_OFFSET   (8)

//dropDownV.png w:13 h:8 < A8 >
const uint8_t dropDownV_png[]={
0x3C, 0xB2, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0xB2, 0x3C,
0xA3, 0xDE, 0xD0, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0xD0, 0xDE, 0xA3,
0x1E, 0xD0, 0xDE, 0xD0, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0xD0, 0xDE, 0xD0, 0x1E,
0x00, 0x1E, 0xD0, 0xDE, 0xD0, 0x1E, 0x00, 0x1E, 0xD0, 0xDE, 0xD0, 0x1E, 0x00,
0x00, 0x00, 0x1E, 0xD0, 0xDE, 0xD0, 0xB2, 0xD0, 0xDE, 0xD0, 0x1E, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1E, 0xD0, 0xDE, 0xDE, 0xDE, 0xD0, 0x1E, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1E, 0xD0, 0xDE, 0xD0, 0x1E, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x2D, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void ldComboBoxDel(ldComboBox_t *pWidget);
void ldComboBoxFrameUpdate(ldComboBox_t* pWidget);
void ldComboBoxLoop(arm_2d_scene_t *pScene,ldComboBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
const ldGuiCommonFunc_t ldComboBoxCommonFunc={
    (ldDelFunc_t)ldComboBoxDel,
    (ldLoopFunc_t)ldComboBoxLoop,
    (ldUpdateFunc_t)ldComboBoxFrameUpdate,
};

static bool _comboBoxDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        for(uint8_t i=0;i<((ldComboBox_t *)pTarget)->itemCount;i++)
        {
            ldFree(((ldComboBox_t *)pTarget)->ppItemStrGroup[i]);
        }
        ldFree(((ldComboBox_t *)pTarget)->ppItemStrGroup);
        ldFree(((ldComboBox_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldComboBoxDel(ldComboBox_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeComboBox)
    {
        return;
    }

    LOG_INFO("[comboBox] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _comboBoxDel);
    }
}

static bool slotComboBoxProcess(xConnectInfo_t info)
{
    ldComboBox_t *pWidget;
    int16_t x,y;

#define SHOW_ITEM_NUM    (0)

    pWidget=ldBaseGetWidgetById(info.receiverId);

    x=(int16_t)GET_SIGNAL_VALUE_X(info.value);
    y=(int16_t)GET_SIGNAL_VALUE_Y(info.value);

    uint8_t clickItemNum=SHOW_ITEM_NUM;

    ldPoint_t globalPos=ldBaseGetGlobalPos((ldCommon_t *)pWidget);

    clickItemNum=((y-globalPos.y))/pWidget->itemHeight;

    switch (info.signalType) {
    case SIGNAL_PRESS:
    {
        if(clickItemNum==SHOW_ITEM_NUM)
        {
            pWidget->isExpand=!pWidget->isExpand;
        }
        else
        {
            pWidget->itemPreSelect=clickItemNum-1;
        }
        pWidget->dirtyRegionState=waitChange;
        break;
    }
    case SIGNAL_RELEASE:
    {
        if(clickItemNum!=SHOW_ITEM_NUM)
        {
            if(pWidget->itemSelect!=pWidget->itemPreSelect)
            {
                pWidget->itemSelect=pWidget->itemPreSelect;
                xEmit(pWidget->nameId,SIGNAL_CLICKED_ITEM,pWidget->itemSelect);
            }
            pWidget->isExpand=false;

        }
        pWidget->dirtyRegionState=waitChange;
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        if(clickItemNum!=SHOW_ITEM_NUM)
        {
            pWidget->itemPreSelect=clickItemNum-1;
            pWidget->dirtyRegionState=waitChange;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

/**
 * @brief   下拉框控件初始化
 * 
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @param   pFontDict       字体指针
 * @param   itemMax         项目最大数量
 * @return  ldComboBox_t*   新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-05
 */
ldComboBox_t *ldComboBoxInit(arm_2d_scene_t *pScene,uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t* pFontDict,uint8_t itemMax)
{
    ldComboBox_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    void **pNewStrGroup = NULL;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
    pNewWidget = LD_CALLOC_WIDGET_INFO(ldComboBox_t);
    pNewStrGroup=(void**)ldCalloc(sizeof (void*)*itemMax);
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
        pNewWidget->widgetType = widgetTypeComboBox;
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
        pNewWidget->itemHeight=height;
        pNewWidget->isExpand=false;
        pNewWidget->isCorner=true;
        pNewWidget->itemMax=itemMax;
        pNewWidget->itemCount=0;
        pNewWidget->itemSelect=0;
        pNewWidget->dropdownImgAddr=(uintptr_t)dropDownV_png;
        pNewWidget->dropdownImgWidth=13;
        pNewWidget->dropdownImgHeight=8;
        pNewWidget->pFontDict=pFontDict;
        pNewWidget->charColor=LD_COLOR_BLACK;
        pNewWidget->ppItemStrGroup=pNewStrGroup;
        pNewWidget->itemPreSelect=pNewWidget->itemSelect;
        for(uint8_t i=0;i<itemMax;i++)
        {
            pNewWidget->ppItemStrGroup[0]=NULL;
        }
        pNewWidget->pFunc=&ldComboBoxCommonFunc;

        arm_2d_user_dynamic_dirty_region_init(&pNewWidget->dirtyRegionListItem,pScene);

        xConnect(nameId,SIGNAL_PRESS,nameId,slotComboBoxProcess);
        xConnect(nameId,SIGNAL_RELEASE,nameId,slotComboBoxProcess);
        xConnect(nameId,SIGNAL_HOLD_DOWN,nameId,slotComboBoxProcess);

        LOG_INFO("[comboBox] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewStrGroup);
        ldFree(pNewWidget);

        LOG_ERROR("[comboBox] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

void ldComboBoxFrameUpdate(ldComboBox_t* pWidget)
{
    arm_2d_user_dynamic_dirty_region_on_frame_start(&pWidget->dirtyRegionListItem,waitChange);

    if(pWidget->dirtyRegionState==waitChange)
    {
        ((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight=pWidget->itemHeight*(pWidget->itemCount+1);
    }

    if(pWidget->isExpand)
    {
        ((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight=pWidget->itemHeight*(pWidget->itemCount+1);
    }
    else
    {
        ((arm_2d_tile_t*)&pWidget->resource)->tRegion.tSize.iHeight=pWidget->itemHeight;
    }
}

void ldComboBoxLoop(arm_2d_scene_t *pScene,ldComboBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
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

#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tempRes=*pResTile;
#else
    arm_2d_vres_t tempRes=*((arm_2d_vres_t*)pResTile);
#endif
    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if(ldBaseDirtyRegionUpdate((ldCommon_t*)pWidget,&tTarget_canvas,&pWidget->dirtyRegionListItem,pWidget->dirtyRegionState))
        {
            pWidget->dirtyRegionState=none;
        }

        arm_2d_region_t displayRegion={
            .tSize={
                .iWidth=tTarget_canvas.tSize.iWidth,
                .iHeight=pWidget->itemHeight,
            }
        };


        if(pWidget->isCorner)
        {
            draw_round_corner_box(&tTarget,&displayRegion,LD_COLOR_WHITE,255,bIsNewFrame);
            draw_round_corner_border(&tTarget,&displayRegion,LD_COLOR_LIGHT_GREY,(arm_2d_border_opacity_t){255,255,255,255},(arm_2d_corner_opacity_t){255,255,255,255});
        }
        else
        {
            ldBaseColor(&tTarget,LD_COLOR_WHITE,255);
            arm_2d_draw_box(&tTarget,&displayRegion,1,LD_COLOR_LIGHT_GREY,255);
        }

        ((arm_2d_tile_t*)(&tempRes))->tRegion.tLocation.iX=pResTile->tRegion.tSize.iWidth-pWidget->dropdownImgWidth-(pWidget->itemHeight-pWidget->dropdownImgWidth)/2;
        ((arm_2d_tile_t*)(&tempRes))->tRegion.tLocation.iY=(pWidget->itemHeight-pWidget->dropdownImgHeight)/2;
        ((arm_2d_tile_t*)(&tempRes))->tRegion.tSize.iWidth=pWidget->dropdownImgWidth;
        ((arm_2d_tile_t*)(&tempRes))->tRegion.tSize.iHeight=pWidget->dropdownImgHeight;
        ((arm_2d_tile_t*)(&tempRes))->pchBuffer = (uint8_t *)pWidget->dropdownImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
        ((arm_2d_vres_t*)&tempRes)->pTarget=pWidget->dropdownImgAddr;
#endif
        ((arm_2d_tile_t*)(&tempRes))->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR_MASK_A8;

        if(pWidget->dropdownImgAddr==(uintptr_t)dropDownV_png)
        {
            ((arm_2d_tile_t*)(&tempRes))->bVirtualResource=false;
        }

        ldBaseMaskImage(&tTarget,&tempRes,LD_COLOR_BLACK,255);

        arm_2d_op_wait_async(NULL);

        arm_2d_tile_t tempTile;
        arm_2d_tile_generate_child(&tTarget,&((arm_2d_region_t){COMBO_BOX_TEXT_LEFT_OFFSET,0,pResTile->tRegion.tSize.iWidth-pWidget->dropdownImgWidth-COMBO_BOX_TEXT_LEFT_OFFSET,pWidget->itemHeight}), &tempTile, false);

        ldBaseLineText(&tempTile,pResTile,pWidget->ppItemStrGroup[pWidget->itemSelect],pWidget->pFontDict,LD_ALIGN_LEFT,pWidget->charColor,0,255);
        arm_2d_op_wait_async(NULL);

        if(pWidget->isExpand)
        {
            arm_2d_tile_generate_child(&tTarget,
                                       &((arm_2d_region_t){0,
                                                           pWidget->itemHeight,
                                                           pResTile->tRegion.tSize.iWidth,
                                                           pWidget->itemHeight*pWidget->itemCount}),
                                       &tempTile,
                                       false);

            displayRegion.tSize.iHeight=tempTile.tRegion.tSize.iHeight;
            if(pWidget->isCorner)
            {
                draw_round_corner_box(&tempTile,&displayRegion,LD_COLOR_WHITE,255,bIsNewFrame);
                draw_round_corner_border(&tempTile,&displayRegion,LD_COLOR_LIGHT_GREY,(arm_2d_border_opacity_t){255,255,255,255},(arm_2d_corner_opacity_t){255,255,255,255});
            }
            else
            {
                ldBaseColor(&tempTile,LD_COLOR_WHITE,255);
                arm_2d_draw_box(&tempTile,&displayRegion,1,LD_COLOR_LIGHT_GREY,255);
            }

            arm_2d_tile_generate_child(&tTarget,
                                       &((arm_2d_region_t){COMBO_BOX_TEXT_LEFT_OFFSET,
                                                           pWidget->itemHeight,
                                                           pResTile->tRegion.tSize.iWidth,
                                                           pWidget->itemHeight}),
                                       &tempTile,
                                       false);

            ldColor lineColor;
            for(uint8_t i=0;i<pWidget->itemCount;i++)
            {
                ldBaseLineText(&tempTile,pResTile,pWidget->ppItemStrGroup[i],pWidget->pFontDict,LD_ALIGN_LEFT,pWidget->charColor,0,255);

                lineColor=LD_COLOR_LIGHT_GREY;
                if(pWidget->itemPreSelect==i)
                {
                    lineColor=LD_COLOR_LIGHT_BLUE;
                }
                ldBaseDrawLine(&tempTile,
                               0,
                               pWidget->itemHeight-4,
                               tempTile.tRegion.tSize.iWidth-COMBO_BOX_TEXT_LEFT_OFFSET*2,
                               pWidget->itemHeight-4,
                               3,lineColor,255,127);

                tempTile.tRegion.tLocation.iY+=pWidget->itemHeight;
            }



        }

#if LD_DEBUG == 1
        arm_2d_draw_box(&tTarget,&tTarget_canvas,1,0xFF,255);
#endif
        arm_2d_op_wait_async(NULL);
    }
}

/**
 * @brief   添加项目
 * 
 * @param   pWidget         目标控件指针
 * @param   pStr            项目显示字符串
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-05
 */
void ldComboBoxAddItem(ldComboBox_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    if(pWidget->itemCount<pWidget->itemMax)
    {
        if(pWidget->ppItemStrGroup[pWidget->itemCount]==NULL)
        {
            pWidget->ppItemStrGroup[pWidget->itemCount]=LD_CALLOC_STRING(pStr);
            strcpy(pWidget->ppItemStrGroup[pWidget->itemCount],(char*)pStr);
            pWidget->itemCount++;
        }
    }
}

/**
 * @brief   实现圆角显示效果
 *
 * @param   pWidget         目标控件指针
 * @param   isCorner        true=圆角 false=方角
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-05
 */
void ldComboBoxSetCorner(ldComboBox_t* pWidget,bool isCorner)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->isCorner=isCorner;
}

/**
 * @brief   设置下拉箭头mask图片
 *
 * @param   pWidget         目标控件指针
 * @param   maskAddr        下拉箭头mask图片地址
 * @param   width           mask图片宽度
 * @param   height          mask图片高度
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2024-03-11
 */
void ldComboBoxSetDropdownMask(ldComboBox_t* pWidget,uintptr_t maskAddr,uint8_t width,uint8_t height)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dropdownImgAddr=maskAddr;
    pWidget->dropdownImgWidth=width;
    pWidget->dropdownImgHeight=height;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
