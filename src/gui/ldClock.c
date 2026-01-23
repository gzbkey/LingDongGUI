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

#define __LD_CLOCK_IMPLEMENT__
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldClock.h"

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

const ldBaseWidgetFunc_t ldClockFunc = {
    .depose = (ldDeposeFunc_t)ldClock_depose,
    .load = (ldLoadFunc_t)ldClock_on_load,
    .frameStart = (ldFrameStartFunc_t)ldClock_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldClock_on_frame_complete,
    .show = (ldShowFunc_t)ldClock_show,
};

extern const arm_2d_tile_t c_tilePointerSecMask;

static
const arm_2d_tile_t c_tilePointerHourMask =
    impl_child_tile(c_tilePointerSecMask, 0, 0, 9, 67);

static
const arm_2d_tile_t c_tilePointerMinMask =
    impl_child_tile(c_tilePointerSecMask, 0, 0, 9, 80);

ldClock_t* ldClock_init(ld_scene_t *ptScene,ldClock_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldClock_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][clock] id:%d", nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldClock_t));
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeClock;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldClockFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;
//    ptScene->use_as__arm_2d_scene_t.bUseDirtyRegionHelper=true;

    do {
            meter_pointer_cfg_t tCFG = {
                .tSpinZoom = {
                    .Indicator = {
                        .LowerLimit = {
                            .fAngleInDegree = 0.0f,
                            .nValue = 0,
                        },
                        .UpperLimit = {
                            .fAngleInDegree = 360.0f,
                            .nValue = 60,
                        },
                    },
                    .ptTransformMode = &SPIN_ZOOM_MODE_FILL_COLOUR,
                    .Source = {
                        .ptMask = &c_tilePointerSecMask,
                        .tColourToFill = GLCD_COLOR_RED,
                    },
                    .ptScene = (arm_2d_scene_t *)ptScene,
                },

                .Pointer = {
                    .bIsSourceHorizontal = false,
                    .iRadius = 100,
                },

                .tPISliderCFG = {
                    .fProportion = 0.3000f,
                    .fIntegration = 0.30f,
                    .nInterval = 10,
                }

            };
            meter_pointer_init(&ptWidget->tSecPointer, &tCFG);
        } while(0);

    // initialize minutes pointer
        do {
            spin_zoom_widget_cfg_t tCFG = {
                .Indicator = {
                    .LowerLimit = {
                        .fAngleInDegree = 0.0f,
                        .nValue = 0,
                    },
                    .UpperLimit = {
                        .fAngleInDegree = 360.0f,
                        .nValue = 3600,
                    },
                    .Step = {
                        .fAngle = 1.0f,
                    },
                },
                .ptTransformMode = &SPIN_ZOOM_MODE_FILL_COLOUR,

                .bUseFloatPointInCentre = true,
                .Source = {
                    .ptMask = &c_tilePointerMinMask,
                    .tCentreFloat = (arm_2d_point_float_t){
                        .fX = c_tilePointerMinMask.tRegion.tSize.iWidth / 2.0f,
                        .fY = c_tilePointerMinMask.tRegion.tSize.iHeight,
                    },
                    .tColourToFill = GLCD_COLOR_BLACK,
                },
                .ptScene = (arm_2d_scene_t *)ptScene,
            };
            spin_zoom_widget_init(&ptWidget->tPointers[1], &tCFG);
        } while(0);

        // initialize hour pointer
        do {
            spin_zoom_widget_cfg_t tCFG = {
                .Indicator = {
                    .LowerLimit = {
                        .fAngleInDegree = 0.0f,
                        .nValue = 0,
                    },
                    .UpperLimit = {
                        .fAngleInDegree = 360.0f,
                        .nValue = 12 * 3600ul,
                    },
                },
                .ptTransformMode = &SPIN_ZOOM_MODE_FILL_COLOUR,
                .bUseFloatPointInCentre = true,
                .Source = {
                    .ptMask = &c_tilePointerHourMask,
                    .tCentreFloat = (arm_2d_point_float_t){
                        .fX = c_tilePointerHourMask.tRegion.tSize.iWidth / 2.0f,
                        .fY = c_tilePointerHourMask.tRegion.tSize.iHeight,
                    },
                    .tColourToFill = GLCD_COLOR_BLACK,
                },
                .ptScene = (arm_2d_scene_t *)ptScene,
            };
            spin_zoom_widget_init(&ptWidget->tPointers[0], &tCFG);

        } while(0);

    LOG_INFO("[init][clock] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    return ptWidget;
}

void ldClock_depose(ld_scene_t *ptScene, ldClock_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeClock)
    {
        return;
    }

    LOG_INFO("[depose][clock] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);

    meter_pointer_depose(&ptWidget->tSecPointer);

        arm_foreach(spin_zoom_widget_t, ptWidget->tPointers, ptPointer) {
            spin_zoom_widget_depose(ptPointer);
        }

    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldClock_on_load(ld_scene_t *ptScene, ldClock_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    arm_foreach(spin_zoom_widget_t, ptWidget->tPointers, ptPointer) {
            spin_zoom_widget_on_load(ptPointer);
        }

        meter_pointer_on_load(&ptWidget->tSecPointer);
}

void ldClock_on_frame_start(ld_scene_t *ptScene, ldClock_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    int64_t lTimeStampInMs = arm_2d_helper_convert_ticks_to_ms(
                                    arm_2d_helper_get_system_timestamp());

        /* calculate the hours */
        do {
            uint32_t wHour = lTimeStampInMs / 1000ul;
            wHour %= 12 * 3600ul;
            ptWidget->wHour = wHour / 3600;
            spin_zoom_widget_on_frame_start(&ptWidget->tPointers[0], wHour, 1.0f);

            lTimeStampInMs %= (3600ul * 1000ul);

        } while(0);

        /* calculate the Minutes */
        do {
            uint32_t wMin = lTimeStampInMs / 1000ul;

            ptWidget->wMin = wMin / 60;
            spin_zoom_widget_on_frame_start(&ptWidget->tPointers[1], wMin, 1.0f);

            lTimeStampInMs %= (60ul * 1000ul);
        } while(0);

        /* calculate the Seconds */
        do {
            uint32_t wSec = lTimeStampInMs;


            wSec /= 1000;
            if (!(ptWidget->nSec == -1 && wSec == 59)) {
                ptWidget->nSec = wSec;
            }

            if (meter_pointer_on_frame_start(&ptWidget->tSecPointer, ptWidget->nSec, 1.0f)) {
                /* when complete, map 59 to -1 */
                if (59 == ptWidget->nSec) {
                    meter_pointer_set_current_value(&ptWidget->tSecPointer, -1);
                    ptWidget->nSec = -1;
                }
                ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
            }
//    #else
//            ptWidget->nSec = wSec / 1000;
//            spin_zoom_widget_on_frame_start(&ptWidget->tPointers[2], ptWidget->nSec, 1.0f);
//    #endif
            lTimeStampInMs %= (1000ul);
        } while(0);

//        /* calculate the Ten-Miliseconds */
//        do {
//            ptWidget->wMs = lTimeStampInMs;
//        } while(0);
}

void ldClock_on_frame_complete(ld_scene_t *ptScene, ldClock_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    arm_foreach(spin_zoom_widget_t, ptWidget->tPointers, ptPointer) {
            spin_zoom_widget_on_frame_complete(ptPointer);
        }

    meter_pointer_on_frame_complete(&ptWidget->tSecPointer);
}

void ldClock_show(ld_scene_t *ptScene, ldClock_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
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

            arm_2d_align_centre(tTarget_canvas, 240, 240) {

                        arm_foreach(spin_zoom_widget_t, ptWidget->tPointers, ptPointer) {
                            spin_zoom_widget_show(ptPointer, &tTarget, &__centre_region, NULL, 255);
                        }

                            meter_pointer_show(&ptWidget->tSecPointer, &tTarget, &__centre_region, NULL, 255);


                            arm_2d_draw_box(&tTarget,&tTarget_canvas,1,0,255);
                    }

        }
    }

    arm_2d_op_wait_async(NULL);
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
