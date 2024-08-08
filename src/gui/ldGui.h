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

#ifndef __LD_GUI_H__
#define __LD_GUI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"



#include "ldBase.h"
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

extern uint8_t cursorBlinkCount;
extern bool cursorBlinkFlag;

#define CURSOR_BLINK_TIMEOUT        50
#define SYS_TICK_CYCLE_MS           10

void ldGuiLoad(ld_scene_t *ptScene);
void ldGuiFrameStart(ld_scene_t *ptScene);
void ldGuiDraw(ld_scene_t *ptScene,const arm_2d_tile_t *ptTile,bool bIsNewFrame);



void ldGuiInit(ldPageFuncGroup_t *ptFuncGroup);




void ldGuiTouchProcess(ld_scene_t *ptScene);

void ldGuiSceneInit(ld_scene_t *ptScene);
void ldGuiLogicLoop(ld_scene_t *ptScene);
void ldGuiQuit(ld_scene_t *ptScene);
void ldGuiFrameComplete(ld_scene_t *ptScene);
void ldGuiJumpPage(ldPageFuncGroup_t *ptFuncGroup,arm_2d_scene_switch_mode_t *ptMode,uint16_t switchTimeMs);
void ldGuiUpdateScene(void);

#ifdef __cplusplus
}
#endif

#endif
