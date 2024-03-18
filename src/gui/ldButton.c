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
 * @file    ldButton.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   button widget
 * @signal  SIGNAL_PRESS
 *          SIGNAL_RELEASE
 */
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

void ldButtonDel(ldButton_t *pWidget);
void ldButtonFrameUpdate(ldButton_t* pWidget);
void ldButtonLoop(arm_2d_scene_t *pScene,ldButton_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
const ldGuiCommonFunc_t ldButtonCommonFunc={
    (ldDelFunc_t)ldButtonDel,
    (ldLoopFunc_t)ldButtonLoop,
    (ldUpdateFunc_t)ldButtonFrameUpdate,
};

static bool _buttonDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldFree(((ldButton_t*)pTarget)->pStr);
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

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _buttonDel);
    }
}

static bool slotButtonToggle(xConnectInfo_t info)
{
    ldButton_t *pWidget;

    pWidget=ldBaseGetWidgetById(info.receiverId);

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        pWidget->isPressed=true;
        pWidget->dirtyRegionState=waitChange;
        break;
    }
    case SIGNAL_RELEASE:
    {
        pWidget->isPressed=false;
        pWidget->dirtyRegionState=waitChange;
        break;
    }
    default:
        break;
    }

    return false;
}

/**
 * @brief   button初始化函数
 * 
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @return  ldButton_t*     新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
ldButton_t* ldButtonInit(arm_2d_scene_t *pScene,uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height)
{
    ldButton_t * pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList;
    arm_2d_tile_t *tResTile;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
    pNewWidget = LD_CALLOC_WIDGET_INFO(ldButton_t);
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
        pNewWidget->isWithReleaseMask = false;
        pNewWidget->isWithPressMask = false;
        pNewWidget->isTransparent=false;
        pNewWidget->isHidden = false;
        pNewWidget->pStr = NULL;
        pNewWidget->pFontDict = NULL;
        pNewWidget->align = 0;
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
        pNewWidget->dirtyRegionState=none;
        pNewWidget->pFunc=&ldButtonCommonFunc;

        arm_2d_user_dynamic_dirty_region_init(&pNewWidget->dirtyRegionListItem,pScene);

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

void ldButtonFrameUpdate(ldButton_t* pWidget)
{
    arm_2d_user_dynamic_dirty_region_on_frame_start(&pWidget->dirtyRegionListItem,waitChange);
}

void ldButtonLoop(arm_2d_scene_t *pScene,ldButton_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    uint32_t btnColor;
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tempRes=*pResTile;
#else
    arm_2d_vres_t tempRes=*((arm_2d_vres_t*)pResTile);
#endif
    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden)||(pWidget->isTransparent))
    {
        return;
    }

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
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
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->pressImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)(&tempRes))->pTarget=pWidget->pressImgAddr;
#endif
                    if(!pWidget->isCorner)
                    {
                        ldBaseImage(&tTarget,&tempRes,pWidget->isWithPressMask,255);
                    }
                }
                else
                {
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->releaseImgAddr;
#if USE_VIRTUAL_RESOURCE == 1
                    ((arm_2d_vres_t*)(&tempRes))->pTarget=pWidget->releaseImgAddr;
#endif
                    if(!pWidget->isCorner)
                    {
                        ldBaseImage(&tTarget,&tempRes,pWidget->isWithReleaseMask,255);
                    }
                }
                if(pWidget->isCorner)
                {
                    draw_round_corner_image((arm_2d_tile_t*)&tempRes,
                                            &tTarget,
                                            &((arm_2d_tile_t*)&tempRes)->tRegion,
                                            bIsNewFrame);
                }
            }
            arm_2d_op_wait_async(NULL);

            if(pWidget->pStr!=NULL)
            {
                ldBaseLineText(&tTarget,&pWidget->resource,pWidget->pStr,pWidget->pFontDict,pWidget->align,pWidget->charColor,0,255);
                arm_2d_op_wait_async(NULL);
            }

            if(pWidget->isSelected)
            {
                if (pWidget->selectMaskAddr==LD_ADDR_NONE)
                {
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
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)pWidget->selectMaskAddr;
                    ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme=ldBaseGetChScheme(pWidget->pFontDict->maskType);
#if USE_VIRTUAL_RESOURCE == 1
                    tempRes.pTarget=pWidget->selectMaskAddr;
#endif
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;
                    ldBaseMaskImage(&tTarget,(arm_2d_tile_t*)&tempRes,pWidget->selectColor,255);
                }
                arm_2d_op_wait_async(NULL);
            }
        }

        if(ldBaseDirtyRegionUpdate((ldCommon_t*)pWidget,&tTarget_canvas,&pWidget->dirtyRegionListItem,pWidget->dirtyRegionState))
        {
            pWidget->dirtyRegionState=none;
        }
    }
}

/**
 * @brief   设置按键显示文本
 * 
 * @param   pWidget         目标控件指针
 * @param   pStr            字符串指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetText(ldButton_t* pWidget,uint8_t *pStr)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    ldFree(pWidget->pStr);
    pWidget->pStr=LD_CALLOC_STRING(pStr);
    strcpy((char*)pWidget->pStr,(char*)pStr);
}

/**
 * @brief   设置按键颜色，设置该函数后，图片设置无效
 * 
 * @param   pWidget         目标控件指针
 * @param   releaseColor    松开显示的颜色
 * @param   pressColor      按下显示的颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetColor(ldButton_t* pWidget,ldColor releaseColor,ldColor pressColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->releaseColor=releaseColor;
    pWidget->pressColor=pressColor;
    pWidget->releaseImgAddr=LD_ADDR_NONE;
    pWidget->pressImgAddr=LD_ADDR_NONE;
}

/**
 * @brief   设置按键图片，设置该函数后，颜色设置无效
 * 
 * @param   pWidget         目标控件指针
 * @param   releaseImgAddr  松开显示的图片
 * @param   isReleaseMask   松开显示的图片是否带透明度(蒙版)
 * @param   pressImgAddr    按下显示的图片
 * @param   isPressMask     按下显示的图片是否带透明度(蒙版)
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetImage(ldButton_t* pWidget,uintptr_t releaseImgAddr,bool isReleaseMask,uintptr_t pressImgAddr,bool isPressMask)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->releaseImgAddr=releaseImgAddr;
    pWidget->pressImgAddr=pressImgAddr;
    pWidget->isWithReleaseMask=isReleaseMask;
    pWidget->isWithPressMask=isPressMask;
}

/**
 * @brief   选中按键的选中框(图片)
 * 
 * @param   pWidget         目标控件指针
 * @param   selectMaskAddr  选择效果显示的图片(蒙版)
 * @param   selectColor     显示的颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetSelectImage(ldButton_t* pWidget, uintptr_t selectMaskAddr, ldColor selectColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->selectMaskAddr=selectMaskAddr;
    pWidget->selectColor=selectColor;
}

/**
 * @brief   设置文字颜色
 * 
 * @param   pWidget         目标控件指针
 * @param   charColor       文字颜色
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetTextColor(ldButton_t* pWidget,ldColor charColor)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->charColor=charColor;
}

/**
 * @brief   按键设置为透明，则不显示，但按下有按键效果
 *          例如触摸某图片的左右两侧后，会切换图片，
 *          则可以使用功能两个透明按键放置图片顶层的两侧，
 *          即可实现该功能
 * @param   pWidget         目标控件指针
 * @param   isTransparent   true=透明 false=不透明
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetTransparent(ldButton_t* pWidget,bool isTransparent)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->isTransparent=isTransparent;
}

/**
 * @brief   按键实现圆角显示效果
 * 
 * @param   pWidget         目标控件指针
 * @param   isCorner        true=圆角 false=方角
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetRoundCorner(ldButton_t* pWidget,bool isCorner)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->isCorner=isCorner;
}

/**
 * @brief   选择按键，触发选中的显示效果
 * 
 * @param   pWidget         目标控件指针
 * @param   isSelected      true=选中 false=不选中
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetSelect(ldButton_t* pWidget,bool isSelected)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->isSelected=isSelected;
}

/**
 * @brief   文本的对齐方式
 * 
 * @param   pWidget         目标控件指针
 * @param   align           LD_ALIGN_CENTER
 *                          LD_ALIGN_TOP
 *                          LD_ALIGN_BOTTOM
 *                          LD_ALIGN_LEFT
 *                          LD_ALIGN_RIGHT
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetAlign(ldButton_t *pWidget,uint8_t align)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->align=align;
}

/**
 * @brief   设置字体
 * 
 * @param   pWidget         目标控件指针
 * @param   pFontDict       字体指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-09
 */
void ldButtonSetFont(ldButton_t *pWidget,ldFontDict_t *pFontDict)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->dirtyRegionState=waitChange;
    pWidget->pFontDict=pFontDict;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
