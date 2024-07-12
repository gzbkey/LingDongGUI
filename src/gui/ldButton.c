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

#define __LD_BUTTON_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include "ldButton.h"
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

const ldBaseWidgetFunc_t ldButtonFunc = {
    .depose = (ldDeposeFunc_t)ldButton_depose,
    .load = (ldLoadFunc_t)ldButton_on_load,
    .frameStart = (ldFrameStartFunc_t)ldButton_on_frame_start,
    .show = (ldShowFunc_t)ldButton_show,
};

static bool slotButtonToggle(xConnectInfo_t info,ld_scene_t *ptScene)
{
    ldButton_t *ptWidget;

    ptWidget = ldBaseGetWidget(ptScene->ptNodeRoot,info.receiverId);

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        if (ptWidget->isCheckable)
        {
            ptWidget->isPressed = !ptWidget->isPressed;
        }
        else
        {
            ptWidget->isPressed = true;
        }
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    case SIGNAL_RELEASE:
    {
        if (!ptWidget->isCheckable)
        {
            ptWidget->isPressed = false;
        }
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    default:
        break;
    }

    return false;
}

ldButton_t *ldButton_init(ld_scene_t *ptScene, ldButton_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldButton_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][button] id:%d", nameId);
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
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeButton;
    ptWidget->use_as__ldBase_t.pFunc = &ldButtonFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    ptWidget->releaseColor = __RGB(217, 225, 244);
    ptWidget->pressColor = __RGB(255, 243, 202);
    ptWidget->selectColor = __RGB(255, 0, 0);
    ptWidget->opacity = 255;

    xConnect(&ptWidget->ptScene->tLink,nameId, SIGNAL_PRESS, nameId, slotButtonToggle);
    xConnect(&ptWidget->ptScene->tLink,nameId, SIGNAL_RELEASE, nameId, slotButtonToggle);

    LOG_INFO("[init][button] id:%d", nameId);
    return ptWidget;
}

void ldButton_depose(ldButton_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeButton)
    {
        return;
    }

    LOG_INFO("[depose][button] id:%d", ptWidget->use_as__ldBase_t.nameId);

    xDeleteConnect(&ptWidget->ptScene->tLink,ptWidget->use_as__ldBase_t.nameId);
    ldBaseNodeRemove(ptWidget->ptScene->ptNodeRoot,(arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget->pStr);
    ldFree(ptWidget);
}

void ldButton_on_load(ldButton_t *ptWidget)
{
    assert(NULL != ptWidget);
}

void ldButton_on_frame_start(ldButton_t *ptWidget)
{
    assert(NULL != ptWidget);
}

void ldButton_show(ld_scene_t *ptScene, ldButton_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);

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

        if ((ptWidget->ptReleaseImgTile == NULL) &&
            (ptWidget->ptReleaseMaskTile == NULL) &&
            (ptWidget->ptPressImgTile == NULL) &&
            (ptWidget->ptPressMaskTile == NULL)) // color
        {

            if (ptWidget->isCorner)
            {
                if (ptWidget->isPressed)
                {
                    draw_round_corner_box(&tTarget,
                                          NULL,
                                          ptWidget->pressColor,
                                          ptWidget->opacity,
                                          bIsNewFrame);
                }
                else
                {
                    draw_round_corner_box(&tTarget,
                                          NULL,
                                          ptWidget->releaseColor,
                                          ptWidget->opacity,
                                          bIsNewFrame);
                }
            }
            else
            {
                if (ptWidget->isPressed)
                {
                    ldBaseColor(&tTarget,
                                NULL,
                                ptWidget->pressColor,
                                ptWidget->opacity);
                }
                else
                {
                    ldBaseColor(&tTarget,
                                NULL,
                                ptWidget->releaseColor,
                                ptWidget->opacity);
                }
            }
        }
        else
        {
            if (ptWidget->isCorner)
            {
                if (ptWidget->isPressed)
                {
                    draw_round_corner_image(ptWidget->ptPressImgTile,
                                            &tTarget,
                                            NULL,
                                            bIsNewFrame);
                }
                else
                {
                    draw_round_corner_image(ptWidget->ptReleaseImgTile,
                                            &tTarget,
                                            NULL,
                                            bIsNewFrame);
                }
            }
            else
            {
                if (ptWidget->isPressed)
                {
                    ldBaseImage(&tTarget,
                                NULL,
                                ptWidget->ptPressImgTile,
                                ptWidget->ptPressMaskTile,
                                ptWidget->pressColor,
                                ptWidget->opacity);
                }
                else
                {
                    ldBaseImage(&tTarget,
                                NULL,
                                ptWidget->ptReleaseImgTile,
                                ptWidget->ptReleaseMaskTile,
                                ptWidget->releaseColor,
                                ptWidget->opacity);
                }
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
