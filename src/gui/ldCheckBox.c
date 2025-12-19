/*
 * Copyright (c) 2023-2025 Ou Jianbo (59935554@qq.com). All rights reserved.
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
#include "__common.h"
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

//static struct {
//    uint16_t nameId;
//    uint8_t group;
//}radioButtonValue;

const ldBaseWidgetFunc_t ldCheckBoxFunc = {
    .depose = (ldDeposeFunc_t)ldCheckBox_depose,
    .load = (ldLoadFunc_t)ldCheckBox_on_load,
    .frameStart  = (ldFrameStartFunc_t)ldCheckBox_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldCheckBox_on_frame_complete,
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
            ldMsgEmit(ptScene->ptMsgQueue,ptWidget,SIGNAL_VALUE_CHANGED,ptWidget->isChecked);
        }
        else
        {
            if(ptWidget->isChecked==false)
            {
                ldCheckBox_t *old=ldCheckBoxGetRadioSelected(ptWidget->radioButtonGroup);
                LOG_DEBUG("old cb=%d",old->use_as__ldBase_t.nameId);
                LOG_DEBUG("new cb=%d",ptWidget->use_as__ldBase_t.nameId);
                old->isChecked=false;
                old->use_as__ldBase_t.isDirtyRegionUpdate = true;

                ptWidget->isChecked=true;
//                radioButtonValue.group=ptWidget->radioButtonGroup;
//                radioButtonValue.nameId=ptWidget->use_as__ldBase_t.nameId;


//                ptWidget->isRadioReady=true;
                ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
                ldMsgEmit(ptScene->ptMsgQueue,ptWidget,SIGNAL_CLICKED_ITEM,(ptWidget->radioButtonGroup<<16)&0xFFFF+ptWidget->use_as__ldBase_t.nameId);
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
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->bgColor=__RGB(255,255,255);
    ptWidget->fgColor=__RGB(0,0,0);
    ptWidget->textColor=__RGB(0,0,0);
    ptWidget->boxWidth=CHECK_BOX_SIZE;

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotCheckBoxToggle);

    LOG_INFO("[init][checkBox] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    return ptWidget;
}

void ldCheckBox_depose(ld_scene_t *ptScene, ldCheckBox_t *ptWidget)
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
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
#if USE_VIRTUAL_RESOURCE == 1
    ldFree(ptWidget->ptUncheckedImgTile);
    ldFree(ptWidget->ptUncheckedMaskTile);
    ldFree(ptWidget->ptCheckedImgTile);
    ldFree(ptWidget->ptCheckedMaskTile);
    ldFree(ptWidget->ptFont);
#endif
    ldFree(ptWidget->pStr);
    ldFree(ptWidget);
}

void ldCheckBox_on_load(ld_scene_t *ptScene, ldCheckBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldCheckBox_on_frame_start(ld_scene_t *ptScene, ldCheckBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldCheckBox_on_frame_complete(ld_scene_t *ptScene, ldCheckBox_t *ptWidget)
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

    arm_2d_region_t globalRegion;
    arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t*)ptWidget,&globalRegion,true);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ldBaseIsHidden((ldBase_t*)ptWidget))
            {
                break;
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

            if ((ptWidget->ptCheckedImgTile==NULL)&&(ptWidget->ptUncheckedImgTile==NULL)&&(ptWidget->ptCheckedMaskTile==NULL)&&(ptWidget->ptUncheckedMaskTile==NULL))//color
            {

                    if((ptWidget->use_as__ldBase_t.isCorner)&&(!ptWidget->isRadioButton))
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
                    arm_2d_op_wait_async(NULL);

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
                        arm_2d_op_wait_async(NULL);
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
                                    ptWidget->textColor,
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
                                    NULL,//&tChildTile.tRegion,
                                    ptWidget->ptCheckedImgTile,
                                    ptWidget->ptCheckedMaskTile,
                                    0,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
                    else
                    {
                        ldBaseImage(&tTarget,
                                    NULL,//&tChildTile.tRegion,
                                    ptWidget->ptUncheckedImgTile,
                                    ptWidget->ptUncheckedMaskTile,
                                    0,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
                    arm_2d_op_wait_async(NULL);
                }while(0);

                if(ptWidget->pStr!=NULL)
                {
                    tBoxRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;
                    tBoxRegion.tLocation.iX=ptWidget->boxWidth;
                    tBoxRegion.tLocation.iY=0;
                    tBoxRegion.tSize.iWidth-=tBoxRegion.tLocation.iX;

                    arm_2d_tile_generate_child(&tTarget, &tBoxRegion, &tChildTile, false);

                    ldBaseLabel(&tTarget,
                                &tChildTile.tRegion,
                                ptWidget->pStr,
                                ptWidget->ptFont,
                                ARM_2D_ALIGN_LEFT,
                                ptWidget->textColor,
                                ptWidget->use_as__ldBase_t.opacity);
                    arm_2d_op_wait_async(NULL);
                }
            }

            LD_BASE_WIDGET_SELECT;

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
    ptWidget->ptUncheckedImgTile=NULL;
    ptWidget->ptUncheckedMaskTile=NULL;
    ptWidget->ptCheckedImgTile=NULL;
    ptWidget->ptCheckedMaskTile=NULL;
    ptWidget->boxWidth=CHECK_BOX_SIZE;
}

void ldCheckBoxSetImage(ldCheckBox_t* ptWidget,arm_2d_tile_t* ptUncheckedImgTile,arm_2d_tile_t* ptUncheckedMaskTile,arm_2d_tile_t* ptCheckedImgTile,arm_2d_tile_t* ptCheckedMaskTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptUncheckedImgTile=ptUncheckedImgTile;
    ptWidget->ptUncheckedMaskTile=ptUncheckedMaskTile;
    ptWidget->ptCheckedImgTile=ptCheckedImgTile;
    ptWidget->ptCheckedMaskTile=ptCheckedMaskTile;
}

void ldCheckBoxSetText(ldCheckBox_t* ptWidget,arm_2d_font_t *ptFont,uint8_t *pStr)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ldFree(ptWidget->pStr);
    ptWidget->pStr=ldCalloc(1,strlen((char*)pStr)+1);
    if(ptWidget->pStr!=NULL)
    {
        strcpy((char*)ptWidget->pStr,(char*)pStr);
    }
    ptWidget->ptFont=ptFont;
}

void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* ptWidget,uint8_t num)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->isRadioButton=true;
    ptWidget->radioButtonGroup=num;
}

ldCheckBox_t* _ldCheckBoxGetRadioSelected(ld_scene_t *ptScene, uint8_t groupNum)
{
    arm_ctrl_enum(ptScene->ptNodeRoot, ptItem, PREORDER_TRAVERSAL)
    {
        if (((ldBase_t *)ptItem)->widgetType == widgetTypeCheckBox)
        {
            ldCheckBox_t *cb=(ldCheckBox_t*)ptItem;
            if(cb->isRadioButton&&(cb->radioButtonGroup==groupNum)&&(cb->isChecked))
            {
                return cb;
            }
        }
    }
    return NULL;
}

void ldCheckBoxSetTextColor(ldCheckBox_t* ptWidget, ldColor textColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->textColor=textColor;
}

void _ldCheckBoxSetChecked(ld_scene_t *ptScene,ldCheckBox_t* ptWidget,bool isChecked)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }

    if(ptWidget->isChecked!=isChecked)
    {
        if(!ptWidget->isRadioButton)
        {
            ptWidget->isChecked=isChecked;
            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
            ldMsgEmit(ptScene->ptMsgQueue,ptWidget,SIGNAL_VALUE_CHANGED,ptWidget->isChecked);
        }
        else
        {
            if(ptWidget->isChecked==false)
            {
                ldCheckBox_t *old=ldCheckBoxGetRadioSelected(ptWidget->radioButtonGroup);
                old->isChecked=false;
                old->use_as__ldBase_t.isDirtyRegionUpdate = true;
                ptWidget->isChecked=true;
                ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
                ldMsgEmit(ptScene->ptMsgQueue,ptWidget,SIGNAL_CLICKED_ITEM,(ptWidget->radioButtonGroup<<16)&0xFFFF+ptWidget->use_as__ldBase_t.nameId);
            }
        }
    }
}

void ldCheckBoxSetStringLeftSpace(ldCheckBox_t* ptWidget,uint16_t space)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->boxWidth=space;
}

uint8_t* ldCheckBoxGetText(ldCheckBox_t* ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return NULL;
    }
    return ptWidget->pStr;
}

bool ldCheckBoxIsChecked(ldCheckBox_t* ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return NULL;
    }
    return ptWidget->isChecked;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
