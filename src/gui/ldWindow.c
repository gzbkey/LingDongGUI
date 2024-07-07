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

#define __LD_WINDOW_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include "ldWindow.h"
#include <assert.h>
#include <string.h>

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

ARM_NONNULL(1)
ldWindow_t* ldWindow_init( arm_2d_scene_t *ptScene,ldWindow_t *ptWidget,uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height)
{
    assert(NULL!= ptScene);

    ldWindow_t * ptNew=ldImage_init(ptScene,ptWidget,nameId,parentNameId,x,y,width,height,NULL,NULL);

    if(ptNew!=NULL)
    {
        if(nameId==0)
        {
            ptNew->widgetType=widgetTypeBackground;
            ldImageSetBgColor(ptNew,__RGB(240,240,240));
        }
        else
        {
            ptNew->isTransparent=true;
            ptNew->widgetType=widgetTypeWindow;
        }

        LOG_INFO("[window] init,id:%d",nameId);
    }

    return ptNew;
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
