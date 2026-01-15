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

#define __LD_WINDOW_IMPLEMENT__
#define __ARM_2D_HELPER_CONTROL_INHERIT__
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldWindow.h"

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

const ldBaseWidgetFunc_t ldWindowFunc = {
    .depose = (ldDeposeFunc_t)ldWindow_depose,
    .load = (ldLoadFunc_t)ldWindow_on_load,
    .frameStart = (ldFrameStartFunc_t)ldWindow_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldWindow_on_frame_complete,
    .show = (ldShowFunc_t)ldWindow_show,
};

ldWindow_t* ldWindow_init(ld_scene_t *ptScene,ldWindow_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldWindow_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][window] id:%d", nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldWindow_t));
    }

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldWindowFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    if(nameId==0)
    {
        ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;
        ptScene->ptNodeRoot=(arm_2d_control_node_t*)ptWidget;
        ptWidget->use_as__ldBase_t.widgetType=widgetTypeBackground;
        ptWidget->bgColor=__RGB(240,240,240);
        LOG_INFO("[init][background] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    }
    else
    {
        ptParent=ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
        ldBaseNodeAdd((arm_2d_control_node_t*)ptParent,(arm_2d_control_node_t*)ptWidget);
        ptWidget->isTransparent=true;
        ptWidget->use_as__ldBase_t.widgetType=widgetTypeWindow;
        LOG_INFO("[init][window] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    }

    LOG_INFO("[init][window] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    return ptWidget;
}

void ldWindow_depose(ld_scene_t *ptScene, ldWindow_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if((ptWidget->use_as__ldBase_t.widgetType!=widgetTypeWindow)&&
       (ptWidget->use_as__ldBase_t.widgetType!=widgetTypeBackground))
    {
        return;
    }

#if (USE_LOG_LEVEL>=LOG_LEVEL_INFO)
    switch (ptWidget->use_as__ldBase_t.widgetType)
    {
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

    if(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.ptChildList!=NULL)
    {
        arm_ctrl_enum(ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.ptChildList, ptItem, PREORDER_TRAVERSAL)
        {
            ((ldBase_t *)ptItem)->ptGuiFunc->depose(ptScene,ptItem);
        }
    }

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
#if USE_VIRTUAL_RESOURCE == 1
    ldFree(ptWidget->ptImgTile);
    ldFree(ptWidget->ptMaskTile);
#endif
    ldFree(ptWidget);
}

void ldWindow_on_load(ld_scene_t *ptScene, ldWindow_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
}

void ldWindow_on_frame_start(ld_scene_t *ptScene, ldWindow_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    if((ptWidget->isLayoutUpdate)&&(ptWidget->layoutTpye!=none))
    {
        ptWidget->isLayoutUpdate=false;
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        arm_2d_region_t globalRegion;
        arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t*)ptWidget,&globalRegion,true);
        arm_2d_layout(globalRegion)
        {
            ldBase_t *child=ldBaseGetChildList((ldBase_t*)ptWidget);
            if(child!=NULL)
            {

                arm_2d_size_t size;
                arm_ctrl_enum(child, ptItem, PREORDER_TRAVERSAL)
                {
                    size=((ldBase_t*)ptItem)->use_as__arm_2d_control_node_t.tRegion.tSize;
                    if(ptWidget->layoutTpye==horizontal)
                    {
                        __item_line_horizontal(size.iWidth,size.iWidth){
                            __item_region.tLocation.iX-=globalRegion.tLocation.iX;
                            __item_region.tLocation.iY-=globalRegion.tLocation.iY;
                            ldBaseSetRegion((ldBase_t*)ptItem,__item_region);
                        }
                    }
                    else
                    {
                        __item_line_vertical(size.iWidth,size.iWidth){
                            __item_region.tLocation.iX-=globalRegion.tLocation.iX;
                            __item_region.tLocation.iY-=globalRegion.tLocation.iY;
                            ldBaseSetRegion((ldBase_t*)ptItem,__item_region);
                        }
                    }
                }
            }
        }
    }
}

void ldWindow_on_frame_complete(ld_scene_t *ptScene, ldWindow_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
}

void ldWindow_show(ld_scene_t *ptScene, ldWindow_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
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
            if(ldBaseIsHidden((ldBase_t*)ptWidget)||(ptWidget->isTransparent))
            {
                break;
            }
            if((ptWidget->ptImgTile==NULL)&&(ptWidget->ptMaskTile==NULL))
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
            LD_BASE_WIDGET_SELECT;

            arm_2d_op_wait_async(NULL);
        }
    }
}

void ldWindowSetColor(ldWindow_t *ptWidget,ldColor bgColor)
{
    assert(NULL!= ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isTransparent=false;
    ptWidget->bgColor=bgColor;
}

ldColor ldWindowGetColor(ldWindow_t *ptWidget)
{
    assert(NULL!= ptWidget);
    if(ptWidget == NULL)
    {
        return 0;
    }
    return ptWidget->bgColor;
}

void ldWindowSetImage(ldWindow_t *ptWidget, arm_2d_tile_t* ptImgTile, arm_2d_tile_t* ptMaskTile)
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

void ldWindowSetLayout(ldWindow_t *ptWidget, ldLayoutType_t type)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isLayoutUpdate=true;
    ptWidget->layoutTpye=type;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
