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

#ifndef __LD_GUI_H__
#define __LD_GUI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"



#include "ldBase.h"
#include "ldWindow.h"
#include "ldProgressWheel.h"
#include "ldCalendar.h"
#include "ldMessageBox.h"
#include "ldList.h"
#include "ldAnimation.h"
#include "ldArc.h"
#include "ldKeyboard.h"
#include "ldLineEdit.h"
#include "ldTable.h"
#include "ldGraph.h"
#include "ldComboBox.h"
#include "ldGauge.h"
#include "ldScrollSelecter.h"
#include "ldQRCode.h"
#include "ldIconSlider.h"
#include "ldDateTime.h"
#include "ldRadialMenu.h"
#include "ldSlider.h"
#include "ldText.h"
#include "ldProgressBar.h"
#include "ldCheckBox.h"
#include "ldLabel.h"
#include "ldButton.h"
#include "ldWindow.h"
#include "ldImage.h"
#include "ldClock.h"
#include "ldSpectrum.h"

extern uint8_t cursorBlinkCount;
extern bool cursorBlinkFlag;

#define CURSOR_BLINK_TIMEOUT        50
#define SYS_TICK_CYCLE_MS           10

void ldGuiLoad(ld_scene_t *ptScene);
void ldGuiDespose(ld_scene_t *ptScene);
void ldGuiFrameStart(ld_scene_t *ptScene);
void ldGuiFrameComplete(ld_scene_t *ptScene);
void ldGuiDraw(ld_scene_t *ptScene,arm_2d_tile_t *ptTile,bool bIsNewFrame);

void ldGuiInit(ldPageFuncGroup_t *ptFuncGroup);
void ldGuiLoop(void);

void ldGuiTouchProcess(ld_scene_t *ptScene);
void ldGuiSceneInit(ld_scene_t *ptScene);
void ldGuiUpdateScene(void);

extern ldBase_t *ptEditingWidget;
#if USE_LCD_TEST == 1
void ldGuiLcdTest(void);
#endif

void __ldGuiJumpPage(ldPageFuncGroup_t *ptFuncGroup,arm_2d_scene_switch_mode_t *ptMode,uint16_t switchTimeMs);

#if USE_SCENE_SWITCHING == 0
#define ldGuiJumpPage(pageFuncGroupName,...) \
    ARM_CONNECT2(ldGuiJumpPage_, __ARM_VA_NUM_ARGS(__VA_ARGS__))(pageFuncGroupName, ##__VA_ARGS__)

#define ldGuiJumpPage_0(page) __ldGuiJumpPage((ldPageFuncGroup_t *)NULL,&ARM_2D_SCENE_SWITCH_MODE_NONE,0)
#define ldGuiJumpPage_1(page,mode) __ldGuiJumpPage((ldPageFuncGroup_t *)NULL,&ARM_2D_SCENE_SWITCH_MODE_NONE,0)
#define ldGuiJumpPage_2(page,mode,ms) __ldGuiJumpPage((ldPageFuncGroup_t *)NULL,&ARM_2D_SCENE_SWITCH_MODE_NONE,0)
#elif USE_SCENE_SWITCHING == 1
#define ldGuiJumpPage(pageFuncGroupName,...) \
    ARM_CONNECT2(ldGuiJumpPage_, __ARM_VA_NUM_ARGS(__VA_ARGS__))(pageFuncGroupName, ##__VA_ARGS__)

#define ldGuiJumpPage_0(page) \
    ({ extern const ldPageFuncGroup_t page; \
       extern arm_2d_scene_switch_mode_t ARM_2D_SCENE_SWITCH_MODE_NONE; \
       __ldGuiJumpPage((ldPageFuncGroup_t *)&page,&ARM_2D_SCENE_SWITCH_MODE_NONE,0); })

#define ldGuiJumpPage_1(page,mode) \
    ({ extern const ldPageFuncGroup_t page; \
       __ldGuiJumpPage((ldPageFuncGroup_t *)&page,&ARM_2D_SCENE_SWITCH_MODE_NONE,0); })

#define ldGuiJumpPage_2(page,mode,ms) \
    ({ extern const ldPageFuncGroup_t page; \
       __ldGuiJumpPage((ldPageFuncGroup_t *)&page,&ARM_2D_SCENE_SWITCH_MODE_NONE,0); })
#else
#define ldGuiJumpPage(pageFuncGroupName,...) \
    ARM_CONNECT2(ldGuiJumpPage_, __ARM_VA_NUM_ARGS(__VA_ARGS__))(pageFuncGroupName, ##__VA_ARGS__)

#define ldGuiJumpPage_0(page) \
    ({ extern const ldPageFuncGroup_t page; \
       extern arm_2d_scene_switch_mode_t ARM_2D_SCENE_SWITCH_MODE_NONE; \
       __ldGuiJumpPage((ldPageFuncGroup_t *)&page,&ARM_2D_SCENE_SWITCH_MODE_NONE,0); })

#define ldGuiJumpPage_1(page,mode) \
    ({ extern const ldPageFuncGroup_t page; \
       __ldGuiJumpPage((ldPageFuncGroup_t *)&page,&(mode),0); })

#define ldGuiJumpPage_2(page,mode,ms) \
    ({ extern const ldPageFuncGroup_t page; \
       __ldGuiJumpPage((ldPageFuncGroup_t *)&page,&(mode),ms); })
#endif

#ifdef __cplusplus
}
#endif

#endif
