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
 * @file    keyboard.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   键盘控件，可以切换数字键盘和字母键盘
 * @version 0.1
 * @date    2023-11-23
 * @signal  SIGNAL_INPUT_ASCII
 */

#include "ldKeyboard.h"
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

#define KB_SPACE    5
#define KB_ASCII_RELEASE_COLOR     LD_COLOR_WHITE
#define KB_ASCII_PRESS_COLOR     __RGB(188,191,206)
#define KB_OTHER_RELEASE_COLOR     __RGB(168,176,189)
#define KB_OTHER_PRESS_COLOR     LD_COLOR_WHITE

#define KB_VALUE_NONE         0
#define KB_VALUE_QWERTY_MODE  1
#define KB_VALUE_NUMBER_MODE  2
#define KB_VALUE_SHIFT        3

static ldColor _shiftColor[3]={LD_COLOR_WHITE,LD_COLOR_BLACK,LD_COLOR_BLUE};

static bool _keyboardDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        //del user object

        ldFree(((ldKeyboard_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldKeyboardDel(ldKeyboard_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeKeyboard)
    {
        return;
    }

    LOG_INFO("[keyboard] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _keyboardDel);
    }
}

static arm_2d_region_t _keyboardGetClickRegion(ldKeyboard_t *pWidget)
{
    arm_2d_region_t retRegion;
    retRegion=((arm_2d_tile_t*)&pWidget->resource)->tRegion;
    retRegion.tLocation.iX=0;
    retRegion.tLocation.iY=0;
    int16_t btnW,btnH;
    if(pWidget->isNumber)
    {
        btnW=(LD_CFG_SCEEN_WIDTH-KB_SPACE)/4-KB_SPACE;
        btnH=(LD_CFG_SCEEN_HEIGHT/2-KB_SPACE)/4-KB_SPACE;

        arm_2d_region_t leftRegion=retRegion;
        leftRegion.tSize.iWidth=leftRegion.tSize.iWidth-btnW-(KB_SPACE*2);

        arm_2d_region_t rightRegion=retRegion;
        rightRegion.tLocation.iX=rightRegion.tSize.iWidth-btnW-(KB_SPACE*2);
        rightRegion.tSize.iWidth=btnW+(KB_SPACE*2);

        arm_2d_layout(leftRegion) {
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }



            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }


            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }


            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,KB_SPACE) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,KB_SPACE) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE) {
                if((gActiveEditType==typeFloat)||(gActiveEditType==typeString))
                {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        retRegion=__item_region;
                        break;
                    }
                }
            }
        }

        arm_2d_layout(rightRegion) {
            __item_vertical(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_vertical(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                if(gActiveEditType==typeString)
                {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        retRegion=__item_region;
                        break;
                    }
                }
            }
            __item_vertical(btnW,btnH*2+KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
        }
    }
    else
    {
        btnW=(LD_CFG_SCEEN_WIDTH-KB_SPACE)/10-KB_SPACE;
        btnH=(LD_CFG_SCEEN_HEIGHT/2-KB_SPACE)/4-KB_SPACE;

        arm_2d_layout(retRegion) {
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }





            __item_horizontal(btnW,btnH,KB_SPACE+(btnW+KB_SPACE)/2,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }




            __item_horizontal(btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }



            __item_horizontal(btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW+(btnW+KB_SPACE)*2,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
            __item_horizontal(btnW+(btnW+KB_SPACE),btnH,KB_SPACE,0,KB_SPACE,0) {
                if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                    retRegion=__item_region;
                    break;
                }
            }
        }
    }
    return retRegion;
}
static bool slotKBProcess(xConnectInfo_t info)
{
    ldKeyboard_t *pWidget;
    arm_2d_tile_t *pResTile;

    pWidget=ldBaseGetWidgetById(info.receiverId);

    pResTile=(arm_2d_tile_t*)&pWidget->resource;

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        ldPoint_t parentPos=ldBaseGetGlobalPos(pWidget->parentWidget);

        pWidget->clickPoint.iX=(int16_t)GET_SIGNAL_VALUE_X(info.value);
        pWidget->clickPoint.iY=(int16_t)GET_SIGNAL_VALUE_Y(info.value);

        pWidget->clickPoint.iX-=(pResTile->tRegion.tLocation.iX+parentPos.x);
        pWidget->clickPoint.iY-=(pResTile->tRegion.tLocation.iY+parentPos.y);

        pWidget->isClick=false;

        pWidget->targetDirtyRegion=_keyboardGetClickRegion(pWidget);
        pWidget->targetDirtyRegion.tLocation.iX+=pResTile->tRegion.tLocation.iX;
        pWidget->targetDirtyRegion.tLocation.iY+=pResTile->tRegion.tLocation.iY;
        pWidget->dirtyRegionState=waitChange;
        pWidget->isDirtyRegionIgnore=false;

        LOG_DEBUG("%d,%d\n",pResTile->tRegion.tLocation.iY,pWidget->clickPoint.iY);
LOG_REGION("=======",pWidget->targetDirtyRegion);
        break;
    }
    case SIGNAL_RELEASE:
    {
        pWidget->clickPoint.iX=-1;
        pWidget->clickPoint.iY=-1;
        pWidget->isClick=true;
        xEmit(pWidget->nameId,SIGNAL_INPUT_ASCII,pWidget->kbValue);
        pWidget->dirtyRegionState=waitChange;
        pWidget->isDirtyRegionIgnore=true;
    }
    default:
        break;
    }
    return false;
}

/**
 * @brief   键盘初始化
 * 
 * @param   nameId          目标控件指针
 * @param   parentNameId    父控件id
 * @param   pFontDict       字体指针
 * @return  ldKeyboard_t*   新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-23
 */
ldKeyboard_t *ldKeyboardInit(uint16_t nameId, uint16_t parentNameId,ldFontDict_t *pFontDict)
{
    ldKeyboard_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldKeyboard_t);
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
        pNewWidget->widgetType = widgetTypeKeyboard;
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isHidden = true;
        tResTile=(arm_2d_tile_t*)&pNewWidget->resource;
        tResTile->tRegion.tLocation.iX=0;
        tResTile->tRegion.tLocation.iY=LD_CFG_SCEEN_HEIGHT/2;
        tResTile->tRegion.tSize.iWidth=LD_CFG_SCEEN_WIDTH;
        tResTile->tRegion.tSize.iHeight=LD_CFG_SCEEN_HEIGHT/2;
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
        pNewWidget->isNumber=false;
        pNewWidget->pFontDict=pFontDict;
        pNewWidget->clickPoint.iX=-1;
        pNewWidget->clickPoint.iY=-1;
        pNewWidget->isClick=false;
        pNewWidget->upperState=0;
        pNewWidget->dirtyRegionListItem.ptNext=NULL;
        pNewWidget->dirtyRegionListItem.tRegion = ldBaseGetGlobalRegion((ldCommon_t *)pNewWidget,&((arm_2d_tile_t*)&pNewWidget->resource)->tRegion);
        pNewWidget->dirtyRegionListItem.bIgnore = true;
        pNewWidget->dirtyRegionListItem.bUpdated = true;
        pNewWidget->dirtyRegionState=none;
        pNewWidget->dirtyRegionTemp=tResTile->tRegion;
        pNewWidget->targetDirtyRegion=tResTile->tRegion;
        pNewWidget->isDirtyRegionIgnore=true;

        xConnect(pNewWidget->nameId,SIGNAL_PRESS,pNewWidget->nameId,slotKBProcess);
        xConnect(pNewWidget->nameId,SIGNAL_RELEASE,pNewWidget->nameId,slotKBProcess);

        LOG_INFO("[keyboard] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[keyboard] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

static void _ldkeyboardNewButton(ldKeyboard_t *pWidget,arm_2d_tile_t *parentTile,arm_2d_region_t *pBtnRegion,uint8_t *pStr,ldColor btnColor,ldColor charColor,bool bIsNewFrame)
{
    arm_2d_tile_t btnTile;
    draw_round_corner_box(parentTile,pBtnRegion,btnColor,255,bIsNewFrame);
    arm_2d_tile_generate_child(parentTile,pBtnRegion,&btnTile,false);
    ldBaseLineText(&btnTile,&pWidget->resource,pStr,pWidget->pFontDict,LD_ALIGN_CENTER,charColor,0,255);
}

/**
 * @brief   键盘显示处理函数
 * 
 * @param   pWidget         目标控件指针
 * @param   pParentTile     父控件tile对象
 * @param   bIsNewFrame     新的一帧开始标志
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-23
 */
void ldKeyboardLoop(ldKeyboard_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;
    ldColor btnColor;
    uint16_t btnW,btnH;
    uint8_t letterOffset=0;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        //强制脏矩阵全控件
        if(pResTile->tRegion.tSize.iWidth!=pWidget->dirtyRegionListItem.tRegion.tSize.iWidth)
        {
            pWidget->dirtyRegionListItem.tRegion = ldBaseGetGlobalRegion((ldCommon_t *)pWidget,&((arm_2d_tile_t*)&pWidget->resource)->tRegion);
            pWidget->dirtyRegionTemp=pResTile->tRegion;
            pWidget->targetDirtyRegion=pResTile->tRegion;
        }
        return;
    }

    if(((gActiveEditType==typeInt)||(gActiveEditType==typeFloat))&&(pWidget->isNumber==false))
    {
        pWidget->isNumber=true;
    }

    if(bIsNewFrame&&pWidget->isClick)
    {
        pWidget->isClick=false;

        if (pWidget->kbValue==KB_VALUE_NUMBER_MODE)
        {
            pWidget->isNumber=true;
        }
        if (pWidget->kbValue==KB_VALUE_QWERTY_MODE)
        {
            pWidget->isNumber=false;
        }

        if(!pWidget->isNumber)
        {
            if (pWidget->kbValue==KB_VALUE_SHIFT)
            {
                pWidget->upperState++;
                if(pWidget->upperState>=3)
                {
                    pWidget->upperState=0;
                }
            }
            else
            {
                if((pWidget->upperState==1)&&(((pWidget->kbValue>='a')&&(pWidget->kbValue<='z'))||((pWidget->kbValue>='A')&&(pWidget->kbValue<='Z'))))
                {
                    pWidget->upperState=0;
                }
            }
        }

        pWidget->kbValue=KB_VALUE_NONE;
    }

    ldBaseDirtyRegionAutoUpdate((ldCommon_t*)pWidget,&pWidget->targetDirtyRegion,pWidget->isDirtyRegionIgnore,bIsNewFrame);
    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        ldBaseColor(&tTarget,__RGB(208,211,220),255);

        if(pWidget->isNumber)
        {
            btnW=(LD_CFG_SCEEN_WIDTH-KB_SPACE)/4-KB_SPACE;
            btnH=(LD_CFG_SCEEN_HEIGHT/2-KB_SPACE)/4-KB_SPACE;

            arm_2d_region_t leftRegion=tTarget_canvas;

            leftRegion.tSize.iWidth=leftRegion.tSize.iWidth-btnW-(KB_SPACE*2);

            arm_2d_layout(leftRegion) {
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='1';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"1",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='2';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"2",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='3';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"3",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }



                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='4';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"4",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='5';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"5",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='6';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"6",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }


                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='7';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"7",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='8';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"8",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='9';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"9",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }


                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,KB_SPACE) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='-';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"±",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,KB_SPACE) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='0';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"0",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE) {
                    if((gActiveEditType==typeFloat)||(gActiveEditType==typeString))
                    {
                        if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                            btnColor=KB_ASCII_PRESS_COLOR;
                            pWidget->kbValue='.';
                        }else{
                            btnColor=KB_ASCII_RELEASE_COLOR;
                        }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)".",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                    }
                }
            }

            arm_2d_region_t rightRegion=tTarget_canvas;

            rightRegion.tLocation.iX=rightRegion.tSize.iWidth-btnW-(KB_SPACE*2);
            rightRegion.tSize.iWidth=btnW+(KB_SPACE*2);

            arm_2d_layout(rightRegion) {
                __item_vertical(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=0x08;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"<-",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_vertical(btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0) {
                    if(gActiveEditType==typeString)
                    {
                        if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                            btnColor=KB_OTHER_PRESS_COLOR;
                            pWidget->kbValue=KB_VALUE_QWERTY_MODE;
                        }else{
                            btnColor=KB_OTHER_RELEASE_COLOR;
                        }
                        _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"ABC",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                    }
                }
                __item_vertical(btnW,btnH*2+KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=0x0d;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"Enter",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
            }
        }
        else
        {
            btnW=(LD_CFG_SCEEN_WIDTH-KB_SPACE)/10-KB_SPACE;
            btnH=(LD_CFG_SCEEN_HEIGHT/2-KB_SPACE)/4-KB_SPACE;

            if(pWidget->upperState==0)
            {
                letterOffset=0;
            }
            else
            {
                letterOffset=32;
            }
            uint8_t charBuf[2]={0};

            arm_2d_layout(tTarget_canvas) {
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='q'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='w'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='e'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='r'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='t'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='y'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='u'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='i'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='o'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='p'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }





                __item_horizontal(btnW,btnH,KB_SPACE+(btnW+KB_SPACE)/2,0,KB_SPACE,0) {
                    charBuf[0]='a'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='s'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='d'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='f'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='g'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='h'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='j'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='k'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='l'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }




                __item_horizontal(btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=KB_VALUE_SHIFT;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"shift",btnColor,_shiftColor[pWidget->upperState],bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='z'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='x'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='c'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='v'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='b'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='n'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='m'-letterOffset;
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=0x08;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"<-",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }



                __item_horizontal(btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"!@",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=KB_VALUE_NUMBER_MODE;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"123",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='.';
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW+(btnW+KB_SPACE)*2,btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=' ';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"space",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW,btnH,KB_SPACE,0,KB_SPACE,0) {
                    charBuf[0]='?';
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"?",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
                __item_horizontal(btnW+(btnW+KB_SPACE),btnH,KB_SPACE,0,KB_SPACE,0) {
                    if(arm_2d_is_point_inside_region(&__item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=0x0d;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&__item_region,(uint8_t*)"Enter",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
            }
        }

        arm_2d_op_wait_async(NULL);
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
