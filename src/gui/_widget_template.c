/*
 * Copyright (c) 2023-2024 Ou Jianbo (59935554@qq.com). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*============================ INCLUDES ======================================*/
#define __<CONTROL_NAME>_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include "template.h"
#include <assert.h>
#include <string.h>

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

ARM_NONNULL(1)
template_t* template_init( arm_2d_scene_t *ptScene,template_t *ptWidget,
                          template_cfg_t *ptCFG)
{
    assert(NULL!= ptScene);
    if(NULL==ptWidget)
    {
        ptWidget=ldMalloc(sizeof (template_t));
        if(ptWidget)
        {
            memset(ptWidget, 0, sizeof(template_t));
        }
        else
        {
            return NULL;
        }
    }

    ptWidget->tRegion.tLocation.iX=x;
    ptWidget->tRegion.tLocation.iY=y;
    ptWidget->tRegion.tSize.iWidth=width;
    ptWidget->tRegion.tSize.iHeight=height;
    ptWidget->nameId=nameId;
    ptWidget->widgetType=widgetTypeTemplate;

    return ptWidget;
}

ARM_NONNULL(1)
void template_depose( template_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

ARM_NONNULL(1)
void template_on_load( template_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

ARM_NONNULL(1)
void template_on_frame_start( template_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

ARM_NONNULL(1)
void template_show( template_t *ptWidget,
                            const arm_2d_tile_t *ptTile, 
                            bool bIsNewFrame)
{
    assert(NULL!= ptWidget);

#if 0
    if (bIsNewFrame) {
        
    }
#endif
    arm_2d_container(ptTile, tTarget, &ptWidget->tRegion) {



    }

    arm_2d_op_wait_async(NULL);
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
