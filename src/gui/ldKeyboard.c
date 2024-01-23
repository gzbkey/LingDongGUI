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
 * @file    ldKeyboard.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   键盘控件，可以切换数字键盘和字母键盘
 * @version 0.2
 * @date    2024-01-15
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

#define KB_SPACE                   5

#define KB_ASCII_RELEASE_COLOR     LD_COLOR_WHITE
#define KB_ASCII_PRESS_COLOR       __RGB(188,191,206)
#define KB_OTHER_RELEASE_COLOR     __RGB(168,176,189)
#define KB_OTHER_PRESS_COLOR       LD_COLOR_WHITE

#define KB_VALUE_NONE              0
#define KB_VALUE_QWERTY_MODE       1
#define KB_VALUE_NUMBER_MODE       2
#define KB_VALUE_SHIFT             3
#define KB_VALUE_SYMBOL_MODE       4

static ldColor _shiftColor[3]={LD_COLOR_WHITE,LD_COLOR_BLACK,LD_COLOR_BLUE};

void ldKeyboardDel(ldKeyboard_t *pWidget);
void ldKeyboardFrameUpdate(ldKeyboard_t* pWidget);
void ldKeyboardLoop(ldKeyboard_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
const ldGuiCommonFunc_t ldKeyboardCommonFunc={
    (ldDelFunc_t)ldKeyboardDel,
    (ldLoopFunc_t)ldKeyboardLoop,
    (ldUpdateFunc_t)ldKeyboardFrameUpdate,
};

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
    arm_2d_region_t retRegion={
        .tLocation={
            .iX=0,
            .iY=0,
        },
        .tSize={
            .iWidth=LD_CFG_SCEEN_WIDTH,
            .iHeight=LD_CFG_SCEEN_HEIGHT/2,
        },
    };

    int16_t btnW,btnH;
    arm_2d_region_t bufferRegion={0},item_region;

    if(pWidget->isNumber)
    {
        btnW=(LD_CFG_SCEEN_WIDTH-KB_SPACE)/4-KB_SPACE;
        btnH=(LD_CFG_SCEEN_HEIGHT/2-KB_SPACE)/4-KB_SPACE;

        bufferRegion.tLocation.iX=0;
        bufferRegion.tLocation.iY=0;
        bufferRegion.tSize.iWidth=0;
        bufferRegion.tSize.iHeight=0;

        arm_2d_region_t leftRegion=retRegion;
        leftRegion.tSize.iWidth=leftRegion.tSize.iWidth-btnW-(KB_SPACE*2);

        arm_2d_region_t rightRegion=retRegion;
        rightRegion.tLocation.iX=rightRegion.tSize.iWidth-btnW-(KB_SPACE*2);
        rightRegion.tSize.iWidth=btnW+(KB_SPACE*2);

        arm_2d_layout(leftRegion) {
            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }




            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }



            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }



            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,KB_SPACE);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,KB_SPACE);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

            item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE);
            if((pWidget->editType==typeFloat)||(pWidget->editType==typeString))
            {
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }
            }

        }

        bufferRegion.tLocation.iX=rightRegion.tLocation.iX;
        bufferRegion.tLocation.iY=0;
        bufferRegion.tSize.iWidth=0;
        bufferRegion.tSize.iHeight=0;

        arm_2d_layout(rightRegion) {
            item_region=ldLayoutVertical(&rightRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

            item_region=ldLayoutVertical(&rightRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
            if(pWidget->editType==typeString)
            {
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    //change ABC
                    retRegion=((arm_2d_tile_t*)&pWidget->resource)->tRegion;
                    retRegion.tLocation.iX=0;
                    retRegion.tLocation.iY=0;
                    break;
                }
            }

            item_region=ldLayoutVertical(&rightRegion,&bufferRegion,btnW,btnH*2+KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE,0);
            if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                retRegion=item_region;
                break;
            }

        }
    }
    else
    {
        btnW=(LD_CFG_SCEEN_WIDTH-KB_SPACE)/10-KB_SPACE;
        btnH=(LD_CFG_SCEEN_HEIGHT/2-KB_SPACE)/4-KB_SPACE;

        bufferRegion.tLocation.iX=0;
        bufferRegion.tLocation.iY=0;
        bufferRegion.tSize.iWidth=0;
        bufferRegion.tSize.iHeight=0;

        if(!pWidget->isSymbol)
        {
            arm_2d_layout(retRegion) {
                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }






                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE+(btnW+KB_SPACE)/2,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }





                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    //change shift
                    retRegion=((arm_2d_tile_t*)&pWidget->resource)->tRegion;
                    retRegion.tLocation.iX=0;
                    retRegion.tLocation.iY=0;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }




                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    //change symbol
                    retRegion=((arm_2d_tile_t*)&pWidget->resource)->tRegion;
                    retRegion.tLocation.iX=0;
                    retRegion.tLocation.iY=0;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    //change 123
                    retRegion=((arm_2d_tile_t*)&pWidget->resource)->tRegion;
                    retRegion.tLocation.iX=0;
                    retRegion.tLocation.iY=0;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW+(btnW+KB_SPACE)*2,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW+(btnW+KB_SPACE),btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }
            }

        }
        else
        {
            arm_2d_layout(retRegion) {
                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }





                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }




                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,(btnW<<1)+KB_SPACE,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    //change ABC
                    retRegion=((arm_2d_tile_t*)&pWidget->resource)->tRegion;
                    retRegion.tLocation.iX=0;
                    retRegion.tLocation.iY=0;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW*4+KB_SPACE*3,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

                item_region=ldLayoutHorizontal(&retRegion,&bufferRegion,btnW+(btnW+KB_SPACE),btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    retRegion=item_region;
                    break;
                }

            }
        }
    }
    retRegion.tLocation.iY+=LD_CFG_SCEEN_HEIGHT/2;
    retRegion.tLocation.iY+=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iY;

    return retRegion;
}

static bool _addAscii(ldKeyboard_t *pWidget,uint16_t textLen,uint8_t ascii,bool isBack)
{
    uint8_t *pBuf;
    if(pWidget->strMax==0)
    {
        if(textLen>0)
        {
            pBuf=ldCalloc(textLen+1);
            strcpy((char*)pBuf,(char*)(*pWidget->ppStr));
        }
        *pWidget->ppStr=ldRealloc(*pWidget->ppStr,textLen+2);
        if(textLen>0)
        {
            strcpy((char*)(*pWidget->ppStr),(char*)pBuf);
        }
    }
    if((textLen<pWidget->strMax)||(pWidget->strMax==0))
    {
        if(isBack)
        {
            xStringPushBack(*pWidget->ppStr,textLen,ascii);
        }
        else
        {
            uint8_t temp[2]={0};
            temp[0]=ascii;
            xStringInsert(*pWidget->ppStr,textLen,0,temp);
        }
        return true;
    }

    return false;
}

static void _inputAsciiProcess(ldKeyboard_t *pWidget,uint8_t ascii)
{
    uint16_t textLen;
    if((*pWidget->ppStr)!=NULL)
    {
        textLen=strlen((char*)(*pWidget->ppStr));
    }
    else
    {
        textLen=0;
    }

    if((pWidget->editType==typeInt)||(pWidget->editType==typeFloat))
    {
        switch (ascii)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        {
            _addAscii(pWidget,textLen,ascii,true);
            break;
        }
        case '.':
        {
            if((pWidget->editType==typeFloat)&&(strstr((char*)(*pWidget->ppStr),".")==NULL))
            {
                _addAscii(pWidget,textLen,ascii,true);
            }
            break;
        }
        case '-':
        {
            if((*pWidget->ppStr)[0]=='-')
            {
                xStringRemove(*pWidget->ppStr,textLen,0,1);
            }
            else
            {
                _addAscii(pWidget,textLen,ascii,false);
            }
            break;
        }
        default:
            break;
        }
    }
    else
    {
        if((ascii>=0x20)&&(ascii<0x7F))
        {
            _addAscii(pWidget,textLen,ascii,true);
        }
        else
        {

        }
    }

    switch (ascii) {
    case 0x08://backspace
    {
        xStringPopBack(*pWidget->ppStr,textLen);
        break;
    }
    case 0x0d://enter
    {
        ldBaseSetHidden((ldCommon_t*)pWidget,true);
        xEmit(0,SIGNAL_EDITING_FINISHED,0);
        ldBaseBgMove(0,0);
        pWidget->dirtyRegionState=waitChange;
        pWidget->isDirtyRegionAutoIgnore=true;
        break;
    }
    default:
        break;
    }
}

static bool slotKBProcess(xConnectInfo_t info)
{
    ldKeyboard_t *pWidget=ldBaseGetWidgetById(info.receiverId);
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    arm_2d_region_t kbRegion={
        .tLocation={
            .iX=0,
            .iY=LD_CFG_SCEEN_HEIGHT/2,
        },
        .tSize={
            .iWidth=LD_CFG_SCEEN_WIDTH,
            .iHeight=LD_CFG_SCEEN_HEIGHT/2,
        },
    };

    kbRegion.tLocation.iY+=pResTile->tRegion.tLocation.iY;

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        ldPoint_t parentPos=ldBaseGetGlobalPos(pWidget->parentWidget);
        pWidget->clickPoint.iX=(int16_t)GET_SIGNAL_VALUE_X(info.value);
        pWidget->clickPoint.iY=(int16_t)GET_SIGNAL_VALUE_Y(info.value);
        pWidget->clickPoint.iX-=(kbRegion.tLocation.iX+parentPos.x);
        pWidget->clickPoint.iY-=(kbRegion.tLocation.iY+parentPos.y);
        pWidget->isClick=false;
        pWidget->targetDirtyRegion=_keyboardGetClickRegion(pWidget);
        LOG_REGION("=====",pWidget->targetDirtyRegion);
        pWidget->dirtyRegionState=waitChange;
        break;
    }
    case SIGNAL_RELEASE:
    {
        pWidget->targetDirtyRegion=_keyboardGetClickRegion(pWidget);
        pWidget->isClick=true;
        pWidget->dirtyRegionState=waitChange;
        pWidget->clickPoint.iX=-1;
        pWidget->clickPoint.iY=-1;
        _inputAsciiProcess(pWidget,pWidget->kbValue);
    }
    default:
        break;
    }
    return true;
}

/**
 * @brief   键盘初始化
 * 
 * @param   nameId          目标控件指针
 * @param   pFontDict       字体指针
 * @return  ldKeyboard_t*   新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-23
 */
ldKeyboard_t *ldKeyboardInit(uint16_t nameId,ldFontDict_t *pFontDict)
{
    ldKeyboard_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldBaseGetWidgetInfoById(0);
    pNewWidget = LD_CALLOC_WIDGET_INFO(ldKeyboard_t);
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
        tResTile->tRegion.tLocation.iY=0;
        tResTile->tRegion.tSize.iWidth=LD_CFG_SCEEN_WIDTH;
        tResTile->tRegion.tSize.iHeight=LD_CFG_SCEEN_HEIGHT;
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
        pNewWidget->dirtyRegionListItem.tRegion = (arm_2d_region_t){0};//ldBaseGetGlobalRegion((ldCommon_t *)pNewWidget,&((arm_2d_tile_t*)&pNewWidget->resource)->tRegion);
        pNewWidget->dirtyRegionListItem.bIgnore = true;
        pNewWidget->dirtyRegionListItem.bUpdated = true;
        pNewWidget->dirtyRegionState=waitChange;
        pNewWidget->dirtyRegionTemp=tResTile->tRegion;
        pNewWidget->targetDirtyRegion=tResTile->tRegion;
        pNewWidget->isDirtyRegionAutoIgnore=true;
        pNewWidget->isWaitInit=true;
        pNewWidget->isSymbol=false;
        pNewWidget->pFunc=&ldKeyboardCommonFunc;

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

void ldKeyboardFrameUpdate(ldKeyboard_t* pWidget)
{
    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        if(pWidget->isWaitInit)
        {
            //强制脏矩阵覆盖控件
            pWidget->targetDirtyRegion=((arm_2d_tile_t*)&pWidget->resource)->tRegion;
            pWidget->isWaitInit=false;
        }
        return;
    }

    if(!pWidget->isWaitInit)
    {
        pWidget->isWaitInit=true;
    }

    if(pWidget->dirtyRegionState==waitChange)
    {
        pWidget->dirtyRegionTemp=pWidget->targetDirtyRegion;
    }
    ldBaseDirtyRegionAutoUpdate((ldCommon_t*)pWidget,pWidget->targetDirtyRegion,pWidget->isDirtyRegionAutoIgnore);
}

void ldKeyboardLoop(ldKeyboard_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;
    ldColor btnColor;
    uint16_t btnW,btnH;
    uint8_t letterOffset=0;
    arm_2d_region_t bufferRegion={0},item_region;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        return;
    }

    if(((pWidget->editType==typeInt)||(pWidget->editType==typeFloat))&&(pWidget->isNumber==false))
    {
        pWidget->isNumber=true;
    }

    if(bIsNewFrame&&pWidget->isClick)
    {
        pWidget->isClick=false;

        if (pWidget->kbValue==KB_VALUE_NUMBER_MODE)
        {
            pWidget->isNumber=true;
            pWidget->isSymbol=false;
        }
        if (pWidget->kbValue==KB_VALUE_QWERTY_MODE)
        {
            pWidget->isNumber=false;
            pWidget->isSymbol=false;
        }
        if (pWidget->kbValue==KB_VALUE_SYMBOL_MODE)
        {
            pWidget->isNumber=false;
            pWidget->isSymbol=true;
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

    arm_2d_region_t kbRegion={
        .tLocation={
            .iX=0,
            .iY=LD_CFG_SCEEN_HEIGHT/2,
        },
        .tSize={
            .iWidth=LD_CFG_SCEEN_WIDTH,
            .iHeight=LD_CFG_SCEEN_HEIGHT/2,
        },
    };
    kbRegion.tLocation.iY+=pResTile->tRegion.tLocation.iY;

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&kbRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        ldBaseColor(&tTarget,__RGB(208,211,220),255);

        if(pWidget->isNumber)
        {
            btnW=(LD_CFG_SCEEN_WIDTH-KB_SPACE)/4-KB_SPACE;
            btnH=(LD_CFG_SCEEN_HEIGHT/2-KB_SPACE)/4-KB_SPACE;

            bufferRegion.tLocation.iX=0;
            bufferRegion.tLocation.iY=0;
            bufferRegion.tSize.iWidth=0;
            bufferRegion.tSize.iHeight=0;

            arm_2d_region_t leftRegion=tTarget_canvas;

            leftRegion.tSize.iWidth=leftRegion.tSize.iWidth-btnW-(KB_SPACE*2);

            arm_2d_layout(leftRegion) {

                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='1';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"1",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='2';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"2",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='3';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"3",btnColor,LD_COLOR_BLACK,bIsNewFrame);




                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='4';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"4",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='5';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"5",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='6';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"6",btnColor,LD_COLOR_BLACK,bIsNewFrame);



                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='7';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"7",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='8';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"8",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='9';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"9",btnColor,LD_COLOR_BLACK,bIsNewFrame);



                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,KB_SPACE);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='-';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"±",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,KB_SPACE);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_ASCII_PRESS_COLOR;
                    pWidget->kbValue='0';
                }else{
                    btnColor=KB_ASCII_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"0",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                item_region=ldLayoutHorizontal(&leftRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE);
                if((pWidget->editType==typeFloat)||(pWidget->editType==typeString))
                {
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue='.';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)".",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }

            }

            arm_2d_region_t rightRegion=tTarget_canvas;

            rightRegion.tLocation.iX=rightRegion.tSize.iWidth-btnW-(KB_SPACE*2);
            rightRegion.tSize.iWidth=btnW+(KB_SPACE*2);

            bufferRegion.tLocation.iX=rightRegion.tLocation.iX;
            bufferRegion.tLocation.iY=0;
            bufferRegion.tSize.iWidth=0;
            bufferRegion.tSize.iHeight=0;

            arm_2d_layout(rightRegion) {
                item_region=ldLayoutVertical(&rightRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_OTHER_PRESS_COLOR;
                    pWidget->kbValue=0x08;
                }else{
                    btnColor=KB_OTHER_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"<-",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                item_region=ldLayoutVertical(&rightRegion,&bufferRegion,btnW,btnH,KB_SPACE,KB_SPACE,KB_SPACE,0);
                if(pWidget->editType==typeString)
                {
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=KB_VALUE_QWERTY_MODE;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"ABC",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }

                item_region=ldLayoutVertical(&rightRegion,&bufferRegion,btnW,btnH*2+KB_SPACE,KB_SPACE,KB_SPACE,KB_SPACE,0);
                if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                    btnColor=KB_OTHER_PRESS_COLOR;
                    pWidget->kbValue=0x0d;
                }else{
                    btnColor=KB_OTHER_RELEASE_COLOR;
                }
                _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"Enter",btnColor,LD_COLOR_BLACK,bIsNewFrame);

            }
        }
        else
        {
            btnW=(LD_CFG_SCEEN_WIDTH-KB_SPACE)/10-KB_SPACE;
            btnH=(LD_CFG_SCEEN_HEIGHT/2-KB_SPACE)/4-KB_SPACE;

            bufferRegion.tLocation.iX=0;
            bufferRegion.tLocation.iY=0;
            bufferRegion.tSize.iWidth=0;
            bufferRegion.tSize.iHeight=0;

            uint8_t charBuf[2]={0};

            if(!pWidget->isSymbol)
            {
                if(pWidget->upperState==0)
                {
                    letterOffset=0;
                }
                else
                {
                    letterOffset=32;
                }

                arm_2d_layout(tTarget_canvas) {
                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='q'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='w'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='e'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='r'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='t'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='y'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='u'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='i'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='o'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='p'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);






                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE+(btnW+KB_SPACE)/2,0,KB_SPACE,0);
                    charBuf[0]='a'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='s'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='d'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='f'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='g'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='h'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='j'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='k'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='l'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);





                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0);
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=KB_VALUE_SHIFT;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"Shift",btnColor,_shiftColor[pWidget->upperState],bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='z'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='x'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='c'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='v'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='b'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='n'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='m'-letterOffset;
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0);
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=0x08;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"<-",btnColor,LD_COLOR_BLACK,bIsNewFrame);




                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0);
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=KB_VALUE_SYMBOL_MODE;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"!@",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW+(btnW+KB_SPACE)/2,btnH,KB_SPACE,0,KB_SPACE,0);
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=KB_VALUE_NUMBER_MODE;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"123",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='.';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW+(btnW+KB_SPACE)*2,btnH,KB_SPACE,0,KB_SPACE,0);
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=' ';
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"Space",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='?';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"?",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW+(btnW+KB_SPACE),btnH,KB_SPACE,0,KB_SPACE,0);
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=0x0d;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"Enter",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                }
            }
            else
            {
                arm_2d_layout(tTarget_canvas) {
                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='-';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='/';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]=':';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]=';';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='(';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]=')';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='_';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='$';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='&';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='"';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);





                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='[';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]=']';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='{';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='}';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='#';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='%';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='^';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='*';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='+';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='=';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);




                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='\\';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='|';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='<';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='>';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='~';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]=',';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='@';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='!';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='`';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='\'';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,(btnW<<1)+KB_SPACE,btnH,KB_SPACE,0,KB_SPACE,0);
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=KB_VALUE_QWERTY_MODE;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t *)"ABC",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='.';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW*4+KB_SPACE*3,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]=' ';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t *)"Space",btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW,btnH,KB_SPACE,0,KB_SPACE,0);
                    charBuf[0]='?';
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_ASCII_PRESS_COLOR;
                        pWidget->kbValue=charBuf[0];
                    }else{
                        btnColor=KB_ASCII_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,charBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);

                    item_region=ldLayoutHorizontal(&tTarget_canvas,&bufferRegion,btnW+(btnW+KB_SPACE),btnH,KB_SPACE,0,KB_SPACE,0);
                    if(arm_2d_is_point_inside_region(&item_region,&pWidget->clickPoint)){
                        btnColor=KB_OTHER_PRESS_COLOR;
                        pWidget->kbValue=0x08;
                    }else{
                        btnColor=KB_OTHER_RELEASE_COLOR;
                    }
                    _ldkeyboardNewButton(pWidget,&tTarget,&item_region,(uint8_t*)"<-",btnColor,LD_COLOR_BLACK,bIsNewFrame);
                }
            }
        }
        arm_2d_op_wait_async(NULL);
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
