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

#define __LD__RADIAL_MENU_IMPLEMENT__

#include "./arm_extra_controls.h"
#include "./__common.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include <assert.h>
#include <string.h>
#include "arm_math.h"

#include "ldRadialMenu.h"

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

#define ITEM_0_ANGLE_OFFSET      90
#define MOVE_CYCLE_MS            0
#define SKIP_ANGLE               5

const ldBaseWidgetFunc_t ldRadialMenuFunc = {
    .depose = (ldDeposeFunc_t)ldRadialMenu_depose,
    .load = (ldLoadFunc_t)ldRadialMenu_on_load,
    .frameStart = (ldFrameStartFunc_t)ldRadialMenu_on_frame_start,
    .show = (ldShowFunc_t)ldRadialMenu_show,
};

static bool slotMenuSelect(ld_scene_t *ptScene,ldMsg_t msg)
{
    int16_t x,y;
    ldRadialMenu_t *ptWidget=msg.ptSender;

    static arm_2d_location_t tClickLocal;

    switch (msg.signal)
    {
    case SIGNAL_PRESS:
    {
        tClickLocal.iX=(int16_t)GET_SIGNAL_VALUE_X(msg.value);
        tClickLocal.iY=(int16_t)GET_SIGNAL_VALUE_Y(msg.value);

        tClickLocal=ldBaseGetRelativeLocation(ptWidget,tClickLocal);
        break;
    }
    case SIGNAL_HOLD_DOWN:
    {
        break;
    }
    case SIGNAL_RELEASE:
    {
        //将移动速度强制转换成item数量，并且限制360度内
        do{
            float preAngle;
            int8_t offsetItem;
            x=(int16_t)GET_SIGNAL_SPEED_X(msg.value);
            preAngle=360.0/ptWidget->use_as__ldBase_t.itemCount;

            if((x>=preAngle)||(x<=(-preAngle)))
            {
                if(x>=360)
                {
                    x=359;
                }
                else
                {
                    if(x<=-360)
                    {
                        x=-359;
                    }
                }
                offsetItem=x/preAngle;
                ptWidget->offsetAngle=offsetItem*preAngle;
                if(offsetItem>0)
                {
                    if((ptWidget->selectItem-offsetItem)>=0)
                    {
                        ptWidget->targetItem=ptWidget->selectItem-offsetItem;
                    }
                    else
                    {
                        ptWidget->targetItem=ptWidget->use_as__ldBase_t.itemCount+(ptWidget->selectItem-offsetItem);
                    }
                }
                else
                {
                    if((ptWidget->selectItem-offsetItem)<ptWidget->use_as__ldBase_t.itemCount)
                    {
                        ptWidget->targetItem=ptWidget->selectItem-offsetItem;
                    }
                    else
                    {
                        ptWidget->targetItem=-offsetItem-(ptWidget->use_as__ldBase_t.itemCount-ptWidget->selectItem);
                    }
                }
            }
        }while(0);

        if((ptWidget->isMove==false)&&(ptWidget->offsetAngle==0))//只允许静止状态下选择
        {
            arm_2d_location_t tReleaseLoc;

            tReleaseLoc.iX=(int16_t)GET_SIGNAL_VALUE_X(msg.value);
            tReleaseLoc.iY=(int16_t)GET_SIGNAL_VALUE_Y(msg.value);

            tReleaseLoc=ldBaseGetRelativeLocation(ptWidget,tReleaseLoc);

//            ldRadialMenuItem_t *ptItemList=(ldRadialMenuItem_t *)ptWidget->use_as__ldBase_t.ptItemList;

            for(int8_t i=ptWidget->use_as__ldBase_t.itemCount-1;i>=0;i--)
            {
                if(((tClickLocal.iX>ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iX)&&
                    (tClickLocal.iX<(ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iX+ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tSize.iWidth-1))&&
                    (tClickLocal.iY>ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iY)&&
                    (tClickLocal.iY<(ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iY+ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tSize.iHeight-1)))&&
                   ((tReleaseLoc.iX>ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iX)&&
                    (tReleaseLoc.iX<(ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iX+ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tSize.iWidth-1))&&
                    (tReleaseLoc.iY>ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iY)&&
                    (tReleaseLoc.iY<(ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iY+ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tSize.iHeight-1))))
                {
                    ldRadialMenuSelectItem(ptWidget,ptWidget->pShowList[i]);
                    LOG_DEBUG("click item %d",ptWidget->pShowList[i]);
                    break;
                }
            }
        }
        break;
    }
    default:break;
    }

    return false;
}

ldRadialMenu_t* ldRadialMenu_init( ld_scene_t *ptScene,ldRadialMenu_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint16_t xAxis, uint16_t yAxis, uint8_t itemMax)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    ldRadialMenuItem_t *ptItemInfoList;
    ldBaseItemRegion_t *ptItemRegionList;
    uint8_t *pShowList;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldRadialMenu_t));
        ptItemInfoList=ldCalloc(1,sizeof(ldRadialMenuItem_t)*itemMax);
        ptItemRegionList=ldCalloc(1,sizeof(ldBaseItemRegion_t)*itemMax);
        pShowList=ldCalloc(1,sizeof (uint8_t)*itemMax);
        if ((NULL == ptWidget)||(NULL == ptItemInfoList)||(NULL == ptItemRegionList)||(NULL == pShowList))
        {
            ldFree(ptWidget);
            ldFree(ptItemInfoList);
            ldFree(ptItemRegionList);
            ldFree(pShowList);
            LOG_ERROR("[init failed][radialMenu] id:%d", nameId);
            return NULL;
        }
    }

    ptParent = ldBaseGetWidget(ptScene->ptNodeRoot,parentNameId);
    ldBaseNodeAdd((arm_2d_control_node_t *)ptParent, (arm_2d_control_node_t *)ptWidget);

    ptWidget->ptScene=ptScene;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iX = x;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tLocation.iY = y;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iWidth = width;
    ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion.tSize.iHeight = height;
    ptWidget->use_as__ldBase_t.nameId = nameId;
    ptWidget->use_as__ldBase_t.widgetType = widgetTypeRadialMenu;
    ptWidget->use_as__ldBase_t.ptGuiFunc = &ldRadialMenuFunc;
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
    ptWidget->use_as__ldBase_t.isDirtyRegionAutoReset = true;
    ptWidget->use_as__ldBase_t.opacity=255;

    ptWidget->use_as__ldBase_t.ptItemRegionList=ptItemRegionList;
    ptWidget->ptItemInfoList=ptItemInfoList;

    ptWidget->itemMax=itemMax;
    ptWidget->originPos.iX=width>>1;
    ptWidget->originPos.iY=height>>1;
    ptWidget->xAxis=xAxis;
    ptWidget->yAxis=yAxis;
    ptWidget->pShowList=pShowList;

    ldMsgConnect(ptWidget,SIGNAL_PRESS,slotMenuSelect);
    ldMsgConnect(ptWidget,SIGNAL_RELEASE,slotMenuSelect);
    ldMsgConnect(ptWidget,SIGNAL_HOLD_DOWN,slotMenuSelect);

    LOG_INFO("[init][radialMenu] id:%d", nameId);
    return ptWidget;
}

void ldRadialMenu_depose( ldRadialMenu_t *ptWidget)
{
    assert(NULL != ptWidget);
    if (ptWidget == NULL)
    {
        return;
    }
    if(ptWidget->use_as__ldBase_t.widgetType!=widgetTypeRadialMenu)
    {
        return;
    }

    LOG_INFO("[depose][radialMenu] id:%d", ptWidget->use_as__ldBase_t.nameId);

    ldMsgDelConnect(ptWidget);
    ldBaseNodeRemove(ptWidget->ptScene->ptNodeRoot,(arm_2d_control_node_t*)ptWidget);
    ldFree(ptWidget->ptItemInfoList);
    ldFree(ptWidget->use_as__ldBase_t.ptItemRegionList);
    ldFree(ptWidget->pShowList);
    ldFree(ptWidget);
}

void ldRadialMenu_on_load( ldRadialMenu_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

static void _autoScalePercent(ldRadialMenu_t *ptWidget)
{
#if USE_RADIA_MENU_SCALE == 1
    for(uint8_t i=0;i<ptWidget->use_as__ldBase_t.itemCount;i++)
    {
        //计算缩放比例
        ptWidget->ptItemInfoList[i].scalePercent=(ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iY)*50/ptWidget->yAxis+50;

        if((ptWidget->ptItemInfoList[i].scalePercent%10)>=5)
        {
            ptWidget->ptItemInfoList[i].scalePercent=(ptWidget->ptItemInfoList[i].scalePercent/10+1)*10;
        }
        else
        {
            ptWidget->ptItemInfoList[i].scalePercent=(ptWidget->ptItemInfoList[i].scalePercent/10)*10;
        }
    }
#endif
}

static void _sortByYAxis(ldBaseItemRegion_t* arr, uint8_t* indexArr, int size)
{
    int i, j;
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - 1 - i; j++)
        {
            if(arr[indexArr[j]].itemRegion.tLocation.iY>arr[indexArr[j+1]].itemRegion.tLocation.iY)
            {
                int temp = indexArr[j];
                indexArr[j] = indexArr[j + 1];
                indexArr[j + 1] = temp;
            }
        }
    }
}

static void _autoSort(ldRadialMenu_t *ptWidget)
{
    ldBaseItemRegion_t *ptItemRegionList=ptWidget->use_as__ldBase_t.ptItemRegionList;
    ldRadialMenuItem_t *ptItemInfoList=ptWidget->ptItemInfoList;
    //计算坐标
    for(uint8_t i=0;i<ptWidget->use_as__ldBase_t.itemCount;i++)
    {
        ptItemRegionList[i].itemRegion.tLocation.iX  = ptWidget->originPos.iX - (arm_cos_f32(ANGLE_2_RADIAN(ptItemInfoList[i].angle+ptWidget->nowAngle)) * (ptWidget->xAxis>>1))-(ptItemRegionList[i].itemRegion.tSize.iWidth>>1);
        ptItemRegionList[i].itemRegion.tLocation.iY  = ptWidget->originPos.iY + (arm_sin_f32(ANGLE_2_RADIAN(ptItemInfoList[i].angle+ptWidget->nowAngle)) * (ptWidget->yAxis>>1))-(ptItemRegionList[i].itemRegion.tSize.iHeight>>1);
#if USE_RADIA_MENU_SCALE == 1
        //计算缩放比例
        ptItemInfoList[i].scalePercent=(ptItemRegionList[ptWidget->pShowList[i]].itemRegion.tLocation.iY)*50/ptWidget->yAxis+50;
#endif
    }

    //计算排序
    for (int i = 0; i < ptWidget->itemMax; i++)
    {
        ptWidget->pShowList[i] = i;
    }
    _sortByYAxis(ptItemRegionList, ptWidget->pShowList, ptWidget->use_as__ldBase_t.itemCount);

    for (int i = 0; i < ptWidget->use_as__ldBase_t.itemCount; i++)
    {
        if((ptItemRegionList[i].tTempItemRegion.tLocation.iX==0)&&
                (ptItemRegionList[i].tTempItemRegion.tLocation.iY==0)&&
                (ptItemRegionList[i].tTempItemRegion.tSize.iWidth==0)&&
                (ptItemRegionList[i].tTempItemRegion.tSize.iHeight==0))
        {
            ptItemRegionList[i].tTempItemRegion=ptItemRegionList[i].itemRegion;
        }
        else
        {
            arm_2d_region_get_minimal_enclosure(&ptItemRegionList[i].tTempItemRegion,
                                                &ptItemRegionList[i].itemRegion,
                                                &ptItemRegionList[i].tTempItemRegion);
        }
    }

}

void ldRadialMenu_on_frame_start( ldRadialMenu_t *ptWidget)
{
    assert(NULL != ptWidget);

}

void ldRadialMenu_show(ld_scene_t *ptScene, ldRadialMenu_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

    if(ptWidget->use_as__ldBase_t.itemCount&&bIsNewFrame)
    {
#if MOVE_CYCLE_MS == 0
        if(ptWidget->offsetAngle!=0)
#else
        bool isTimeOut=false;
        if(ldTimeOut(MOVE_CYCLE_MS,&ptWidget->timer,true))
        {
            isTimeOut=true;
        }
        if((bIsNewFrame)&&(isTimeOut)&&(ptWidget->offsetAngle!=0))
#endif
        {
            if(ptWidget->offsetAngle>0)
            {
#if SKIP_ANGLE == 0
                ptWidget->nowAngle++;
                ptWidget->offsetAngle--;
#else
                if(ptWidget->offsetAngle<SKIP_ANGLE)
                {
                    ptWidget->nowAngle=ITEM_0_ANGLE_OFFSET-ptWidget->ptItemInfoList[ptWidget->targetItem].angle;
                    ptWidget->offsetAngle=0;
                }
                else
                {
                    ptWidget->nowAngle+=SKIP_ANGLE;
                    ptWidget->offsetAngle-=SKIP_ANGLE;
                }
#endif
            }
            else
            {
#if SKIP_ANGLE == 0
                ptWidget->nowAngle--;
                ptWidget->offsetAngle++;
#else
                if(ptWidget->offsetAngle>(-SKIP_ANGLE))
                {
                    ptWidget->nowAngle=ITEM_0_ANGLE_OFFSET-ptWidget->ptItemInfoList[ptWidget->targetItem].angle+360;
                    ptWidget->offsetAngle++;
                }
                else
                {
                    ptWidget->nowAngle-=SKIP_ANGLE;
                    ptWidget->offsetAngle+=SKIP_ANGLE;
                }
#endif
            }

            _autoSort(ptWidget);

            //旋转结束
            if((ptWidget->offsetAngle==0)&&((ptWidget->selectItem!=ptWidget->targetItem)))
            {
                ptWidget->selectItem=ptWidget->targetItem;
                ptWidget->nowAngle%=360;
                _autoScalePercent(ptWidget);
            }

            ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
        }
    }

    arm_2d_region_t globalRegion=ldBaseGetAbsoluteRegion(ptWidget);

    if(arm_2d_helper_pfb_is_region_active(ptTile,&globalRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &globalRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }

            if(ptWidget->use_as__ldBase_t.itemCount)
            {
                //刷新item
                for(uint8_t i=0;i<ptWidget->use_as__ldBase_t.itemCount;i++)
                {
                    do {
                        arm_2d_tile_t tChildTile;
                        arm_2d_tile_generate_child(&tTarget, &ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->pShowList[i]].itemRegion, &tChildTile, false);

#if USE_RADIA_MENU_SCALE == 1
                        if(ptWidget->ptItemInfoList[i].scalePercent>=100)
                        {
                            ldBaseImage(&tChildTile,NULL,ptWidget->ptItemInfoList[ptWidget->pShowList[i]].ptImgTile,ptWidget->ptItemInfoList[ptWidget->pShowList[i]].ptMaskTile,0,ptWidget->use_as__ldBase_t.opacity);
                        }
                        else
                        {
                            ldBaseImageScale(&tChildTile,NULL,(ptWidget->ptItemInfoList[ptWidget->pShowList[i]]).ptImgTile,(ptWidget->ptItemInfoList[ptWidget->pShowList[i]]).ptMaskTile,(ptWidget->ptItemInfoList[i]).scalePercent/100.0,&(ptWidget->ptItemInfoList[i]).op,bIsNewFrame);
                        }
#else
                        ldBaseImage(&tChildTile,NULL,ptWidget->ptItemInfoList[ptWidget->pShowList[i]].ptImgTile,ptWidget->ptItemInfoList[ptWidget->pShowList[i]].ptMaskTile,0,ptWidget->use_as__ldBase_t.opacity);
#endif
                        arm_2d_op_wait_async(NULL);
                    } while (0);
                }
            }

        }
    }

    arm_2d_op_wait_async(NULL);
}

void ldRadialMenuAddItem(ldRadialMenu_t *ptWidget, arm_2d_tile_t *ptImgTile, arm_2d_tile_t *ptMaskTile)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }
    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;

    if(ptWidget->use_as__ldBase_t.itemCount<ptWidget->itemMax)
    {
        ptWidget->ptItemInfoList[ptWidget->use_as__ldBase_t.itemCount].ptImgTile=ptImgTile;
        ptWidget->ptItemInfoList[ptWidget->use_as__ldBase_t.itemCount].ptMaskTile=ptMaskTile;
        ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->use_as__ldBase_t.itemCount].itemRegion=ptImgTile->tRegion;

//        ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->use_as__ldBase_t.itemCount].tTempItemRegion=ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->use_as__ldBase_t.itemCount].itemRegion;

        ptWidget->use_as__ldBase_t.itemCount++;

        float preAngle=360.0/ptWidget->use_as__ldBase_t.itemCount;

        for(uint8_t i=0;i<ptWidget->use_as__ldBase_t.itemCount;i++)
        {
            ptWidget->ptItemInfoList[i].angle=preAngle*i+ITEM_0_ANGLE_OFFSET;
        }

        //初始化数据
        _autoSort(ptWidget);
        _autoScalePercent(ptWidget);
    }
}

void ldRadialMenuSelectItem(ldRadialMenu_t *ptWidget,uint8_t num)
{
    if(ptWidget==NULL)
    {
        return;
    }

    if(ptWidget->selectItem==num)
    {
        return;
    }

    arm_2d_region_t *ptRegion=&ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion;

    num=num%ptWidget->use_as__ldBase_t.itemCount;

    ptWidget->targetItem=num;

//    ldRadialMenuItem_t *ptItemList=(ldRadialMenuItem_t *)ptWidget->use_as__ldBase_t.ptItemList;

    if((ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->targetItem].itemRegion.tLocation.iX+(ptWidget->use_as__ldBase_t.ptItemRegionList[ptWidget->targetItem].itemRegion.tSize.iWidth>>1))<(ptRegion->tSize.iWidth>>1))// left
    {
        if(ptWidget->targetItem<ptWidget->selectItem)
        {
            ptWidget->offsetAngle=ptWidget->ptItemInfoList[ptWidget->selectItem].angle-ptWidget->ptItemInfoList[ptWidget->targetItem].angle;
        }
        else
        {
            ptWidget->offsetAngle=360-ptWidget->ptItemInfoList[ptWidget->targetItem].angle+ptWidget->ptItemInfoList[ptWidget->selectItem].angle;
        }
    }
    else// right
    {
        if(ptWidget->targetItem>ptWidget->selectItem)
        {
            ptWidget->offsetAngle=ptWidget->ptItemInfoList[ptWidget->targetItem].angle-ptWidget->ptItemInfoList[ptWidget->selectItem].angle;
        }
        else
        {
            ptWidget->offsetAngle=360-ptWidget->ptItemInfoList[ptWidget->selectItem].angle+ptWidget->ptItemInfoList[ptWidget->targetItem].angle;
        }
        ptWidget->offsetAngle=0-ptWidget->offsetAngle;
    }

    ptWidget->offsetAngle=ptWidget->offsetAngle%360;
}

void ldRadialMenuOffsetItem(ldRadialMenu_t *ptWidget,int8_t offset)
{
    if(ptWidget==NULL)
    {
        return;
    }

    if(offset==0)
    {
        return;
    }

    offset=offset%ptWidget->use_as__ldBase_t.itemCount;
    offset=ptWidget->selectItem+offset;
    if(offset<0)
    {
        offset=ptWidget->use_as__ldBase_t.itemCount+offset;
    }
    ldRadialMenuSelectItem(ptWidget,offset);
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
