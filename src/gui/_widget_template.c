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

#define __<CONTROL_NAME>_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include "template.h"
#include <assert.h>
#include <string.h>

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

const templateWidgetFunc_t templateFunc = {
    .depose = (ldDeposeFunc_t)template_depose,
    .show = (ldShowFunc_t)template_show,
    .frameStart = (ldFrameStartFunc_t)template_on_frame_start,
};

template_t* template_init( arm_2d_scene_t *ptScene,template_t *ptWidget,
                          template_cfg_t *ptCFG)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(template_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][template] id:%d", nameId);
            return NULL;
        }
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeTemplate;
    ptWidget->use_as__ldBase_t.pFunc = &ldButtonFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    LOG_INFO("[init][template] id:%d", nameId);
    return ptWidget;
}

void template_depose( template_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeTemplate)
    {
        return;
    }

    LOG_INFO("[depose][template] id:%d", ptWidget->use_as__ldBase_t.nameId);

    xDeleteConnect(&ptWidget->ptScene->tLink,ptWidget->use_as__ldBase_t.nameId);
    ldBaseNodeRemove(ptWidget->ptScene->ptNodeRoot,(arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void template_on_load( template_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void template_on_frame_start( template_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void template_show(arm_2d_scene_t *pScene, template_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL!= ptWidget);

    if ((ptWidget->isHidden) || (ptWidget->isTransparent))
    {
        return;
    }

#if 0
    if (bIsNewFrame) {
        
    }
#endif
    arm_2d_container(ptTile, tTarget, &ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion)
    {



    }

    arm_2d_op_wait_async(NULL);
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
