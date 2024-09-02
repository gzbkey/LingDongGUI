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

/**
 * @file    ldScene0.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   arm2d的场景文件，是arm2d的关键用户文件
 */

#include "arm_2d.h"

#define __USER_SCENE0_IMPLEMENT__
#include "ldScene0.h"
#include "arm_2d_helper.h"
#include "arm_2d_example_controls.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ldGui.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#elif __IS_COMPILER_ARM_COMPILER_5__
#   pragma diag_suppress 64,177
#elif __IS_COMPILER_IAR__
#   pragma diag_suppress=Pa089,Pe188,Pe177,Pe174
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wunused-variable"
#   pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#endif

#undef this
#define this (*ptThis)

enum {
//    SCENE_DR_START = 0,
    SCENE_DR_UPDATE=0,
    SCENE_DR_DONE,
};

static void __on_scene0_load(arm_2d_scene_t *ptScene)
{
    ld_scene_t *ptThis = (ld_scene_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    ldGuiLoad(ptThis);
}

static void __on_scene0_depose(arm_2d_scene_t *ptScene)
{
    ld_scene_t *ptThis = (ld_scene_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    ldGuiQuit(ptThis);

    ptScene->ptPlayer = NULL;

    arm_2d_scene_player_dynamic_dirty_region_depose(
                &ptThis->tDirtyRegionItem,
                &ptThis->use_as__arm_2d_scene_t);

    if (!this.bUserAllocated) {
        __arm_2d_free_scratch_memory(ARM_2D_MEM_TYPE_UNSPECIFIED, ptScene);
    }
}

/*----------------------------------------------------------------------------*
 * Scene 0                                                                    *
 *----------------------------------------------------------------------------*/

static void __on_scene0_background_start(arm_2d_scene_t *ptScene)
{
    ld_scene_t *ptThis = (ld_scene_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
}

static void __on_scene0_background_complete(arm_2d_scene_t *ptScene)
{
    ld_scene_t *ptThis = (ld_scene_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
}

static void __on_scene0_frame_start(arm_2d_scene_t *ptScene)
{
    ld_scene_t *ptThis = (ld_scene_t *)ptScene;

    ldGuiFrameStart(ptThis);
    ldGuiLogicLoop(ptThis);
    ldGuiTouchProcess(ptThis);
    ldMsgProcess(ptThis);

    arm_2d_dynamic_dirty_region_on_frame_start(
                                                &ptThis->tDirtyRegionItem,
                                                SCENE_DR_UPDATE);//SCENE_DR_START);

    arm_2d_helper_control_enum_init(&ptThis->tEnum,&ARM_2D_CONTROL_ENUMERATION_POLICY_PREORDER_TRAVERSAL,ptThis->ptNodeRoot);
}

static void __on_scene0_frame_complete(arm_2d_scene_t *ptScene)
{
    ld_scene_t *ptThis = (ld_scene_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    if(ptThis->ptNodeRoot!=NULL)
    {
        arm_2d_helper_control_enum_depose(&ptThis->tEnum);
    }
    ldGuiFrameComplete(ptThis);
}

static void __before_scene0_switching_out(arm_2d_scene_t *ptScene)
{
    ld_scene_t *ptThis = (ld_scene_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene0_handler)
{
    ld_scene_t *ptThis = (ld_scene_t *)pTarget;
    arm_2d_size_t tScreenSize = ptTile->tRegion.tSize;
    static ldBase_t* ptCurrentWidget = NULL;
    static uint8_t itemCount=0;
    static arm_2d_location_t widgetLoca={0};

    ARM_2D_UNUSED(tScreenSize);

    ldGuiDraw(pTarget,ptTile,bIsNewFrame);

    if(ptThis->ptNodeRoot!=NULL)
    {
        switch (arm_2d_dynamic_dirty_region_wait_next(&ptThis->tDirtyRegionItem))
        {
        case SCENE_DR_UPDATE:
        {
            if(ptCurrentWidget==NULL)
            {
                do {
                    if (arm_2d_helper_control_enum_get_next_node(&ptThis->tEnum))
                    {
                        if(((ldBase_t *)ptThis->tEnum.ptCurrent)->isDirtyRegionUpdate)
                        {
                            if(((ldBase_t *)ptThis->tEnum.ptCurrent)->isDirtyRegionAutoReset)
                            {
                                ((ldBase_t *)ptThis->tEnum.ptCurrent)->isDirtyRegionUpdate = false;
                            }

                            arm_2d_region_t tRegion;
                            ldBase_t *ptWidget=(ldBase_t *)ptThis->tEnum.ptCurrent;
#if 0
                            if((ptWidget->tTempRegion.tLocation.iX==ptWidget->use_as__arm_2d_control_node_t.tRegion.tLocation.iX)&&
                                    (ptWidget->tTempRegion.tLocation.iY==ptWidget->use_as__arm_2d_control_node_t.tRegion.tLocation.iY)&&
                                    (ptWidget->tTempRegion.tSize.iWidth==ptWidget->use_as__arm_2d_control_node_t.tRegion.tSize.iWidth)&&
                                    (ptWidget->tTempRegion.tSize.iHeight==ptWidget->use_as__arm_2d_control_node_t.tRegion.tSize.iHeight))
#else
                            if(memcmp(&ptWidget->tTempRegion,&ptWidget->use_as__arm_2d_control_node_t.tRegion,sizeof (arm_2d_region_t))==0)
#endif
                            {
                                widgetLoca.iX=0;
                                widgetLoca.iY=0;
                                widgetLoca= ldBaseGetAbsoluteLocation(ldBaseGetParent(ptWidget),widgetLoca);

                                //region no change
                                tRegion=ptWidget->use_as__arm_2d_control_node_t.tRegion;

                                tRegion.tLocation.iX+=widgetLoca.iX;
                                tRegion.tLocation.iY+=widgetLoca.iY;
                            }
                            else
                            {
                                if(ptWidget->itemCount)// widget has item
                                {
                                    ptCurrentWidget=ptWidget;
                                    itemCount=0;
                                    widgetLoca.iX=0;
                                    widgetLoca.iY=0;
                                    widgetLoca= ldBaseGetAbsoluteLocation(ptWidget,widgetLoca);
                                    ptCurrentWidget->ptItemRegionList[itemCount].tTempItemRegion.tLocation.iX+=widgetLoca.iX;
                                    ptCurrentWidget->ptItemRegionList[itemCount].tTempItemRegion.tLocation.iY+=widgetLoca.iY;
                                    arm_2d_dynamic_dirty_region_update(
                                                &ptThis->tDirtyRegionItem,
                                                (arm_2d_tile_t*)ptTile,
                                                &ptCurrentWidget->ptItemRegionList[itemCount].tTempItemRegion,
                                                SCENE_DR_UPDATE);
                                    ptCurrentWidget->ptItemRegionList[itemCount].tTempItemRegion=ptCurrentWidget->ptItemRegionList[itemCount].itemRegion;
                                    itemCount++;
                                    break;
                                }
                                else// widget update region
                                {
                                    widgetLoca.iX=0;
                                    widgetLoca.iY=0;
                                    widgetLoca= ldBaseGetAbsoluteLocation(ldBaseGetParent(ptWidget),widgetLoca);

                                    tRegion=ptWidget->tTempRegion;
                                    ptWidget->tTempRegion=ptWidget->use_as__arm_2d_control_node_t.tRegion;
                                    tRegion.tLocation.iX+=widgetLoca.iX;
                                    tRegion.tLocation.iY+=widgetLoca.iY;
                                }
                            }

                            arm_2d_dynamic_dirty_region_update(
                                        &ptThis->tDirtyRegionItem,
                                        (arm_2d_tile_t*)ptTile,
                                        &tRegion,
                                        SCENE_DR_UPDATE);
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        arm_2d_dynamic_dirty_region_change_user_region_index_only(
                                    &ptThis->tDirtyRegionItem,
                                    SCENE_DR_DONE);
                    }
                    break;
                } while (true);
            }
            else
            {
                if(itemCount<ptCurrentWidget->itemCount)
                {
                    ptCurrentWidget->ptItemRegionList[itemCount].tTempItemRegion.tLocation.iX+=widgetLoca.iX;
                    ptCurrentWidget->ptItemRegionList[itemCount].tTempItemRegion.tLocation.iY+=widgetLoca.iY;
                    arm_2d_dynamic_dirty_region_update(
                                &ptThis->tDirtyRegionItem,
                                (arm_2d_tile_t*)ptTile,
                                &ptCurrentWidget->ptItemRegionList[itemCount].tTempItemRegion,
                                SCENE_DR_UPDATE);
                    ptCurrentWidget->ptItemRegionList[itemCount].tTempItemRegion=ptCurrentWidget->ptItemRegionList[itemCount].itemRegion;
                    itemCount++;
                }
                else
                {
                    ptCurrentWidget=NULL;
                    arm_2d_dynamic_dirty_region_change_user_region_index_only(
                                &ptThis->tDirtyRegionItem,
                                SCENE_DR_UPDATE);
                }
            }
            break;
        }
        case SCENE_DR_DONE:
            break;
        default:
            break;
        }
    }
    ARM_2D_OP_WAIT_ASYNC();

    return arm_fsm_rt_cpl;
}

ARM_NONNULL(1)
ld_scene_t *__arm_2d_scene0_init(   arm_2d_scene_player_t *ptDispAdapter,
                                        ld_scene_t *ptThis,
                                    const ldPageFuncGroup_t *ptFunc)
{
    bool bUserAllocated = false;
    assert(NULL != ptDispAdapter);

    if (NULL == ptThis) {
        ptThis = (ld_scene_t *)
                    __arm_2d_allocate_scratch_memory(   sizeof(ld_scene_t),
                                                        __alignof__(ld_scene_t),
                                                        ARM_2D_MEM_TYPE_UNSPECIFIED);
        assert(NULL != ptThis);
        if (NULL == ptThis) {
            return NULL;
        }
    } else {
        bUserAllocated = true;
    }
    memset(ptThis, 0, sizeof(ld_scene_t));

    *ptThis = (ld_scene_t){
        .use_as__arm_2d_scene_t = {

            /* the canvas colour */
            .tCanvas = {GLCD_COLOR_WHITE},

            /* Please uncommon the callbacks if you need them
             */
            .fnOnLoad       = &__on_scene0_load,
            .fnScene        = &__pfb_draw_scene0_handler,

            //.fnOnBGStart    = &__on_scene0_background_start,
            //.fnOnBGComplete = &__on_scene0_background_complete,
            .fnOnFrameStart = &__on_scene0_frame_start,
            //.fnBeforeSwitchOut = &__before_scene0_switching_out,
            .fnOnFrameCPL   = &__on_scene0_frame_complete,
            .fnDepose       = &__on_scene0_depose,

            .bUseDirtyRegionHelper = false,
        },
        .bUserAllocated = bUserAllocated,
        .ldGuiFuncGroup=ptFunc,
        .ptMsgQueue=NULL,
        .ptNodeRoot=NULL,
    };

    ldGuiSceneInit(ptThis);

    arm_2d_scene_player_dynamic_dirty_region_init(
                                                &ptThis->tDirtyRegionItem,
                                                &ptThis->use_as__arm_2d_scene_t);

    arm_2d_scene_player_append_scenes(  ptDispAdapter,
                                        &this.use_as__arm_2d_scene_t,
                                        1);

    return ptThis;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
