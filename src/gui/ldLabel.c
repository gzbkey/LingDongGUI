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

#define __LD_LABEL_IMPLEMENT__
#include "__common.h"
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
    .frameComplete = (ldFrameCompleteFunc_t)ldLabel_on_frame_complete,
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
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->tAlign = ARM_2D_ALIGN_CENTRE;
    ptWidget->bgColor=GLCD_COLOR_WHITE;
    ptWidget->textColor = GLCD_COLOR_BLACK;
    ptWidget->ptFont = ptFont;

    LOG_INFO("[init][label] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    return ptWidget;
}

void ldLabel_depose(ld_scene_t *ptScene, ldLabel_t *ptWidget)
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
#if USE_VIRTUAL_RESOURCE == 1
    ldFree(ptWidget->ptFont);
    ldFree(ptWidget->ptImgTile);
    ldFree(ptWidget->ptMaskTile);
#endif
    ldFree(ptWidget->pStr);
    ldFree(ptWidget);
}

void ldLabel_on_load(ld_scene_t *ptScene, ldLabel_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldLabel_on_frame_start(ld_scene_t *ptScene, ldLabel_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldLabel_on_frame_complete(ld_scene_t *ptScene, ldLabel_t *ptWidget)
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

            if(!ptWidget->isTransparent)
            {
                if (ptWidget->ptImgTile==NULL)//color
                {
                    if(ptWidget->use_as__ldBase_t.isCorner)
                    {
                        draw_round_corner_box(&tTarget,
                                              NULL,
                                              ptWidget->bgColor,
                                              ptWidget->use_as__ldBase_t.opacity,
                                              bIsNewFrame);
                    }
                    else
                    {
                        ldBaseColor(&tTarget,
                                    NULL,
                                    ptWidget->bgColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
                }
                else
                {
                    if(ptWidget->use_as__ldBase_t.isCorner)
                    {
                        draw_round_corner_image(ptWidget->ptImgTile,
                                                &tTarget,
                                                NULL,
                                                bIsNewFrame,
                                                ptWidget->use_as__ldBase_t.opacity);
                    }
                    else
                    {
                        ldBaseImage(&tTarget,
                                    NULL,
                                    ptWidget->ptImgTile,
                                    ptWidget->ptMaskTile,
                                    ptWidget->bgColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                    }
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

            LD_BASE_WIDGET_SELECT;
            arm_2d_op_wait_async(NULL);
        }
    }
}

void ldLabelSetTransparent(ldLabel_t* ptWidget,bool isTransparent)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isTransparent=isTransparent;
}

void ldLabelSetText(ldLabel_t* ptWidget,uint8_t *pStr)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL || pStr==NULL)
    {
        return;
    }
    int len = strlen((char*)pStr);
    if(len == 0)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ldFree(ptWidget->pStr);
    ptWidget->pStr=ldCalloc(1,len+1);
    if(ptWidget->pStr!=NULL)
    {
        strcpy((char*)ptWidget->pStr,(char*)pStr);
    }
}

void ldLabelSetTextColor(ldLabel_t* ptWidget,ldColor textColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->textColor=textColor;
}

void ldLabelSetAlign(ldLabel_t *ptWidget,arm_2d_align_t tAlign)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->tAlign=tAlign;
}

void ldLabelSetBackgroundImage(ldLabel_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptImgTile=ptImgTile;
    ptWidget->ptMaskTile=ptMaskTile;
    ptWidget->isTransparent=false;
}

void ldLabelSetBackgroundColor(ldLabel_t *ptWidget, ldColor bgColor)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->bgColor=bgColor;
    ptWidget->isTransparent=false;
    ptWidget->ptImgTile=NULL;
}

void ldLabelSetFont(ldLabel_t* ptWidget,arm_2d_font_t *ptFont)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptFont=ptFont;
}

bool ldLabelGetTransparent(ldLabel_t* ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return false;
    }
    return ptWidget->isTransparent;
}

uint8_t *ldLabelGetText(ldLabel_t* ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return NULL;
    }
    return ptWidget->pStr;
}

ldColor ldLabelGetTextColor(ldLabel_t* ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return 0;
    }
    return ptWidget->textColor;
}

arm_2d_align_t ldLabelGetAlign(ldLabel_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return ARM_2D_ALIGN_CENTRE;
    }
    return ptWidget->tAlign;
}

ldColor ldLabelGetBackgroundColor(ldLabel_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return 0;
    }
    return ptWidget->bgColor;
}

arm_2d_font_t *ldLabelGetFont(ldLabel_t* ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget==NULL)
    {
        return NULL;
    }
    return ptWidget->ptFont;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
