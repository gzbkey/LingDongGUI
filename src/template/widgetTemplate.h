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

#ifndef __LD_TEMPLATE_H__
#define __LD_TEMPLATE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-declarations"
#pragma clang diagnostic ignored "-Wmicrosoft-anon-tag"
#pragma clang diagnostic ignored "-Wpadded"
#endif



/* OOC header, please DO NOT modify  */
#ifdef __LD_TEMPLATE_IMPLEMENT__
#undef __LD_TEMPLATE_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_TEMPLATE_INHERIT__)
#undef __LD_TEMPLATE_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct ldTemplate_t ldTemplate_t;

struct ldTemplate_t
{
    implement(ldBase_t);
ARM_PRIVATE(
    ld_scene_t *ptScene;
)
    /* place your public member here */
    
};

ldTemplate_t* ldTemplate_init(ld_scene_t *ptScene, ldTemplate_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
void ldTemplate_depose( ldTemplate_t *ptWidget);
void ldTemplate_on_load( ldTemplate_t *ptWidget);
void ldTemplate_on_frame_start( ldTemplate_t *ptWidget);
void ldTemplate_show(ld_scene_t *pScene, ldTemplate_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

#define ldTemplateInit(nameId,parentNameId,x,y,width,height) \
        ldTemplate_init(ptScene,NULL,nameId,parentNameId,x,y,width,height)

#define ldTemplateSetHidden                ldBaseSetHidden
#define ldTemplateMove                     ldBaseMove
#define ldTemplateSetOpacity               ldBaseSetOpacity

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
