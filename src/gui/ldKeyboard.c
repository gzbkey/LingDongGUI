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

#define __LD_KEYBOARD_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
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

#define KB_ASCII_RELEASE_COLOR     GLCD_COLOR_WHITE
#define KB_ASCII_PRESS_COLOR       __RGB(188,191,206)
#define KB_OTHER_RELEASE_COLOR     __RGB(168,176,189)
#define KB_OTHER_PRESS_COLOR       GLCD_COLOR_WHITE

enum {
    KB_VALUE_NONE,
    KB_VALUE_QWERTY_MODE,
    KB_VALUE_NUMBER_MODE,
    KB_VALUE_SHIFT,
    KB_VALUE_SYMBOL_MODE
}kbValueSpecial;

typedef struct {
    arm_2d_region_t region;
    uint8_t* pText;
    uint8_t value;
    bool colorType;
}kbBtnInfo_t;

const ldColor btnDefaultColor[2][2]={
    {KB_ASCII_RELEASE_COLOR,KB_ASCII_PRESS_COLOR},
    {KB_OTHER_RELEASE_COLOR,KB_OTHER_PRESS_COLOR}
};

const uint8_t num0Str[]="0";
const uint8_t num1Str[]="1";
const uint8_t num2Str[]="2";
const uint8_t num3Str[]="3";
const uint8_t num4Str[]="4";
const uint8_t num5Str[]="5";
const uint8_t num6Str[]="6";
const uint8_t num7Str[]="7";
const uint8_t num8Str[]="8";
const uint8_t num9Str[]="9";
const uint8_t pointStr[]=".";
const uint8_t backspaceStr[]="<-";
const uint8_t abcStr[]="ABC";
const uint8_t enterStr[]="Enter";
const uint8_t addSubStr[]="±";

#define NUM_COL_NUM      (4)

#define NUM_BTN_W_SPACE   ((LD_CFG_SCEEN_WIDTH-KB_SPACE)/NUM_COL_NUM)

#define NUM_START      ((LD_CFG_SCEEN_WIDTH-NUM_BTN_W_SPACE*NUM_COL_NUM-KB_SPACE)/2)

#define NUM_BTN_W   (NUM_BTN_W_SPACE-KB_SPACE)
#define NUM_BTN_H   (((LD_CFG_SCEEN_HEIGHT>>1)-KB_SPACE)/4-KB_SPACE)

#define NUM_OFFSET_W(num)    (KB_SPACE+(NUM_BTN_W+KB_SPACE)*num)
#define NUM_OFFSET_H(num)    (KB_SPACE+(NUM_BTN_H+KB_SPACE)*num+(LD_CFG_SCEEN_HEIGHT>>1))

const kbBtnInfo_t numBtnInfo[4][4]={
    {
        {NUM_START+NUM_OFFSET_W(0),NUM_OFFSET_H(0),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num1Str,'1',0},
        {NUM_START+NUM_OFFSET_W(1),NUM_OFFSET_H(0),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num2Str,'2',0},
        {NUM_START+NUM_OFFSET_W(2),NUM_OFFSET_H(0),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num3Str,'3',0},
        {NUM_START+NUM_OFFSET_W(3),NUM_OFFSET_H(0),NUM_BTN_W,NUM_BTN_H,(uint8_t*)backspaceStr,0x08,1},
    },
    {
        {NUM_START+NUM_OFFSET_W(0),NUM_OFFSET_H(1),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num4Str,'4',0},
        {NUM_START+NUM_OFFSET_W(1),NUM_OFFSET_H(1),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num5Str,'5',0},
        {NUM_START+NUM_OFFSET_W(2),NUM_OFFSET_H(1),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num6Str,'6',0},
        {NUM_START+NUM_OFFSET_W(3),NUM_OFFSET_H(1),NUM_BTN_W,NUM_BTN_H,(uint8_t*)abcStr,KB_VALUE_QWERTY_MODE,1},
    },
    {
        {NUM_START+NUM_OFFSET_W(0),NUM_OFFSET_H(2),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num7Str,'7',0},
        {NUM_START+NUM_OFFSET_W(1),NUM_OFFSET_H(2),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num8Str,'8',0},
        {NUM_START+NUM_OFFSET_W(2),NUM_OFFSET_H(2),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num9Str,'9',0},
        {NUM_START+NUM_OFFSET_W(3),NUM_OFFSET_H(2),0,0,0,0,0},
    },
    {
        {NUM_START+NUM_OFFSET_W(0),NUM_OFFSET_H(3),NUM_BTN_W,NUM_BTN_H,(uint8_t*)addSubStr,'-',1},
        {NUM_START+NUM_OFFSET_W(1),NUM_OFFSET_H(3),NUM_BTN_W,NUM_BTN_H,(uint8_t*)num0Str,'0',0},
        {NUM_START+NUM_OFFSET_W(2),NUM_OFFSET_H(3),NUM_BTN_W,NUM_BTN_H,(uint8_t*)pointStr,'.',1},
        {NUM_START+NUM_OFFSET_W(3),NUM_OFFSET_H(2),NUM_BTN_W,(NUM_BTN_H<<1)+KB_SPACE,(uint8_t*)enterStr,0x0d,1},
    }
};

const uint8_t asciiQStr[]="q";
const uint8_t asciiWStr[]="w";
const uint8_t asciiEStr[]="e";
const uint8_t asciiRStr[]="r";
const uint8_t asciiTStr[]="t";
const uint8_t asciiYStr[]="y";
const uint8_t asciiUStr[]="u";
const uint8_t asciiIStr[]="i";
const uint8_t asciiOStr[]="o";
const uint8_t asciiPStr[]="p";
const uint8_t asciiAStr[]="a";
const uint8_t asciiSStr[]="s";
const uint8_t asciiDStr[]="d";
const uint8_t asciiFStr[]="f";
const uint8_t asciiGStr[]="g";
const uint8_t asciiHStr[]="h";
const uint8_t asciiJStr[]="j";
const uint8_t asciiKStr[]="k";
const uint8_t asciiLStr[]="l";
const uint8_t asciiZStr[]="z";
const uint8_t asciiXStr[]="x";
const uint8_t asciiCStr[]="c";
const uint8_t asciiVStr[]="v";
const uint8_t asciiBStr[]="b";
const uint8_t asciiNStr[]="n";
const uint8_t asciiMStr[]="m";
const uint8_t asciiShiftStr[]="shift";
const uint8_t asciiDelStr[]="<-";
const uint8_t asciiNumStr[]="123";
const uint8_t asciiPointStr[]=".";
const uint8_t asciiSpaceStr[]=" ";
const uint8_t asciiQuestionStr[]="?";
const uint8_t asciiEnterStr[]="Enter";
const uint8_t asciiSymbolStr[]="!@";

const uint8_t asciiShiftUpperStr[]="SHIFT";

#define QWERT_COL_NUM      (10)

#define QWERT_BTN_W_SPACE   ((LD_CFG_SCEEN_WIDTH-KB_SPACE)/QWERT_COL_NUM)

#define QWERT_START      ((LD_CFG_SCEEN_WIDTH-QWERT_BTN_W_SPACE*QWERT_COL_NUM-KB_SPACE)/2)

#define QWERT_BTN_W   (QWERT_BTN_W_SPACE-KB_SPACE)
#define QWERT_BTN_H   (((LD_CFG_SCEEN_HEIGHT>>1)-KB_SPACE)/4-KB_SPACE)

#define QWERT_OFFSET_W(num)    (QWERT_BTN_W_SPACE*(num/2)+KB_SPACE+QWERT_BTN_W*(num%2)/2) // (KB_SPACE+((QWERT_BTN_W+KB_SPACE)>>1)*num)
#define QWERT_OFFSET_H(num)    (KB_SPACE+(QWERT_BTN_H+KB_SPACE)*num+(LD_CFG_SCEEN_HEIGHT>>1))

const kbBtnInfo_t asciiBtnInfo[4][10]={
    {
        {QWERT_START+QWERT_OFFSET_W(0),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'q',0},
        {QWERT_START+QWERT_OFFSET_W(2),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'w',0},
        {QWERT_START+QWERT_OFFSET_W(4),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'e',0},
        {QWERT_START+QWERT_OFFSET_W(6),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'r',0},
        {QWERT_START+QWERT_OFFSET_W(8),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'t',0},
        {QWERT_START+QWERT_OFFSET_W(10),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'y',0},
        {QWERT_START+QWERT_OFFSET_W(12),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'u',0},
        {QWERT_START+QWERT_OFFSET_W(14),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'i',0},
        {QWERT_START+QWERT_OFFSET_W(16),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'o',0},
        {QWERT_START+QWERT_OFFSET_W(18),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'p',0},
    },
    {
        {QWERT_START+QWERT_OFFSET_W(1),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'a',0},
        {QWERT_START+QWERT_OFFSET_W(3),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'s',0},
        {QWERT_START+QWERT_OFFSET_W(5),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'d',0},
        {QWERT_START+QWERT_OFFSET_W(7),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'f',0},
        {QWERT_START+QWERT_OFFSET_W(9),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'g',0},
        {QWERT_START+QWERT_OFFSET_W(11),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'h',0},
        {QWERT_START+QWERT_OFFSET_W(13),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'j',0},
        {QWERT_START+QWERT_OFFSET_W(15),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'k',0},
        {QWERT_START+QWERT_OFFSET_W(17),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'l',0},
        {0,0,0,0,0,0,0},
    },
    {
        {QWERT_START+QWERT_OFFSET_W(0),QWERT_OFFSET_H(2),QWERT_BTN_W/2+QWERT_BTN_W,QWERT_BTN_H,(uint8_t*)asciiShiftStr,KB_VALUE_SHIFT,1},
        {QWERT_START+QWERT_OFFSET_W(3),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'z',0},
        {QWERT_START+QWERT_OFFSET_W(5),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'x',0},
        {QWERT_START+QWERT_OFFSET_W(7),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'c',0},
        {QWERT_START+QWERT_OFFSET_W(9),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'v',0},
        {QWERT_START+QWERT_OFFSET_W(11),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'b',0},
        {QWERT_START+QWERT_OFFSET_W(13),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'n',0},
        {QWERT_START+QWERT_OFFSET_W(15),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'m',0},
        {QWERT_START+QWERT_OFFSET_W(17),QWERT_OFFSET_H(2),QWERT_BTN_W/2+QWERT_BTN_W,QWERT_BTN_H,(uint8_t*)asciiDelStr,0x08,1},
        {0,0,0,0,0,0,0},
    },
    {
        {QWERT_START+QWERT_OFFSET_W(0),QWERT_OFFSET_H(3),QWERT_BTN_W,QWERT_BTN_H,(uint8_t*)asciiSymbolStr,KB_VALUE_SYMBOL_MODE,1},
        {QWERT_START+QWERT_OFFSET_W(2),QWERT_OFFSET_H(3),QWERT_BTN_W,QWERT_BTN_H,(uint8_t*)asciiNumStr,KB_VALUE_NUMBER_MODE,1},
        {QWERT_START+QWERT_OFFSET_W(4),QWERT_OFFSET_H(3),QWERT_BTN_W,QWERT_BTN_H,(uint8_t*)asciiPointStr,'.',0},
        {QWERT_START+QWERT_OFFSET_W(6),QWERT_OFFSET_H(3),QWERT_BTN_W_SPACE*3+QWERT_BTN_W,QWERT_BTN_H,(uint8_t*)asciiSpaceStr,' ',0},
        {QWERT_START+QWERT_OFFSET_W(14),QWERT_OFFSET_H(3),QWERT_BTN_W,QWERT_BTN_H,(uint8_t*)asciiQuestionStr,'?',0},
        {QWERT_START+QWERT_OFFSET_W(16),QWERT_OFFSET_H(3),QWERT_BTN_W*2+KB_SPACE,QWERT_BTN_H,(uint8_t*)asciiEnterStr,0x0d,1},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
    },
};

const kbBtnInfo_t symbolBtnInfo[4][10]={
    {
        {QWERT_START+QWERT_OFFSET_W(0),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'-',0},
        {QWERT_START+QWERT_OFFSET_W(2),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'/',0},
        {QWERT_START+QWERT_OFFSET_W(4),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,':',0},
        {QWERT_START+QWERT_OFFSET_W(6),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,';',0},
        {QWERT_START+QWERT_OFFSET_W(8),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'(',0},
        {QWERT_START+QWERT_OFFSET_W(10),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,')',0},
        {QWERT_START+QWERT_OFFSET_W(12),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'_',0},
        {QWERT_START+QWERT_OFFSET_W(14),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'$',0},
        {QWERT_START+QWERT_OFFSET_W(16),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'&',0},
        {QWERT_START+QWERT_OFFSET_W(18),QWERT_OFFSET_H(0),QWERT_BTN_W,QWERT_BTN_H,0,'"',0},
    },
    {
        {QWERT_START+QWERT_OFFSET_W(0),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'[',0},
        {QWERT_START+QWERT_OFFSET_W(2),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,']',0},
        {QWERT_START+QWERT_OFFSET_W(4),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'{',0},
        {QWERT_START+QWERT_OFFSET_W(6),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'}',0},
        {QWERT_START+QWERT_OFFSET_W(8),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'#',0},
        {QWERT_START+QWERT_OFFSET_W(10),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'%',0},
        {QWERT_START+QWERT_OFFSET_W(12),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'^',0},
        {QWERT_START+QWERT_OFFSET_W(14),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'*',0},
        {QWERT_START+QWERT_OFFSET_W(16),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'+',0},
        {QWERT_START+QWERT_OFFSET_W(18),QWERT_OFFSET_H(1),QWERT_BTN_W,QWERT_BTN_H,0,'=',0},
    },
    {
        {QWERT_START+QWERT_OFFSET_W(0),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'\\',0},
        {QWERT_START+QWERT_OFFSET_W(2),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'|',0},
        {QWERT_START+QWERT_OFFSET_W(4),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'<',0},
        {QWERT_START+QWERT_OFFSET_W(6),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'>',0},
        {QWERT_START+QWERT_OFFSET_W(8),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'~',0},
        {QWERT_START+QWERT_OFFSET_W(10),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,',',0},
        {QWERT_START+QWERT_OFFSET_W(12),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'@',0},
        {QWERT_START+QWERT_OFFSET_W(14),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'!',0},
        {QWERT_START+QWERT_OFFSET_W(16),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'`',0},
        {QWERT_START+QWERT_OFFSET_W(18),QWERT_OFFSET_H(2),QWERT_BTN_W,QWERT_BTN_H,0,'\'',0},
    },
    {
        {QWERT_START+QWERT_OFFSET_W(0),QWERT_OFFSET_H(3),QWERT_BTN_W*2+KB_SPACE,QWERT_BTN_H,(uint8_t*)abcStr,KB_VALUE_QWERTY_MODE,1},
        {QWERT_START+QWERT_OFFSET_W(4),QWERT_OFFSET_H(3),QWERT_BTN_W,QWERT_BTN_H,0,'.',0},
        {QWERT_START+QWERT_OFFSET_W(6),QWERT_OFFSET_H(3),QWERT_BTN_W_SPACE*3+QWERT_BTN_W,QWERT_BTN_H,0,' ',0},
        {QWERT_START+QWERT_OFFSET_W(14),QWERT_OFFSET_H(3),QWERT_BTN_W,QWERT_BTN_H,0,'?',0},
        {QWERT_START+QWERT_OFFSET_W(16),QWERT_OFFSET_H(3),QWERT_BTN_W*2+KB_SPACE,QWERT_BTN_H,(uint8_t*)backspaceStr,0x08,1},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
    },
};

static ldColor _shiftColor[3]={GLCD_COLOR_WHITE,GLCD_COLOR_BLACK,GLCD_COLOR_BLUE};
static bool isExit=false;

static arm_2d_location_t clickPos={-1,-1};

static uint8_t _letterOffset[3]={0,32,32};

const ldBaseWidgetFunc_t ldKeyboardFunc = {
    .depose = (ldDeposeFunc_t)ldKeyboard_depose,
    .load = (ldLoadFunc_t)ldKeyboard_on_load,
    .frameStart = (ldFrameStartFunc_t)ldKeyboard_on_frame_start,
    .show = (ldShowFunc_t)ldKeyboard_show,
};

static arm_2d_region_t _keyboardGetClickRegion(ldKeyboard_t *ptWidget)
{
    arm_2d_region_t retRegion={
        .tLocation={
            .iX=0,
            .iY=0,
        },
        .tSize={
            .iWidth=LD_CFG_SCEEN_WIDTH,
            .iHeight=LD_CFG_SCEEN_HEIGHT,
        },
    };

    if(ptWidget->isNumber)
    {

        for(uint8_t y=0;y<4;y++)
        {
            for(uint8_t x=0;x<4;x++)
            {
                if((numBtnInfo[y][x].region.tLocation.iX)&&(numBtnInfo[y][x].region.tLocation.iY))
                {
                    if(arm_2d_is_point_inside_region(&numBtnInfo[y][x].region,&ptWidget->clickPoint))
                    {
                        retRegion=numBtnInfo[y][x].region;
                        clickPos.iX=x;
                        clickPos.iY=y;
                        ptWidget->kbValue=numBtnInfo[y][x].value;
                        x=4;
                        y=4;
                    }
                }
            }
        }
    }
    else
    {
        if(ptWidget->isSymbol)
        {
            for(uint8_t y=0;y<4;y++)
            {
                for(uint8_t x=0;x<10;x++)
                {
                    if((symbolBtnInfo[y][x].region.tLocation.iX)&&(symbolBtnInfo[y][x].region.tLocation.iY))
                    {
                        if(arm_2d_is_point_inside_region(&symbolBtnInfo[y][x].region,&ptWidget->clickPoint))
                        {
                            retRegion=symbolBtnInfo[y][x].region;
                            clickPos.iX=x;
                            clickPos.iY=y;
                            ptWidget->kbValue=symbolBtnInfo[y][x].value;
                            x=10;
                            y=4;
                        }
                    }
                }
            }
        }
        else
        {
            for(uint8_t y=0;y<4;y++)
            {
                for(uint8_t x=0;x<10;x++)
                {
                    if((asciiBtnInfo[y][x].region.tLocation.iX)&&(asciiBtnInfo[y][x].region.tLocation.iY))
                    {
                        if(arm_2d_is_point_inside_region(&asciiBtnInfo[y][x].region,&ptWidget->clickPoint))
                        {
                            retRegion=asciiBtnInfo[y][x].region;
                            clickPos.iX=x;
                            clickPos.iY=y;
                            ptWidget->kbValue=asciiBtnInfo[y][x].value;
                            x=10;
                            y=4;
                        }
                    }
                }
            }
        }

    }
    if(ptWidget->kbValue<=sizeof (kbValueSpecial))
    {
        retRegion.tLocation.iX=0;
        retRegion.tLocation.iY=LD_CFG_SCEEN_HEIGHT>>1;
        retRegion.tSize.iWidth=LD_CFG_SCEEN_WIDTH;
        retRegion.tSize.iHeight=LD_CFG_SCEEN_HEIGHT>>1;
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
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
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
            _addAscii(ptWidget,textLen,ascii-_letterOffset[ptWidget->upperState],true);
        }
        else
        {

        }
    }

    switch (ascii) {
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

static bool slotKBProcess(ld_scene_t *ptScene,ldMsg_t msg)
{
    ldKeyboard_t *ptWidget=msg.ptSender;

//    arm_2d_region_t kbRegion={
//        .tLocation={
//            .iX=0,
//            .iY=LD_CFG_SCEEN_HEIGHT>>1,
//        },
//        .tSize={
//            .iWidth=LD_CFG_SCEEN_WIDTH,
//            .iHeight=LD_CFG_SCEEN_HEIGHT>>1,
//        },
//    };

//    kbRegion.tLocation.iY+=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;

    switch (msg.signal)
    {
    case SIGNAL_PRESS:
    {
//        arm_2d_location_t parentPos={0};
//        parentPos=ldBaseGetAbsoluteLocation(ptWidget,parentPos);
//        ldPoint_t parentPos=ldBaseGetGlobalPos(ptWidget->parentWidget);
        ptWidget->clickPoint.iX=(int16_t)GET_SIGNAL_VALUE_X(msg.value);
        ptWidget->clickPoint.iY=(int16_t)GET_SIGNAL_VALUE_Y(msg.value);
//        ptWidget->clickPoint.iX-=(kbRegion.tLocation.iX+parentPos.iX);
//        ptWidget->clickPoint.iY-=(kbRegion.tLocation.iY+parentPos.iY);
//        LOG_LOCATION("kb",ptWidget->clickPoint);
//        LOG_REGION("kb",ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion);
//        ptWidget->clickPoint.iY-=LD_CFG_SCEEN_HEIGHT>>1;
//        LOG_LOCATION("",ptWidget->clickPoint);
        ptWidget->isClick=false;
        ptWidget->use_as__ldBase_t.tTempRegion =_keyboardGetClickRegion(ptWidget);
        ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iY+=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        break;
    }
    case SIGNAL_RELEASE:
    {
        ptWidget->use_as__ldBase_t.tTempRegion =_keyboardGetClickRegion(ptWidget);
        ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iY+=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
        ptWidget->isClick=true;
        ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        ptWidget->clickPoint.iX=-1;
        ptWidget->clickPoint.iY=-1;
        clickPos.iX=-1;
        clickPos.iY=-1;

        switch (ptWidget->kbValue)
        {
        case KB_VALUE_NUMBER_MODE:
        {
            ptWidget->isNumber=true;
            ptWidget->isSymbol=false;
            break;
        }
        case KB_VALUE_QWERTY_MODE:
        {
            ptWidget->isNumber=false;
            ptWidget->isSymbol=false;
            break;
        }
        case KB_VALUE_SYMBOL_MODE:
        {
            ptWidget->isNumber=false;
            ptWidget->isSymbol=true;
            break;
        }
        case KB_VALUE_SHIFT:
        {
            if((!ptWidget->isNumber)&&(!ptWidget->isSymbol))
            {
                ptWidget->upperState++;
                if(ptWidget->upperState>=3)
                {
                    ptWidget->upperState=0;
                }
            }
            break;
        }
        default:
            break;
        }

        _inputAsciiProcess(ptWidget,ptWidget->kbValue);

        // process end,reset
        if((ptWidget->upperState==1)&&(ptWidget->kbValue>='a')&&(ptWidget->kbValue<='z'))
        {
            ptWidget->upperState=0;

            ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iX=0;
            ptWidget->use_as__ldBase_t.tTempRegion.tLocation.iY=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY;
            ptWidget->use_as__ldBase_t.tTempRegion.tSize.iWidth=LD_CFG_SCEEN_WIDTH;
            ptWidget->use_as__ldBase_t.tTempRegion.tSize.iHeight=LD_CFG_SCEEN_HEIGHT>>1;
        }

        ptWidget->kbValue=KB_VALUE_NONE;

    }
    default:
        break;
    }
    return true;
}

ldKeyboard_t* ldKeyboard_init( ld_scene_t *ptScene,ldKeyboard_t *ptWidget, uint16_t nameId, uint16_t parentNameId,arm_2d_font_t *ptFont)
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

    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = 0;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = LD_CFG_SCEEN_HEIGHT;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = LD_CFG_SCEEN_WIDTH;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = LD_CFG_SCEEN_HEIGHT;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeKeyboard;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldKeyboardFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->isNumber=false;
    ptWidget->ptFont=ptFont;
    ptWidget->clickPoint.iX=-1;
    ptWidget->clickPoint.iY=-1;
    ptWidget->isClick=false;
    ptWidget->upperState=0;
    ptWidget->use_as__ldBase_t.tTempRegion=ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;
    ptWidget->isWaitInit=true;
    ptWidget->isSymbol=false;
    ptWidget->use_as__ldBase_t.isHidden=true;

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotKBProcess);
    ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotKBProcess);

    LOG_INFO("[init][keyboard] id:%d", nameId);
    return ptWidget;
}

void ldKeyboard_depose( ldKeyboard_t *ptWidget)
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
    ldBaseNodeRemove(ptWidget->ptScene->ptNodeRoot,(arm_2d_control_node_t*)ptWidget);

    ldFree(ptWidget);
}

void ldKeyboard_on_load( ldKeyboard_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldKeyboard_on_frame_start( ldKeyboard_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

static void _ldkeyboardNewButton(ldKeyboard_t *ptWidget,arm_2d_tile_t *parentTile,arm_2d_region_t *pBtnRegion,uint8_t *pStr,ldColor btnColor,ldColor charColor,bool bIsNewFrame)
{
//    arm_2d_tile_t btnTile;
    draw_round_corner_box(parentTile,pBtnRegion,btnColor,255,bIsNewFrame);
//    arm_2d_tile_generate_child(parentTile,pBtnRegion,&btnTile,false);
//    LOG_REGION("",btnTile.tRegion);
//    LOG_REGION("",*pBtnRegion);
//    ldBaseLineText(&btnTile,&ptWidget->resource,pStr,ptWidget->pFontDict,LD_ALIGN_CENTER,charColor,0,255);
    ldBaseLabel(parentTile,
                pBtnRegion,
                pStr,
                ptWidget->ptFont,
                ARM_2D_ALIGN_CENTRE,
                charColor,
                ptWidget->use_as__ldBase_t.opacity);
}

void ldKeyboard_show(ld_scene_t *ptScene, ldKeyboard_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    ldColor btnColor;
    uint8_t letterTextBuf[2]={0};
    arm_2d_region_t kbRegion={
            .tLocation={
                .iX=0,
                .iY=0,//(LD_CFG_SCEEN_HEIGHT>>1),//+ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY,
            },
            .tSize={
                .iWidth=LD_CFG_SCEEN_WIDTH,
                .iHeight=LD_CFG_SCEEN_HEIGHT,//>>1,
            },
        };

#if 0
    if (bIsNewFrame) {
        
    }
#endif
//    arm_2d_region_t globalRegion=ldBaseGetAbsoluteRegion(ptWidget);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&kbRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &kbRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            if(((ptWidget->editType==typeInt)||(ptWidget->editType==typeFloat))&&(ptWidget->isNumber==false))
            {
                ptWidget->isNumber=true;
            }
            if(isExit)
            {
                isExit=false;
                ptWidget->upperState=0;
                ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY=LD_CFG_SCEEN_HEIGHT;
                ldKeyboardSetHidden(ptWidget,true);
                emit(ptWidget->editorId,SIGNAL_EDITING_FINISHED,0);

                ldBaseBgMove(ptScene,LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT,0,0);
                ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
            }

            ldBaseColor(&tTarget,
                        &((arm_2d_region_t){0,
                                            (LD_CFG_SCEEN_HEIGHT>>1),
                                            LD_CFG_SCEEN_WIDTH,
                                            (LD_CFG_SCEEN_HEIGHT>>1)}),
                        __RGB(208,211,220),
                        ptWidget->use_as__ldBase_t.opacity);

            if(ptWidget->isNumber)
            {
                for(uint8_t y=0;y<4;y++)
                {
                    for(uint8_t x=0;x<4;x++)
                    {
                        if((numBtnInfo[y][x].region.tLocation.iX)&&(numBtnInfo[y][x].region.tLocation.iY))
                        {
                            if((clickPos.iX==x)&&(clickPos.iY==y))
                            {
                                btnColor=btnDefaultColor[numBtnInfo[y][x].colorType][1];
                            }
                            else
                            {
                                btnColor=btnDefaultColor[numBtnInfo[y][x].colorType][0];
                            }

                            _ldkeyboardNewButton(ptWidget,&tTarget,(arm_2d_region_t*)&numBtnInfo[y][x].region ,(uint8_t*)numBtnInfo[y][x].pText,btnColor,GLCD_COLOR_BLACK,bIsNewFrame);
                        }
                    }
                }
            }
            else
            {
                if(ptWidget->isSymbol)
                {
                    for(uint8_t y=0;y<4;y++)
                    {
                        for(uint8_t x=0;x<10;x++)
                        {
                            if((symbolBtnInfo[y][x].region.tLocation.iX)&&(symbolBtnInfo[y][x].region.tLocation.iY))
                            {
                                if((clickPos.iX==x)&&(clickPos.iY==y))
                                {
                                    btnColor=btnDefaultColor[symbolBtnInfo[y][x].colorType][1];
                                }
                                else
                                {
                                    btnColor=btnDefaultColor[symbolBtnInfo[y][x].colorType][0];
                                }
                                if(symbolBtnInfo[y][x].pText==0)
                                {
                                    letterTextBuf[0]=symbolBtnInfo[y][x].value;
                                    _ldkeyboardNewButton(ptWidget,&tTarget,(arm_2d_region_t*)&symbolBtnInfo[y][x].region,letterTextBuf,btnColor,GLCD_COLOR_BLACK,bIsNewFrame);
                                }
                                else
                                {
                                    _ldkeyboardNewButton(ptWidget,&tTarget,(arm_2d_region_t*)&symbolBtnInfo[y][x].region,(uint8_t*)symbolBtnInfo[y][x].pText,btnColor,GLCD_COLOR_BLACK,bIsNewFrame);
                                }
                            }
                        }
                    }
                }
                else
                {
                    for(uint8_t y=0;y<4;y++)
                    {
                        for(uint8_t x=0;x<10;x++)
                        {
                            if((asciiBtnInfo[y][x].region.tLocation.iX)&&(asciiBtnInfo[y][x].region.tLocation.iY))
                            {
                                if((clickPos.iX==x)&&(clickPos.iY==y))
                                {
                                    btnColor=btnDefaultColor[asciiBtnInfo[y][x].colorType][1];
                                }
                                else
                                {
                                    btnColor=btnDefaultColor[asciiBtnInfo[y][x].colorType][0];
                                }

                                if((asciiBtnInfo[y][x].value==KB_VALUE_SHIFT)&&(ptWidget->upperState>0))
                                {
                                    _ldkeyboardNewButton(ptWidget,&tTarget,(arm_2d_region_t*)&asciiBtnInfo[y][x].region,(uint8_t*)asciiShiftUpperStr,btnColor,_shiftColor[ptWidget->upperState],bIsNewFrame);
                                }
                                else
                                {
                                    if(asciiBtnInfo[y][x].pText==0)
                                    {
                                        letterTextBuf[0]=asciiBtnInfo[y][x].value-_letterOffset[ptWidget->upperState];
                                        _ldkeyboardNewButton(ptWidget,&tTarget,(arm_2d_region_t*)&asciiBtnInfo[y][x].region,letterTextBuf,btnColor,GLCD_COLOR_BLACK,bIsNewFrame);
                                    }
                                    else
                                    {
                                        _ldkeyboardNewButton(ptWidget,&tTarget,(arm_2d_region_t*)&asciiBtnInfo[y][x].region,(uint8_t*)asciiBtnInfo[y][x].pText,btnColor,GLCD_COLOR_BLACK,bIsNewFrame);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    arm_2d_op_wait_async(NULL);
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
