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

#define __LD_PROGRESS_BAR_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldProgressBar.h"

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

const ldBaseWidgetFunc_t ldProgressBarFunc = {
    .depose = (ldDeposeFunc_t)ldProgressBar_depose,
    .load = (ldLoadFunc_t)ldProgressBar_on_load,
    .frameStart = (ldFrameStartFunc_t)ldProgressBar_on_frame_start,
    .show = (ldShowFunc_t)ldProgressBar_show,
};

ldProgressBar_t* ldProgressBar_init( ld_scene_t *ptScene,ldProgressBar_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldProgressBar_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][progressBar] id:%d", nameId);
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
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeProgressBar;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldProgressBarFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->bgColor=GLCD_COLOR_WHITE;
    ptWidget->fgColor=__RGB(0x94, 0xd2, 0x52);
    ptWidget->frameColor=__RGB(0xa5, 0xc6, 0xef);
    ptWidget->isHorizontal=true;
    ptWidget->timer=0;

    LOG_INFO("[init][progressBar] id:%d", nameId);
    return ptWidget;
}

void ldProgressBar_depose( ldProgressBar_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeProgressBar)
    {
        return;
    }

    LOG_INFO("[depose][progressBar] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove(ptWidget->ptScene->ptNodeRoot,(arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldProgressBar_on_load( ldProgressBar_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldProgressBar_on_frame_start( ldProgressBar_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}


static void _progressBarColorShow(ldProgressBar_t *ptWidget,arm_2d_tile_t *ptTarget)
{
    arm_2d_fill_colour(ptTarget, NULL, ptWidget->bgColor);

    if (ptWidget->permille > 0)
    {
        arm_2d_region_t tBarRegion = {
            .tLocation = {
                .iX = 1,
                .iY = 1,
            },
            .tSize = {
                .iWidth=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth-2,
                .iHeight=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight-2,
            },
        };

        if(ptWidget->isHorizontal)
        {
            tBarRegion.tSize.iWidth = tBarRegion.tSize.iWidth * ptWidget->permille / 1000;
        }
        else
        {
            int16_t temp=tBarRegion.tSize.iHeight*(1000 - ptWidget->permille) / 1000;;
            tBarRegion.tLocation.iY += temp;
            tBarRegion.tSize.iHeight -= temp;
        }
        arm_2d_fill_colour( ptTarget,&tBarRegion,ptWidget->fgColor);
    }

    arm_2d_draw_box(ptTarget,NULL,1,ptWidget->frameColor,ptWidget->use_as__ldBase_t.opacity);
}

static void _progressBarImageShow(ldProgressBar_t *ptWidget,arm_2d_tile_t *ptTarget,bool bIsNewFrame)
{
    arm_2d_region_t tBarRegion = {
        .tLocation = {
            .iX = 0,
            .iY = 0,
        },
        .tSize = ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize,
    };

    // bg image
    if(ptWidget->isHorizontal)
    {
        do{
            arm_2d_region_t tInnerRegion = {
                .tSize = {
                    .iWidth = tBarRegion.tSize.iWidth + ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth,
                    .iHeight = tBarRegion.tSize.iHeight,
                },
                .tLocation = {
                    .iX = -ptWidget->ptBgImgTile->tRegion.tSize.iWidth + ptWidget->bgOffset,
                },
            };
            arm_2d_tile_t tileInnerSlot;
            arm_2d_tile_generate_child(ptTarget, &tBarRegion, &tileInnerSlot, false);
            arm_2d_tile_fill_only(ptWidget->ptBgImgTile,&tileInnerSlot,&tInnerRegion);
        }while(0);
    }
    else
    {
        do{
            arm_2d_region_t tInnerRegion = {
                .tSize = {
                    .iWidth = tBarRegion.tSize.iWidth,
                    .iHeight = tBarRegion.tSize.iHeight + ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight,
                },
                .tLocation = {
                    .iY = -ptWidget->ptBgImgTile->tRegion.tSize.iHeight - ptWidget->bgOffset,
                },
            };
            arm_2d_tile_t tileInnerSlot;
            arm_2d_tile_generate_child(ptTarget, &tBarRegion, &tileInnerSlot, false);
            arm_2d_tile_fill_only(ptWidget->ptBgImgTile,&tileInnerSlot,&tInnerRegion);
        }while(0);
    }


    if (ptWidget->permille > 0)
    {
        // fg image
        if(ptWidget->isHorizontal)
        {
            do{
                arm_2d_region_t tInnerRegion = {
                    .tSize = {
                        .iWidth = tBarRegion.tSize.iWidth + ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth,
                        .iHeight = tBarRegion.tSize.iHeight,
                    },
                    .tLocation = {
                        .iX = -ptWidget->ptFgImgTile->tRegion.tSize.iWidth + ptWidget->fgOffset,
                    },
                };
                tBarRegion.tSize.iWidth = tBarRegion.tSize.iWidth * ptWidget->permille / 1000;

                arm_2d_tile_t tileInnerSlot;
                arm_2d_tile_generate_child(ptTarget, &tBarRegion, &tileInnerSlot, false);
                arm_2d_tile_fill_only(ptWidget->ptFgImgTile,&tileInnerSlot,&tInnerRegion);
            }while(0);
        }
        else
        {
            do{
                arm_2d_region_t tInnerRegion = {
                    .tSize = {
                        .iWidth = tBarRegion.tSize.iWidth,
                        .iHeight = tBarRegion.tSize.iHeight + ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight,
                    },
                    .tLocation = {
                        .iY = -ptWidget->ptFgImgTile->tRegion.tSize.iHeight - ptWidget->fgOffset,
                    },
                };
                int16_t temp=tBarRegion.tSize.iHeight*(1000 - ptWidget->permille) / 1000;;
                tBarRegion.tLocation.iY += temp;
                tBarRegion.tSize.iHeight -= temp;

                arm_2d_tile_t tileInnerSlot;
                arm_2d_tile_generate_child(ptTarget, &tBarRegion, &tileInnerSlot, false);
                arm_2d_tile_fill_only(ptWidget->ptFgImgTile,&tileInnerSlot,&tInnerRegion);
            }while(0);
        }

    }

    if(ptWidget->ptFrameImgTile!=NULL)
    {
        // frame image
        ldBaseImage(ptTarget,
                    NULL,
                    ptWidget->ptFrameImgTile,
                    NULL,
                    0,
                    ptWidget->use_as__ldBase_t.opacity);
    }

    if (bIsNewFrame)
    {
        if(ldTimeOut(PROGRESS_BAR_SPEED,&ptWidget->timer,true))
        {
            if(ptWidget->isHorizontal)
            {
                if(ptWidget->ptBgImgTile->tRegion.tSize.iWidth!=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth)
                {
                    ptWidget->bgOffset++;
                    if (ptWidget->bgOffset >= ptWidget->ptBgImgTile->tRegion.tSize.iWidth)
                    {
                        ptWidget->bgOffset = 0;
                    }
                }

                if(ptWidget->ptFgImgTile->tRegion.tSize.iWidth!=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth)
                {
                    ptWidget->fgOffset++;
                    if (ptWidget->fgOffset >= ptWidget->ptFgImgTile->tRegion.tSize.iWidth)
                    {
                        ptWidget->fgOffset = 0;
                    }
                }
            }
            else
            {
                if(ptWidget->ptBgImgTile->tRegion.tSize.iHeight!=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight)
                {
                    ptWidget->bgOffset++;
                    if (ptWidget->bgOffset >= ptWidget->ptBgImgTile->tRegion.tSize.iHeight)
                    {
                        ptWidget->bgOffset = 0;
                    }
                }

                if(ptWidget->ptFgImgTile->tRegion.tSize.iHeight!=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight)
                {
                    ptWidget->fgOffset++;
                    if (ptWidget->fgOffset >= ptWidget->ptFgImgTile->tRegion.tSize.iHeight)
                    {
                        ptWidget->fgOffset = 0;
                    }
                }
            }
        }

    }
}

void ldProgressBar_show(ld_scene_t *ptScene, ldProgressBar_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

#if 0
    if (bIsNewFrame) {
        
    }
#endif
    if(arm_2d_helper_pfb_is_region_active(ptTile,&ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            if(ptWidget->ptBgImgTile==NULL&&ptWidget->ptFgImgTile==NULL)//color
            {
                _progressBarColorShow(ptWidget,&tTarget);
            }
            else
            {
                _progressBarImageShow(ptWidget,&tTarget,bIsNewFrame);
                ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldProgressBarSetPercent(ldProgressBar_t *ptWidget,float percent)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    if(percent>=0)
    {
        if(percent>100)
        {
            percent=100;
        }
        ptWidget->permille=percent*10;
    }
    else
    {
        if(percent<-100)
        {
            percent=-100;
        }
        ptWidget->permille=1000+percent*10;
    }
}

void ldProgressBarSetImage(ldProgressBar_t *ptWidget,arm_2d_tile_t *ptBgImgTile,arm_2d_tile_t *ptFgImgTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptBgImgTile=ptBgImgTile;
    ptWidget->ptFgImgTile=ptFgImgTile;
//    ptWidget->bgWidth=bgWidth;
//    ptWidget->isBgMove=isMove;
}

//void ldProgressBarSetFgImage(ldProgressBar_t *ptWidget,arm_2d_tile_t *ptFgImgTile)
//{
//    assert(NULL != ptWidget);
//    if(ptWidget == NULL)
//    {
//        return;
//    }
//    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

////    ptWidget->fgWidth=fgWidth;
////    ptWidget->isFgMove=isMove;
//}

void ldProgressBarSetFrameImage(ldProgressBar_t *ptWidget, arm_2d_tile_t *ptFrameImgTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptFrameImgTile=ptFrameImgTile;
//    ptWidget->frameWidth=frameWidth;
}

void ldProgressBarSetColor(ldProgressBar_t *ptWidget,ldColor bgColor,ldColor fgColor,ldColor frameColor)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->ptBgImgTile=NULL;
    ptWidget->ptFgImgTile=NULL;
    ptWidget->ptFrameImgTile=NULL;
    ptWidget->bgColor=bgColor;
    ptWidget->fgColor=fgColor;
    ptWidget->frameColor=frameColor;
}

void ldProgressBarSetHorizontal(ldProgressBar_t *ptWidget,bool isHorizontal)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->isHorizontal=isHorizontal;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif