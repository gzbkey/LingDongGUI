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

#define __LD_ARC_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldArc.h"

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

const ldBaseWidgetFunc_t ldArcFunc = {
    .depose = (ldDeposeFunc_t)ldArc_depose,
    .load = (ldLoadFunc_t)ldArc_on_load,
    .frameStart = (ldFrameStartFunc_t)ldArc_on_frame_start,
    .show = (ldShowFunc_t)ldArc_show,
};

ldArc_t* ldArc_init( ld_scene_t *ptScene,ldArc_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,arm_2d_tile_t *ptQuarterImgTile,arm_2d_tile_t *ptQuarterMaskTile,ldColor parentColor)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldArc_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][arc] id:%d", nameId);
            return NULL;
        }
    }
    else
    {
        memset(ptWidget, 0, sizeof(ldArc_t));
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

//    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeArc;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldArcFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->color[0]=GLCD_COLOR_LIGHT_GREY;
    ptWidget->color[1]=__RGB(173, 216, 230);
    ptWidget->parentColor=parentColor;
    ptWidget->ptImgTile=ptQuarterImgTile;
    ptWidget->ptMaskTile=ptQuarterMaskTile;

    ptWidget->startAngle_x10[0]=0;
    ptWidget->endAngle_x10[0]=3600;
    ptWidget->startAngle_x10[1]=0;
    ptWidget->endAngle_x10[1]=1800;
    ptWidget->rotationAngle_x10=0;

    LOG_INFO("[init][arc] id:%d", nameId);
    return ptWidget;
}

void ldArc_depose( ldArc_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeArc)
    {
        return;
    }

    LOG_INFO("[depose][arc] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);

    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldArc_on_load( ldArc_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldArc_on_frame_start( ldArc_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

arm_2d_location_t _ldArcGetStartEndAreaPos(uint8_t quarterNum,arm_2d_size_t widgetFullSize)
{
    arm_2d_location_t retPos={0};
    switch (quarterNum)
    {
    case 7:
    case 3:
    {
        retPos.iX=0;
        retPos.iY=0;
        break;
    }
    case 8:
    case 4:
    case 0:
    {
        retPos.iX=widgetFullSize.iWidth>>1;
        retPos.iY=0;
        break;
    }
    case 5:
    case 1:
    {
        retPos.iX=widgetFullSize.iWidth>>1;
        retPos.iY=widgetFullSize.iHeight>>1;
        break;
    }
    case 6:
    case 2:
    {
        retPos.iX=0;
        retPos.iY=widgetFullSize.iHeight>>1;
        break;
    }
    }
    return retPos;
}

static uint8_t _ldArcGetQuarterDraw(float fStartAngle,float fEndAngle)
{
    uint8_t startQuarter, endQuarter;
    uint8_t retFlag=0;
    int startAngleX10;

    startQuarter = fStartAngle / 90;
    endQuarter = fEndAngle / 90;

    if(fEndAngle-fStartAngle>=90.0)
    {
        startAngleX10=fStartAngle*10;
        if(startAngleX10%900==0)
        {
            SETBIT(retFlag,(startQuarter%4));
        }
    }

    startQuarter++;
    while (endQuarter>startQuarter)
    {
        SETBIT(retFlag,(startQuarter%4));
        startQuarter++;
    }
    return retFlag;
}

static void _ldArcDrawQuarter(arm_2d_tile_t *pTarget,arm_2d_region_t canvas,arm_2d_tile_t *pMaskRes,uint8_t quarterFlag,ldColor color,uint8_t opacity)
{
    arm_2d_region_t maskRegion;
    for(uint8_t j=0;j<4;j++)
    {
        if(GETBIT(quarterFlag,j))
        {
            maskRegion=canvas;
            switch (j)
            {
            case 0:
            {
                maskRegion.tLocation.iX+=canvas.tSize.iWidth>>1;
                arm_2d_fill_colour_with_mask_x_mirror_and_opacity(pTarget,&maskRegion,pMaskRes,(__arm_2d_color_t)color,opacity);
                break;
            }
            case 1:
            {
                maskRegion.tLocation.iX+=canvas.tSize.iWidth>>1;
                maskRegion.tLocation.iY+=canvas.tSize.iHeight>>1;
                arm_2d_fill_colour_with_mask_xy_mirror_and_opacity(pTarget,&maskRegion,pMaskRes,(__arm_2d_color_t)color,opacity);
                break;
            }
            case 2:
            {
                maskRegion.tLocation.iY+=canvas.tSize.iHeight>>1;
                arm_2d_fill_colour_with_mask_y_mirror_and_opacity(pTarget,&maskRegion,pMaskRes,(__arm_2d_color_t)color,opacity);
                break;
            }
            case 3:
            {
                arm_2d_fill_colour_with_mask_and_opacity(pTarget,&maskRegion,pMaskRes,(__arm_2d_color_t)color,opacity);
                break;
            }
            }
        }
    }
}

void ldArc_show(ld_scene_t *ptScene, ldArc_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    //         270                   0
    //          |                    |
    //        2 | 1                3 | 0
    //    180-------0    ==>>  270-------90
    //        3 | 4                2 | 1
    //          |                    |
    //         90                   180

    arm_2d_location_t center;
    arm_2d_region_t showRegion;
    float fStartAngle[2],fEndAngle[2],tempAngle;
    arm_2d_location_t maskCenter;
    uint8_t startQuarter,endQuarter;
    uint8_t quarterDrawFlag[2]={0};

#if 0
    if (bIsNewFrame) {
        
    }
#endif
    arm_2d_region_t globalRegion=ldBaseGetAbsoluteRegion(ptWidget);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

//            ((arm_2d_tile_t*)&tempRes)->pchBuffer = (uint8_t *)ptWidget->srcAddr;
//#if USE_VIRTUAL_RESOURCE == 1
//            ((arm_2d_vres_t*)(&tempRes))->pTarget=ptWidget->srcAddr;
//#endif
//            ((arm_2d_tile_t*)&tempRes)->tInfo.tColourInfo.chScheme=ARM_2D_COLOUR_MASK_A8;

//            ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iWidth=(((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iWidth>>1)+1;
//            ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iHeight=(((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iHeight>>1)+1;

            center.iX=ptWidget->ptImgTile->tRegion.tSize.iWidth-1;
            center.iY=ptWidget->ptImgTile->tRegion.tSize.iHeight-1;

            maskCenter.iX=tTarget_canvas.tSize.iWidth>>1;
            maskCenter.iY=tTarget_canvas.tSize.iHeight>>1;

            for(uint8_t i=0;i<2;i++)
            {
                fStartAngle[i]=(float)(ptWidget->startAngle_x10[i]+ptWidget->rotationAngle_x10)/10.0;
                fEndAngle[i]=(float)(ptWidget->endAngle_x10[i]+ptWidget->rotationAngle_x10)/10.0;

                startQuarter = fStartAngle[i] / 90;
                endQuarter = fEndAngle[i] / 90;

                showRegion.tSize.iWidth=(tTarget_canvas.tSize.iWidth>>1)+1;
                showRegion.tSize.iHeight=(tTarget_canvas.tSize.iHeight>>1)+1;

                quarterDrawFlag[i]=_ldArcGetQuarterDraw(fStartAngle[i],fEndAngle[i]);

                if(i==1)//draw bg quarter,except overlaps fg quarter
                {
                    uint8_t flag=quarterDrawFlag[0]^quarterDrawFlag[1];

                    _ldArcDrawQuarter(&tTarget,tTarget_canvas,ptWidget->ptImgTile,flag,ptWidget->color[0],ptWidget->use_as__ldBase_t.opacity);
                    arm_2d_op_wait_async(NULL);
                }

                if((i==1)&&(ptWidget->endAngle_x10[i]==0))
                {
                    continue;
                }

                if(((startQuarter%4)==(endQuarter%4))&&(GETBIT(quarterDrawFlag[i],startQuarter%4)==0))//同一象限,且不为90度
                {
                    showRegion.tLocation=_ldArcGetStartEndAreaPos (endQuarter,tTarget_canvas.tSize);
                    if((fEndAngle[i]-fStartAngle[i])>90)// 大于270度圆弧
                    {

                        tempAngle=fEndAngle[i];
                        if(tempAngle>=360.0)
                        {
                            tempAngle-=360.0;
                        }
                        arm_2d_fill_colour_with_mask_opacity_and_transform(ptWidget->ptImgTile,
                                                                           &tTarget,
                                                                           &showRegion,
                                                                           center,
                                                                           ARM_2D_ANGLE(tempAngle),
                                                                           1.0f,
                                                                           ptWidget->color[i],
                                                                           ptWidget->use_as__ldBase_t.opacity,
                                                                           &maskCenter
                                                                           );
                        tempAngle=fStartAngle[i];
                        tempAngle+=90;
                        if(tempAngle>=360.0)
                        {
                            tempAngle-=360.0;
                        }
                        arm_2d_fill_colour_with_mask_opacity_and_transform(ptWidget->ptImgTile,
                                                                           &tTarget,
                                                                           &showRegion,
                                                                           center,
                                                                           ARM_2D_ANGLE(tempAngle),
                                                                           1.0f,
                                                                           ptWidget->color[i],
                                                                           ptWidget->use_as__ldBase_t.opacity,
                                                                           &maskCenter
                                                                           );
                    }
                    else// 小于90度圆弧
                    {
                        tempAngle=fEndAngle[i];
                        if(tempAngle>=360.0)
                        {
                            tempAngle-=360.0;
                        }

                        arm_2d_fill_colour_with_mask_opacity_and_transform(ptWidget->ptImgTile,
                                                                           &tTarget,
                                                                           &showRegion,
                                                                           center,
                                                                           ARM_2D_ANGLE(tempAngle),
                                                                           1.0f,
                                                                           ptWidget->color[i],
                                                                           ptWidget->use_as__ldBase_t.opacity,
                                                                           &maskCenter
                                                                           );

                        tempAngle=fStartAngle[i];
                        if(tempAngle>=360.0)
                        {
                            tempAngle-=360.0;
                        }

                        if((tempAngle!=0)&&(tempAngle!=90)&&(tempAngle!=180)&&(tempAngle!=270))
                        {
                            arm_2d_fill_colour_with_mask_opacity_and_transform(ptWidget->ptMaskTile,
                                                                               &tTarget,
                                                                               &showRegion,
                                                                               center,
                                                                               ARM_2D_ANGLE(tempAngle),
                                                                               1.0f,
                                                                               ptWidget->parentColor,
                                                                               ptWidget->use_as__ldBase_t.opacity,
                                                                               &maskCenter
                                                                               );
                            if((fEndAngle[0]-fStartAngle[0])>90)// 大于270度圆弧
                            {

                                tempAngle=fEndAngle[0];
                                if(tempAngle>=360.0)
                                {
                                    tempAngle-=360.0;
                                }
                                arm_2d_fill_colour_with_mask_opacity_and_transform(ptWidget->ptImgTile,
                                                                                   &tTarget,
                                                                                   &showRegion,
                                                                                   center,
                                                                                   ARM_2D_ANGLE(tempAngle),
                                                                                   1.0f,
                                                                                   ptWidget->color[0],
                                        ptWidget->use_as__ldBase_t.opacity,
                                        &maskCenter
                                        );
                            }
                        }
                    }
                }
                else
                {
                    showRegion.tLocation=_ldArcGetStartEndAreaPos (endQuarter,tTarget_canvas.tSize);
                    tempAngle=fEndAngle[i];
                    if(tempAngle>=360.0)
                    {
                        tempAngle-=360.0;
                    }
                    arm_2d_fill_colour_with_mask_opacity_and_transform(ptWidget->ptImgTile,
                                                                       &tTarget,
                                                                       &showRegion,
                                                                       center,
                                                                       ARM_2D_ANGLE(tempAngle),
                                                                       1.0f,
                                                                       ptWidget->color[i],
                                                                       ptWidget->use_as__ldBase_t.opacity,
                                                                       &maskCenter
                                                                       );

                    showRegion.tLocation=_ldArcGetStartEndAreaPos (startQuarter,tTarget_canvas.tSize);

                    tempAngle=fStartAngle[i];
                    tempAngle+=90;
                    if(tempAngle>=360.0)
                    {
                        tempAngle-=360.0;
                    }
                    if((tempAngle!=90)&&(tempAngle!=180)&&(tempAngle!=270)&&(tempAngle!=360))
                    {
                        arm_2d_fill_colour_with_mask_opacity_and_transform(ptWidget->ptImgTile,
                                                                           &tTarget,
                                                                           &showRegion,
                                                                           center,
                                                                           ARM_2D_ANGLE(tempAngle),
                                                                           1.0f,
                                                                           ptWidget->color[i],
                                                                           ptWidget->use_as__ldBase_t.opacity,
                                                                           &maskCenter
                                                                           );
                    }
                }
                arm_2d_op_wait_async(NULL);
            }

            _ldArcDrawQuarter(&tTarget,tTarget_canvas,ptWidget->ptImgTile,quarterDrawFlag[1],ptWidget->color[1],ptWidget->use_as__ldBase_t.opacity);

        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldArcSetBgAngle(ldArc_t *ptWidget,float bgStart,float bgEnd)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    ptWidget->startAngle_x10[0]=bgStart*10;
    ptWidget->endAngle_x10[0]=bgEnd*10;
    ptWidget->startAngle_x10[1]=ptWidget->startAngle_x10[0];
}

void ldArcSetFgAngle(ldArc_t *ptWidget,float fgEnd)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->startAngle_x10[1]=ptWidget->startAngle_x10[0];
    ptWidget->endAngle_x10[1]=fgEnd*10;
}

void ldArcSetRotationAngle(ldArc_t *ptWidget,float rotationAngle)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->rotationAngle_x10=rotationAngle*10;
}

void ldArcSetColor(ldArc_t *ptWidget,ldColor bgColor,ldColor fgColor)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->color[0]=bgColor;
    ptWidget->color[1]=fgColor;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
