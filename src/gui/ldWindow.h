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

#ifndef __LD_WINDOW_H__
#define __LD_WINDOW_H__

#ifdef   __cplusplus
extern "C" {
#endif

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmicrosoft-anon-tag"
#   pragma clang diagnostic ignored "-Wpadded"
#endif

#include "ldImage.h"

#define ldWindow_t                      ldImage_t

#define ldWindow_init(ptScene,ptWidget,nameId,parentNameId,x,y,width,height) \
        ldImage_init(ptScene,ptWidget,nameId,parentNameId,x,y,width,height,NULL,NULL,true)

#define ldWindowInit(nameId,parentNameId,x,y,width,height) \
        ldImageInit(nameId,parentNameId,x,y,width,height,NULL,NULL,true)

#define ldWindow_depose                 ldImage_depose
#define ldWindow_on_load                ldImage_on_load
#define ldWindow_on_frame_start         ldImage_on_frame_start
#define ldWindow_show                   ldImage_show
#define ldWindowSetBgColor              ldImageSetBgColor

#define ldWindowSetHidden               ldBaseSetHidden
#define ldWindowMove                    ldBaseMove
#define ldWindowSetOpacity              ldBaseSetOpacity

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif

#endif
