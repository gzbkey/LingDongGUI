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

#define __LD_ANIMATION_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldAnimation.h"

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

const ldBaseWidgetFunc_t ldAnimationFunc = {
    .depose = (ldDeposeFunc_t)ldAnimation_depose,
    .load = (ldLoadFunc_t)ldAnimation_on_load,
    .frameStart = (ldFrameStartFunc_t)ldAnimation_on_frame_start,
    .show = (ldShowFunc_t)ldAnimation_show,
};

ldAnimation_t* ldAnimation_init( ld_scene_t *ptScene,ldAnimation_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_tile_t* ptImgTile, uint16_t periodMs)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldAnimation_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][animation] id:%d", nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldAnimation_t));
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeAnimation;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldAnimationFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = false;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->ptImgTile=ptImgTile;
    ptWidget->showRegion.tSize=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize;
    ptWidget->periodMs=periodMs;

    LOG_INFO("[init][animation] id:%d, size:%llu", nameId,sizeof (*ptWidget));
    return ptWidget;
}

void ldAnimation_depose( ldAnimation_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeAnimation)
    {
        return;
    }

    LOG_INFO("[depose][animation] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldAnimation_on_load( ldAnimation_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
}

void ldAnimation_on_frame_start( ldAnimation_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
}

void ldAnimation_show(ld_scene_t *ptScene, ldAnimation_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    if (bIsNewFrame) {
        if (arm_2d_helper_is_time_out(ptWidget->periodMs,&ptWidget->frameTimer))
        {
            ptWidget->showRegion.tLocation.iX+=ptWidget->showRegion.tSize.iWidth;
            if(ptWidget->showRegion.tLocation.iX>=ptWidget->ptImgTile->tRegion.tSize.iWidth)
            {
                ptWidget->showRegion.tLocation.iX=0;
                ptWidget->showRegion.tLocation.iY+=ptWidget->showRegion.tSize.iHeight;
            }
            if(ptWidget->showRegion.tLocation.iY>=ptWidget->ptImgTile->tRegion.tSize.iHeight)
            {
                ptWidget->showRegion.tLocation.iY=0;
            }
        }
    }

    arm_2d_region_t globalRegion;
    arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t*)ptWidget,&globalRegion,false);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            arm_2d_tile_t childTile= impl_child_tile(*ptWidget->ptImgTile, ptWidget->showRegion.tLocation.iX, ptWidget->showRegion.tLocation.iY,ptWidget->showRegion.tSize.iWidth,ptWidget->showRegion.tSize.iHeight);

            arm_2d_tile_copy_with_opacity(&childTile,
                                          &tTarget,
                                          &tTarget_canvas,
                                          ptWidget->use_as__ldBase_t.opacity);

        }
    }

    arm_2d_op_wait_async(NULL);
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
