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

#ifndef LD_SPECTRUM_USE_BUILTIN_FFT
#define LD_SPECTRUM_USE_BUILTIN_FFT    1
#endif

typedef enum {
    SPECTRUM_STYLE_BAR,
    SPECTRUM_STYLE_MIRROR,
} ldSpectrumStyle_t;

typedef struct ldSpectrumFFT_t ldSpectrumFFT_t;

struct ldSpectrumFFT_t {
    uint16_t fftSize;
    uint8_t barCount;
    uint8_t *pOutputData;
    void *pFftInstance;
    float *pFftBuffer;
    float *pMagBuffer;
};

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
    uint8_t barGap;
    ldColor startColor;
    ldColor endColor;
    ldColor bgColor;
    arm_2d_tile_t *ptBgImgTile;
    arm_2d_tile_t *ptBgMaskTile;
    uint8_t fallSpeed;
    uint8_t riseSpeed;
    uint8_t peakFallSpeed;
    uint8_t gain;
    ldSpectrumStyle_t style;
    ldSpectrumFFT_t *pFFT;
};

ldSpectrum_t* ldSpectrum_init(ld_scene_t *ptScene, ldSpectrum_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t barCount);
#define ldSpectrumInit(nameId,parentNameId,x,y,width,height,barCount) \
        ldSpectrum_init(ptScene,NULL,nameId,parentNameId,x,y,width,height,barCount)
void ldSpectrum_depose(ld_scene_t *ptScene, ldSpectrum_t *ptWidget);
void ldSpectrum_on_load(ld_scene_t *ptScene, ldSpectrum_t *ptWidget);
void ldSpectrum_on_frame_start(ld_scene_t *ptScene, ldSpectrum_t *ptWidget);
void ldSpectrum_on_frame_complete(ld_scene_t *ptScene, ldSpectrum_t *ptWidget);
void ldSpectrum_show(ld_scene_t *pScene, ldSpectrum_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame);

void ldSpectrumSetData(ldSpectrum_t *ptWidget, uint8_t *pData, uint8_t len);
void ldSpectrumSetRawData(ldSpectrum_t *ptWidget, int16_t *pData, uint16_t len);
void ldSpectrumSetBarStyle(ldSpectrum_t *ptWidget, uint8_t barWidth, uint8_t barGap);
void ldSpectrumSetColor(ldSpectrum_t *ptWidget, ldColor startColor, ldColor endColor);
void ldSpectrumSetBgColor(ldSpectrum_t *ptWidget, ldColor bgColor);
void ldSpectrumSetBgImage(ldSpectrum_t *ptWidget, arm_2d_tile_t *ptBgImgTile, arm_2d_tile_t *ptBgMaskTile);
void ldSpectrumSetAnimation(ldSpectrum_t *ptWidget, uint8_t riseSpeed, uint8_t fallSpeed);
void ldSpectrumSetPeakFallSpeed(ldSpectrum_t *ptWidget, uint8_t peakFallSpeed);
void ldSpectrumSetStyle(ldSpectrum_t *ptWidget, ldSpectrumStyle_t style);
void ldSpectrumSetGain(ldSpectrum_t *ptWidget, uint8_t gain);
void ldSpectrumEnableBuiltinFFT(ldSpectrum_t *ptWidget, uint16_t fftSize);

#if LD_SPECTRUM_USE_BUILTIN_FFT
ldSpectrumFFT_t* ldSpectrumFFT_init(uint16_t fftSize, uint8_t barCount);
void ldSpectrumFFT_depose(ldSpectrumFFT_t *pFFT);
void ldSpectrumFFT_process(ldSpectrumFFT_t *pFFT, int16_t *pInputData, uint16_t len, uint8_t gain);
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif

#endif
