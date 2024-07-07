/*
 * Copyright (c) 2021-2024 Ou Jianbo (59935554@qq.com). All rights reserved.
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

/**
 * @file    xString.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   字符串处理
 */
#include "xString.h"

//反转字符串
static char *reverse(char *s)
{
    char temp;
    char *p = s;    //p指向s的头部
    char *q = s;    //q指向s的尾部
    while(*q)
        ++q;
    q--;

    //交换移动指针，直到p和q交叉
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}

static char *xItoa(int value, char *string,int radix)
{
    int i = 0,isNegative = 0;
    if((isNegative = value) < 0) //如果是负数，先转为正数
    {
        value = -value;
    }
    do      //从个位开始变为字符，直到最高位，最后应该反转
    {
        string[i++] = (char)(value%radix + '0');
        value = value/radix;
    }while(value > 0);

    if(isNegative < 0)   //如果是负数，补上负号
    {
        string[i++] = '-';
    }
    string[i] = '\0';    //最后加上字符串结束符
    return reverse(string);
}

int32_t xStrToI32(uint8_t * str)
{
    return atoi((const char*)str);
}

uint8_t* xI32ToStr(int32_t inInt32,uint8_t *buf)
{
    return (uint8_t*)xItoa(inInt32,(char*)buf,10);
//    sprintf((char*)buf,"%d",inInt32);
//    return buf;
}

float xStrToFloat(uint8_t * str)
{
    return (float)atof((const char*)str);
}

uint8_t* xFloatToStr(float inFloat,uint8_t afterDecimalPointNum,uint8_t *buf)
{
    uint8_t len,i,j;
    bool pointFlag=false;
    
    sprintf((char*)buf,"%f",(double)inFloat);
    
    len=(uint8_t)strlen((const char*)buf);

    j=0;
    for(i=0;i<len;i++)
    {
        if(pointFlag==true)
        {
            j++;
            if(j>afterDecimalPointNum)
            {
                buf[i]=0;
            }
        }
        if(buf[i]=='.')
        {
            pointFlag=true;
        }
    }

    len=(uint8_t)strlen((const char*)buf);

    if(pointFlag==true)
    {
        for(i=(len-1);i>0;i--)
        {
            if(buf[i]=='0')
            {
                buf[i]=0;
            }
            else
            {
                if(buf[i]=='.')
                {
                    buf[i]=0;
                }
                break;
            }
        }
    }
    return buf;
}


//在字符串末尾添加一个字符
void xStringPushBack(uint8_t *pStr,uint16_t strLen,uint8_t ch)
{
    if(strLen==0)
    {
        strLen=strlen((char*)pStr);
    }
    pStr[strLen]=ch;
    pStr[strLen+1]=0;
}

//删除字符串末尾的字符。
void xStringPopBack(uint8_t *pStr,uint16_t strLen)
{
    if(strLen==0)
    {
        strLen=strlen((char*)pStr);
    }
    if(strLen>0)
    {
        pStr[strLen-1]=0;
    }
}

void xStringInsert(uint8_t *pStr,uint16_t strLen,uint16_t pos,uint8_t *insertStr)
{
    uint16_t insertLen;
    if(strLen==0)
    {
        strLen=strlen((char*)pStr);
    }
    insertLen=strlen((char*)insertStr);
    for (uint16_t i = strLen-1; i >= pos; i--)
    {
        pStr[i+insertLen] = pStr[i];
    }
    memcpy(pStr+pos,insertStr,insertLen);
    pStr[strLen+insertLen] = 0;
}

void xStringRemove(uint8_t *pStr,uint16_t strLen,uint16_t pos,uint16_t rmLen)
{
    uint16_t newLen;
    if(strLen==0)
    {
        strLen=strlen((char*)pStr);
    }
    newLen=strLen-rmLen;
    for (uint16_t i = pos; i <newLen; i++)
    {
        pStr[i] = pStr[rmLen+i];
    }
    pStr[newLen]=0;
}
