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
 * @file    lineEdit.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   输入框控件
 * @version 0.1
 * @date    2023-11-24
 */

#include "ldLineEdit.h"
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

static bool _lineEditDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        ldFree(((ldLineEdit_t *)pTarget)->pText);
        ldFree(((ldLineEdit_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldLineEditDel(ldLineEdit_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeLineEdit)
    {
        return;
    }

    LOG_INFO("[lineEdit] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _lineEditDel);
    }
}

static bool _addAscii(ldLineEdit_t *pWidget,uint8_t ascii)
{
    if(pWidget->textLen<pWidget->textMax)
    {
        pWidget->pText[pWidget->textLen]=ascii;
        pWidget->pText[pWidget->textLen+1]=' ';
        pWidget->pText[pWidget->textLen+2]=0;
        pWidget->textLen++;
        return true;
    }
    return false;
}

static void _inputAsciiProcess(ldLineEdit_t *pWidget,uint8_t ascii)
{
    if((pWidget->editType==typeInt)||(pWidget->editType==typeFloat))
    {
        if((ascii>='0')&&(ascii<='9'))
        {
            _addAscii(pWidget,ascii);
        }
        if((pWidget->editType==typeFloat)&&(ascii=='.')&&(pWidget->hasFloatPoint==false))
        {
            if(_addAscii(pWidget,ascii))
            {
                pWidget->hasFloatPoint=true;
            }
        }

        if(ascii=='-')
        {
            if(pWidget->pText[0]=='-')
            {

                for (uint8_t i = 1; i <pWidget->textLen; i++)
                {
                    pWidget->pText[i-1] = pWidget->pText[i];
                }
                pWidget->pText[pWidget->textLen-1]=' ';
                pWidget->pText[pWidget->textLen]=0;
                pWidget->textLen--;

            }
            else
            {
                if(pWidget->textLen<pWidget->textMax)
                {

                    for (uint8_t i = pWidget->textLen ; i >0; i--)
                    {
                        pWidget->pText[i] = pWidget->pText[i-1];
                    }
                    pWidget->pText[pWidget->textLen+1]=' ';
                    pWidget->pText[pWidget->textLen+2]=0;
                    pWidget->textLen++;

                    pWidget->pText[0]='-';
                }
            }
        }
    }
    else
    {
        if((ascii>=0x20)&&(ascii<0x7F))
        {
            _addAscii(pWidget,ascii);
        }
        else
        {

        }
    }

    switch (ascii) {
    case 0x08://backspace
    {
        if(pWidget->textLen>0)
        {
            if((pWidget->pText[pWidget->textLen-1]=='.')&&(pWidget->editType==typeFloat))
            {
                pWidget->hasFloatPoint=false;
            }
            pWidget->pText[pWidget->textLen-1]=' ';
            pWidget->pText[pWidget->textLen]=0;
            pWidget->textLen--;
        }
        break;
    }
    case 0x0d://enter
    {
        ldBaseSetHidden(ldGetWidgetById(pWidget->kbNameId),true);
        pWidget->isEditing=false;
        xEmit(pWidget->nameId,SIGNAL_EDITING_FINISHED,0);
        break;
    }
    default:
        break;
    }
}

static bool slotLineEditProcess(xConnectInfo_t info)
{
    ldLineEdit_t *pWidget;

    pWidget=ldGetWidgetById(info.receiverId);

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        pWidget->isEditing=true;
        pWidget->pText[pWidget->textLen]=' ';
        gActiveEditType=pWidget->editType;
        ldBaseSetHidden(ldGetWidgetById(pWidget->kbNameId),false);
        break;
    }
    case SIGNAL_INPUT_ASCII:
    {
        _inputAsciiProcess(pWidget,info.value);
        break;
    }
    default:
        break;
    }
    return false;
}

/**
 * @brief   输入框控件初始化
 * 
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @param   pFontDict       字体指针
 * @param   textMax         字符串最大长度 0-255
 * @return  ldLineEdit_t*   新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-24
 */
ldLineEdit_t *ldLineEditInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t *pFontDict,uint8_t textMax)
{
    ldLineEdit_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    uint8_t *pText = NULL;

    parentInfo = ldGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldLineEdit_t);
    pText = (uint8_t *)ldMalloc((textMax+2)*sizeof(uint8_t));//光标位置+结尾
    if ((pNewWidget != NULL)&&(pText!=NULL))
    {
        pNewWidget->isParentHidden=false;
        
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeLineEdit;
        xListInfoAdd(parentList, pNewWidget);
        pNewWidget->parentWidget = parentInfo->info;
        pNewWidget->isHidden = false;

        tResTile=(arm_2d_tile_t*)&pNewWidget->resource;
        tResTile->tRegion.tLocation.iX=x;
        tResTile->tRegion.tLocation.iY=y;
        tResTile->tRegion.tSize.iWidth=width;
        tResTile->tRegion.tSize.iHeight=height;
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

        strset((char*)pText,0);
        pNewWidget->pText=pText;
        pNewWidget->textMax=textMax;
        pNewWidget->pFontDict=pFontDict;
        pNewWidget->isCorner=false;
        pNewWidget->isEditing=false;
        pNewWidget->textColor=GLCD_COLOR_BLACK;
        pNewWidget->textLen=0;
        pNewWidget->timer=0;
        pNewWidget->blinkFlag=false;
        pNewWidget->editType=typeString;
        pNewWidget->hasFloatPoint=false;

        xConnect(nameId,SIGNAL_PRESS,nameId,slotLineEditProcess);
//        xConnect(nameId,SIGNAL_RELEASE,nameId,slotLineEditProcess);

        LOG_INFO("[lineEdit] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pText);
        ldFree(pNewWidget);

        LOG_ERROR("[lineEdit] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

/**
 * @brief   输入框显示处理
 * 
 * @param   pWidget         目标控件指针
 * @param   pParentTile     父控件tile对象
 * @param   bIsNewFrame     新的一帧开始标志
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-24
 */
void ldLineEditLoop(ldLineEdit_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
{
    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        return;
    }

    if(ldTimeOut(500,&pWidget->timer,true)&&bIsNewFrame&&pWidget->isEditing)
    {
        pWidget->blinkFlag=!pWidget->blinkFlag;
    }

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if(pWidget->isCorner)
        {
            draw_round_corner_box(&tTarget,&tTarget_canvas,GLCD_COLOR_WHITE,255,bIsNewFrame);
            draw_round_corner_border(&tTarget,&tTarget_canvas,GLCD_COLOR_LIGHT_GREY,(arm_2d_border_opacity_t){255,255,255,255},(arm_2d_corner_opacity_t){255,255,255,255});
        }
        else
        {
            ldBaseColor(&tTarget,GLCD_COLOR_WHITE,255);
            arm_2d_draw_box(&tTarget,&tTarget_canvas,1,GLCD_COLOR_LIGHT_GREY,255);
        }
        arm_2d_op_wait_async(NULL);

        arm_2d_tile_t textTile;
        arm_2d_region_t tempRegion={
            .tLocation={
                .iX=tTarget_canvas.tLocation.iX+2,
                .iY=tTarget_canvas.tLocation.iY+2,
            },
            .tSize={
                .iWidth=tTarget_canvas.tSize.iWidth-4,
                .iHeight=tTarget_canvas.tSize.iHeight-4,
            },
        };



        arm_2d_tile_generate_child(&tTarget,&tempRegion,&textTile,false);
        arm_2d_region_t showRegion=ldBaseLineText(&textTile,pResTile,pWidget->pText,pWidget->pFontDict,LD_ALIGN_LEFT_AUTO,pWidget->textColor,0,255);



        if((pWidget->blinkFlag)&&(pWidget->isEditing))
        {
            showRegion.tLocation.iX=showRegion.tLocation.iX+showRegion.tSize.iWidth-2;
            showRegion.tSize.iWidth=2;
            arm_2d_draw_box(&tTarget,&showRegion,1,0,255);
        }



        arm_2d_op_wait_async(NULL);
    }
}

/**
 * @brief   设置显示文本
 *
 * @param   pWidget         目标控件指针
 * @param   pText           字符串指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-24
 */
void ldLineEditSetText(ldLineEdit_t* pWidget,uint8_t *pText)
{
    if(pWidget==NULL)
    {
        return;
    }

    int16_t len=strlen((char*)pText);
    if(len>pWidget->textMax)
    {
        len=pWidget->textMax;
    }
    memcpy((char*)pWidget->pText,(char*)pText,len);
    pWidget->pText[len]=0;
    pWidget->pText[len+1]=0;
    pWidget->textLen=len;
}

/**
 * @brief   关联键盘
 * 
 * @param   pWidget         目标控件指针
 * @param   kbNameId        目标键盘id
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-24
 */
void ldLineEditSetKeyboard(ldLineEdit_t* pWidget,uint16_t kbNameId)
{
    if(pWidget==NULL)
    {
        return;
    }
    pWidget->kbNameId=kbNameId;
    xDisconnect(kbNameId,SIGNAL_INPUT_ASCII,pWidget->nameId,slotLineEditProcess);
    xConnect(kbNameId,SIGNAL_INPUT_ASCII,pWidget->nameId,slotLineEditProcess);
}

/**
 * @brief   设置输入框的数据类型
 * 
 * @param   pWidget         目标控件指针
 * @param   editType        typeString、typeInt、typeFloat
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-11-24
 */
void ldLineEditSetType(ldLineEdit_t* pWidget,ldEditType_t editType)
{
    if(pWidget==NULL)
    {
        return;
    }

    pWidget->editType=editType;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
