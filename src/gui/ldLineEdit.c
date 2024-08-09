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

#define __LD_LINE_EDIT_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldLineEdit.h"
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

const ldBaseWidgetFunc_t ldLineEditFunc = {
    .depose = (ldDeposeFunc_t)ldLineEdit_depose,
    .load = (ldLoadFunc_t)ldLineEdit_on_load,
    .frameStart = (ldFrameStartFunc_t)ldLineEdit_on_frame_start,
    .show = (ldShowFunc_t)ldLineEdit_show,
};

static bool slotEditEnd(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldLineEdit_t *ptWidget=msg.ptSender;
    ptWidget->isEditing=false;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    return false;
}

static bool slotLineEditProcess(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldKeyboard_t *kb;

    ldLineEdit_t *ptWidget=msg.ptSender;

    if(msg.signal==SIGNAL_PRESS)
    {
        ptWidget->isEditing=true;
        if(ptWidget->kbNameId)
        {
            kb=ldBaseGetWidgetById(ptWidget->kbNameId);
            if(kb!=NULL)
            {
                kb->editType=ptWidget->editType;
                kb->ppStr=&ptWidget->pText;
                kb->strMax=ptWidget->textMax;
                kb->editorId=ptWidget->use_as__ldBase_t.nameId;
                cursorBlinkFlag=true;
                cursorBlinkCount=0;
                ldKeyboardSetHidden(kb,false);

                if((ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY+ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight)>(LD_CFG_SCEEN_HEIGHT>>1))
                {
                    ldKeyboardMove(kb,0,LD_CFG_SCEEN_HEIGHT>>1);
                    ldBaseBgMove(ptScene,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT,0,-(LD_CFG_SCEEN_HEIGHT>>1));

                }
                else
                {
                    ldKeyboardMove(kb,0,0);
                }
            }
        }
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    }

    return false;
}

ldLineEdit_t* ldLineEdit_init( ld_scene_t *ptScene,ldLineEdit_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,arm_2d_font_t *ptFont,uint8_t textMax)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    uint8_t *pText = NULL;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldLineEdit_t));
        pText = (uint8_t *)ldCalloc(1,(textMax+1)*sizeof(uint8_t));
        if ((NULL == ptWidget)||(NULL == pText))
        {
            ldFree(pText);
            ldFree(ptWidget);
            LOG_ERROR("[init failed][lineEdit] id:%d", nameId);
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
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeLineEdit;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldLineEditFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->pText=pText;
    ptWidget->textMax=textMax;
    ptWidget->ptFont=ptFont;
    ptWidget->textColor=GLCD_COLOR_BLACK;
    ptWidget->editType=typeString;

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotLineEditProcess);
    ldMsgConnect(ptWidget,SIGNAL_EDITING_FINISHED,slotEditEnd);

    LOG_INFO("[init][lineEdit] id:%d", nameId);
    return ptWidget;
}

void ldLineEdit_depose( ldLineEdit_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeLineEdit)
    {
        return;
    }

    LOG_INFO("[depose][lineEdit] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget->pText);
    ldFree(ptWidget);
}

void ldLineEdit_on_load( ldLineEdit_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldLineEdit_on_frame_start( ldLineEdit_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldLineEdit_show(ld_scene_t *ptScene, ldLineEdit_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    if (bIsNewFrame) {
        if(ptWidget->isEditing)
        {
            if(cursorBlinkCount>CURSOR_BLINK_TIMEOUT)
            {
                cursorBlinkCount=0;
                cursorBlinkFlag=!cursorBlinkFlag;
            }
            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        }
    }

    arm_2d_region_t globalRegion=ldBaseGetAbsoluteRegion(ptWidget);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }
            if(ptWidget->isCorner)
            {
                draw_round_corner_box(&tTarget,&tTarget_canvas,GLCD_COLOR_WHITE,255,bIsNewFrame);
                draw_round_corner_border(&tTarget,&tTarget_canvas,GLCD_COLOR_LIGHT_GREY,(arm_2d_border_opacity_t){255,255,255,255},(arm_2d_corner_opacity_t){255,255,255,255});
            }
            else
            {
                ldBaseColor(&tTarget,&tTarget_canvas,GLCD_COLOR_WHITE,255);
                arm_2d_draw_box(&tTarget,&tTarget_canvas,1,GLCD_COLOR_LIGHT_GREY,255);
            }
            arm_2d_op_wait_async(NULL);

            arm_2d_region_t tempRegion={
                .tLocation={
                    .iX=2,
                    .iY=2,
                },
                .tSize={
                    .iWidth=tTarget_canvas.tSize.iWidth-4,
                    .iHeight=tTarget_canvas.tSize.iHeight-4,
                },
            };

            if(ptWidget->isEditing)
            {
                //预留光标位置
                tempRegion.tSize.iWidth-=CURSOR_WIDTH;
            }

            arm_2d_size_t strSize=arm_lcd_text_get_box(ptWidget->pText, ptWidget->ptFont);
            if(ptWidget->pText!=NULL)
            {
                arm_2d_align_t tAlign=ARM_2D_ALIGN_LEFT;
                if(strSize.iWidth>tempRegion.tSize.iWidth)
                {
                    tAlign=ARM_2D_ALIGN_RIGHT;
                }
                ldBaseLabel(&tTarget,
                            &tempRegion,
                            ptWidget->pText,
                            ptWidget->ptFont,
                            tAlign,
                            ptWidget->textColor,
                            ptWidget->use_as__ldBase_t.opacity);
                arm_2d_op_wait_async(NULL);
            }

            if(cursorBlinkFlag&&ptWidget->isEditing)
            {
                arm_2d_draw_box(&tTarget,
                                &((arm_2d_region_t){tempRegion.tLocation.iX+strSize.iWidth,
                                                    ((tTarget_canvas.tSize.iHeight-ptWidget->ptFont->tCharSize.iHeight)>>1)+2,
                                                    CURSOR_WIDTH,
                                                    ptWidget->ptFont->tCharSize.iHeight}),
                                1,
                                0,
                                ptWidget->use_as__ldBase_t.opacity);
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldLineEditSetText(ldLineEdit_t* ptWidget,uint8_t *pText)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    uint16_t textLen=strlen((char*)pText);
    if(ptWidget->textMax==0)
    {
        ptWidget->pText=ldRealloc(ptWidget->pText,textLen+1);
    }

    if((textLen<ptWidget->textMax)||(ptWidget->textMax==0))
    {
        strcpy((char*)ptWidget->pText,(char*)pText);
    }
}

void ldLineEditSetKeyboard(ldLineEdit_t* ptWidget,uint16_t kbNameId)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    ptWidget->kbNameId=kbNameId;
}

void ldLineEditSetType(ldLineEdit_t* ptWidget,ldEditType_t editType)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    ptWidget->editType=editType;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
