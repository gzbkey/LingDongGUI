/*
 * Copyright 2023-2024 Ou Jianbo (59935554@qq.com)
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

/**
 * @file    ldKeyboard.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   键盘控件，可以切换数字键盘和字母键盘
 * @signal  SIGNAL_INPUT_ASCII
 */

#include "ldKeyboard.h"
#include "ldGui.h"

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

#define KB_ASCII_RELEASE_COLOR     LD_COLOR_WHITE
#define KB_ASCII_PRESS_COLOR       __RGB(188,191,206)
#define KB_OTHER_RELEASE_COLOR     __RGB(168,176,189)
#define KB_OTHER_PRESS_COLOR       LD_COLOR_WHITE

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
#define NUM_OFFSET_H(num)    (KB_SPACE+(NUM_BTN_H+KB_SPACE)*num)

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
#define QWERT_OFFSET_H(num)    (KB_SPACE+(QWERT_BTN_H+KB_SPACE)*num)

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

static ldColor _shiftColor[3]={LD_COLOR_WHITE,LD_COLOR_BLACK,LD_COLOR_BLUE};
static bool isExit=false;

static ldPoint_t clickPos={-1,-1};

static uint8_t _letterOffset[3]={0,32,32};

void ldKeyboardDel(ldKeyboard_t *pWidget);
void ldKeyboardFrameUpdate(ldKeyboard_t* pWidget);
void ldKeyboardLoop(arm_2d_scene_t *pScene,ldKeyboard_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
const ldGuiCommonFunc_t ldKeyboardCommonFunc={
    (ldDelFunc_t)ldKeyboardDel,
    (ldLoopFunc_t)ldKeyboardLoop,
    (ldUpdateFunc_t)ldKeyboardFrameUpdate,
};

static bool _keyboardDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        //del user object

        ldFree(((ldKeyboard_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldKeyboardDel(ldKeyboard_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeKeyboard)
    {
        return;
    }

    LOG_INFO("[keyboard] del,id:%d",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _keyboardDel);
    }
}

static arm_2d_region_t _keyboardGetClickRegion(ldKeyboard_t *pWidget)
{
    arm_2d_region_t retRegion={
        .tLocation={
            .iX=0,
            .iY=0,
        },
        .tSize={
            .iWidth=LD_CFG_SCEEN_WIDTH,
            .iHeight=LD_CFG_SCEEN_HEIGHT>>1,
        },
    };

    if(pWidget->isNumber)
    {

        for(uint8_t y=0;y<4;y++)
        {
            for(uint8_t x=0;x<4;x++)
            {
                if((numBtnInfo[y][x].region.tLocation.iX)&&(numBtnInfo[y][x].region.tLocation.iY))
                {
                    if(arm_2d_is_point_inside_region(&numBtnInfo[y][x].region,&pWidget->clickPoint))
                    {
                        retRegion=numBtnInfo[y][x].region;
                        clickPos.x=x;
                        clickPos.y=y;
                        pWidget->kbValue=numBtnInfo[y][x].value;
                        x=4;
                        y=4;
                    }
                }
            }
        }
    }
    else
    {
        if(pWidget->isSymbol)
        {
            for(uint8_t y=0;y<4;y++)
            {
                for(uint8_t x=0;x<10;x++)
                {
                    if((symbolBtnInfo[y][x].region.tLocation.iX)&&(symbolBtnInfo[y][x].region.tLocation.iY))
                    {
                        if(arm_2d_is_point_inside_region(&symbolBtnInfo[y][x].region,&pWidget->clickPoint))
                        {
                            retRegion=symbolBtnInfo[y][x].region;
                            clickPos.x=x;
                            clickPos.y=y;
                            pWidget->kbValue=symbolBtnInfo[y][x].value;
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
                        if(arm_2d_is_point_inside_region(&asciiBtnInfo[y][x].region,&pWidget->clickPoint))
                        {
                            retRegion=asciiBtnInfo[y][x].region;
                            clickPos.x=x;
                            clickPos.y=y;
                            pWidget->kbValue=asciiBtnInfo[y][x].value;
                            x=10;
                            y=4;
                        }
                    }
                }
            }
        }

    }
    if(pWidget->kbValue<=sizeof (kbValueSpecial))
    {
        retRegion.tLocation.iX=0;
        retRegion.tLocation.iY=0;
        retRegion.tSize.iWidth=LD_CFG_SCEEN_WIDTH;
        retRegion.tSize.iHeight=LD_CFG_SCEEN_HEIGHT>>1;
    }
    return retRegion;
}

static bool _addAscii(ldKeyboard_t *pWidget,uint16_t textLen,uint8_t ascii,bool isBack)
{
    uint8_t *pBuf;
    if(pWidget->strMax==0)
    {
        if(textLen>0)
        {
            pBuf=ldCalloc(textLen+1);
            strcpy((char*)pBuf,(char*)(*pWidget->ppStr));
        }
        *pWidget->ppStr=ldRealloc(*pWidget->ppStr,textLen+2);
        if(textLen>0)
        {
            strcpy((char*)(*pWidget->ppStr),(char*)pBuf);
        }
    }
    if((textLen<pWidget->strMax)||(pWidget->strMax==0))
    {
        if(isBack)
        {
            xStringPushBack(*pWidget->ppStr,textLen,ascii);
        }
        else
        {
            uint8_t temp[2]={0};
            temp[0]=ascii;
            xStringInsert(*pWidget->ppStr,textLen,0,temp);
        }
        return true;
    }

    return false;
}

static void _inputAsciiProcess(ldKeyboard_t *pWidget,uint8_t ascii)
{
    uint16_t textLen;
    if((*pWidget->ppStr)!=NULL)
    {
        textLen=strlen((char*)(*pWidget->ppStr));
    }
    else
    {
        textLen=0;
    }

    if((pWidget->editType==typeInt)||(pWidget->editType==typeFloat))
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
            _addAscii(pWidget,textLen,ascii,true);
            break;
        }
        case '.':
        {
            if((pWidget->editType==typeFloat)&&(strstr((char*)(*pWidget->ppStr),".")==NULL))
            {
                _addAscii(pWidget,textLen,ascii,true);
            }
            break;
        }
        case '-':
        {
            if((*pWidget->ppStr)[0]=='-')
            {
                xStringRemove(*pWidget->ppStr,textLen,0,1);
            }
            else
            {
                _addAscii(pWidget,textLen,ascii,false);
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
            _addAscii(pWidget,textLen,ascii-_letterOffset[pWidget->upperState],true);
        }
        else
        {

        }
    }

    switch (ascii) {
    case 0x08://backspace
    {
        xStringPopBack(*pWidget->ppStr,textLen);
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

static bool slotKBProcess(xConnectInfo_t info)
{
    ldKeyboard_t *pWidget=ldBaseGetWidgetById(info.receiverId);
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    arm_2d_region_t kbRegion={
        .tLocation={
            .iX=0,
            .iY=LD_CFG_SCEEN_HEIGHT>>1,
        },
        .tSize={
            .iWidth=LD_CFG_SCEEN_WIDTH,
            .iHeight=LD_CFG_SCEEN_HEIGHT>>1,
        },
    };

    kbRegion.tLocation.iY+=pResTile->tRegion.tLocation.iY;

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        ldPoint_t parentPos=ldBaseGetGlobalPos(pWidget->parentWidget);
        pWidget->clickPoint.iX=(int16_t)GET_SIGNAL_VALUE_X(info.value);
        pWidget->clickPoint.iY=(int16_t)GET_SIGNAL_VALUE_Y(info.value);
        pWidget->clickPoint.iX-=(kbRegion.tLocation.iX+parentPos.x);
        pWidget->clickPoint.iY-=(kbRegion.tLocation.iY+parentPos.y);
        pWidget->isClick=false;
        pWidget->targetDirtyRegion=_keyboardGetClickRegion(pWidget);
        pWidget->targetDirtyRegion.tLocation.iY+=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iY;
        LOG_REGION("kb click region",pWidget->targetDirtyRegion);
        pWidget->dirtyRegionState=waitChange;
        break;
    }
    case SIGNAL_RELEASE:
    {
        pWidget->targetDirtyRegion=_keyboardGetClickRegion(pWidget);
        pWidget->targetDirtyRegion.tLocation.iY+=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iY;
        pWidget->isClick=true;
        pWidget->dirtyRegionState=waitChange;
        pWidget->clickPoint.iX=-1;
        pWidget->clickPoint.iY=-1;
        clickPos.x=-1;
        clickPos.y=-1;

        switch (pWidget->kbValue)
        {
        case KB_VALUE_NUMBER_MODE:
        {
            pWidget->isNumber=true;
            pWidget->isSymbol=false;
            break;
        }
        case KB_VALUE_QWERTY_MODE:
        {
            pWidget->isNumber=false;
            pWidget->isSymbol=false;
            break;
        }
        case KB_VALUE_SYMBOL_MODE:
        {
            pWidget->isNumber=false;
            pWidget->isSymbol=true;
            break;
        }
        case KB_VALUE_SHIFT:
        {
            if((!pWidget->isNumber)&&(!pWidget->isSymbol))
            {
                pWidget->upperState++;
                if(pWidget->upperState>=3)
                {
                    pWidget->upperState=0;
                }
            }
            break;
        }
        default:
            break;
        }

        _inputAsciiProcess(pWidget,pWidget->kbValue);

        // process end,reset
        if((pWidget->upperState==1)&&(pWidget->kbValue>='a')&&(pWidget->kbValue<='z'))
        {
            pWidget->upperState=0;

            pWidget->targetDirtyRegion.tLocation.iX=0;
            pWidget->targetDirtyRegion.tLocation.iY=((arm_2d_tile_t*)&pWidget->resource)->tRegion.tLocation.iY;
            pWidget->targetDirtyRegion.tSize.iWidth=LD_CFG_SCEEN_WIDTH;
            pWidget->targetDirtyRegion.tSize.iHeight=LD_CFG_SCEEN_HEIGHT>>1;
        }

        pWidget->kbValue=KB_VALUE_NONE;

    }
    default:
        break;
    }
    return true;
}

/**
 * @brief   键盘初始化
 * 
 * @param   nameId          目标控件指针
 * @param   pFontDict       字体指针
 * @return  ldKeyboard_t*   新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-23
 */
ldKeyboard_t *ldKeyboardInit(arm_2d_scene_t *pScene,uint16_t nameId,ldFontDict_t *pFontDict)
{
    ldKeyboard_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;

    parentInfo = ldBaseGetWidgetInfoById(0);
    pNewWidget = LD_CALLOC_WIDGET_INFO(ldKeyboard_t);
    if (pNewWidget != NULL)
    {
        pNewWidget->isParentHidden=false;
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeKeyboard;
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isHidden = true;
        tResTile=(arm_2d_tile_t*)&pNewWidget->resource;
        tResTile->tRegion.tLocation.iX=0;
        tResTile->tRegion.tLocation.iY=LD_CFG_SCEEN_HEIGHT>>1;
        tResTile->tRegion.tSize.iWidth=LD_CFG_SCEEN_WIDTH;
        tResTile->tRegion.tSize.iHeight=LD_CFG_SCEEN_HEIGHT;
        tResTile->tInfo.bIsRoot = true;
        tResTile->tInfo.bHasEnforcedColour = true;
        tResTile->tInfo.tColourInfo.chScheme = ARM_2D_COLOUR;
        tResTile->pchBuffer = (uint8_t*)LD_ADDR_NONE;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        ((arm_2d_vres_t*)tResTile)->pTarget = LD_ADDR_NONE;
        ((arm_2d_vres_t*)tResTile)->Load = &__disp_adapter0_vres_asset_loader;
        ((arm_2d_vres_t*)tResTile)->Depose = &__disp_adapter0_vres_buffer_deposer;
#endif
        pNewWidget->isNumber=false;
        pNewWidget->pFontDict=pFontDict;
        pNewWidget->clickPoint.iX=-1;
        pNewWidget->clickPoint.iY=-1;
        pNewWidget->isClick=false;
        pNewWidget->upperState=0;
        pNewWidget->targetDirtyRegion=tResTile->tRegion;
        pNewWidget->targetDirtyRegion.tLocation.iY=0;
        pNewWidget->isWaitInit=true;
        pNewWidget->isSymbol=false;
        pNewWidget->pFunc=&ldKeyboardCommonFunc;

        arm_2d_scene_player_dynamic_dirty_region_init(&pNewWidget->dirtyRegionListItem,pScene);

        xConnect(pNewWidget->nameId,SIGNAL_PRESS,pNewWidget->nameId,slotKBProcess);
        xConnect(pNewWidget->nameId,SIGNAL_RELEASE,pNewWidget->nameId,slotKBProcess);

        LOG_INFO("[keyboard] init,id:%d",nameId);
    }
    else
    {
        ldFree(pNewWidget);

        LOG_ERROR("[keyboard] init failed,id:%d",nameId);
    }

    return pNewWidget;
}

static void _ldkeyboardNewButton(ldKeyboard_t *pWidget,arm_2d_tile_t *parentTile,arm_2d_region_t *pBtnRegion,uint8_t *pStr,ldColor btnColor,ldColor charColor,bool bIsNewFrame)
{
    arm_2d_tile_t btnTile;
    draw_round_corner_box(parentTile,pBtnRegion,btnColor,255,bIsNewFrame);
    arm_2d_tile_generate_child(parentTile,pBtnRegion,&btnTile,false);
    ldBaseLineText(&btnTile,&pWidget->resource,pStr,pWidget->pFontDict,LD_ALIGN_CENTER,charColor,0,255);
}

void ldKeyboardFrameUpdate(ldKeyboard_t* pWidget)
{
    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        if(pWidget->isWaitInit)
        {
            //强制脏矩阵覆盖控件
            pWidget->targetDirtyRegion=((arm_2d_tile_t*)&pWidget->resource)->tRegion;
            pWidget->targetDirtyRegion.tLocation.iY=0;
            pWidget->isWaitInit=false;
        }
        return;
    }

    if(!pWidget->isWaitInit)
    {
        pWidget->isWaitInit=true;
    }

    arm_2d_dynamic_dirty_region_on_frame_start(&pWidget->dirtyRegionListItem,waitChange);
}

void ldKeyboardLoop(arm_2d_scene_t *pScene,ldKeyboard_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;
    ldColor btnColor;
    uint8_t letterTextBuf[2]={0};

    if (pWidget == NULL)
    {
        return;
    }

//    if((pWidget->isParentHidden)||(pWidget->isHidden))
//    {
//        return;
//    }

    if(((pWidget->editType==typeInt)||(pWidget->editType==typeFloat))&&(pWidget->isNumber==false))
    {
        pWidget->isNumber=true;
    }


    arm_2d_region_t kbRegion={
        .tLocation={
            .iX=0,
            .iY=LD_CFG_SCEEN_HEIGHT>>1,
        },
        .tSize={
            .iWidth=LD_CFG_SCEEN_WIDTH,
            .iHeight=LD_CFG_SCEEN_HEIGHT>>1,
        },
    };
    kbRegion.tLocation.iY+=pResTile->tRegion.tLocation.iY;

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&kbRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if(ldBaseDirtyRegionUpdate((ldCommon_t*)pWidget,&pWidget->targetDirtyRegion,&pWidget->dirtyRegionListItem,pWidget->dirtyRegionState))
        {
            pWidget->dirtyRegionState=none;
        }

        if((pWidget->isParentHidden)||(pWidget->isHidden))
        {
            return;
        }

        if(isExit)
        {
            isExit=false;
            pWidget->upperState=0;
            ldBaseSetHidden((ldCommon_t*)pWidget,true);
            xEmit(0,SIGNAL_EDITING_FINISHED,0);
            ldBaseBgMove(LD_CFG_SCEEN_WIDTH,LD_CFG_SCEEN_HEIGHT,0,0);
        }

        ldBaseColor(&tTarget,__RGB(208,211,220),255);

        if(pWidget->isNumber)
        {
            for(uint8_t y=0;y<4;y++)
            {
                for(uint8_t x=0;x<4;x++)
                {
                    if((numBtnInfo[y][x].region.tLocation.iX)&&(numBtnInfo[y][x].region.tLocation.iY))
                    {
                        if((clickPos.x==x)&&(clickPos.y==y))
                        {
                            btnColor=btnDefaultColor[numBtnInfo[y][x].colorType][1];
                        }
                        else
                        {
                            btnColor=btnDefaultColor[numBtnInfo[y][x].colorType][0];
                        }

                        _ldkeyboardNewButton(pWidget,&tTarget,(arm_2d_region_t*)&numBtnInfo[y][x].region ,(uint8_t*)numBtnInfo[y][x].pText,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                    }
                }
            }
        }
        else
        {
            if(pWidget->isSymbol)
            {
                for(uint8_t y=0;y<4;y++)
                {
                    for(uint8_t x=0;x<10;x++)
                    {
                        if((symbolBtnInfo[y][x].region.tLocation.iX)&&(symbolBtnInfo[y][x].region.tLocation.iY))
                        {
                            if((clickPos.x==x)&&(clickPos.y==y))
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
                                _ldkeyboardNewButton(pWidget,&tTarget,(arm_2d_region_t*)&symbolBtnInfo[y][x].region,letterTextBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                            }
                            else
                            {
                                _ldkeyboardNewButton(pWidget,&tTarget,(arm_2d_region_t*)&symbolBtnInfo[y][x].region,(uint8_t*)symbolBtnInfo[y][x].pText,btnColor,LD_COLOR_BLACK,bIsNewFrame);
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
                            if((clickPos.x==x)&&(clickPos.y==y))
                            {
                                btnColor=btnDefaultColor[asciiBtnInfo[y][x].colorType][1];
                            }
                            else
                            {
                                btnColor=btnDefaultColor[asciiBtnInfo[y][x].colorType][0];
                            }

                            if((asciiBtnInfo[y][x].value==KB_VALUE_SHIFT)&&(pWidget->upperState>0))
                            {
                                _ldkeyboardNewButton(pWidget,&tTarget,(arm_2d_region_t*)&asciiBtnInfo[y][x].region,(uint8_t*)asciiShiftUpperStr,btnColor,_shiftColor[pWidget->upperState],bIsNewFrame);
                            }
                            else
                            {
                                if(asciiBtnInfo[y][x].pText==0)
                                {
                                    letterTextBuf[0]=asciiBtnInfo[y][x].value-_letterOffset[pWidget->upperState];
                                    _ldkeyboardNewButton(pWidget,&tTarget,(arm_2d_region_t*)&asciiBtnInfo[y][x].region,letterTextBuf,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                                }
                                else
                                {
                                    _ldkeyboardNewButton(pWidget,&tTarget,(arm_2d_region_t*)&asciiBtnInfo[y][x].region,(uint8_t*)asciiBtnInfo[y][x].pText,btnColor,LD_COLOR_BLACK,bIsNewFrame);
                                }
                            }
                        }
                    }
                }
            }
        }
        arm_2d_op_wait_async(NULL);
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
