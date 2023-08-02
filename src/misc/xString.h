/*
 * Copyright 2021-2023 Ou Jianbo 59935554@qq.com
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

#ifndef _X_STRING_H_
#define _X_STRING_H_


#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int32_t xStrToI32(uint8_t * str);
uint8_t* xI32ToStr(int32_t inInt32,uint8_t *buf);

float xStrToFloat(uint8_t * str);
uint8_t* xFloatToStr(float inFloat,uint8_t afterDecimalPointNum,uint8_t *buf);




#endif //_X_STRING_H_
