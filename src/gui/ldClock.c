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
#define __SPIN_ZOOM_WIDGET_IMPLEMENT__
#define __METER_POINTER_IMPLEMENT__
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
    ldBaseItemRegion_t *ptItemRegionList;
    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldClock_t));
        ptItemRegionList = ldCalloc(1,sizeof (ldBaseItemRegion_t)*3);
        if ((NULL == ptWidget)||(NULL == ptItemRegionList))
        {
            ldFree(ptWidget);
            ldFree(ptItemRegionList);
            LOG_ERROR("[init failed][clock] id:%d", nameId);
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
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeClock;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldClockFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->use_as__ldBase_t.itemCount=3;
    ptWidget->use_as__ldBase_t.ptItemRegionList=ptItemRegionList;

    ptWidget->pointerInfo[0].ptMaskTile=(arm_2d_tile_t*)&c_tilePointerHourMask;
    ptWidget->pointerInfo[0].rotationCentre.fX=c_tilePointerHourMask.tRegion.tSize.iWidth>>1;
    ptWidget->pointerInfo[0].rotationCentre.fY=c_tilePointerHourMask.tRegion.tSize.iHeight;
    ptWidget->use_as__ldBase_t.ptItemRegionList[0].itemRegion=ptWidget->pointerInfo[0].ptMaskTile->tRegion;
    ptWidget->use_as__ldBase_t.ptItemRegionList[0].itemRegion.tLocation.iX+=x;
    ptWidget->use_as__ldBase_t.ptItemRegionList[0].itemRegion.tLocation.iY+=y;
    ptWidget->use_as__ldBase_t.ptItemRegionList[0].isDRReset=true;
    ptWidget->use_as__ldBase_t.ptItemRegionList[0].isDRUpdate=true;

    ptWidget->pointerInfo[1].ptMaskTile=(arm_2d_tile_t*)&c_tilePointerMinMask;
    ptWidget->pointerInfo[1].rotationCentre.fX=c_tilePointerMinMask.tRegion.tSize.iWidth>>1;
    ptWidget->pointerInfo[1].rotationCentre.fY=c_tilePointerMinMask.tRegion.tSize.iHeight;
    ptWidget->use_as__ldBase_t.ptItemRegionList[1].itemRegion=ptWidget->pointerInfo[1].ptMaskTile->tRegion;
    ptWidget->use_as__ldBase_t.ptItemRegionList[1].itemRegion.tLocation.iX+=x;
    ptWidget->use_as__ldBase_t.ptItemRegionList[1].itemRegion.tLocation.iY+=y;
    ptWidget->use_as__ldBase_t.ptItemRegionList[1].isDRReset=true;
    ptWidget->use_as__ldBase_t.ptItemRegionList[1].isDRUpdate=true;

    ptWidget->pointerInfo[2].ptMaskTile=(arm_2d_tile_t*)&c_tilePointerSecMask;
    ptWidget->pointerInfo[2].rotationCentre.fX=c_tilePointerSecMask.tRegion.tSize.iWidth>>1;
    ptWidget->pointerInfo[2].rotationCentre.fY=100;
    ptWidget->use_as__ldBase_t.ptItemRegionList[2].itemRegion=ptWidget->pointerInfo[2].ptMaskTile->tRegion;
    ptWidget->use_as__ldBase_t.ptItemRegionList[2].itemRegion.tLocation.iX+=x;
    ptWidget->use_as__ldBase_t.ptItemRegionList[2].itemRegion.tLocation.iY+=y;
    ptWidget->use_as__ldBase_t.ptItemRegionList[2].isDRReset=true;
    ptWidget->use_as__ldBase_t.ptItemRegionList[2].isDRUpdate=true;

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
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget->use_as__ldBase_t.ptItemRegionList);
    ldFree(ptWidget);
}

void ldClock_on_load(ld_scene_t *ptScene, ldClock_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
}

void ldClock_on_frame_start(ld_scene_t *ptScene, ldClock_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL) return;

    int64_t lTimeStampInMs = arm_2d_helper_convert_ticks_to_ms(arm_2d_helper_get_system_timestamp());
    uint32_t total_seconds = lTimeStampInMs / 1000UL;

    bool bNeedHourUpdate = false;
    bool bNeedMinuteUpdate = false;
    bool bNeedSecondUpdate = false;

    uint32_t hour_sec = total_seconds % (12UL * 3600UL);
    uint32_t min_sec = total_seconds % 3600UL;
    uint32_t sec_ms = lTimeStampInMs % 60000UL;

    if(ptWidget->isStepSecond)
    {
        uint32_t current_second = total_seconds % 60UL;
        if(current_second != (ptWidget->lastTotalSeconds % 60UL))
        {
            bNeedSecondUpdate = true;
        }
    }
    else
    {
        bNeedSecondUpdate = true;
    }

    uint32_t current_minute = min_sec / 60UL;
    uint32_t last_minute = (ptWidget->lastTotalSeconds % 3600UL) / 60UL;
    if(current_minute != last_minute)
    {
        bNeedMinuteUpdate = true;
    }

    uint32_t current_hour = hour_sec / 3600UL;
    uint32_t last_hour = (ptWidget->lastTotalSeconds % (12UL * 3600UL)) / 3600UL;

    if(current_hour != last_hour ||
       (current_minute / 2) != (last_minute / 2))
    {
        bNeedHourUpdate = true;
    }

    if(bNeedHourUpdate || bNeedMinuteUpdate || bNeedSecondUpdate)
    {
        ptWidget->lastTotalSeconds = total_seconds;

        arm_2d_region_t globalRegion;
        arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t*)ptWidget,&globalRegion,true);

        float hour_angle = (hour_sec / 3600UL) * 30.0f +
                           ((hour_sec % 3600UL) / 60UL) * 0.5f;
        ptWidget->pointerInfo[0].radian = ANGLE_2_RADIAN(hour_angle);

        float min_angle = (min_sec / 60UL) * 6.0f;
        ptWidget->pointerInfo[1].radian = ANGLE_2_RADIAN(min_angle);

        float sec_angle;
        if(ptWidget->isStepSecond)
        {
            sec_angle = (total_seconds % 60UL) * 6.0f;
        }
        else
        {
            sec_angle = (sec_ms / 1000.0f) * 6.0f;
        }
        ptWidget->pointerInfo[2].radian = ANGLE_2_RADIAN(sec_angle);

        if(bNeedHourUpdate && ptWidget->pointerInfo[0].op.Target.ptRegion!=NULL)
        {
            ptWidget->use_as__ldBase_t.ptItemRegionList[0].itemRegion = *ptWidget->pointerInfo[0].op.Target.ptRegion;
            ptWidget->use_as__ldBase_t.ptItemRegionList[0].itemRegion.tLocation.iX+=globalRegion.tLocation.iX;
            ptWidget->use_as__ldBase_t.ptItemRegionList[0].itemRegion.tLocation.iY+=globalRegion.tLocation.iY;
            ptWidget->use_as__ldBase_t.ptItemRegionList[0].isDRUpdate=true;
            arm_2d_region_get_minimal_enclosure(&ptWidget->use_as__ldBase_t.ptItemRegionList[0].tTempItemRegion,
                                                &ptWidget->use_as__ldBase_t.ptItemRegionList[0].itemRegion,
                                                &ptWidget->use_as__ldBase_t.ptItemRegionList[0].tTempItemRegion);
        }

        if(bNeedMinuteUpdate && ptWidget->pointerInfo[1].op.Target.ptRegion!=NULL)
        {
            ptWidget->use_as__ldBase_t.ptItemRegionList[1].itemRegion = *ptWidget->pointerInfo[1].op.Target.ptRegion;
            ptWidget->use_as__ldBase_t.ptItemRegionList[1].itemRegion.tLocation.iX+=globalRegion.tLocation.iX;
            ptWidget->use_as__ldBase_t.ptItemRegionList[1].itemRegion.tLocation.iY+=globalRegion.tLocation.iY;
            ptWidget->use_as__ldBase_t.ptItemRegionList[1].isDRUpdate=true;
            arm_2d_region_get_minimal_enclosure(&ptWidget->use_as__ldBase_t.ptItemRegionList[1].tTempItemRegion,
                                                &ptWidget->use_as__ldBase_t.ptItemRegionList[1].itemRegion,
                                                &ptWidget->use_as__ldBase_t.ptItemRegionList[1].tTempItemRegion);
        }

        if(bNeedSecondUpdate && ptWidget->pointerInfo[2].op.Target.ptRegion!=NULL)
        {
            ptWidget->use_as__ldBase_t.ptItemRegionList[2].itemRegion = *ptWidget->pointerInfo[2].op.Target.ptRegion;
            ptWidget->use_as__ldBase_t.ptItemRegionList[2].itemRegion.tLocation.iX+=globalRegion.tLocation.iX;
            ptWidget->use_as__ldBase_t.ptItemRegionList[2].itemRegion.tLocation.iY+=globalRegion.tLocation.iY;
            ptWidget->use_as__ldBase_t.ptItemRegionList[2].isDRUpdate=true;
            arm_2d_region_get_minimal_enclosure(&ptWidget->use_as__ldBase_t.ptItemRegionList[2].tTempItemRegion,
                                                &ptWidget->use_as__ldBase_t.ptItemRegionList[2].itemRegion,
                                                &ptWidget->use_as__ldBase_t.ptItemRegionList[2].tTempItemRegion);
        }

        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    }
}

void ldClock_on_frame_complete(ld_scene_t *ptScene, ldClock_t *ptWidget)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
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

            int16_t w,h;
            if(ptWidget->ptBgImgTile != NULL)
            {
                w=ptWidget->ptBgImgTile->tRegion.tSize.iWidth;
                h=ptWidget->ptBgImgTile->tRegion.tSize.iHeight;
            }
            else
            {
                if(ptWidget->ptBgMaskTile != NULL)
                {
                    w=ptWidget->ptBgMaskTile->tRegion.tSize.iWidth;
                    h=ptWidget->ptBgMaskTile->tRegion.tSize.iHeight;
                }
                else
                {
                    w=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth;
                    h=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
                }
            }
            
            arm_2d_align_centre(tTarget_canvas, w, h)
            {
                if(ptWidget->ptBgImgTile||ptWidget->ptBgMaskTile)
                {
                    ldBaseImage(&tTarget,
                                &__centre_region,
                                ptWidget->ptBgImgTile,
                                ptWidget->ptBgMaskTile,
                                ptWidget->bgMaskColor,
                                ptWidget->use_as__ldBase_t.opacity);
                }

                arm_2d_point_float_t bgRotationCentre=
                {
                    .fX = (tTarget_canvas.tSize.iWidth>>1),
                    .fY = (tTarget_canvas.tSize.iHeight>>1),
                };

                for(uint8_t i=0;i<3;i++)
                {
                    if((ptWidget->pointerInfo[i].ptImgTile!=NULL)&&(ptWidget->pointerInfo[i].ptMaskTile!=NULL))
                    {
                        arm_2dp_tile_transform_xy_with_src_mask_and_opacity(&ptWidget->pointerInfo[i].op,
                                                                            ptWidget->pointerInfo[i].ptImgTile,
                                                                            ptWidget->pointerInfo[i].ptMaskTile,
                                                                            &tTarget,
                                                                            &__centre_region,
                                                                            ptWidget->pointerInfo[i].rotationCentre,
                                                                            ptWidget->pointerInfo[i].radian,
                                                                            1.0,
                                                                            1.0,
                                                                            ptWidget->use_as__ldBase_t.opacity,
                                                                            &bgRotationCentre);
                    }
                    else if((ptWidget->pointerInfo[i].ptImgTile==NULL)&&(ptWidget->pointerInfo[i].ptMaskTile!=NULL))
                    {
                        arm_2dp_fill_colour_with_mask_opacity_and_transform_xy((arm_2d_op_trans_opa_t*)&ptWidget->pointerInfo[i].op,
                                                                               ptWidget->pointerInfo[i].ptMaskTile,
                                                                               &tTarget,
                                                                               &__centre_region,
                                                                               ptWidget->pointerInfo[i].rotationCentre,
                                                                               ptWidget->pointerInfo[i].radian,
                                                                               1.0,
                                                                               1.0,
                                                                               ptWidget->pointerInfo[i].maskColor,
                                                                               ptWidget->use_as__ldBase_t.opacity,
                                                                               &bgRotationCentre);
                    }
                    else if((ptWidget->pointerInfo[i].ptImgTile!=NULL)&&(ptWidget->pointerInfo[i].ptMaskTile==NULL))
                    {
                        arm_2dp_tile_transform_xy_with_opacity((arm_2d_op_trans_opa_t*)&ptWidget->pointerInfo[i].op,
                                                               ptWidget->pointerInfo[i].ptImgTile,
                                                               &tTarget,
                                                               &__centre_region,
                                                               ptWidget->pointerInfo[i].rotationCentre,
                                                               ptWidget->pointerInfo[i].radian,
                                                               1.0,
                                                               1.0,
                                                               ptWidget->pointerInfo[i].maskColor,
                                                               ptWidget->use_as__ldBase_t.opacity,
                                                               &bgRotationCentre);
                    }

                }

                arm_2d_op_wait_async(NULL);
            }
        }
    }
}

void ldClockSetBackgroundImage(ldClock_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile, ldColor maskColor)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptBgImgTile = ptImgTile;
    ptWidget->ptBgMaskTile = ptMaskTile;
    ptWidget->bgMaskColor = maskColor;
}

void ldClockSetHourPointerImage(ldClock_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile, ldColor maskColor, float x, float y)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;    
    ptWidget->use_as__ldBase_t.ptItemRegionList[0].isDRUpdate=true;
    ptWidget->pointerInfo[0].ptImgTile = ptImgTile;
    ptWidget->pointerInfo[0].ptMaskTile = ptMaskTile;
    ptWidget->pointerInfo[0].maskColor = maskColor;
    ptWidget->pointerInfo[0].rotationCentre = (arm_2d_point_float_t){x,y};
}

void ldClockSetMinutePointerImage(ldClock_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile, ldColor maskColor, float x, float y)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.ptItemRegionList[1].isDRUpdate=true;
    ptWidget->pointerInfo[1].ptImgTile = ptImgTile;
    ptWidget->pointerInfo[1].ptMaskTile = ptMaskTile;
    ptWidget->pointerInfo[1].maskColor = maskColor;
    ptWidget->pointerInfo[1].rotationCentre = (arm_2d_point_float_t){x,y};
}

void ldClockSetSecondPointerImage(ldClock_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile, ldColor maskColor, float x, float y)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.ptItemRegionList[2].isDRUpdate=true;
    ptWidget->pointerInfo[2].ptImgTile = ptImgTile;
    ptWidget->pointerInfo[2].ptMaskTile = ptMaskTile;
    ptWidget->pointerInfo[2].maskColor = maskColor;
    ptWidget->pointerInfo[2].rotationCentre = (arm_2d_point_float_t){x,y};
}

void ldClockSetStepSecond(ldClock_t *ptWidget, bool isStepSecond)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->isStepSecond=isStepSecond;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
