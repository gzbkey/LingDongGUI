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

#define __LD_COMBO_BOX_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldComboBox.h"

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

#define COMBO_BOX_TEXT_LEFT_OFFSET      (8)

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

const arm_2d_tile_t c_tile_dropDownV_Mask = {
    .tRegion = {
        .tSize = {
            .iWidth = 13,
            .iHeight = 8,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_8BIT,
        },
    },
    .pchBuffer = (uint8_t *)dropDownV_png,
};

const ldBaseWidgetFunc_t ldComboBoxFunc = {
    .depose = (ldDeposeFunc_t)ldComboBox_depose,
    .load = (ldLoadFunc_t)ldComboBox_on_load,
    .frameStart = (ldFrameStartFunc_t)ldComboBox_on_frame_start,
    .show = (ldShowFunc_t)ldComboBox_show,
};

static bool slotComboBoxProcess(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldComboBox_t *ptWidget=msg.ptSender;
    int16_t x,y;

#define SHOW_ITEM_NUM    (0)

    x=(int16_t)GET_SIGNAL_VALUE_X(msg.value);
    y=(int16_t)GET_SIGNAL_VALUE_Y(msg.value);

    uint8_t clickItemNum=SHOW_ITEM_NUM;

    arm_2d_location_t tLocation={0};

    tLocation = ldBaseGetAbsoluteLocation(ptWidget,tLocation);

    int16_t tempOffset=y-tLocation.iY;

    if((tempOffset<0)||(tempOffset>=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight))
    {
        clickItemNum=SHOW_ITEM_NUM;
    }
    else
    {
        clickItemNum=tempOffset/ptWidget->itemHeight;
    }


    switch (msg.signal) {
    case SIGNAL_PRESS:
    {
        if(clickItemNum==SHOW_ITEM_NUM)
        {
            ptWidget->isExpand=!ptWidget->isExpand;
        }
        else
        {
            ptWidget->itemPreSelect=clickItemNum-1;
        }
        ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;
        ptWidget->use_as__ldBase_t.tTempRegion.tSize.iHeight=ptWidget->itemHeight*(ptWidget->itemCount+1);
        if(ptWidget->isExpand)
        {
            ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight=ptWidget->use_as__ldBase_t.tTempRegion.tSize.iHeight;
        }
        else
        {
            ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight=ptWidget->itemHeight;
        }
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    case SIGNAL_RELEASE:
    {
        if(clickItemNum!=SHOW_ITEM_NUM)
        {
            if(ptWidget->itemSelect!=ptWidget->itemPreSelect)
            {
                ptWidget->itemSelect=ptWidget->itemPreSelect;
                ldMsgEmit(ptScene->ptMsgQueue,ptWidget,SIGNAL_CLICKED_ITEM,ptWidget->itemSelect);
            }
            ptWidget->isExpand=false;
        }
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        if(clickItemNum!=SHOW_ITEM_NUM)
        {
            ptWidget->itemPreSelect=clickItemNum-1;
            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

ldComboBox_t* ldComboBox_init( ld_scene_t *ptScene,ldComboBox_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldComboBox_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][comboBox] id:%d", nameId);
            return NULL;
        }
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

//    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeComboBox;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldComboBoxFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->itemHeight=height;
    ptWidget->isExpand=false;
    ptWidget->isCorner=true;
    ptWidget->ptFont=ptFont;
    ptWidget->textColor=GLCD_COLOR_BLACK;

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotComboBoxProcess);
    ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotComboBoxProcess);
    ldMsgConnect(ptWidget,SIGNAL_HOLD_DOWN,slotComboBoxProcess);

    LOG_INFO("[init][comboBox] id:%d", nameId);
    return ptWidget;
}

void ldComboBox_depose( ldComboBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeComboBox)
    {
        return;
    }

    LOG_INFO("[depose][comboBox] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldComboBox_on_load( ldComboBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldComboBox_on_frame_start( ldComboBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldComboBox_show(ld_scene_t *ptScene, ldComboBox_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
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
    arm_2d_region_t globalRegion=ldBaseGetAbsoluteRegion(ptWidget);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            arm_2d_region_t displayRegion={
                .tSize={
                    .iWidth=tTarget_canvas.tSize.iWidth,
                    .iHeight=ptWidget->itemHeight,
                }
            };

            if(ptWidget->isCorner)
            {
                draw_round_corner_box(&tTarget,&displayRegion,GLCD_COLOR_WHITE,255,bIsNewFrame);
                draw_round_corner_border(&tTarget,&displayRegion,GLCD_COLOR_LIGHT_GREY,(arm_2d_border_opacity_t){255,255,255,255},(arm_2d_corner_opacity_t){255,255,255,255});
            }
            else
            {
                ldBaseColor(&tTarget,NULL,GLCD_COLOR_WHITE,255);
                arm_2d_draw_box(&tTarget,&displayRegion,1,GLCD_COLOR_LIGHT_GREY,255);
            }

            arm_2d_tile_t tChildTile;
            arm_2d_tile_generate_child(&tTarget,
                                       &((arm_2d_region_t){ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-ptWidget->itemHeight,
                                                           0,
                                                           ptWidget->itemHeight,
                                                           ptWidget->itemHeight}),
                                       &tChildTile,
                                       false);

            if((NULL!=ptWidget->ptDropdownImgTile)||(NULL!=ptWidget->ptDropdownMaskTile))
            {
                arm_2d_align_centre(tChildTile.tRegion, ptWidget->ptDropdownImgTile->tRegion.tSize)
                {
                    ldBaseImage(&tTarget,&__centre_region,ptWidget->ptDropdownImgTile,ptWidget->ptDropdownMaskTile,GLCD_COLOR_BLACK,ptWidget->use_as__ldBase_t.opacity);
                }
            }
            else
            {
                arm_2d_align_centre(tChildTile.tRegion, c_tile_dropDownV_Mask.tRegion.tSize)
                {
                    ldBaseImage(&tTarget,&__centre_region,NULL,&c_tile_dropDownV_Mask,GLCD_COLOR_BLACK,ptWidget->use_as__ldBase_t.opacity);
                }
            }

            arm_2d_op_wait_async(NULL);

            if(ptWidget->ppItemStrGroup)
            {
                arm_2d_tile_t tempTile;
                arm_2d_tile_generate_child(&tTarget,
                                           &((arm_2d_region_t){COMBO_BOX_TEXT_LEFT_OFFSET,
                                                               0,
                                                               ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-ptWidget->itemHeight-COMBO_BOX_TEXT_LEFT_OFFSET,
                                                               ptWidget->itemHeight}),
                                           &tempTile,
                                           false);
                ldBaseLabel(&tTarget,&tempTile.tRegion,ptWidget->ppItemStrGroup[ptWidget->itemSelect],ptWidget->ptFont,ARM_2D_ALIGN_LEFT,ptWidget->textColor,ptWidget->use_as__ldBase_t.opacity);

                arm_2d_op_wait_async(NULL);


                if(ptWidget->isExpand)
                {
                    arm_2d_tile_generate_child(&tTarget,
                                               &((arm_2d_region_t){0,
                                                                   ptWidget->itemHeight,
                                                                   ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth,
                                                                   ptWidget->itemHeight*ptWidget->itemCount}),
                                               &tempTile,
                                               false);

                    displayRegion.tSize.iHeight=tempTile.tRegion.tSize.iHeight;
                    if(ptWidget->isCorner)
                    {
                        draw_round_corner_box(&tempTile,&displayRegion,GLCD_COLOR_WHITE,255,bIsNewFrame);
                        draw_round_corner_border(&tempTile,&displayRegion,GLCD_COLOR_LIGHT_GREY,(arm_2d_border_opacity_t){255,255,255,255},(arm_2d_corner_opacity_t){255,255,255,255});
                    }
                    else
                    {
                        ldBaseColor(&tempTile,NULL,GLCD_COLOR_WHITE,255);
                        arm_2d_draw_box(&tempTile,&displayRegion,1,GLCD_COLOR_LIGHT_GREY,255);
                    }

                    arm_2d_tile_generate_child(&tTarget,
                                               &((arm_2d_region_t){COMBO_BOX_TEXT_LEFT_OFFSET,
                                                                   ptWidget->itemHeight,
                                                                   ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-COMBO_BOX_TEXT_LEFT_OFFSET,
                                                                   ptWidget->itemHeight}),
                                               &tempTile,
                                               false);

                    ldColor lineColor;
                    for(uint8_t i=0;i<ptWidget->itemCount;i++)
                    {
                        ldBaseLabel(&tTarget,&tempTile.tRegion,ptWidget->ppItemStrGroup[i],ptWidget->ptFont,ARM_2D_ALIGN_LEFT,ptWidget->textColor,ptWidget->use_as__ldBase_t.opacity);

                        lineColor=GLCD_COLOR_LIGHT_GREY;
                        if(ptWidget->itemPreSelect==i)
                        {
                            lineColor=__RGB( 0x7F, 0xB0, 0xEF  );
                        }

                        ldBaseColor(&tTarget,
                                    &((arm_2d_region_t){tempTile.tRegion.tLocation.iX,
                                                        tempTile.tRegion.tLocation.iY+ptWidget->itemHeight-5,
                                                        ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-COMBO_BOX_TEXT_LEFT_OFFSET*2,
                                                        3}),
                                    lineColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                        tempTile.tRegion.tLocation.iY+=ptWidget->itemHeight;
                    }
                }
            }

        }
    }
    arm_2d_op_wait_async(NULL);
}

void ldComboBoxSetItems(ldComboBox_t* ptWidget,const uint8_t *pStrArray[],uint8_t arraySize)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ppItemStrGroup=pStrArray;
    ptWidget->itemCount=arraySize;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
