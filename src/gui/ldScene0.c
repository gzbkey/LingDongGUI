/*
 * Copyright (c) 2009-2022 Arm Limited. All rights reserved.
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

/**
 * @file    ldScene0.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   arm2d的场景文件，是arm2d的关键用户文件
 * @version 0.1
 * @date    2023-11-03
 */
/*============================ INCLUDES ======================================*/

#include "arm_2d.h"

#define __USER_SCENE0_IMPLEMENT__
#include "ldScene0.h"
#include "arm_2d_helper.h"
#include "arm_extra_controls.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ldCommon.h"
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
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_IAR__
#   pragma diag_suppress=Pa089,Pe188,Pe177,Pe174
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#   pragma GCC diagnostic ignored "-Wunused-function"
#   pragma GCC diagnostic ignored "-Wunused-variable"
#   pragma GCC diagnostic ignored "-Wunused-value"
#endif

/*============================ MACROS ========================================*/

/*============================ MACROFIED FUNCTIONS ===========================*/
#undef this
#define this (*ptThis)

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/


static void __on_scene0_depose(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    ptScene->ptPlayer = NULL;
    
    /* reset timestamp */
    arm_foreach(int64_t,this.lTimestamp, ptItem) {
        *ptItem = 0;
    }

    if (!this.bUserAllocated) {
        free(ptScene);
    }
}

/*----------------------------------------------------------------------------*
 * Scene 0                                                                    *
 *----------------------------------------------------------------------------*/

static void __on_scene0_background_start(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);    
}

static void __on_scene0_background_complete(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

}

static void __on_scene0_frame_start(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

    ldGuiFrameStart();
    ldGuiLogicLoop();
}

static void __on_scene0_frame_complete(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);
    
    
    if(pageNumNow!=pageTarget)
    {
        ldGuiQuit();
        pageNumNow=pageTarget;
        ldGuiInit(ptScene);
//        arm_2d_scene1_init(&DISP0_ADAPTER);
//        arm_2d_scene_player_switch_to_next_scene(ptScene->ptPlayer);
    }
    /* switch to next scene after 3s */
//    if (arm_2d_helper_is_time_out(3000, &this.lTimestamp[0])) {
//        
//    }
}

static void __before_scene0_switching_out(arm_2d_scene_t *ptScene)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)ptScene;
    ARM_2D_UNUSED(ptThis);

//    ldGuiQuit();
//    pageNumNow=pageTarget;
}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene0_background_handler)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)pTarget;
    ARM_2D_UNUSED(ptTile);
    ARM_2D_UNUSED(bIsNewFrame);
    /*-----------------------draw back ground begin-----------------------*/


    /*-----------------------draw back ground end  -----------------------*/
    arm_2d_op_wait_async(NULL);

    return arm_fsm_rt_cpl;
}

static
IMPL_PFB_ON_DRAW(__pfb_draw_scene0_handler)
{
    user_scene_0_t *ptThis = (user_scene_0_t *)pTarget;
    ARM_2D_UNUSED(ptTile);
    ARM_2D_UNUSED(bIsNewFrame);
    
    arm_2d_canvas(ptTile, __top_canvas) {
    /*-----------------------draw the foreground begin-----------------------*/

        ldGuiLoop(ptThis,ptTile,bIsNewFrame);


    /*-----------------------draw the foreground end  -----------------------*/
    }
    arm_2d_op_wait_async(NULL);

    return arm_fsm_rt_cpl;
}

ARM_NONNULL(1)
user_scene_0_t *__arm_2d_scene0_init(   arm_2d_scene_player_t *ptDispAdapter, 
                                        user_scene_0_t *ptThis)
{
    bool bUserAllocated = false;
    assert(NULL != ptDispAdapter);
    
    if (NULL == ptThis) {
        ptThis = (user_scene_0_t *)malloc(sizeof(user_scene_0_t));
        assert(NULL != ptThis);
        if (NULL == ptThis) {
            return NULL;
        }
    } else {
        bUserAllocated = true;
    }
    memset(ptThis, 0, sizeof(user_scene_0_t));

    *ptThis = (user_scene_0_t){
        .use_as__arm_2d_scene_t = {
            /* Please uncommon the callbacks if you need them
             */
            //.fnBackground   = &__pfb_draw_scene0_background_handler,
            .fnScene        = &__pfb_draw_scene0_handler,
            .ptDirtyRegion  = NULL,
            

            //.fnOnBGStart    = &__on_scene0_background_start,
            //.fnOnBGComplete = &__on_scene0_background_complete,
            .fnOnFrameStart = &__on_scene0_frame_start,
            //.fnBeforeSwitchOut = &__before_scene0_switching_out,
            .fnOnFrameCPL   = &__on_scene0_frame_complete,
            .fnDepose       = &__on_scene0_depose,
        },
        .bUserAllocated = bUserAllocated,
    };

    ldGuiInit(ptThis);
    
    arm_2d_scene_player_append_scenes(  ptDispAdapter, 
                                        &this.use_as__arm_2d_scene_t, 
                                        1);

    return ptThis;
}




#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
