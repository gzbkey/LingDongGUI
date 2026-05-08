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

#define __LD_LINE_EDIT_IMPLEMENT__

#include "__common.h"
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
    .frameComplete = (ldFrameCompleteFunc_t)ldLineEdit_on_frame_complete,
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
                ldBaseSetHidden((ldBase_t *)kb,false);

                if((ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY+ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight)>(LD_CFG_SCREEN_HEIGHT>>1))
                {
                    ldBaseMove((ldBase_t *)kb,0,LD_CFG_SCREEN_HEIGHT>>1);
                    ldBaseBgMove(ptScene,LD_CFG_SCREEN_WIDTH,LD_CFG_SCREEN_HEIGHT,0,-(LD_CFG_SCREEN_HEIGHT>>1));

                }
                else
                {
                    ldBaseMove((ldBase_t *)kb,0,0);
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
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->pText=pText;
    ptWidget->textMax=textMax;
    ptWidget->ptFont=ptFont;
    ptWidget->textColor=GLCD_COLOR_BLACK;
    ptWidget->backgroundColor=GLCD_COLOR_WHITE;
    ptWidget->frameColor=GLCD_COLOR_LIGHT_GREY;
    ptWidget->editType=typeString;
    ptWidget->tAlign=ARM_2D_ALIGN_LEFT;

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotLineEditProcess);
    ldMsgConnect(ptWidget,SIGNAL_FINISHED,slotEditEnd);

    LOG_INFO("[init][lineEdit] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    return ptWidget;
}

void ldLineEdit_depose(ld_scene_t *ptScene, ldLineEdit_t *ptWidget)
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
#if USE_VIRTUAL_RESOURCE == 1
    ldFree(ptWidget->ptFont);
#endif
    ldFree(ptWidget->pText);
    ldFree(ptWidget);
}

void ldLineEdit_on_load(ld_scene_t *ptScene, ldLineEdit_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldLineEdit_on_frame_start(ld_scene_t *ptScene, ldLineEdit_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget->isEditing)
    {
        if(cursorBlinkCount>CURSOR_BLINK_TIMEOUT)
        {
            cursorBlinkCount=0;
            cursorBlinkFlag=!cursorBlinkFlag;
            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        }
    }
    ptWidget->_strSize=arm_lcd_printf_to_buffer(ptWidget->ptFont,"%s",ptWidget->pText);
}

void ldLineEdit_on_frame_complete(ld_scene_t *ptScene, ldLineEdit_t *ptWidget)
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
            if(ptWidget->use_as__ldBase_t.isCorner)
            {
                draw_round_corner_box(&tTarget,&tTarget_canvas,ptWidget->backgroundColor,ptWidget->use_as__ldBase_t.opacity,bIsNewFrame);
                draw_round_corner_border(&tTarget,
                                         &tTarget_canvas,
                                         ptWidget->frameColor,
                                         (arm_2d_border_opacity_t){ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity},
                                         (arm_2d_corner_opacity_t){ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity,ptWidget->use_as__ldBase_t.opacity});
            }
            else
            {
                ldBaseColor(&tTarget,&tTarget_canvas,ptWidget->backgroundColor,ptWidget->use_as__ldBase_t.opacity);
                arm_2d_draw_box(&tTarget,&tTarget_canvas,1,ptWidget->frameColor,ptWidget->use_as__ldBase_t.opacity);
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

            
            if(ptWidget->pText!=NULL)
            {
                arm_2d_align_t tAlign=ptWidget->tAlign;
                if(ptWidget->_strSize.iWidth>tempRegion.tSize.iWidth)
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
                arm_2d_region_t cursorRegion={
                    tempRegion.tLocation.iX+ptWidget->_strSize.iWidth,
                    ((tTarget_canvas.tSize.iHeight-ptWidget->ptFont->tCharSize.iHeight)>>1)+2,
                    CURSOR_WIDTH,
                    ptWidget->ptFont->tCharSize.iHeight
                };
                if(ptWidget->tAlign==ARM_2D_ALIGN_RIGHT)
                {
                    cursorRegion.tLocation.iX=tempRegion.tLocation.iX+tempRegion.tSize.iWidth;
                }
                arm_2d_draw_box(&tTarget,
                                &cursorRegion,
                                1,
                                ptWidget->textColor,
                                ptWidget->use_as__ldBase_t.opacity);
            }

            LD_BASE_WIDGET_SELECT;
            arm_2d_op_wait_async(NULL);
        }
    }


}

void ldLineEditSetText(ldLineEdit_t* ptWidget,uint8_t *pText)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL || pText == NULL)
    {
        return;
    }
    uint16_t textLen=strlen((char*)pText);
    if(textLen == 0)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    if(ptWidget->textMax==0)
    {
        ptWidget->pText=ldRealloc(ptWidget->pText,textLen+1);
        memset(ptWidget->pText, 0, textLen + 1);
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

void ldLineEditSetAlign(ldLineEdit_t *ptWidget,arm_2d_align_t tAlign)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->tAlign=tAlign;
}

void ldLineEditSetColor(ldLineEdit_t *ptWidget, ldColor textColor, ldColor backgroundColor, ldColor frameColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->textColor=textColor;
    ptWidget->backgroundColor=backgroundColor;
    ptWidget->frameColor=frameColor;
}

uint8_t *ldLineEditGetText(ldLineEdit_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return NULL;
    }
    return ptWidget->pText;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
