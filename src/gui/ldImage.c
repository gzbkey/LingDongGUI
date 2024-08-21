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

#define __LD_IMAGE_IMPLEMENT__
#define __ARM_2D_HELPER_CONTROL_INHERIT__
#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldImage.h"

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

const ldBaseWidgetFunc_t ldImageFunc={
    .depose=(ldDeposeFunc_t)ldImage_depose,
    .load = (ldLoadFunc_t)ldImage_on_load,
    .frameStart=(ldFrameStartFunc_t)ldImage_on_frame_start,
    .show=(ldShowFunc_t)ldImage_show,
};

ldImage_t* ldImage_init( ld_scene_t *ptScene,ldImage_t *ptWidget,uint16_t nameId,uint16_t parentNameId,int16_t x,int16_t y,int16_t width,int16_t height,arm_2d_tile_t* ptImgTile,arm_2d_tile_t* ptMaskTile,bool isWindow)
{
    assert(NULL!= ptScene);
    ldBase_t* ptParent;

    if(NULL==ptWidget)
    {
        ptWidget=ldCalloc(1,sizeof (ldImage_t));
        if(NULL==ptWidget)
        {
            LOG_ERROR("[init failed][image] id:%d",nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldImage_t));
    }

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX=x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY=y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth=width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight=height;
    ptWidget->use_as__ldBase_t.nameId=nameId;
    ptWidget->use_as__ldBase_t.ptGuiFunc=&ldImageFunc;
    ptWidget->use_as__ldBase_t.opacity = 255;
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->ptImgTile=ptImgTile;
    ptWidget->ptMaskTile=ptMaskTile;

    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate=true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;

    if(isWindow)
    {
        if(nameId==0)
        {
            ptScene->ptNodeRoot=(arm_2d_control_node_t*)ptWidget;
            ptWidget->use_as__ldBase_t.widgetType=widgetTypeBackground;
            ptWidget->bgColor=__RGB(240,240,240);
            LOG_INFO("[init][background] id:%d, size:%d", nameId,sizeof (*ptWidget));
        }
        else
        {
            ptParent=ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
            ldBaseNodeAdd((arm_2d_control_node_t*)ptParent,(arm_2d_control_node_t*)ptWidget);
            ptWidget->isTransparent=true;
            ptWidget->use_as__ldBase_t.widgetType=widgetTypeWindow;
            LOG_INFO("[init][window] id:%d, size:%d", nameId,sizeof (*ptWidget));
        }
    }
    else
    {
        ptParent=ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
        ldBaseNodeAdd((arm_2d_control_node_t*)ptParent,(arm_2d_control_node_t*)ptWidget);
        ptWidget->use_as__ldBase_t.widgetType=widgetTypeImage;
        LOG_INFO("[init][image] id:%d, size:%d", nameId,sizeof (*ptWidget));
    }
    return ptWidget;
}

void ldImage_depose( ldImage_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if((ptWidget->use_as__ldBase_t.widgetType!=widgetTypeImage)&&
            (ptWidget->use_as__ldBase_t.widgetType!=widgetTypeWindow)&&
            (ptWidget->use_as__ldBase_t.widgetType!=widgetTypeBackground))
    {
        return;
    }

#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    switch (ptWidget->use_as__ldBase_t.widgetType)
    {
    case widgetTypeImage:
    {
        LOG_INFO("[depose][image] id:%d", ptWidget->use_as__ldBase_t.nameId);
        break;
    }
    case widgetTypeWindow:
    {
        LOG_INFO("[depose][window] id:%d", ptWidget->use_as__ldBase_t.nameId);
        break;
    }
    case widgetTypeBackground:
    {
        LOG_INFO("[depose][background] id:%d", ptWidget->use_as__ldBase_t.nameId);
        break;
    }
    default:
        break;
    }
#endif

    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeImage)
    {
        if(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.ptChildList!=NULL)
        {
            arm_ctrl_enum(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.ptChildList, ptItem, PREORDER_TRAVERSAL)
            {
                ((ldBase_t *)ptItem)->ptGuiFunc->depose(ptItem);
            }
        }
    }

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget);
}

void ldImage_on_load( ldImage_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldImage_on_frame_start( ldImage_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldImage_show( ld_scene_t *ptScene,ldImage_t *ptWidget,const arm_2d_tile_t *ptTile,bool bIsNewFrame)
{
    assert(NULL!= ptWidget);
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
            if((ptWidget->use_as__ldBase_t.isHidden)||(ptWidget->isTransparent))
            {
                break;
            }

            if((ptWidget->ptImgTile==NULL)&&(ptWidget->ptMaskTile==NULL))
            {
                ldBaseColor(&tTarget,
                            NULL,
                            ptWidget->bgColor,
                            ptWidget->use_as__ldBase_t.opacity);
            }
            else
            {
                ldBaseImage(&tTarget,
                            &ptWidget->ptImgTile->tRegion,
                            ptWidget->ptImgTile,
                            ptWidget->ptMaskTile,
                            ptWidget->fgColor,
                            ptWidget->use_as__ldBase_t.opacity);
            }
        }
    }
    arm_2d_op_wait_async(NULL);
}

void ldImageSetBgColor(ldImage_t *ptWidget,ldColor bgColor)
{
    assert(NULL!= ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isTransparent=false;
    ptWidget->ptImgTile=NULL;
    ptWidget->ptMaskTile=NULL;
    ptWidget->bgColor=bgColor;
}

void ldImageSetImage(ldImage_t *ptWidget, arm_2d_tile_t* ptImgTile, arm_2d_tile_t* ptMaskTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isTransparent=false;
    ptWidget->ptImgTile=ptImgTile;
    ptWidget->ptMaskTile=ptMaskTile;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
