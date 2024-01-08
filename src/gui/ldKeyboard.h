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

#ifndef _LD_KEYBOARD_H_
#define _LD_KEYBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isNumber:1;
    bool isSymbol:1;
    bool isClick:1;
    uint8_t upperState:2;
    bool isWaitInit:1;
    ldFontDict_t *pFontDict;
    arm_2d_location_t clickPoint;
    arm_2d_region_t targetDirtyRegion;
    uint8_t kbValue;
}ldKeyboard_t;

ldKeyboard_t* ldKeyboardInit(uint16_t nameId, ldFontDict_t *pFontDict);
void ldKeyboardFrameUpdate(ldKeyboard_t* pWidget);
void ldKeyboardLoop(ldKeyboard_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldKeyboardDel(ldKeyboard_t *pWidget);


#define ldKeyboardSetHidden          ldBaseSetHidden
#define ldKeyboardMove               ldBaseMove

#ifdef __cplusplus
}
#endif

#endif //_LD_KEYBOARD_H_
