/*
 * Copyright (c) 2023-2024 Ou Jianbo (59935554@qq.com). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
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

#define __LD_CHECK_BOX_IMPLEMENT__
#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldCheckBox.h"

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

static struct {
    uint16_t nameId;
    uint8_t group;
}radioButtonValue;

const ldBaseWidgetFunc_t ldCheckBoxFunc = {
    .depose = (ldDeposeFunc_t)ldCheckBox_depose,
    .load = (ldLoadFunc_t)ldCheckBox_on_load,
    .frameStart = (ldFrameStartFunc_t)ldCheckBox_on_frame_start,
    .show = (ldShowFunc_t)ldCheckBox_show,
};

static bool slotCheckBoxToggle(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldCheckBox_t *ptWidget=msg.ptSender;
    if(msg.signal==SIGNAL_PRESS)
    {
        if(!ptWidget->isRadioButton)
        {
            ptWidget->isChecked=!ptWidget->isChecked;
            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        }
        else
        {
            if(ptWidget->isChecked==false)
            {
                ptWidget->isChecked=true;
                radioButtonValue.group=ptWidget->radioButtonGroup;
                radioButtonValue.nameId=ptWidget->use_as__ldBase_t.nameId;
                ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
            }
        }
    }
    return false;
}

ldCheckBox_t* ldCheckBox_init( ld_scene_t *ptScene,ldCheckBox_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldCheckBox_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][checkBox] id:%d", nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldCheckBox_t));
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeCheckBox;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldCheckBoxFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->bgColor=__RGB(255,255,255);
    ptWidget->fgColor=__RGB(0,0,0);
    ptWidget->charColor=__RGB(0,0,0);
    ptWidget->boxWidth=CHECK_BOX_SIZE;

    connect(nameId,SIGNAL_PRESS,slotCheckBoxToggle);

    LOG_INFO("[init][checkBox] id:%d", nameId);
    return ptWidget;
}

void ldCheckBox_depose( ldCheckBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeCheckBox)
    {
        return;
    }

    LOG_INFO("[depose][checkBox] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove(ptWidget->ptScene->ptNodeRoot,(arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget->pStr);
    ldFree(ptWidget);
}

void ldCheckBox_on_load( ldCheckBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldCheckBox_on_frame_start( ldCheckBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldCheckBox_show(ld_scene_t *ptScene, ldCheckBox_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

#if 0
    if (bIsNewFrame) {
        
    }
#endif
    if(arm_2d_helper_pfb_is_region_active(ptTile,&ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            //自动清除radioButton选中状态
            if((ptWidget->isChecked)&&(ptWidget->isRadioButton))
            {
                if((ptWidget->radioButtonGroup==radioButtonValue.group)&&(ptWidget->use_as__ldBase_t.nameId!=radioButtonValue.nameId))
                {
                    ptWidget->isChecked=false;
                    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
                }
            }

            arm_2d_region_t tBoxRegion = {
                .tLocation = {
                    .iX = 0,
                    .iY = (ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-ptWidget->boxWidth)/2,
                },
                .tSize = {
                    .iWidth=ptWidget->boxWidth,
                    .iHeight=ptWidget->boxWidth,
                }
            };
            arm_2d_tile_t tChildTile;
            arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tChildTile, false);

            if ((ptWidget->ptCheckedImgTile==NULL)&&(ptWidget->ptUncheckedImgTile==NULL))//color
            {

                    if((ptWidget->isCorner)&&(!ptWidget->isRadioButton))
                    {
                        draw_round_corner_box(&tTarget,
                                              &tChildTile.tRegion,
                                              ptWidget->fgColor,
                                              ptWidget->use_as__ldBase_t.opacity,
                                              bIsNewFrame,
                                              &checkBoxCircleMask);

                        tChildTile.tRegion.tLocation.iX+=1;
                        tChildTile.tRegion.tLocation.iY+=1;
                        tChildTile.tRegion.tSize.iWidth-=2;
                        tChildTile.tRegion.tSize.iHeight-=2;
                        draw_round_corner_box(&tTarget,
                                              &tChildTile.tRegion,
                                              ptWidget->bgColor,
                                              ptWidget->use_as__ldBase_t.opacity,
                                              bIsNewFrame,
                                              &checkBoxCircleSmallMask);
                    }
                    else
                    {
                        ldBaseColor(&tChildTile,
                                    NULL,
                                    ptWidget->bgColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                        arm_2d_draw_box(&tTarget,
                                        &tChildTile.tRegion,
                                        1,
                                        ptWidget->fgColor,
                                        ptWidget->use_as__ldBase_t.opacity);
                    }

                    if(ptWidget->isChecked)
                    {
                        tBoxRegion.tLocation.iX+=2;
                        tBoxRegion.tLocation.iY+=2;
                        tBoxRegion.tSize=checkBoxCheckedMask.tRegion.tSize;
                        arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tChildTile, false);

                        if(ptWidget->isRadioButton)
                        {
                            arm_2d_draw_box(&tTarget,
                                            &tChildTile.tRegion,
                                            5,
                                            ptWidget->fgColor,
                                            ptWidget->use_as__ldBase_t.opacity);
                        }
                        else
                        {
                            ldBaseImage(&tTarget,
                                        &tChildTile.tRegion,
                                        NULL,
                                        (arm_2d_tile_t*)&checkBoxCheckedMask,
                                        ptWidget->fgColor,
                                        ptWidget->use_as__ldBase_t.opacity);
                        }

                    }

                    if(ptWidget->pStr!=NULL)
                    {
                        tBoxRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;
                        tBoxRegion.tLocation.iX=ptWidget->boxWidth+2;
                        tBoxRegion.tLocation.iY=0;
                        tBoxRegion.tSize.iWidth-=tBoxRegion.tLocation.iX;

                        arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tChildTile, false);

                        ldBaseLabel(&tTarget,
                                    &tChildTile.tRegion,
                                    ptWidget->pStr,
                                    ptWidget->ptFont,
                                    ARM_2D_ALIGN_LEFT,
                                    ptWidget->charColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                        arm_2d_op_wait_async(NULL);
                    }
            }
            else
            {
                do{
                    if(ptWidget->isChecked)
                    {
                        ldBaseImage(&tTarget,
                                    &tChildTile.tRegion,
                                    ptWidget->ptCheckedImgTile,
                                    NULL,
                                    0,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
                    else
                    {
                        ldBaseImage(&tTarget,
                                    &tChildTile.tRegion,
                                    ptWidget->ptUncheckedImgTile,
                                    NULL,
                                    0,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
                }while(0);
            }
            arm_2d_op_wait_async(NULL);


        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldCheckBoxSetColor(ldCheckBox_t* ptWidget,ldColor bgColor,ldColor fgColor)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->bgColor=bgColor;
    ptWidget->fgColor=fgColor;
    ptWidget->ptCheckedImgTile=NULL;
    ptWidget->ptUncheckedImgTile=NULL;
    ptWidget->boxWidth=CHECK_BOX_SIZE;
}

void ldCheckBoxSetImage(ldCheckBox_t* ptWidget,uint16_t boxWidth,uintptr_t uncheckedImgAddr,bool isUncheckedMask,uintptr_t checkedImgAddr,bool isCheckedMask)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptUncheckedImgTile=uncheckedImgAddr;
    ptWidget->ptCheckedImgTile=checkedImgAddr;
    ptWidget->isWithUncheckedMask=isUncheckedMask;
    ptWidget->isWithCheckedMask=isCheckedMask;
    ptWidget->boxWidth=boxWidth;
}

void ldCheckBoxSetText(ldCheckBox_t* ptWidget,arm_2d_font_t *ptFont,uint8_t *pStr)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ldFree(ptWidget->pStr);
    ptWidget->pStr=ldCalloc(1,strlen((char*)pStr)+1);
    strcpy((char*)ptWidget->pStr,(char*)pStr);
    ptWidget->ptFont=ptFont;
}

void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* ptWidget,uint8_t num)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->isRadioButton=true;
    ptWidget->radioButtonGroup=num;
}

void ldCheckBoxSetCorner(ldCheckBox_t* ptWidget,bool isCorner)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isCorner=isCorner;
}

void ldCheckBoxSetCharColor(ldCheckBox_t* ptWidget,ldColor charColor)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->charColor=charColor;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif