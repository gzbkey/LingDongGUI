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

#define __LD_KEYBOARD_IMPLEMENT__

#include "__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>

#include "ldKeyboard.h"
#include "xString.h"

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


#define KB_SPACE                   (5)

#define KB_NORMAL_RELEASE_COLOR     GLCD_COLOR_WHITE
#define KB_NORMAL_PRESS_COLOR       __RGB(188,191,206)
#define KB_OTHER_RELEASE_COLOR     __RGB(168,176,189)
#define KB_OTHER_PRESS_COLOR       GLCD_COLOR_WHITE


//占用无效ascii
#define KEY_CODE_NONE         0
#define KEY_CODE_SHIFT        1
#define KEY_CODE_SYMBOL_MODE  2
#define KEY_CODE_NUMBER_MODE  3
#define KEY_CODE_QWERTY_MODE  4



#define NUM_COL_NUM      (4)

#define NUM_BTN_W_SPACE   ((LD_CFG_SCREEN_WIDTH-KB_SPACE)/NUM_COL_NUM)

#define NUM_START      ((LD_CFG_SCREEN_WIDTH-NUM_BTN_W_SPACE*NUM_COL_NUM-KB_SPACE)/2)

#define NUM_BTN_W   (NUM_BTN_W_SPACE-KB_SPACE)
#define NUM_BTN_H   (((LD_CFG_SCREEN_HEIGHT>>1)-KB_SPACE)/4-KB_SPACE)

#define NUM_OFFSET_W(num)    (KB_SPACE+(NUM_BTN_W+KB_SPACE)*num)
#define NUM_OFFSET_H(num)    (KB_SPACE+(NUM_BTN_H+KB_SPACE)*num+(LD_CFG_SCREEN_HEIGHT>>1))

const kbBtnInfo_t numBtnInfo[]={
    {NUM_START+NUM_OFFSET_W(0),NUM_OFFSET_H(0),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"1",'1', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(1),NUM_OFFSET_H(0),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"2",'2', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(2),NUM_OFFSET_H(0),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"3",'3', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(3),NUM_OFFSET_H(0),NUM_BTN_W,NUM_BTN_H,(uint8_t*)(uint8_t *)"<-", 0x08, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},

    {NUM_START+NUM_OFFSET_W(0),NUM_OFFSET_H(1),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"4",'4', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(1),NUM_OFFSET_H(1),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"5",'5', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(2),NUM_OFFSET_H(1),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"6",'6', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(3),NUM_OFFSET_H(1),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"ABC", KEY_CODE_QWERTY_MODE, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},

    {NUM_START+NUM_OFFSET_W(0),NUM_OFFSET_H(2),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"7",'7', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(1),NUM_OFFSET_H(2),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"8",'8', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(2),NUM_OFFSET_H(2),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"9",'9', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(3),NUM_OFFSET_H(2),0,0,0,0,0},

    {NUM_START+NUM_OFFSET_W(0),NUM_OFFSET_H(3),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"±",'-', NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(1),NUM_OFFSET_H(3),NUM_BTN_W,NUM_BTN_H,(uint8_t*)"0",'0', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(2),NUM_OFFSET_H(3),NUM_BTN_W,NUM_BTN_H,(uint8_t*)".",'.', NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},
    {NUM_START+NUM_OFFSET_W(3),NUM_OFFSET_H(2),NUM_BTN_W,(NUM_BTN_H<<1)+KB_SPACE,(uint8_t*)"Enter",0x0d, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},
    {0},
};

#define QWERTY_COL_NUM      (10)

#define QWERTY_BTN_W_SPACE   ((LD_CFG_SCREEN_WIDTH-KB_SPACE)/QWERTY_COL_NUM)

#define QWERTY_START      ((LD_CFG_SCREEN_WIDTH-QWERTY_BTN_W_SPACE*QWERTY_COL_NUM-KB_SPACE)/2)

#define QWERTY_BTN_W   (QWERTY_BTN_W_SPACE-KB_SPACE)
#define QWERTY_BTN_H   (((LD_CFG_SCREEN_HEIGHT>>1)-KB_SPACE)/4-KB_SPACE)

#define QWERTY_OFFSET_W(num)    (QWERTY_BTN_W_SPACE*(num/2)+KB_SPACE+QWERTY_BTN_W*(num%2)/2) // (KB_SPACE+((QWERTY_BTN_W+KB_SPACE)>>1)*num)
#define QWERTY_OFFSET_H(num)    (KB_SPACE+(QWERTY_BTN_H+KB_SPACE)*num+(LD_CFG_SCREEN_HEIGHT>>1))

const kbBtnInfo_t qwertyBtnList[] = {
    {QWERTY_START + QWERTY_OFFSET_W(0), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"q", 'q', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(2), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"w", 'w', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(4), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"e", 'e', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(6), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"r", 'r', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(8), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"t", 't', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(10), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"y", 'y', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(12), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"u", 'u', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(14), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"i", 'i', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(16), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"o", 'o', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(18), QWERTY_OFFSET_H(0), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"p", 'p', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},

    {QWERTY_START + QWERTY_OFFSET_W(1), QWERTY_OFFSET_H(1), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"a", 'a', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(3), QWERTY_OFFSET_H(1), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"s", 's', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(5), QWERTY_OFFSET_H(1), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"d", 'd', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(7), QWERTY_OFFSET_H(1), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"f", 'f', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(9), QWERTY_OFFSET_H(1), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"g", 'g', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(11), QWERTY_OFFSET_H(1), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"h", 'h', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(13), QWERTY_OFFSET_H(1), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"j", 'j', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(15), QWERTY_OFFSET_H(1), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"k", 'k', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(17), QWERTY_OFFSET_H(1), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"l", 'l', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},

    {QWERTY_START + QWERTY_OFFSET_W(0), QWERTY_OFFSET_H(2), QWERTY_BTN_W / 2 + QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"Shift", KEY_CODE_SHIFT, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(3), QWERTY_OFFSET_H(2), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"z", 'z', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(5), QWERTY_OFFSET_H(2), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"x", 'x', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(7), QWERTY_OFFSET_H(2), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"c", 'c', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(9), QWERTY_OFFSET_H(2), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"v", 'v', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(11), QWERTY_OFFSET_H(2), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"b", 'b', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(13), QWERTY_OFFSET_H(2), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"n", 'n', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(15), QWERTY_OFFSET_H(2), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"m", 'm', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(17), QWERTY_OFFSET_H(2), QWERTY_BTN_W / 2 + QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"<-", 0x08, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},

    {QWERTY_START + QWERTY_OFFSET_W(0), QWERTY_OFFSET_H(3), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"!@", KEY_CODE_SYMBOL_MODE, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(2), QWERTY_OFFSET_H(3), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"123", KEY_CODE_NUMBER_MODE, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(4), QWERTY_OFFSET_H(3), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)".", '.', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(6), QWERTY_OFFSET_H(3), QWERTY_BTN_W_SPACE * 3 + QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)" ", ' ', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(14), QWERTY_OFFSET_H(3), QWERTY_BTN_W, QWERTY_BTN_H, (uint8_t *)"?", '?', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START + QWERTY_OFFSET_W(16), QWERTY_OFFSET_H(3), QWERTY_BTN_W * 2 + KB_SPACE, QWERTY_BTN_H, (uint8_t *)"Enter", 0x0d, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},
    {0},
};

const kbBtnInfo_t symbolBtnInfo[]={
    {QWERTY_START+QWERTY_OFFSET_W(0),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H, (uint8_t *)"-" ,'-', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(2),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"/" ,'/', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(4),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)":" ,':', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(6),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)";" ,';', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(8),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"(" ,'(', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(10),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)")" ,')', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(12),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"_" ,'_', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(14),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"$" ,'$', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(16),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"&" ,'&', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(18),QWERTY_OFFSET_H(0),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"\"" ,'"', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},

    {QWERTY_START+QWERTY_OFFSET_W(0),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"[" ,'[', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(2),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"]" ,']', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(4),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"{" ,'{', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(6),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"}" ,'}', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(8),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"#" ,'#', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(10),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"%" ,'%', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(12),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"^" ,'^', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(14),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"*" ,'*', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(16),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"+" ,'+', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(18),QWERTY_OFFSET_H(1),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"=" ,'=', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},

    {QWERTY_START+QWERTY_OFFSET_W(0),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"\\" ,'\\', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(2),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"|" ,'|', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(4),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"<" ,'<', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(6),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)">" ,'>', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(8),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"~" ,'~', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(10),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"," ,',', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(12),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"@" ,'@', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(14),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"!" ,'!', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(16),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"`" ,'`', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(18),QWERTY_OFFSET_H(2),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"\'" ,'\'', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},

    {QWERTY_START+QWERTY_OFFSET_W(0),QWERTY_OFFSET_H(3),QWERTY_BTN_W*2+KB_SPACE,QWERTY_BTN_H,(uint8_t*)"ABC",KEY_CODE_QWERTY_MODE, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(4),QWERTY_OFFSET_H(3),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"." ,'.', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(6),QWERTY_OFFSET_H(3),QWERTY_BTN_W_SPACE*3+QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)" " ,' ', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(14),QWERTY_OFFSET_H(3),QWERTY_BTN_W,QWERTY_BTN_H,(uint8_t *)"?" ,'?', NULL, NULL, NULL, NULL, KB_NORMAL_PRESS_COLOR, KB_NORMAL_RELEASE_COLOR},
    {QWERTY_START+QWERTY_OFFSET_W(16),QWERTY_OFFSET_H(3),QWERTY_BTN_W*2+KB_SPACE,QWERTY_BTN_H,(uint8_t*)"<-",0x08, NULL, NULL, NULL, NULL, KB_OTHER_PRESS_COLOR, KB_OTHER_RELEASE_COLOR},
    {0},
};

static bool isExit=false;

const ldBaseWidgetFunc_t ldKeyboardFunc = {
    .depose = (ldDeposeFunc_t)ldKeyboard_depose,
    .load = (ldLoadFunc_t)ldKeyboard_on_load,
    .frameStart  = (ldFrameStartFunc_t)ldKeyboard_on_frame_start,
    .frameComplete = (ldFrameCompleteFunc_t)ldKeyboard_on_frame_complete,
    .show = (ldShowFunc_t)ldKeyboard_show,
};

static arm_2d_region_t _keyboardGetClickRegion(ldKeyboard_t *ptWidget, arm_2d_location_t clickPoint)
{
    arm_2d_region_t retRegion={
        .tLocation={
            .iX=0,
            .iY=0,
        },
        .tSize={
            .iWidth=LD_CFG_SCREEN_WIDTH,
            .iHeight=LD_CFG_SCREEN_HEIGHT>>1,
        },
    };
    ptWidget->keyCode=KEY_CODE_NONE;

    kbBtnInfo_t *pBtnInfo=(kbBtnInfo_t*)ptWidget->pBtnList;
    if(pBtnInfo)
    {
        while (pBtnInfo->keyCode)
        {
            if(arm_2d_is_point_inside_region(&pBtnInfo->region,&clickPoint))
            {
                retRegion=pBtnInfo->region;
                ptWidget->keyCode=pBtnInfo->keyCode;
                break;
            }
            pBtnInfo++;
        }
    }

    return retRegion;
}

static bool _addAscii(ldKeyboard_t *ptWidget,uint16_t textLen,uint8_t ascii,bool isBack)
{
    uint8_t *pBuf;
    if(ptWidget->strMax==0)
    {
        if(textLen>0)
        {
            pBuf=ldCalloc(1,textLen+1);
            strcpy((char*)pBuf,(char*)(*ptWidget->ppStr));
        }
        *ptWidget->ppStr=ldRealloc(*ptWidget->ppStr,textLen+2);
        if(textLen>0)
        {
            strcpy((char*)(*ptWidget->ppStr),(char*)pBuf);
        }
    }
    if((textLen<ptWidget->strMax)||(ptWidget->strMax==0))
    {
        if(isBack)
        {
            xStringPushBack(*ptWidget->ppStr,textLen,ascii);
        }
        else
        {
            uint8_t temp[2]={0};
            temp[0]=ascii;
            xStringInsert(*ptWidget->ppStr,textLen,0,temp);
        }
        return true;
    }

    return false;
}

static void _inputAsciiProcess(ldKeyboard_t *ptWidget,uint8_t ascii)
{
    uint16_t textLen;
    if((*ptWidget->ppStr)!=NULL)
    {
        textLen=strlen((char*)(*ptWidget->ppStr));
    }
    else
    {
        textLen=0;
    }

    if((ptWidget->editType==typeInt)||(ptWidget->editType==typeFloat))
    {
        switch (ascii)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            _addAscii(ptWidget,textLen,ascii,true);
            break;
        }
        case '.':
        {
            if((ptWidget->editType==typeFloat)&&(strstr((char*)(*ptWidget->ppStr),".")==NULL))
            {
                _addAscii(ptWidget,textLen,ascii,true);
            }
            break;
        }
        case '-':
        {
            if((*ptWidget->ppStr)[0]=='-')
            {
                xStringRemove(*ptWidget->ppStr,textLen,0,1);
            }
            else
            {
                _addAscii(ptWidget,textLen,ascii,false);
            }
            break;
        }
        default:
            break;
        }
    }
    else
    {
        if((ascii>=0x20)&&(ascii<0x7F))
        {
            if(ptWidget->isUpper&&(ascii>='a')&&(ascii<='z'))
            {
                ascii-=32;
            }
            _addAscii(ptWidget,textLen,ascii,true);
        }
    }

    switch(ascii)
    {
    case 0x08://backspace
    {
        xStringPopBack(*ptWidget->ppStr,textLen);
        break;
    }
    case 0x0d://enter
    {
        isExit=true;
        break;
    }
    default:
        break;
    }
}

void ldKeyboardUpdate(ldKeyboard_t *ptWidget)
{
    ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iX=0;
    ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iY=LD_CFG_SCREEN_HEIGHT>>1;
    ptWidget->use_as__ldBase_t.tTempRegion.tSize.iWidth=LD_CFG_SCREEN_WIDTH;
    ptWidget->use_as__ldBase_t.tTempRegion.tSize.iHeight=LD_CFG_SCREEN_HEIGHT>>1;

    ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iY+=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
}

static bool slotKBProcess(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldKeyboard_t *ptWidget=msg.ptSender;
    arm_2d_location_t clickPoint;
    clickPoint.iX=(int16_t)GET_SIGNAL_VALUE_X(msg.value);
    clickPoint.iY=(int16_t)GET_SIGNAL_VALUE_Y(msg.value);
    
    switch (msg.signal)
    {
     case SIGNAL_PRESS:
     {
         ptWidget->isClick=true;
         ptWidget->use_as__ldBase_t.tTempRegion =_keyboardGetClickRegion(ptWidget,clickPoint);
         ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iY+=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
         ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

         ldKeyboardCallback(ptWidget,msg.signal);

         if(ptWidget->keyCode!=0)
         {
             _inputAsciiProcess(ptWidget,ptWidget->keyCode);

             if(ptWidget->keyCode==KEY_CODE_SHIFT)
             {
                 ldKeyboardUpdate(ptWidget);
             }
         }

         break;
     }
     case SIGNAL_RELEASE:
     {
         ptWidget->isClick=false;
         ptWidget->use_as__ldBase_t.tTempRegion =_keyboardGetClickRegion(ptWidget,clickPoint);
         ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iY+=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
         ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

         // process end,reset
         if((ptWidget->isUpper)&&(!ptWidget->isLockUpper)&&(ptWidget->keyCode>='a')&&(ptWidget->keyCode<='z'))
         {
             ptWidget->isUpper=false;

             ldKeyboardUpdate(ptWidget);
         }

         ldKeyboardCallback(ptWidget,msg.signal);
     }
     default:
         break;
     }
     return true;
 }

ldKeyboard_t* ldKeyboard_init(ld_scene_t *ptScene, ldKeyboard_t *ptWidget, uint16_t nameId, uint16_t parentNameId,arm_2d_font_t *ptFont)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldKeyboard_t));
        if (NULL == ptWidget)
        {
            LOG_ERROR("[init failed][keyboard] id:%d", nameId);
            return NULL;
        }
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = 0;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = 0;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = LD_CFG_SCREEN_WIDTH;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = LD_CFG_SCREEN_HEIGHT;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeKeyboard;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldKeyboardFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    ptWidget->ptFont=ptFont;
    ptWidget->isWaitInit=true;
    ldBaseSetHidden((ldBase_t*)ptWidget,true);

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotKBProcess);
    ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotKBProcess);

    LOG_INFO("[init][keyboard] id:%d, size:%d", nameId,(int)sizeof (*ptWidget));
    return ptWidget;
}

void ldKeyboard_depose(ld_scene_t *ptScene, ldKeyboard_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeKeyboard)
    {
        return;
    }

    LOG_INFO("[depose][keyboard] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove((arm_2d_control_node_t*)ptWidget);
#if USE_VIRTUAL_RESOURCE == 1
    ldFree(ptWidget->ptFont);
#endif
    ldFree(ptWidget);
}

void ldKeyboard_on_load(ld_scene_t *ptScene, ldKeyboard_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldKeyboard_on_frame_start(ld_scene_t *ptScene, ldKeyboard_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldKeyboard_on_frame_complete(ld_scene_t *ptScene, ldKeyboard_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

static void _ldkeyboardNewButton(ldKeyboard_t *ptWidget,arm_2d_tile_t *parentTile,arm_2d_region_t *ptBtnRegion,uint8_t *pStr,ldColor btnColor,ldColor charColor,bool bIsNewFrame)
{
    draw_round_corner_box(parentTile,ptBtnRegion,btnColor,255,bIsNewFrame);
    ldBaseLabel(parentTile,
                ptBtnRegion,
                pStr,
                ptWidget->ptFont,
                ARM_2D_ALIGN_CENTRE,
                charColor,
                ptWidget->use_as__ldBase_t.opacity);
}

__WEAK const kbBtnInfo_t *ldKeyboardGetTargetBtnList(ldKeyboard_t *ptWidget)
{
    switch(ptWidget->editType)
    {
    case typeString:
        return qwertyBtnList;
    case typeInt:
        return numBtnInfo;
    case typeFloat:
        return numBtnInfo;
    }
    return qwertyBtnList;
}

__WEAK void ldKeyboardCallback(ldKeyboard_t *ptWidget, uint8_t signal)
{
    switch (ptWidget->keyCode)
    {
    case KEY_CODE_SHIFT:
    {
        if(signal==SIGNAL_PRESS)
        {
            if(!ptWidget->isUpper)
            {
                ptWidget->isUpper=true;
            }
            else
            {
                if(!ptWidget->isLockUpper)
                {
                    ptWidget->isLockUpper=true;
                }
                else
                {
                    ptWidget->isUpper=false;
                    ptWidget->isLockUpper=false;
                }
            }
        }
        break;
    }

    case KEY_CODE_QWERTY_MODE:
    {
        if(signal==SIGNAL_RELEASE)
        {
            ptWidget->pBtnList=qwertyBtnList;
            ldKeyboardUpdate(ptWidget);
        }
        break;
    }

    case KEY_CODE_NUMBER_MODE:
    {
        if(signal==SIGNAL_RELEASE)
        {
            ptWidget->pBtnList=numBtnInfo;
            ldKeyboardUpdate(ptWidget);
        }
        break;
    }

    case KEY_CODE_SYMBOL_MODE:
    {
        if(signal==SIGNAL_RELEASE)
        {
            ptWidget->pBtnList=symbolBtnInfo;
            ldKeyboardUpdate(ptWidget);
        }
        break;
    }
    }
}

__WEAK ldColor ldKeyboardGetShiftColor(bool isUpper, bool isLock)
{
    if(isUpper)
    {
        if(isLock)
        {
            return GLCD_COLOR_BLUE;
        }
        else
        {
            return GLCD_COLOR_BLACK;
        }
    }
    else
    {
        return GLCD_COLOR_WHITE;
    }
}

void ldKeyboard_show(ld_scene_t *ptScene, ldKeyboard_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    arm_2d_region_t kbRegion={
        .tLocation={
            .iX=0,
            .iY=0,
        },
        .tSize={
            .iWidth=LD_CFG_SCREEN_WIDTH,
            .iHeight=LD_CFG_SCREEN_HEIGHT,
        },
    };

    if(arm_2d_helper_pfb_is_region_active(ptTile,&kbRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &kbRegion)
        {
            if(ldBaseIsHidden((ldBase_t*)ptWidget))
            {
                if(!ptWidget->isWaitInit)
                {
                    ptWidget->isWaitInit=true;
                }
                break;
            }

            if(ptWidget->isWaitInit)
            {
                ptWidget->isWaitInit=false;

                ptWidget->pBtnList=ldKeyboardGetTargetBtnList(ptWidget);
            }

            if(isExit)
            {
                isExit=false;
                ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY=LD_CFG_SCREEN_HEIGHT;
                ldBaseSetHidden((ldBase_t *)ptWidget,true);
                emit(ptWidget->editorId,SIGNAL_FINISHED,0);

                ldBaseBgMove(ptScene,LD_CFG_SCREEN_WIDTH,LD_CFG_SCREEN_HEIGHT,0,0);
            }

            ldBaseColor(&tTarget,
                        &((arm_2d_region_t){0,(LD_CFG_SCREEN_HEIGHT>>1),LD_CFG_SCREEN_WIDTH,(LD_CFG_SCREEN_HEIGHT>>1)}),
                        __RGB(208,211,220),
                        ptWidget->use_as__ldBase_t.opacity);

            kbBtnInfo_t *pBtnInfo=(kbBtnInfo_t*)ptWidget->pBtnList;
            if(pBtnInfo)
            {
                while (pBtnInfo->keyCode)
                {
                    if ((pBtnInfo->ptReleaseImgTile == NULL) &&
                            (pBtnInfo->ptReleaseMaskTile == NULL) &&
                            (pBtnInfo->ptPressImgTile == NULL) &&
                            (pBtnInfo->ptPressMaskTile == NULL)) // color
                    {
                        if((ptWidget->isClick)&&(ptWidget->keyCode==pBtnInfo->keyCode))
                        {
                            draw_round_corner_box(&tTarget,&pBtnInfo->region,pBtnInfo->pressColor,ptWidget->use_as__ldBase_t.opacity,bIsNewFrame);
                        }
                        else
                        {
                            draw_round_corner_box(&tTarget,&pBtnInfo->region,pBtnInfo->releaseColor,ptWidget->use_as__ldBase_t.opacity,bIsNewFrame);
                        }
                    }
                    else
                    {
                        if((ptWidget->isClick)&&(ptWidget->keyCode==pBtnInfo->keyCode))
                        {
                            ldBaseImage(&tTarget,
                                    &pBtnInfo->region,
                                    pBtnInfo->ptPressImgTile,
                                    pBtnInfo->ptPressMaskTile,
                                    pBtnInfo->pressColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                        }
                        else
                        {
                            ldBaseImage(&tTarget,
                                    &pBtnInfo->region,
                                    pBtnInfo->ptReleaseImgTile,
                                    pBtnInfo->ptReleaseMaskTile,
                                    pBtnInfo->releaseColor,
                                    ptWidget->use_as__ldBase_t.opacity);
                        }
                    }

                    if((ptWidget->isKeySelect)&&(ptWidget->keyCode==pBtnInfo->keyCode))
                    {
                        draw_round_corner_box(&tTarget,&pBtnInfo->region,LD_SELECT_COLOR,ptWidget->use_as__ldBase_t.opacity,bIsNewFrame);
                    }

                    ldColor textColor=GLCD_COLOR_BLACK;
                    //特殊按键处理
                    if(pBtnInfo->keyCode==KEY_CODE_SHIFT)
                    {
                        textColor=ldKeyboardGetShiftColor(ptWidget->isUpper,ptWidget->isLockUpper);
                    }

                    if((ptWidget->isUpper)&&(pBtnInfo->keyCode>='a')&&(pBtnInfo->keyCode<='z'))
                    {
                        uint8_t tempStr[2]={
                            pBtnInfo->pText[0]-32,0
                        };

                        ldBaseLabel(&tTarget,
                                        &pBtnInfo->region,
                                        tempStr,
                                        ptWidget->ptFont,
                                        ARM_2D_ALIGN_CENTRE,
                                        GLCD_COLOR_BLACK,
                                        ptWidget->use_as__ldBase_t.opacity);
                    }
                    else
                    {
                        ldBaseLabel(&tTarget,
                                        &pBtnInfo->region,
                                        pBtnInfo->pText,
                                        ptWidget->ptFont,
                                        ARM_2D_ALIGN_CENTRE,
                                        textColor,
                                        ptWidget->use_as__ldBase_t.opacity);
                    }
                    pBtnInfo++;
                    
                    arm_2d_op_wait_async(NULL);
                }
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

static int32_t _kbManhattan(const kbBtnInfo_t *current, const kbBtnInfo_t *target, ldNavDir_t dir)
{
    int16_t dx = target->region.tLocation.iX - current->region.tLocation.iX;
    int16_t dy = target->region.tLocation.iY - current->region.tLocation.iY;

    switch (dir)
    {
        case NAV_UP:
        {
            if (dy >= 0) return INT32_MAX;
            break;
        }
        case NAV_DOWN:
        {
            if (dy <= 0) return INT32_MAX;
            break;
        }
        case NAV_LEFT:
        {
            if (dx >= 0) return INT32_MAX;
            break;
        }
        case NAV_RIGHT:
        {
            if (dx <= 0) return INT32_MAX;
            break;
        }
        default:
            return INT32_MAX;
    }
    return abs(dx) + abs(dy);
}

static const kbBtnInfo_t *getBtnByKeyCode(const kbBtnInfo_t *array, uint8_t key)
{
    if (!array)
    {
        return NULL;
    }

    for (const kbBtnInfo_t *p = array; p->keyCode; ++p)
    {
        if (p->keyCode == key)
        {
            return p;
        }
    }
    return NULL;
}

static const kbBtnInfo_t *getBtnByPos(ldKeyboard_t *ptWidget, int16_t x, int16_t y)
{
    if (!ptWidget) return 0;

    const kbBtnInfo_t *p = (const kbBtnInfo_t *)ptWidget->pBtnList;
    if (!p || p->keyCode == 0) return 0;

    const kbBtnInfo_t *best = NULL;
    uint32_t           bestDist = UINT32_MAX;

    for (; p->keyCode; ++p)
    {
        int16_t bcx = p->region.tLocation.iX + p->region.tSize.iWidth  / 2;
        int16_t bcy = p->region.tLocation.iY + p->region.tSize.iHeight / 2;

        uint32_t d = abs(bcx - x) + abs(bcy - y);
        if (d < bestDist) {
            bestDist = d;
            best     = p;
        }
    }
    return best;
}

static uint8_t _kbNavigate(ldKeyboard_t *ptWidget, uint8_t currentKeyCode, ldNavDir_t dir)
{
    if (!ptWidget) return currentKeyCode;

    const kbBtnInfo_t *array = (const kbBtnInfo_t *)ptWidget->pBtnList;
    const kbBtnInfo_t *pCur;
    if(currentKeyCode==KEY_CODE_NONE)
    {
        pCur = getBtnByPos(ptWidget,ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth>>1,ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight/4*3);
    }
    else
    {
        pCur  = getBtnByKeyCode(array, currentKeyCode);
    }
    
    if (!pCur)
    {
        return currentKeyCode;
    }

    const kbBtnInfo_t *best = NULL;
    int32_t bestDist = INT32_MAX;

    for (const kbBtnInfo_t *p = array; p->keyCode; ++p)
    {
        if (p == pCur)
        {
            continue;
        }
        int32_t d = _kbManhattan(pCur, p, dir);
        if (d < bestDist)
        {
            bestDist = d;
            best     = p;
        }
    }

    arm_2d_region_get_minimal_enclosure(&pCur->region,
                                        &best->region,
                                        &ptWidget->use_as__ldBase_t.tTempRegion);
    ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iY+=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    
    return best ? best->keyCode : currentKeyCode;
}

void ldKeyboardNavigate(ldKeyboard_t *ptWidget, ldNavDir_t dir)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    ptWidget->keyCode = _kbNavigate(ptWidget,ptWidget->keyCode, dir);

    ptWidget->isKeySelect=true;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
