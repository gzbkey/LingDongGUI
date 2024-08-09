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

#define __LD_LABEL_IMPLEMENT__
#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldLabel.h"

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

const ldBaseWidgetFunc_t ldLabelFunc = {
    .depose = (ldDeposeFunc_t)ldLabel_depose,
    .load = (ldLoadFunc_t)ldLabel_on_load,
    .frameStart = (ldFrameStartFunc_t)ldLabel_on_frame_start,
    .show = (ldShowFunc_t)ldLabel_show,
};

ldLabel_t* ldLabel_init( ld_scene_t *ptScene,ldLabel_t *ptWidget,uint16_t nameId,uint16_t parentNameId,int16_t x,int16_t y,int16_t width,int16_t height,arm_2d_font_t *ptFont)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldLabel_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][label] id:%d", nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldLabel_t));
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

//    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeLabel;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldLabelFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->tAlign = ARM_2D_ALIGN_CENTRE;
    ptWidget->bgColor=GLCD_COLOR_WHITE;
    ptWidget->textColor = GLCD_COLOR_BLACK;
    ptWidget->ptFont = ptFont;

    LOG_INFO("[init][label] id:%d", nameId);
    return ptWidget;
}

void ldLabel_depose( ldLabel_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeLabel)
    {
        return;
    }

    LOG_INFO("[depose][label] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget->pStr);
    ldFree(ptWidget);
}

void ldLabel_on_load( ldLabel_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldLabel_on_frame_start( ldLabel_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldLabel_show(ld_scene_t *ptScene, ldLabel_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
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

            if(!ptWidget->isTransparent)
            {
                if (ptWidget->ptImgTile==NULL)//color
                {
                    ldBaseColor(&tTarget,NULL,ptWidget->bgColor,ptWidget->use_as__ldBase_t.opacity);
                }
                else
                {
                    ldBaseImage(&tTarget,NULL,ptWidget->ptImgTile,NULL,0,ptWidget->use_as__ldBase_t.opacity);
                }
                arm_2d_op_wait_async(NULL);
            }

            if(ptWidget->pStr!=NULL)
            {
                ldBaseLabel(&tTarget,
                            &tTarget_canvas,
                            ptWidget->pStr,
                            ptWidget->ptFont,
                            ptWidget->tAlign,
                            ptWidget->textColor,
                            ptWidget->use_as__ldBase_t.opacity);
                arm_2d_op_wait_async(NULL);
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldLabelSetTransparent(ldLabel_t* ptWidget,bool isTransparent)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isTransparent=isTransparent;
}

void ldLabelSetText(ldLabel_t* ptWidget,uint8_t *pStr)
{
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
}

void ldLabelSetTextColor(ldLabel_t* ptWidget,ldColor textColor)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->textColor=textColor;
}

void ldLabelSetAlign(ldLabel_t *ptWidget,arm_2d_align_t tAlign)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->tAlign=tAlign;
}

void ldLabelSetBgImage(ldLabel_t *ptWidget, arm_2d_tile_t *ptImgTile)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptImgTile=ptImgTile;
    ptWidget->isTransparent=false;
}

void ldLabelSetBgColor(ldLabel_t *ptWidget, ldColor bgColor)
{
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->bgColor=bgColor;
    ptWidget->isTransparent=false;
    ptWidget->ptImgTile=NULL;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
