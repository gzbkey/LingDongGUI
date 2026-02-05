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

#define __LD_COMBO_BOX_IMPLEMENT__

#include "__common.h"
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
    .frameComplete = (ldFrameCompleteFunc_t)ldComboBox_on_frame_complete,
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

    tLocation = ldBaseGetAbsoluteLocation((ldBase_t*)ptWidget,tLocation);

    int16_t tempOffset=y-tLocation.iY;

    if((tempOffset<0)||(tempOffset>=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight))
    {
        clickItemNum=SHOW_ITEM_NUM;
    }
    else
    {
        clickItemNum=tempOffset/ptWidget->itemHeight;
    }


    switch (msg.signal)
    {
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
            ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = ptWidget->itemHeight;
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

ldComboBox_t* ldComboBox_init(ld_scene_t *ptScene,ldComboBox_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_font_t *ptFont)
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
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;
    ptWidget->use_as__ldBase_t.isCorner=true;

    ptWidget->itemHeight=height;
    ptWidget->isExpand=false;
    ptWidget->ptFont=ptFont;
    ptWidget->textColor=GLCD_COLOR_BLACK;
    ptWidget->bgColor=GLCD_COLOR_WHITE;
    ptWidget->frameColor=GLCD_COLOR_LIGHT_GREY;
    ptWidget->selectColor=__RGB( 0x7F, 0xB0, 0xEF );
    ptWidget->isStatic=true;

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotComboBoxProcess);
    ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotComboBoxProcess);
    ldMsgConnect(ptWidget,SIGNAL_HOLD_DOWN,slotComboBoxProcess);

    LOG_INFO("[init][comboBox] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    return ptWidget;
}

void ldComboBox_depose(ld_scene_t *ptScene, ldComboBox_t *ptWidget)
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

    if(ptWidget->isStatic==false)
    {
        for (uint8_t i = 0; i < ptWidget->itemCount; ++i)
        {
            ldFree(ptWidget->ppItemStrGroup[i]);
        }
        ldFree(ptWidget->ppItemStrGroup);
    }
    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
#if USE_VIRTUAL_RESOURCE == 1
    ldFree(ptWidget->ptDropdownImgTile);
    ldFree(ptWidget->ptDropdownMaskTile);
    ldFree(ptWidget->ptFont);
#endif
    ldFree(ptWidget);
}

void ldComboBox_on_load(ld_scene_t *ptScene, ldComboBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldComboBox_on_frame_start(ld_scene_t *ptScene, ldComboBox_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldComboBox_on_frame_complete(ld_scene_t *ptScene, ldComboBox_t *ptWidget)
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

            arm_2d_region_t displayRegion={
                .tSize={
                    .iWidth=tTarget_canvas.tSize.iWidth,
                    .iHeight=ptWidget->itemHeight,
                }
            };

            if(ptWidget->use_as__ldBase_t.isCorner)
            {
                draw_round_corner_box(&tTarget,&displayRegion,ptWidget->bgColor,ptWidget->use_as__ldBase_t.opacity,bIsNewFrame);
                draw_round_corner_border(&tTarget,
                                         &displayRegion,
                                         ptWidget->frameColor,
                                         (arm_2d_border_opacity_t){ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity},
                                         (arm_2d_corner_opacity_t){ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity});
            }
            else
            {
                ldBaseColor(&tTarget,&displayRegion,ptWidget->bgColor,ptWidget->use_as__ldBase_t.opacity);
                arm_2d_draw_box(&tTarget,&displayRegion,2,ptWidget->frameColor,ptWidget->use_as__ldBase_t.opacity);
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
                    ldBaseImage(&tTarget,&__centre_region,NULL,(arm_2d_tile_t*)&c_tile_dropDownV_Mask,GLCD_COLOR_BLACK,ptWidget->use_as__ldBase_t.opacity);
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
                    if(ptWidget->use_as__ldBase_t.isCorner)
                    {
                        draw_round_corner_box(&tempTile,&displayRegion,ptWidget->bgColor,ptWidget->use_as__ldBase_t.opacity,bIsNewFrame);
                        draw_round_corner_border(&tempTile,
                                                 &displayRegion,
                                                 ptWidget->frameColor,
                                                 (arm_2d_border_opacity_t){ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity},
                                                 (arm_2d_corner_opacity_t){ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity});
                    }
                    else
                    {
                        ldBaseColor(&tempTile,NULL,ptWidget->bgColor,ptWidget->use_as__ldBase_t.opacity);
                        arm_2d_draw_box(&tempTile,&displayRegion,2,ptWidget->frameColor,ptWidget->use_as__ldBase_t.opacity);
                    }

                    arm_2d_tile_generate_child(&tTarget,
                                               &((arm_2d_region_t){COMBO_BOX_TEXT_LEFT_OFFSET,
                                                                   ptWidget->itemHeight,
                                                                   ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-COMBO_BOX_TEXT_LEFT_OFFSET,
                                                                   ptWidget->itemHeight}),
                                               &tempTile,
                                               false);

                    arm_2d_tile_t selectTile;
                    int frameSize=2;
                    if(ptWidget->frameColor==ptWidget->bgColor)
                    {
                        frameSize=0;
                    }
                    arm_2d_tile_generate_child(&tTarget,
                                               &((arm_2d_region_t){frameSize,
                                                                   ptWidget->itemHeight+frameSize,
                                                                   ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-frameSize*2,
                                                                   ptWidget->itemHeight-frameSize*2}),
                                               &selectTile,
                                               false);

                    ldColor selectColor;
                    for(uint8_t i=0;i<ptWidget->itemCount;i++)
                    {
                        selectColor=ptWidget->frameColor;
                        if(ptWidget->itemPreSelect==i)
                        {
                            selectColor=ptWidget->selectColor;

                            if(ptWidget->use_as__ldBase_t.isCorner)
                            {
                                draw_round_corner_box(&selectTile,NULL,selectColor,ptWidget->use_as__ldBase_t.opacity,bIsNewFrame);
                            }
                            else
                            {
                                ldBaseColor(&selectTile,NULL,selectColor,ptWidget->use_as__ldBase_t.opacity);
                            }
                        }

                        ldBaseLabel(&tTarget,&tempTile.tRegion,ptWidget->ppItemStrGroup[i],ptWidget->ptFont,ARM_2D_ALIGN_LEFT,ptWidget->textColor,ptWidget->use_as__ldBase_t.opacity);
                        tempTile.tRegion.tLocation.iY+=ptWidget->itemHeight;
                        selectTile.tRegion.tLocation.iY+=ptWidget->itemHeight;
                    }
                }
            }

            LD_BASE_WIDGET_SELECT;
        }
    }
    arm_2d_op_wait_async(NULL);
}

void ldComboBoxSetStaticItems(ldComboBox_t* ptWidget,uint8_t *pStrArray[],uint8_t arraySize)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    if(ptWidget->isStatic==false)//只要使用了动态就不能再使用静态
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ppItemStrGroup=pStrArray;
    ptWidget->itemCount=arraySize;
}

void ldComboBoxSetItemMax(ldComboBox_t *ptWidget, uint8_t itemMax)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    if (ptWidget->isStatic == false)
    {
        return;
    }
    ptWidget->ppItemStrGroup = malloc(sizeof(uint8_t*) * itemMax);
    ptWidget->itemMax         = itemMax;
    ptWidget->itemCount      = 0;
}

void ldComboBoxAddItem(ldComboBox_t* ptWidget,uint8_t *pStr)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    if(ptWidget->itemMax==ptWidget->itemCount)
    {
        return;
    }
    ptWidget->isStatic=false;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    uint8_t *dup = ldMalloc(strlen((char*)pStr) + 1);
    strcpy((char*)dup, (char*)pStr);
    ptWidget->ppItemStrGroup[ptWidget->itemCount++] = dup;
}

void ldComboBoxSetTextColor(ldComboBox_t* ptWidget, ldColor textColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->textColor=textColor;
}

void ldComboBoxSetBackgroundColor(ldComboBox_t* ptWidget, ldColor backgroundColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->bgColor=backgroundColor;
}

void ldComboBoxSetFrameColor(ldComboBox_t* ptWidget, ldColor frameColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->frameColor=frameColor;
}

void ldComboBoxSetSelectColor(ldComboBox_t* ptWidget, ldColor selectColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->selectColor=selectColor;
}

void ldComboBoxSetSelectItem(ldComboBox_t* ptWidget, uint8_t itemIndex)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    if(itemIndex>=ptWidget->itemCount)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->itemSelect=itemIndex;
    ptWidget->itemPreSelect=itemIndex;
}

void ldComboBoxSetDropdownImage(ldComboBox_t* ptWidget,arm_2d_tile_t* ptDropdownImgTile,arm_2d_tile_t* ptDropdownMaskTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptDropdownImgTile=ptDropdownImgTile;
    ptWidget->ptDropdownMaskTile=ptDropdownMaskTile;
}

uint8_t ldComboBoxGetSelectItem(ldComboBox_t* ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return 0;
    }
    return ptWidget->itemSelect;
}

uint8_t* ldComboBoxGetText(ldComboBox_t* ptWidget, uint8_t num)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return 0;
    }
    if(num<ptWidget->itemCount)
    {
        return ptWidget->ppItemStrGroup[num];
    }
    return NULL;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
