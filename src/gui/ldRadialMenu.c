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

ldRadialMenu_t* ldRadialMenu_init( ld_scene_t *ptScene,ldRadialMenu_t *ptWidget, uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint16_t xAxis, uint16_t yAxis, uint8_t itemMax)
{
    assert(NULL != ptScene);
    ldBase_t *ptParent;
    ldRadialMenuItem_t *ptItemList;
    uint8_t *pShowList;

    if (NULL == ptWidget)
    {
        ptWidget = ldCalloc(1, sizeof(ldRadialMenu_t));
        ptItemList=ldCalloc(1,sizeof(ldRadialMenuItem_t)*itemMax);
        pShowList=ldCalloc(1,sizeof (uint8_t)*itemMax);
        if ((NULL == ptWidget)||(NULL == ptItemList)||(NULL == pShowList))
        {
            ldFree(ptWidget);
            ldFree(ptItemList);
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

    ptWidget->pItemList=ptItemList;
    ptWidget->itemMax=itemMax;
    ptWidget->originPos.iX=width>>1;
    ptWidget->originPos.iY=height>>1;
    ptWidget->xAxis=xAxis;
    ptWidget->yAxis=yAxis;
    ptWidget->pShowList=pShowList;

//    xConnect(nameId,SIGNAL_PRESS,nameId,slotMenuSelect);
//    xConnect(nameId,SIGNAL_RELEASE,nameId,slotMenuSelect);
//    xConnect(nameId,SIGNAL_HOLD_DOWN,nameId,slotMenuSelect);

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

    ldFree(ptWidget);
}

void ldRadialMenu_on_load( ldRadialMenu_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}

void ldRadialMenu_on_frame_start( ldRadialMenu_t *ptWidget)
{
    assert(NULL != ptWidget);
    
}


static void _autoScalePercent(ldRadialMenu_t *ptWidget)
{
#if USE_RADIA_MENU_SCALE == 1
    for(uint8_t i=0;i<ptWidget->itemCount;i++)
    {
        //计算缩放比例
        ptWidget->pItemList[i].scalePercent=(ptWidget->pItemList[ptWidget->pShowList[i]].itemRegion.tLocation.iY)*50/ptWidget->yAxis+50;

        if((ptWidget->pItemList[i].scalePercent%10)>=5)
        {
            ptWidget->pItemList[i].scalePercent=(ptWidget->pItemList[i].scalePercent/10+1)*10;
        }
        else
        {
            ptWidget->pItemList[i].scalePercent=(ptWidget->pItemList[i].scalePercent/10)*10;
        }
    }
#endif
}

static void _sortByYAxis(ldRadialMenuItem_t* arr, uint8_t* indexArr, int size)
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
    //计算坐标
    for(uint8_t i=0;i<ptWidget->itemCount;i++)
    {
        ptWidget->pItemList[i].itemRegion.tLocation.iX  = ptWidget->originPos.iX - (arm_cos_f32(ANGLE_2_RADIAN(ptWidget->pItemList[i].angle+ptWidget->nowAngle)) * (ptWidget->xAxis>>1))-(ptWidget->pItemList[i].itemRegion.tSize.iWidth>>1);
        ptWidget->pItemList[i].itemRegion.tLocation.iY  = ptWidget->originPos.iY + (arm_sin_f32(ANGLE_2_RADIAN(ptWidget->pItemList[i].angle+ptWidget->nowAngle)) * (ptWidget->yAxis>>1))-(ptWidget->pItemList[i].itemRegion.tSize.iHeight>>1);
#if USE_RADIA_MENU_SCALE == 1
        //计算缩放比例
        ptWidget->pItemList[i].scalePercent=(ptWidget->pItemList[ptWidget->pShowList[i]].itemRegion.tLocation.iY)*50/ptWidget->yAxis+50;
#endif
        LOG_REGION("",ptWidget->pItemList[i].itemRegion);
    }

    //计算排序
    for (int i = 0; i < ptWidget->itemMax; i++)
    {
        ptWidget->pShowList[i] = i;
    }
    _sortByYAxis(ptWidget->pItemList, ptWidget->pShowList, ptWidget->itemCount);
}

void ldRadialMenu_show(ld_scene_t *ptScene, ldRadialMenu_t *ptWidget, const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    assert(NULL != ptWidget);
    if(ptWidget == NULL)
    {
        return;
    }

#if 0
    if (bIsNewFrame) {
        
    }
#endif
    if(arm_2d_helper_pfb_is_region_active(ptTile,&ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion,true))
    {
        arm_2d_container(ptTile, tTarget, &ptWidget->use_as__ldBase_t.use_as__arm_2d_control_node_t.tRegion)
        {
            if(ptWidget->use_as__ldBase_t.isHidden)
            {
                break;
            }
            if(ptWidget->itemCount)
            {
#if MOVE_CYCLE_MS == 0
                if((bIsNewFrame)&&((ptWidget->offsetAngle!=0)||(ptWidget->isWaitInit)))
#else
                bool isTimeOut=false;
                if(ldTimeOut(MOVE_CYCLE_MS,&ptWidget->timer,true))
                {
                    isTimeOut=true;
                }
                if((bIsNewFrame)&&(isTimeOut)&&(ptWidget->offsetAngle!=0))
#endif
//                {
//                    if(ptWidget->offsetAngle>0)
//                    {
//#if SKIP_ANGLE == 0
//                        ptWidget->nowAngle++;
//                        ptWidget->offsetAngle--;
//#else
//                        if(ptWidget->offsetAngle<SKIP_ANGLE)
//                        {
//                            ptWidget->nowAngle=ITEM_0_ANGLE_OFFSET-ptWidget->pItemList[ptWidget->targetItem].angle;
//                            ptWidget->offsetAngle=0;
//                        }
//                        else
//                        {
//                            ptWidget->nowAngle+=SKIP_ANGLE;
//                            ptWidget->offsetAngle-=SKIP_ANGLE;
//                        }
//#endif
//                    }
//                    else
//                    {
//#if SKIP_ANGLE == 0
//                        ptWidget->nowAngle--;
//                        ptWidget->offsetAngle++;
//#else
//                        if(ptWidget->offsetAngle>(-SKIP_ANGLE))
//                        {
//                            ptWidget->nowAngle=ITEM_0_ANGLE_OFFSET-ptWidget->pItemList[ptWidget->targetItem].angle+360;
//                            ptWidget->offsetAngle++;
//                        }
//                        else
//                        {
//                            ptWidget->nowAngle-=SKIP_ANGLE;
//                            ptWidget->offsetAngle+=SKIP_ANGLE;
//                        }
//#endif
//                    }

//                    _autoSort(ptWidget);

//                    //旋转结束
//                    if((ptWidget->offsetAngle==0)&&((ptWidget->selectItem!=ptWidget->targetItem)||(ptWidget->isWaitInit)))
//                    {
//                        ptWidget->selectItem=ptWidget->targetItem;
//                        ptWidget->nowAngle%=360;
//                        ptWidget->isWaitInit=false;
//                        _autoScalePercent(ptWidget);
//                    }

//                    ptWidget->use_as__ldBase_t.isDirtyRegionUpdate = true;
//                }

//                arm_2d_region_t tempRegion;
//                for(uint8_t i=0;i<ptWidget->itemCount;i++)
//                {

//                    arm_2d_region_get_minimal_enclosure(&ptWidget->pItemList[i].itemRegion,&ptWidget->pItemList[i].tTempItemRegion,&tempRegion);
//                    if(ldBaseDirtyRegionUpdate((ldCommon_t*)ptWidget,&tempRegion,&ptWidget->pItemList[i].dirtyRegionListItem,ptWidget->dirtyRegionState))
//                    {
//                        ptWidget->pItemList[i].dirtyRegionTemp=ptWidget->pItemList[i].itemRegion;
//                    }
//                }

                //刷新item
                for(uint8_t i=0;i<ptWidget->itemCount;i++)
                {
                    do {
                        arm_2d_tile_t tChildTile;
                        arm_2d_tile_generate_child(&tTarget, &ptWidget->pItemList[ptWidget->pShowList[i]].itemRegion, &tChildTile, false);


#if USE_RADIA_MENU_SCALE == 1
                        if(ptWidget->pItemList[i].scalePercent>=100)
                        {
                            ldBaseImage(&tempTile,(arm_2d_tile_t*)&tempRes,ptWidget->pItemList[ptWidget->pShowList[i]].isWithMask,255);
                        }
                        else
                        {
                            ldBaseImageScale(&tempTile,(arm_2d_tile_t*)&tempRes,ptWidget->pItemList[ptWidget->pShowList[i]].isWithMask,ptWidget->pItemList[i].scalePercent/100.0,&ptWidget->pItemList[i].op,bIsNewFrame);
                        }
#else
                        ldBaseImage(&tChildTile,NULL,ptWidget->pItemList[ptWidget->pShowList[i]].ptImgTile,ptWidget->pItemList[ptWidget->pShowList[i]].ptMaskTile,0,ptWidget->use_as__ldBase_t.opacity);
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

    if(ptWidget->itemCount<ptWidget->itemMax)
    {
        ptWidget->pItemList[ptWidget->itemCount].ptImgTile=ptImgTile;
        ptWidget->pItemList[ptWidget->itemCount].ptMaskTile=ptMaskTile;
        ptWidget->pItemList[ptWidget->itemCount].itemRegion=ptImgTile->tRegion;

        ptWidget->itemCount++;

        float preAngle=360.0/ptWidget->itemCount;

        for(uint8_t i=0;i<ptWidget->itemCount;i++)
        {
            ptWidget->pItemList[i].angle=preAngle*i+ITEM_0_ANGLE_OFFSET;
            LOG_DEBUG("angle %f %hu",preAngle,ptWidget->pItemList[i].angle);
        }

        //初始化数据
        _autoSort(ptWidget);
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
