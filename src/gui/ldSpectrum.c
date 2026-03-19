/*
 * Copyright (c) 2023-2026 Ou Jianbo (59935554@qq.com). All rights reserved.
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

#define __LD_SPECTRUM_IMPLEMENT__

#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "ldSpectrum.h"

#define LD_SPECTRUM_GRADIENT_HEIGHT  200

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

#include "arm_math.h"

#if __GLCD_CFG_COLOUR_DEPTH__ == 16
ARM_SECTION("arm2d.asset.c_bmpSpectrumGradientRGB565")
static const uint16_t c_bmpSpectrumGradientRGB565[LD_SPECTRUM_GRADIENT_HEIGHT] = {
0xFD40, 0xFC40, 0xFB41, 0xFA41, 0xF942, 0xF842, 0xF743, 0xF643,
0xF544, 0xF444, 0xF345, 0xF245, 0xF146, 0xF046, 0xEF47, 0xEE47,
0xED48, 0xEC48, 0xEB49, 0xEA49, 0xE94A, 0xE84A, 0xE74B, 0xE64B,
0xE54C, 0xE44C, 0xE34D, 0xE24D, 0xE14E, 0xE04E, 0xDF4F, 0xDE4F,
0xDD50, 0xDC50, 0xDB51, 0xDA51, 0xD952, 0xD852, 0xD753, 0xD653,
0xD554, 0xD454, 0xD355, 0xD255, 0xD156, 0xD056, 0xCF57, 0xCE57,
0xCD58, 0xCC58, 0xCB59, 0xCA59, 0xC95A, 0xC85A, 0xC75B, 0xC65B,
0xC55C, 0xC45C, 0xC35D, 0xC25D, 0xC15E, 0xC05E, 0xBF5F, 0xBE5F,
0xBD60, 0xBC60, 0xBB61, 0xBA61, 0xB962, 0xB862, 0xB763, 0xB663,
0xB564, 0xB464, 0xB365, 0xB265, 0xB166, 0xB066, 0xAF67, 0xAE67,
0xAD68, 0xAC68, 0xAB69, 0xAA69, 0xA96A, 0xA86A, 0xA76B, 0xA66B,
0xA56C, 0xA46C, 0xA36D, 0xA26D, 0xA16E, 0xA06E, 0x9F6F, 0x9E6F,
0x9D70, 0x9C70, 0x9B71, 0x9A71, 0x9972, 0x9872, 0x9773, 0x9673,
0x9574, 0x9474, 0x9375, 0x9275, 0x9176, 0x9076, 0x8F77, 0x8E77,
0x8D78, 0x8C78, 0x8B79, 0x8A79, 0x897A, 0x887A, 0x877B, 0x867B,
0x857C, 0x847C, 0x837D, 0x827D, 0x817E, 0x807E, 0x7F7F, 0x7E7F,
0x7D80, 0x7C80, 0x7B81, 0x7A81, 0x7982, 0x7882, 0x7783, 0x7683,
0x7584, 0x7484, 0x7385, 0x7285, 0x7186, 0x7086, 0x6F87, 0x6E87,
0x6D88, 0x6C88, 0x6B89, 0x6A89, 0x698A, 0x688A, 0x678B, 0x668B,
0x658C, 0x648C, 0x638D, 0x628D, 0x618E, 0x608E, 0x5F8F, 0x5E8F,
0x5D90, 0x5C90, 0x5B91, 0x5A91, 0x5992, 0x5892, 0x5793, 0x5693,
0x5594, 0x5494, 0x5395, 0x5295, 0x5196, 0x5096, 0x4F97, 0x4E97,
0x4D98, 0x4C98, 0x4B99, 0x4A99, 0x499A, 0x489A, 0x479B, 0x469B,
0x459C, 0x449C, 0x439D, 0x429D, 0x419E, 0x409E, 0x3F9F, 0x3E9F,
0x3DA0, 0x3CA0, 0x3BA1, 0x3AA1, 0x39A2, 0x38A2, 0x37A3, 0x36A3,
};

static const arm_2d_tile_t c_tileSpectrumGradientRGB565 = {
    .tRegion = {
        .tSize = {
            .iWidth = 1,
            .iHeight = LD_SPECTRUM_GRADIENT_HEIGHT,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_RGB565,
        },
    },
    .phwBuffer = (uint16_t*)c_bmpSpectrumGradientRGB565,
};

#elif __GLCD_CFG_COLOUR_DEPTH__ == 32
ARM_SECTION("arm2d.asset.c_bmpSpectrumGradientCCCA8888")
static const uint32_t c_bmpSpectrumGradientCCCA8888[LD_SPECTRUM_GRADIENT_HEIGHT] = {
0xFF00A5FF, 0xFF01A1FB, 0xFF029DF7, 0xFF0399F3,
0xFF0495EF, 0xFF0591EB, 0xFF068DE7, 0xFF0789E3,
0xFF0885DF, 0xFF0981DB, 0xFF0A7DD7, 0xFF0B79D3,
0xFF0C75CF, 0xFF0D71CB, 0xFF0E6DC7, 0xFF0F69C3,
0xFF1065BF, 0xFF1161BB, 0xFF125DB7, 0xFF1359B3,
0xFF1455AF, 0xFF1551AB, 0xFF164DA7, 0xFF1749A3,
0xFF18459F, 0xFF19419B, 0xFF1A3D97, 0xFF1B3993,
0xFF1C358F, 0xFF1D318B, 0xFF1E2D87, 0xFF1F2983,
0xFF20257F, 0xFF21217B, 0xFF221D77, 0xFF231973,
0xFF24156F, 0xFF25116B, 0xFF260D67, 0xFF270963,
0xFF28055F, 0xFF29015B, 0xFF2A0057, 0xFF2B0053,
0xFF2C004F, 0xFF2D004B, 0xFF2E0047, 0xFF2F0043,
0xFF30003F, 0xFF31003B, 0xFF320037, 0xFF330033,
0xFF34002F, 0xFF35002B, 0xFF360027, 0xFF370023,
0xFF38001F, 0xFF39001B, 0xFF3A0017, 0xFF3B0013,
0xFF3C000F, 0xFF3D000B, 0xFF3E0007, 0xFF3F0003,
0xFF400000, 0xFF410000, 0xFF420000, 0xFF430000,
0xFF440000, 0xFF450000, 0xFF460000, 0xFF470000,
0xFF480000, 0xFF490000, 0xFF4A0000, 0xFF4B0000,
0xFF4C0000, 0xFF4D0000, 0xFF4E0000, 0xFF4F0000,
0xFF500000, 0xFF510000, 0xFF520000, 0xFF530000,
0xFF540000, 0xFF550000, 0xFF560000, 0xFF570000,
0xFF580000, 0xFF590000, 0xFF5A0000, 0xFF5B0000,
0xFF5C0000, 0xFF5D0000, 0xFF5E0000, 0xFF5F0000,
0xFF600000, 0xFF610000, 0xFF620000, 0xFF630000,
0xFF640000, 0xFF650000, 0xFF660000, 0xFF670000,
0xFF680000, 0xFF690000, 0xFF6A0000, 0xFF6B0000,
0xFF6C0000, 0xFF6D0000, 0xFF6E0000, 0xFF6F0000,
0xFF700000, 0xFF710000, 0xFF720000, 0xFF730000,
0xFF740000, 0xFF750000, 0xFF760000, 0xFF770000,
0xFF780000, 0xFF790000, 0xFF7A0000, 0xFF7B0000,
0xFF7C0000, 0xFF7D0000, 0xFF7E0000, 0xFF7F0000,
0xFF800000, 0xFF810000, 0xFF820000, 0xFF830000,
0xFF840000, 0xFF850000, 0xFF860000, 0xFF870000,
0xFF880000, 0xFF890000, 0xFF8A0000, 0xFF8B0000,
0xFF8C0000, 0xFF8D0000, 0xFF8E0000, 0xFF8F0000,
0xFF900000, 0xFF910000, 0xFF920000, 0xFF930000,
0xFF940000, 0xFF950000, 0xFF960000, 0xFF970000,
0xFF980000, 0xFF990000, 0xFF9A0000, 0xFF9B0000,
0xFF9C0000, 0xFF9D0000, 0xFF9E0000, 0xFF9F0000,
0xFFA00000, 0xFFA10000, 0xFFA20000, 0xFFA30000,
0xFFA40000, 0xFFA50000, 0xFFA60000, 0xFFA70000,
};

static const arm_2d_tile_t c_tileSpectrumGradientCCCA8888 = {
    .tRegion = {
        .tSize = {
            .iWidth = 1,
            .iHeight = LD_SPECTRUM_GRADIENT_HEIGHT,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_BGRA8888,
        },
    },
    .pwBuffer = (uint32_t*)c_bmpSpectrumGradientCCCA8888,
};

#else
ARM_SECTION("arm2d.asset.c_bmpSpectrumGradientGRAY8")
static const uint8_t c_bmpSpectrumGradientGRAY8[LD_SPECTRUM_GRADIENT_HEIGHT] = {
0xB8, 0xB7, 0xB6, 0xB5, 0xB4, 0xB3, 0xB2, 0xB1,
0xB0, 0xAF, 0xAE, 0xAD, 0xAC, 0xAB, 0xAA, 0xA9,
0xA8, 0xA7, 0xA6, 0xA5, 0xA4, 0xA3, 0xA2, 0xA1,
0xA0, 0x9F, 0x9E, 0x9D, 0x9C, 0x9B, 0x9A, 0x99,
0x98, 0x97, 0x96, 0x95, 0x94, 0x93, 0x92, 0x91,
0x90, 0x8F, 0x8E, 0x8D, 0x8C, 0x8B, 0x8A, 0x89,
0x88, 0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x81,
0x80, 0x7F, 0x7E, 0x7D, 0x7C, 0x7B, 0x7A, 0x79,
0x78, 0x77, 0x76, 0x75, 0x74, 0x73, 0x72, 0x71,
0x70, 0x6F, 0x6E, 0x6D, 0x6C, 0x6B, 0x6A, 0x69,
0x68, 0x67, 0x66, 0x65, 0x64, 0x63, 0x62, 0x61,
0x60, 0x5F, 0x5E, 0x5D, 0x5C, 0x5B, 0x5A, 0x59,
0x58, 0x57, 0x56, 0x55, 0x54, 0x53, 0x52, 0x51,
0x50, 0x4F, 0x4E, 0x4D, 0x4C, 0x4B, 0x4A, 0x49,
0x48, 0x47, 0x46, 0x45, 0x44, 0x43, 0x42, 0x41,
0x40, 0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39,
0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31,
0x30, 0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29,
0x28, 0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21,
0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19,
0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11,
0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09,
0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const arm_2d_tile_t c_tileSpectrumGradientGRAY8 = {
    .tRegion = {
        .tSize = {
            .iWidth = 1,
            .iHeight = LD_SPECTRUM_GRADIENT_HEIGHT,
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_GRAY8,
        },
    },
    .pchBuffer = (uint8_t*)c_bmpSpectrumGradientGRAY8,
};
#endif

const ldBaseWidgetFunc_t ldSpectrumFunc = {
    .depose = (ldDeposeFunc_t)ldSpectrum_depose,
    .load = (ldLoadFunc_t)ldSpectrum_on_load,
    .frameStart = (ldFrameStartFunc_t)ldSpectrum_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldSpectrum_on_frame_complete,
    .show = (ldShowFunc_t)ldSpectrum_show,
};

#if LD_SPECTRUM_USE_BUILTIN_FFT

ldSpectrumFFT_t* ldSpectrumFFT_init(uint16_t fftSize, uint8_t barCount)
{
    ldSpectrumFFT_t *pFFT = ldCalloc(1, sizeof(ldSpectrumFFT_t));
    if (pFFT == NULL)
    {
        return NULL;
    }

    pFFT->fftSize = fftSize;
    pFFT->barCount = barCount;
    pFFT->pOutputData = ldCalloc(1, barCount);
    pFFT->pFftBuffer = ldCalloc(1, fftSize * sizeof(float));
    pFFT->pMagBuffer = ldCalloc(1, (fftSize / 2) * sizeof(float));

    if (pFFT->pOutputData == NULL || pFFT->pFftBuffer == NULL || pFFT->pMagBuffer == NULL)
    {
        ldFree(pFFT->pOutputData);
        ldFree(pFFT->pFftBuffer);
        ldFree(pFFT->pMagBuffer);
        ldFree(pFFT);
        return NULL;
    }

    arm_rfft_fast_instance_f32 *pInst = ldCalloc(1, sizeof(arm_rfft_fast_instance_f32));
    if (pInst == NULL)
    {
        ldFree(pFFT->pOutputData);
        ldFree(pFFT->pFftBuffer);
        ldFree(pFFT->pMagBuffer);
        ldFree(pFFT);
        return NULL;
    }

    arm_rfft_fast_init_f32(pInst, fftSize);
    pFFT->pFftInstance = pInst;

    return pFFT;
}

void ldSpectrumFFT_depose(ldSpectrumFFT_t *pFFT)
{
    if (pFFT == NULL)
    {
        return;
    }

    ldFree(pFFT->pOutputData);
    ldFree(pFFT->pFftBuffer);
    ldFree(pFFT->pMagBuffer);
    ldFree(pFFT->pFftInstance);
    ldFree(pFFT);
}

void ldSpectrumFFT_process(ldSpectrumFFT_t *pFFT, int16_t *pInputData, uint16_t len, uint8_t gain)
{
    if (pFFT == NULL || pInputData == NULL)
    {
        return;
    }

    uint16_t processLen = (len < pFFT->fftSize) ? len : pFFT->fftSize;

    int32_t dataSum = 0;
    for (uint16_t i = 0; i < processLen; i++)
    {
        dataSum += (pInputData[i] > 0) ? pInputData[i] : -pInputData[i];
    }

    if (dataSum < processLen)
    {
        memset(pFFT->pOutputData, 0, pFFT->barCount);
        return;
    }

    for (uint16_t i = 0; i < processLen; i++)
    {
        pFFT->pFftBuffer[i] = (float)pInputData[i];
    }
    for (uint16_t i = processLen; i < pFFT->fftSize; i++)
    {
        pFFT->pFftBuffer[i] = 0.0f;
    }

    arm_rfft_fast_instance_f32 *pInst = (arm_rfft_fast_instance_f32 *)pFFT->pFftInstance;
    arm_rfft_fast_f32(pInst, pFFT->pFftBuffer, pFFT->pFftBuffer, 0);

    uint16_t magLen = pFFT->fftSize / 2;
    arm_cmplx_mag_f32(pFFT->pFftBuffer, pFFT->pMagBuffer, magLen);

    int binsPerBar = magLen / pFFT->barCount;
    if (binsPerBar < 1) binsPerBar = 1;

    float refMag = (float)pFFT->fftSize * 2048.0f / (float)gain;

    for (uint8_t i = 0; i < pFFT->barCount; i++)
    {
        float sum = 0.0f;
        int startBin = i * binsPerBar;
        int endBin = startBin + binsPerBar;
        if (endBin > magLen) endBin = magLen;
        if (startBin < 1) startBin = 1;

        for (int j = startBin; j < endBin; j++)
        {
            sum += pFFT->pMagBuffer[j];
        }

        float avg = sum / (endBin - startBin);
        float normalized = (avg / refMag) * 255.0f;
        if (normalized > 255.0f) normalized = 255.0f;

        pFFT->pOutputData[i] = (uint8_t)normalized;
    }
}

#endif

ldSpectrum_t* ldSpectrum_init(ld_scene_t *ptScene, ldSpectrum_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t barCount)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    uint8_t *pValueList = NULL;
    uint8_t *pDisplayValueList = NULL;
    uint8_t *pPeakValueList = NULL;
    ldSpectrumParticle_t *pParticles = NULL;
    uint8_t defaultBarWidth = 8;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldSpectrum_t));
        pValueList = ldCalloc(1, barCount);
        pDisplayValueList = ldCalloc(1, barCount);
        pPeakValueList = ldCalloc(1, barCount);
        pParticles = ldCalloc(barCount, sizeof(ldSpectrumParticle_t));
        if ((NULL == ptWidget) || (NULL == pValueList) || (NULL == pDisplayValueList) || (NULL == pPeakValueList) || (NULL == pParticles))
        {
            ldFree(ptWidget);
            ldFree(pValueList);
            ldFree(pDisplayValueList);
            ldFree(pPeakValueList);
            ldFree(pParticles);
            LOG_ERROR("[init failed][spectrum] id:%d", nameId);
            return NULL;
        }
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot, parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeSpectrum;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldSpectrumFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity = 255;

    ptWidget->pValueList = pValueList;
    ptWidget->pDisplayValueList = pDisplayValueList;
    ptWidget->pPeakValueList = pPeakValueList;
    ptWidget->pParticles = pParticles;
    ptWidget->barCount = barCount;
    ptWidget->barWidth = defaultBarWidth;
    ptWidget->barGap = 2;
    ptWidget->startColor = __RGB(0, 255, 128);
    ptWidget->endColor = __RGB(255, 165, 0);
    ptWidget->bgColor = 0;
    ptWidget->ptBgImgTile = NULL;
    ptWidget->ptBgMaskTile = NULL;
    ptWidget->fallSpeed = 3;
    ptWidget->riseSpeed = 8;
    ptWidget->peakFallSpeed = 1;
    ptWidget->gain = 64;
    ptWidget->style = SPECTRUM_STYLE_BAR;
    ptWidget->pFFT = NULL;

    LOG_INFO("[init][spectrum] id:%d, size:%d", nameId, (int)sizeof(*ptWidget));
    return ptWidget;
}

void ldSpectrum_depose(ld_scene_t *ptScene, ldSpectrum_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if (ptWidget->use_as__ldBase_t.widgetType != widgetTypeSpectrum)
    {
        return;
    }

    LOG_INFO("[depose][spectrum] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t *)ptWidget);

#if LD_SPECTRUM_USE_BUILTIN_FFT
    if (ptWidget->pFFT != NULL)
    {
        ldSpectrumFFT_depose(ptWidget->pFFT);
    }
#endif

    ldFree(ptWidget->pValueList);
    ldFree(ptWidget->pDisplayValueList);
    ldFree(ptWidget->pPeakValueList);
    ldFree(ptWidget->pParticles);
    ldFree(ptWidget);
}

void ldSpectrum_on_load(ld_scene_t *ptScene, ldSpectrum_t *ptWidget)
{
    (void)ptScene;
    (void)ptWidget;
}

void ldSpectrum_on_frame_start(ld_scene_t *ptScene, ldSpectrum_t *ptWidget)
{
    (void)ptScene;
    if (ptWidget == NULL)
    {
        return;
    }

    for (uint8_t i = 0; i < ptWidget->barCount; i++)
    {
        int16_t target = ptWidget->pValueList[i];
        int16_t current = ptWidget->pDisplayValueList[i];

        if (target > current)
        {
            current += ptWidget->riseSpeed;
            if (current > target)
            {
                current = target;
            }
        }
        else if (target < current)
        {
            if (current > ptWidget->fallSpeed)
            {
                current -= ptWidget->fallSpeed;
            }
            else
            {
                current = 0;
            }
        }

        ptWidget->pDisplayValueList[i] = (uint8_t)current;

        int16_t peak = ptWidget->pPeakValueList[i];
        if (current > peak)
        {
            peak = current;
        }
        else if (peak > current)
        {
            if (peak > ptWidget->peakFallSpeed)
            {
                peak -= ptWidget->peakFallSpeed;
            }
            else
            {
                peak = 0;
            }
        }
        ptWidget->pPeakValueList[i] = (uint8_t)peak;

        ldSpectrumParticle_t *pParticle = &ptWidget->pParticles[i];
        if (pParticle->active)
        {
            pParticle->offsetY += pParticle->speed;
            if (pParticle->offsetY > 30)
            {
                pParticle->active = 0;
            }
        }

        if (current > peak + 5)
        {
            if (!pParticle->active)
            {
                pParticle->active = 1;
                pParticle->offsetY = -(int16_t)(rand() % 10);
                pParticle->speed = (uint8_t)(1 + (rand() % 3));
            }
        }
    }
}

void ldSpectrum_on_frame_complete(ld_scene_t *ptScene, ldSpectrum_t *ptWidget)
{
    (void)ptScene;
    (void)ptWidget;
}

void ldSpectrum_show(ld_scene_t *ptScene, ldSpectrum_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    (void)ptScene;
    (void)bIsNewFrame;

    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

#if __GLCD_CFG_COLOUR_DEPTH__ == 16
    const arm_2d_tile_t *ptGradientTile = &c_tileSpectrumGradientRGB565;
#elif __GLCD_CFG_COLOUR_DEPTH__ == 32
    const arm_2d_tile_t *ptGradientTile = &c_tileSpectrumGradientCCCA8888;
#else
    const arm_2d_tile_t *ptGradientTile = &c_tileSpectrumGradientGRAY8;
#endif

    arm_2d_region_t globalRegion;
    arm_2d_helper_control_get_absolute_region((arm_2d_control_node_t *)ptWidget, &globalRegion, true);

    if (arm_2d_helper_pfb_is_region_active(ptTile, &globalRegion, true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if (ldBaseIsHidden((ldBase_t *)ptWidget))
            {
                break;
            }

            int16_t widgetHeight = ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight;
            int16_t widgetWidth = ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth;

            uint8_t totalBarWidth = ptWidget->barWidth + ptWidget->barGap;
            int16_t startX = (widgetWidth - (int16_t)ptWidget->barCount * totalBarWidth + ptWidget->barGap) / 2;

            if (ptWidget->ptBgImgTile != NULL || ptWidget->ptBgMaskTile != NULL)
            {
                ldBaseImage(&tTarget, NULL, ptWidget->ptBgImgTile, ptWidget->ptBgMaskTile, ptWidget->bgColor, ptWidget->use_as__ldBase_t.opacity);
            }
            else
            {
                ldBaseColor(&tTarget, NULL, ptWidget->bgColor, ptWidget->use_as__ldBase_t.opacity);
            }

            for (uint8_t i = 0; i < ptWidget->barCount; i++)
            {
                uint8_t displayValue = ptWidget->pDisplayValueList[i];
                uint8_t peakValue = ptWidget->pPeakValueList[i];
                uint16_t barHeight = (uint32_t)displayValue * widgetHeight / 255;

                int16_t barX = startX + i * totalBarWidth;

                if (barHeight > 0)
                {
                    int16_t barY;

                    if (ptWidget->style == SPECTRUM_STYLE_MIRROR)
                    {
                        barY = widgetHeight / 2 - barHeight / 2;
                    }
                    else
                    {
                        barY = widgetHeight - barHeight;
                    }

                    int16_t srcY = (int16_t)LD_SPECTRUM_GRADIENT_HEIGHT - widgetHeight + barY;
                    if (srcY < 0) srcY = 0;
                    if (srcY + barHeight > LD_SPECTRUM_GRADIENT_HEIGHT)
                    {
                        barHeight = LD_SPECTRUM_GRADIENT_HEIGHT - srcY;
                    }

                    arm_2d_tile_t childTile = impl_child_tile(*ptGradientTile, 0, srcY, 1, barHeight);

                    arm_2d_region_t barRegion = {
                        .tLocation = {barX, barY},
                        .tSize = {ptWidget->barWidth, barHeight}
                    };

                    ldBaseImage(&tTarget, &barRegion, &childTile, NULL, 0, ptWidget->use_as__ldBase_t.opacity);
                }

                if (peakValue > displayValue)
                {
                    uint16_t peakHeight = (uint32_t)peakValue * widgetHeight / 255;
                    int16_t peakY;

                    if (ptWidget->style == SPECTRUM_STYLE_MIRROR)
                    {
                        peakY = widgetHeight / 2 - peakHeight / 2;
                    }
                    else
                    {
                        peakY = widgetHeight - peakHeight;
                    }

                    int16_t peakSrcY = (int16_t)LD_SPECTRUM_GRADIENT_HEIGHT - widgetHeight + peakY;
                    if (peakSrcY < 0) peakSrcY = 0;
                    if (peakSrcY >= LD_SPECTRUM_GRADIENT_HEIGHT) peakSrcY = LD_SPECTRUM_GRADIENT_HEIGHT - 1;

                    arm_2d_tile_t peakTile = impl_child_tile(*ptGradientTile, 0, peakSrcY, 1, 2);

                    arm_2d_region_t peakRegion = {
                        .tLocation = {barX, peakY},
                        .tSize = {ptWidget->barWidth, 2}
                    };

                    ldBaseImage(&tTarget, &peakRegion, &peakTile, NULL, 0, ptWidget->use_as__ldBase_t.opacity);

                    ldSpectrumParticle_t *pParticle = &ptWidget->pParticles[i];
                    if (pParticle->active)
                    {
                        int16_t particleY = peakY + pParticle->offsetY;

                        if (particleY >= 0 && particleY < widgetHeight)
                        {
                            arm_2d_region_t particleRegion = {
                                .tLocation = {barX, particleY},
                                .tSize = {ptWidget->barWidth, 1}
                            };
                            ldBaseColor(&tTarget, &particleRegion, ptWidget->endColor, ptWidget->use_as__ldBase_t.opacity);
                        }
                    }
                }
            }

            ldBaseSetOpacity((ldBase_t *)ptWidget, ptWidget->use_as__ldBase_t.opacity);
        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldSpectrumSetData(ldSpectrum_t *ptWidget, uint8_t *pData, uint8_t len)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL || pData == NULL)
    {
        return;
    }

    uint8_t copyLen = (len < ptWidget->barCount) ? len : ptWidget->barCount;
    memcpy(ptWidget->pValueList, pData, copyLen);
}

void ldSpectrumSetRawData(ldSpectrum_t *ptWidget, int16_t *pData, uint16_t len)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL || pData == NULL)
    {
        return;
    }

#if LD_SPECTRUM_USE_BUILTIN_FFT
    if (ptWidget->pFFT != NULL)
    {
        ldSpectrumFFT_process(ptWidget->pFFT, pData, len, ptWidget->gain);
        ldSpectrumSetData(ptWidget, ptWidget->pFFT->pOutputData, ptWidget->barCount);
    }
#else
    (void)len;
#endif
}

void ldSpectrumSetBarStyle(ldSpectrum_t *ptWidget, uint8_t barWidth, uint8_t barGap)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->barWidth = barWidth;
    ptWidget->barGap = barGap;
}

void ldSpectrumSetColor(ldSpectrum_t *ptWidget, ldColor startColor, ldColor endColor)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->startColor = startColor;
    ptWidget->endColor = endColor;
}

void ldSpectrumSetBgColor(ldSpectrum_t *ptWidget, ldColor bgColor)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->bgColor = bgColor;
}

void ldSpectrumSetBgImage(ldSpectrum_t *ptWidget, arm_2d_tile_t *ptBgImgTile, arm_2d_tile_t *ptBgMaskTile)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->ptBgImgTile = ptBgImgTile;
    ptWidget->ptBgMaskTile = ptBgMaskTile;
}

void ldSpectrumSetAnimation(ldSpectrum_t *ptWidget, uint8_t riseSpeed, uint8_t fallSpeed)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->riseSpeed = riseSpeed;
    ptWidget->fallSpeed = fallSpeed;
}

void ldSpectrumSetPeakFallSpeed(ldSpectrum_t *ptWidget, uint8_t peakFallSpeed)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->peakFallSpeed = peakFallSpeed;
}

void ldSpectrumSetStyle(ldSpectrum_t *ptWidget, ldSpectrumStyle_t style)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->style = style;
}

void ldSpectrumSetGain(ldSpectrum_t *ptWidget, uint8_t gain)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->gain = gain;
}

void ldSpectrumEnableBuiltinFFT(ldSpectrum_t *ptWidget, uint16_t fftSize)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

#if LD_SPECTRUM_USE_BUILTIN_FFT
    if (ptWidget->pFFT != NULL)
    {
        ldSpectrumFFT_depose(ptWidget->pFFT);
    }

    ptWidget->pFFT = ldSpectrumFFT_init(fftSize, ptWidget->barCount);
#else
    (void)fftSize;
#endif
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
