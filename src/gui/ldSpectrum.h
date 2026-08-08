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

#ifndef __LD_SPECTRUM_H__
#define __LD_SPECTRUM_H__

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

#ifdef __LD_SPECTRUM_IMPLEMENT__
#undef __LD_SPECTRUM_IMPLEMENT__
#define __ARM_2D_IMPL__
#elif defined(__LD_SPECTRUM_INHERIT__)
#undef __LD_SPECTRUM_INHERIT__
#define __ARM_2D_INHERIT__
#endif
#include "arm_2d_utils.h"
#include "ldBase.h"

typedef struct {
    int16_t offsetY;
    uint8_t speed;
    uint8_t active;
} ldSpectrumParticle_t;

typedef struct ldSpectrum_t ldSpectrum_t;

struct ldSpectrum_t
{
    implement(ldBase_t);
    uint8_t *pValueList;
    uint8_t *pDisplayValueList;
    uint8_t *pPeakValueList;
    ldSpectrumParticle_t *pParticles;
    uint8_t barCount;
    uint8_t barWidth;
    ldColor barColor;
    ldColor bgColor;
    arm_2d_tile_t *ptBarImgTile;
    arm_2d_tile_t *ptBarMaskTile;
    arm_2d_tile_t *ptBgImgTile;
    arm_2d_tile_t *ptBgMaskTile;
    uint8_t gain;
    uint16_t fftSize;
    void *pFftInstance;
    float *pFftBuffer;
    float *pMagBuffer;
};

ldSpectrum_t* ldSpectrum_init(ld_scene_t *ptScene, ldSpectrum_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t barCount, uint16_t fftSize);
#define ldSpectrumInit(nameId,parentNameId,x,y,width,height,barCount,fftSize) \
        ldSpectrum_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,barCount,fftSize)
void ldSpectrum_depose(ld_scene_t *ptScene, ldSpectrum_t *ptWidget);
void ldSpectrum_on_load(ld_scene_t *ptScene, ldSpectrum_t *ptWidget);
void ldSpectrum_on_frame_start(ld_scene_t *ptScene, ldSpectrum_t *ptWidget);
void ldSpectrum_on_frame_complete(ld_scene_t *ptScene, ldSpectrum_t *ptWidget);
void ldSpectrum_show(ld_scene_t *pScene, ldSpectrum_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldSpectrumSetData(ldSpectrum_t *ptWidget, int16_t *pAudioData, uint16_t len);
void ldSpectrumSetBarWidth(ldSpectrum_t *ptWidget, uint8_t barWidth);
void ldSpectrumSetBarImage(ldSpectrum_t *ptWidget, arm_2d_tile_t *ptBarImgTile, arm_2d_tile_t *ptBarMaskTile);
void ldSpectrumSetBarColor(ldSpectrum_t *ptWidget, ldColor barColor);
void ldSpectrumSetBgColor(ldSpectrum_t *ptWidget, ldColor bgColor);
void ldSpectrumSetBgImage(ldSpectrum_t *ptWidget, arm_2d_tile_t *ptBgImgTile, arm_2d_tile_t *ptBgMaskTile);
void ldSpectrumSetGain(ldSpectrum_t *ptWidget, uint8_t gain);

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
