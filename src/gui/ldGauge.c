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

#define __LD_GAUGE_IMPLEMENT__

#include "__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldGauge.h"

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

#define ANGLE_OFFSET       180 //调整指针起始位置

const ldBaseWidgetFunc_t ldGaugeFunc = {
    .depose = (ldDeposeFunc_t)ldGauge_depose,
    .load = (ldLoadFunc_t)ldGauge_on_load,
    .frameStart = (ldFrameStartFunc_t)ldGauge_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldGauge_on_frame_complete,
    .show = (ldShowFunc_t)ldGauge_show,
};

ldGauge_t* ldGauge_init( ld_scene_t *ptScene,ldGauge_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, arm_2d_tile_t *ptBgImgTile,arm_2d_tile_t *ptBgMaskTile, int16_t centreOffsetX, int16_t centreOffsetY)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    ldBaseItemRegion_t *ptItemRegion;
    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldGauge_t));
        ptItemRegion = ldCalloc(1,sizeof (ldBaseItemRegion_t));
        if ((NULL == ptWidget)||(NULL == ptItemRegion))
        {
            ldFree(ptWidget);
            ldFree(ptItemRegion);
            LOG_ERROR("[init failed][gauge] id:%d", nameId);
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
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeGauge;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldGaugeFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptItemRegion->tTempItemRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;
    ptItemRegion->itemRegion=ptItemRegion->tTempItemRegion;

    ptWidget->use_as__ldBase_t.ptItemRegionList=ptItemRegion;
    ptWidget->use_as__ldBase_t.itemCount=1;

    ptWidget->ptBgImgTile=ptBgImgTile;
    ptWidget->ptBgMaskTile=ptBgMaskTile;
    ptWidget->centreOffsetX=centreOffsetX;
    ptWidget->centreOffsetY=centreOffsetY;
    
    LOG_INFO("[init][gauge] id:%d, size:%llu", nameId,sizeof (*ptWidget));
    return ptWidget;
}

void ldGauge_depose(ld_scene_t *pScene, ldGauge_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeGauge)
    {
        return;
    }

    LOG_INFO("[depose][gauge] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
#if USE_VIRTUAL_RESOURCE == 1
    ldFree(ptWidget->ptBgImgTile);
    ldFree(ptWidget->ptBgMaskTile);
    ldFree(ptWidget->ptPointerImgTile);
    ldFree(ptWidget->ptPointerMaskTile);
#endif
    ldFree(ptWidget->use_as__ldBase_t.ptItemRegionList);
    ldFree(ptWidget);
}

void ldGauge_on_load(ld_scene_t *pScene, ldGauge_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldGauge_on_frame_start(ld_scene_t *pScene, ldGauge_t *ptWidget)
{
    assert(NULL != ptWidget);

    if (!ptWidget->isAutoMove || ptWidget->_nowAngle_x10 == ptWidget->endAngle_x10)
    {
        return;
    }

    int16_t endDiff = ptWidget->endAngle_x10 - ptWidget->_nowAngle_x10;
    if(endDiff > 1800)
    {
        endDiff -= 3600;
    }
    else if(endDiff < -1800)
    {
        endDiff += 3600;
    }

    int16_t startDiff = ptWidget->startAngle_x10 - ptWidget->_nowAngle_x10;
    if (startDiff > 1800)
    {
        startDiff -= 3600;
    }
    else if (startDiff < -1800)
    {
        startDiff += 3600;
    }

    uint16_t dStart = (uint16_t)abs(startDiff);
    dStart=(dStart > 1800)?(3600 - dStart):dStart;
    uint16_t dEnd   = (uint16_t)abs(endDiff);
    dEnd=(dEnd > 1800)?(3600 - dEnd):dEnd;
    uint16_t fadeDist = (dStart < dEnd) ? dStart : dEnd;

    ptWidget->_trailOpacity = (fadeDist > 510) ? 255 : (uint8_t)(fadeDist >>1);

    if(endDiff > 10)
    {
        ptWidget->_nowAngle_x10 += 10;
    }
    else if(endDiff < -10)
    {
        ptWidget->_nowAngle_x10 -= 10;
    }
    else
    {
        ptWidget->_nowAngle_x10 = ptWidget->endAngle_x10;
    }

    ptWidget->_nowAngle_x10 %= 3600;
    if (ptWidget->_nowAngle_x10 < 0)
    {
        ptWidget->_nowAngle_x10 += 3600;
    }

    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
}

void ldGauge_on_frame_complete(ld_scene_t *pScene, ldGauge_t *ptWidget)
{
    assert(NULL != ptWidget);
}

void ldGauge_show(ld_scene_t *ptScene, ldGauge_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    if (bIsNewFrame)
    {
        if((ptWidget->op.Target.ptRegion!=NULL)&&(ptWidget->use_as__ldBase_t.isDirtyRegionUpdate==true))
        {
        ptWidget->use_as__ldBase_t.ptItemRegionList->itemRegion= *ptWidget->op.Target.ptRegion;

        arm_2d_region_get_minimal_enclosure(&ptWidget->use_as__ldBase_t.ptItemRegionList->tTempItemRegion,
                                            &ptWidget->use_as__ldBase_t.ptItemRegionList->itemRegion,
                                            &ptWidget->use_as__ldBase_t.ptItemRegionList->tTempItemRegion);
        }
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

            ldBaseImage(&tTarget,
                        NULL,
                        ptWidget->ptBgImgTile,
                        ptWidget->ptBgMaskTile,
                        0,
                        ptWidget->use_as__ldBase_t.opacity);

            arm_2d_op_wait_async(NULL);

            do {

                arm_2d_point_float_t pointerRotationCentre =
                {
                    .fX = ptWidget->pointerOriginOffsetX,
                    .fY = ptWidget->pointerOriginOffsetY,
                };

                arm_2d_point_float_t bgRotationCentre=
                {
                    .fX = (tTarget_canvas.tSize.iWidth>>1)+ptWidget->centreOffsetX,
                    .fY = (tTarget_canvas.tSize.iHeight>>1)+ptWidget->centreOffsetY,
                };

                if((ptWidget->isAutoMove)&&(ptWidget->ptBgTrailMaskTile!=NULL)&&(ptWidget->ptPointerTrailMaskTile!=NULL))
                {
                    arm_2d_point_float_t maskPointerRotationCentre={
                        .fX=ptWidget->ptPointerTrailMaskTile->tRegion.tSize.iWidth,
                        .fY=ptWidget->ptPointerTrailMaskTile->tRegion.tSize.iHeight,
                    };

                    float scale=1.0;

                    if((ptWidget->endAngle_x10-ptWidget->_nowAngle_x10)<0)
                    {
                        scale=-1.0;
                    }

                    arm_2dp_fill_colour_with_transformed_mask_target_mask_and_opacity(&ptWidget->trailOp,
                                                                                      ptWidget->ptPointerTrailMaskTile,
                                                                                      &tTarget,
                                                                                      ptWidget->ptBgTrailMaskTile,
                                                                                      &tTarget_canvas,
                                                                                      maskPointerRotationCentre,
                                                                                      ANGLE_2_RADIAN((ptWidget->_nowAngle_x10)/10.0+ANGLE_OFFSET),
                                                                                      scale,
                                                                                      1.0,
                                                                                      ptWidget->maskColor,
                                                                                      ptWidget->_trailOpacity,
                                                                                      &bgRotationCentre);

                    arm_2d_op_wait_async(NULL);
                }

                if((ptWidget->ptPointerImgTile!=NULL)&&(ptWidget->ptPointerMaskTile!=NULL))
                {
                    arm_2dp_tile_transform_xy_with_src_mask_and_opacity(&ptWidget->op,
                                                                     ptWidget->ptPointerImgTile,
                                                                     ptWidget->ptPointerMaskTile,
                                                                     &tTarget,
                                                                     &tTarget_canvas,
                                                                     pointerRotationCentre,
                                                                     ANGLE_2_RADIAN((ptWidget->_nowAngle_x10)/10.0+ANGLE_OFFSET),
                                                                     1.0,
                                                                     1.0,
                                                                     ptWidget->use_as__ldBase_t.opacity,
                                                                     &bgRotationCentre);
                }
                else if((ptWidget->ptPointerImgTile==NULL)&&(ptWidget->ptPointerMaskTile!=NULL))
                {
                    arm_2dp_fill_colour_with_mask_opacity_and_transform_xy((arm_2d_op_trans_opa_t *)&ptWidget->op,
                                                                           ptWidget->ptPointerMaskTile,
                                                                           &tTarget,
                                                                           &tTarget_canvas,
                                                                           pointerRotationCentre,
                                                                           ANGLE_2_RADIAN((ptWidget->_nowAngle_x10)/10.0+ANGLE_OFFSET),//ptWidget->helper.fAngle,
                                                                           1.0,
                                                                           1.0,
                                                                           ptWidget->maskColor,
                                                                           ptWidget->use_as__ldBase_t.opacity,
                                                                           &bgRotationCentre);
                }
                else if((ptWidget->ptPointerImgTile!=NULL)&&(ptWidget->ptPointerMaskTile==NULL))
                {
                    arm_2dp_tile_transform_xy_with_opacity((arm_2d_op_trans_opa_t *)&ptWidget->op,
                                                           ptWidget->ptPointerImgTile,
                                                           &tTarget,
                                                           &tTarget_canvas,
                                                           pointerRotationCentre,
                                                           ANGLE_2_RADIAN((ptWidget->_nowAngle_x10)/10.0+ANGLE_OFFSET),
                                                           1.0,
                                                           1.0,
                                                           ptWidget->maskColor,
                                                           ptWidget->use_as__ldBase_t.opacity,
                                                           &bgRotationCentre);
                }
                else if(ptWidget->isKeying)
                {
                    if (bIsNewFrame)
                    {
                        arm_2dp_rgb565_tile_transform_xy_with_colour_keying_prepare((arm_2d_op_trans_t *)&ptWidget->op,
                                                                                    ptWidget->ptPointerImgTile,
                                                                                    pointerRotationCentre,
                                                                                    ANGLE_2_RADIAN((ptWidget->_nowAngle_x10)/10.0+ANGLE_OFFSET),
                                                                                    1.0,
                                                                                    1.0,
                                                                                    ptWidget->maskColor);
                    };
                    arm_2dp_tile_transform_xy((arm_2d_op_trans_t *)&ptWidget->op,
                                              &tTarget,
                                              &tTarget_canvas,
                                              &bgRotationCentre);

                    break;
                }
                arm_2d_op_wait_async(NULL);

            } while(0);

            LD_BASE_WIDGET_SELECT;
            arm_2d_op_wait_async(NULL);
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldGaugeSetPointerImage(ldGauge_t *ptWidget,arm_2d_tile_t *ptPointerImgTile,arm_2d_tile_t *ptPointerMaskTile,int16_t pointerOriginOffsetX,int16_t pointerOriginOffsetY)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->ptPointerImgTile=ptPointerImgTile;
    ptWidget->ptPointerMaskTile=ptPointerMaskTile;
    ptWidget->pointerOriginOffsetX=pointerOriginOffsetX;
    ptWidget->pointerOriginOffsetY=pointerOriginOffsetY;
}

void ldGaugeSetPointerColor(ldGauge_t *ptWidget,ldColor color)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->maskColor=color;
}

void ldGaugeSetAngle(ldGauge_t *ptWidget, float angle)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    int16_t newEnd = ((int16_t)(angle * 10)) % 3600;
    if(newEnd < 0)
    {
        newEnd += 3600;
    }

    if(ptWidget->_nowAngle_x10 == ptWidget->endAngle_x10)
    {
        ptWidget->startAngle_x10 = ptWidget->_nowAngle_x10;
    }

    ptWidget->endAngle_x10 = newEnd;

    if(!ptWidget->isAutoMove)
    {
        ptWidget->_nowAngle_x10=ptWidget->endAngle_x10;
    }
}

void ldGaugeSetTrail(ldGauge_t *ptWidget,arm_2d_tile_t *ptBgTrailMaskTile,arm_2d_tile_t *ptPointerTrailMaskTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptBgTrailMaskTile=ptBgTrailMaskTile;
    ptWidget->ptPointerTrailMaskTile=ptPointerTrailMaskTile;
}

void ldGaugeSetAutoMove(ldGauge_t *ptWidget, bool enable)
{
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->isAutoMove = enable;
    if(enable && ptWidget->_nowAngle_x10 == 0)
    {
        ptWidget->_nowAngle_x10 = ptWidget->endAngle_x10;
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
