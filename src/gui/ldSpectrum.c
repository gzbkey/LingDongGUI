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

#include "ldSpectrum.h"

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

#if LD_SPECTRUM_USE_BUILTIN_FFT
#if defined(ARM_MATH_CM0) || defined(ARM_MATH_CM0PLUS) || \
    defined(ARM_MATH_CM3) || defined(ARM_MATH_CM4) || \
    defined(ARM_MATH_CM7) || defined(__ARM_ARCH)
    #include "arm_math.h"
    #define LD_SPECTRUM_FFT_CMSIS    1
#else
    #define LD_SPECTRUM_FFT_CMSIS    0
#endif
#endif

const ldBaseWidgetFunc_t ldSpectrumFunc = {
    .depose = (ldDeposeFunc_t)ldSpectrum_depose,
    .load = (ldLoadFunc_t)ldSpectrum_on_load,
    .frameStart = (ldFrameStartFunc_t)ldSpectrum_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldSpectrum_on_frame_complete,
    .show = (ldShowFunc_t)ldSpectrum_show,
};

static inline ldColor _get_gradient_color(ldColor startColor, ldColor endColor, uint8_t ratio)
{
    __arm_2d_color_fast_rgb_t tStart, tEnd;

#if __GLCD_CFG_COLOUR_DEPTH__ == 8
    __arm_2d_gray8_unpack(startColor, &tStart);
    __arm_2d_gray8_unpack(endColor, &tEnd);
#elif __GLCD_CFG_COLOUR_DEPTH__ == 16
    __arm_2d_rgb565_unpack(startColor, &tStart);
    __arm_2d_rgb565_unpack(endColor, &tEnd);
#else
    __arm_2d_ccca8888_unpack(startColor, &tStart);
    __arm_2d_ccca8888_unpack(endColor, &tEnd);
#endif

    uint16_t r = (tStart.R * (255 - ratio) + tEnd.R * ratio) >> 8;
    uint16_t g = (tStart.G * (255 - ratio) + tEnd.G * ratio) >> 8;
    uint16_t b = (tStart.B * (255 - ratio) + tEnd.B * ratio) >> 8;

    __arm_2d_color_fast_rgb_t tResult = {b, g, r, 0xFF};

#if __GLCD_CFG_COLOUR_DEPTH__ == 8
    return __arm_2d_gray8_pack(&tResult);
#elif __GLCD_CFG_COLOUR_DEPTH__ == 16
    return __arm_2d_rgb565_pack(&tResult);
#else
    return __arm_2d_ccca8888_pack(&tResult);
#endif
}

#if LD_SPECTRUM_USE_BUILTIN_FFT

#if LD_SPECTRUM_FFT_CMSIS

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

#else

static const int16_t c_sinTable[64] = {
    0, 1608, 3212, 4808, 6393, 7962, 9512, 11039,
    12540, 14010, 15447, 16846, 18205, 19520, 20788, 22006,
    23170, 24279, 25330, 26320, 27246, 28106, 28899, 29622,
    30274, 30853, 31357, 31786, 32138, 32413, 32610, 32729,
    32768, 32729, 32610, 32413, 32138, 31786, 31357, 30853,
    30274, 29622, 28899, 28106, 27246, 26320, 25330, 24279,
    23170, 22006, 20788, 19520, 18205, 16846, 15447, 14010,
    12540, 11039, 9512, 7962, 6393, 4808, 3212, 1608
};

static inline int16_t _get_sin(uint8_t angle)
{
    return c_sinTable[angle & 0x3F];
}

static inline int16_t _get_cos(uint8_t angle)
{
    return c_sinTable[(angle + 16) & 0x3F];
}

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

    if (pFFT->pOutputData == NULL)
    {
        ldFree(pFFT->pOutputData);
        ldFree(pFFT);
        return NULL;
    }

    return pFFT;
}

void ldSpectrumFFT_depose(ldSpectrumFFT_t *pFFT)
{
    if (pFFT == NULL)
    {
        return;
    }

    ldFree(pFFT->pOutputData);
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

    float refMag = (float)processLen * 2048.0f / (float)gain;

    for (uint8_t bar = 0; bar < pFFT->barCount; bar++)
    {
        float realSum = 0.0f;
        float imagSum = 0.0f;

        for (uint16_t k = 0; k < processLen; k++)
        {
            uint8_t angle = (uint8_t)((k * 64 * bar) / processLen);
            int16_t cosVal = _get_cos(angle);
            int16_t sinVal = _get_sin(angle);

            realSum += (float)pInputData[k] * cosVal;
            imagSum -= (float)pInputData[k] * sinVal;
        }

        realSum /= 32768.0f;
        imagSum /= 32768.0f;

        float mag = realSum * realSum + imagSum * imagSum;
        mag = sqrtf(mag);

        float normalized = (mag / refMag) * 255.0f;
        if (normalized > 255.0f) normalized = 255.0f;

        pFFT->pOutputData[bar] = (uint8_t)normalized;
    }
}

#endif

#endif

ldSpectrum_t* ldSpectrum_init(ld_scene_t *ptScene, ldSpectrum_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t barCount)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    uint8_t *pValueList = NULL;
    uint8_t *pDisplayValueList = NULL;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldSpectrum_t));
        pValueList = ldCalloc(1, barCount);
        pDisplayValueList = ldCalloc(1, barCount);
        if ((NULL == ptWidget) || (NULL == pValueList) || (NULL == pDisplayValueList))
        {
            ldFree(ptWidget);
            ldFree(pValueList);
            ldFree(pDisplayValueList);
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
    ptWidget->barCount = barCount;
    ptWidget->barWidth = 8;
    ptWidget->barGap = 2;
    ptWidget->startColor = __RGB(0, 255, 128);
    ptWidget->endColor = __RGB(255, 64, 64);
    ptWidget->bgColor = 0;
    ptWidget->ptBgImgTile = NULL;
    ptWidget->ptBgMaskTile = NULL;
    ptWidget->fallSpeed = 3;
    ptWidget->riseSpeed = 8;
    ptWidget->gain = 64;
    ptWidget->style = SPECTRUM_STYLE_BAR;
    ptWidget->isGradient = true;
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
    }
}

void ldSpectrum_on_frame_complete(ld_scene_t *ptScene, ldSpectrum_t *ptWidget)
{
    (void)ptScene;
    (void)ptWidget;
}

static inline void _draw_bar(arm_2d_tile_t *ptTile, int16_t x, int16_t y, uint8_t width, uint8_t height, ldColor color)
{
    if (height == 0 || width == 0)
    {
        return;
    }

    arm_2d_region_t barRegion = {
        .tLocation = {x, y},
        .tSize = {width, height},
    };

    arm_2d_fill_colour_with_opacity(ptTile, &barRegion, (__arm_2d_color_t){color}, 255);
}

static void _draw_bar_gradient_vertical(arm_2d_tile_t *ptTile, int16_t x, int16_t y, uint8_t width, uint8_t height, ldColor bottomColor, ldColor topColor)
{
    if (height == 0 || width == 0)
    {
        return;
    }

    uint8_t segments = (height > 16) ? 16 : height;
    uint8_t segmentHeight = height / segments;

    for (uint8_t i = 0; i < segments; i++)
    {
        uint8_t ratio = (i * 255) / (segments - 1);
        ldColor segColor = _get_gradient_color(bottomColor, topColor, ratio);

        arm_2d_region_t segRegion = {
            .tLocation = {x, y + i * segmentHeight},
            .tSize = {width, segmentHeight},
        };

        if (i == segments - 1)
        {
            segRegion.tSize.iHeight = height - i * segmentHeight;
        }

        arm_2d_fill_colour_with_opacity(ptTile, &segRegion, (__arm_2d_color_t){segColor}, 255);
    }
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
                uint8_t barHeight = (uint16_t)displayValue * widgetHeight / 255;

                if (barHeight == 0)
                {
                    continue;
                }

                int16_t barX = startX + i * totalBarWidth;
                int16_t barY;

                if (ptWidget->style == SPECTRUM_STYLE_MIRROR)
                {
                    barY = widgetHeight / 2 - barHeight / 2;
                }
                else
                {
                    barY = widgetHeight - barHeight;
                }

                if (ptWidget->isGradient)
                {
                    ldColor topColor = _get_gradient_color(ptWidget->startColor, ptWidget->endColor, displayValue);
                    _draw_bar_gradient_vertical(
                        &tTarget,
                        barX,
                        barY,
                        ptWidget->barWidth,
                        barHeight,
                        ptWidget->startColor,
                        topColor
                    );
                }
                else
                {
                    _draw_bar(
                        &tTarget,
                        barX,
                        barY,
                        ptWidget->barWidth,
                        barHeight,
                        ptWidget->startColor
                    );
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

void ldSpectrumSetColor(ldSpectrum_t *ptWidget, ldColor startColor, ldColor endColor, bool isGradient)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }

    ptWidget->startColor = startColor;
    ptWidget->endColor = endColor;
    ptWidget->isGradient = isGradient;
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
