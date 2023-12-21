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
 * @file    ldRadialMenu.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   radial menu widget
 * @version 0.1
 * @date    2023-11-03
 */
#include "ldRadialMenu.h"
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

#define ITEM_0_ANGLE_OFFSET      90
#define MOVE_CYCLE_MS            0
#define SKIP_ANGLE               5

static bool _radialMenuDel(xListNode *pEachInfo, void *pTarget)
{
    if (pEachInfo->info == pTarget)
    {
        //del user object
        ldFree(((ldRadialMenu_t *)pTarget)->showList);
        ldFree(((ldRadialMenu_t *)pTarget)->pItemList);
        ldFree(((ldRadialMenu_t *)pTarget));
        xListInfoDel(pEachInfo);
    }
    return false;
}

void ldRadialMenuDel(ldRadialMenu_t *pWidget)
{
    xListNode *listInfo;

    if (pWidget == NULL)
    {
        return;
    }

    if(pWidget->widgetType!=widgetTypeRadialMenu)
    {
        return;
    }

    LOG_INFO("[radialMenu] del,id:%d\n",pWidget->nameId);

    xDeleteConnect(pWidget->nameId);

    listInfo = ldBaseGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _radialMenuDel);
    }
}

static bool slotMenuSelect(xConnectInfo_t info)
{
    ldRadialMenu_t *pWidget;
    size_t value;
    static int16_t pressX,pressY;
    int16_t x,y;

    value=info.value;
    pWidget=ldBaseGetWidgetById(info.receiverId);

    ldPoint_t widgetPos=ldBaseGetGlobalPos(pWidget);

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        pressX=(int16_t)GET_SIGNAL_VALUE_X(info.value)-widgetPos.x;
        pressY=(int16_t)GET_SIGNAL_VALUE_Y(info.value)-widgetPos.y;
        break;
    }
    case SIGNAL_TOUCH_HOLD_MOVE:
    {
        break;
    }
    case SIGNAL_RELEASE:
    {
        if((pWidget->isMove==false)&&(pWidget->offsetAngle==0))//只允许静止状态下选择
        {
            x=(int16_t)GET_SIGNAL_VALUE_X(info.value)-widgetPos.x;
            y=(int16_t)GET_SIGNAL_VALUE_Y(info.value)-widgetPos.y;

            for(int8_t i=pWidget->itemCount-1;i>=0;i--)
            {
                if(((pressX>pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iX)&&(pressX<(pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iX+pWidget->pItemList[pWidget->showList[i]].itemRegion.tSize.iWidth-1))&&(pressY>pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iY)&&(pressY<(pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iY+pWidget->pItemList[pWidget->showList[i]].itemRegion.tSize.iHeight-1)))&&
                   ((x     >pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iX)&&(x     <(pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iX+pWidget->pItemList[pWidget->showList[i]].itemRegion.tSize.iWidth-1))&&(y     >pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iY)&&(y     <(pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iY+pWidget->pItemList[pWidget->showList[i]].itemRegion.tSize.iHeight-1))))
                {
                    ldRadialMenuSelectItem(pWidget,pWidget->showList[i]);
                    break;
                }
            }
        }
        break;
    }
    case SIGNAL_MOVE_SPEED:
    {
        //将移动速度强制转换成item数量，并且限制360度内
        do{
            float preAngle;
            int8_t offsetItem;
            x=(int16_t)GET_SIGNAL_VALUE_X(info.value);
            preAngle=360.0/pWidget->itemCount;

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
                pWidget->offsetAngle=offsetItem*preAngle;
                if(offsetItem>0)
                {
                    if((pWidget->selectItem-offsetItem)>=0)
                    {
                        pWidget->targetItem=pWidget->selectItem-offsetItem;
                    }
                    else
                    {
                        pWidget->targetItem=pWidget->itemCount+(pWidget->selectItem-offsetItem);
                    }
                }
                else
                {
                    if((pWidget->selectItem-offsetItem)<pWidget->itemCount)
                    {
                        pWidget->targetItem=pWidget->selectItem-offsetItem;
                    }
                    else
                    {
                        pWidget->targetItem=-offsetItem-(pWidget->itemCount-pWidget->selectItem);
                    }
                }
            }
        }while(0);
        break;
    }
    default:break;
    }

    return false;
}

/**
 * @brief   旋转菜单初始化
 * 
 * @param   nameId          新控件id
 * @param   parentNameId    父控件id
 * @param   x               相对坐标x轴
 * @param   y               相对坐标y轴
 * @param   width           控件宽度
 * @param   height          控件高度
 * @param   xAxis           x轴长度
 * @param   yAxis           y轴长度
 * @param   itemMax         项目数量最大值
 * @return  ldRadialMenu_t* 新控件指针
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
ldRadialMenu_t *ldRadialMenuInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint16_t xAxis, uint16_t yAxis, uint8_t itemMax)
{
    ldRadialMenu_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    ldRadialMenuItem_t *pNewItemList;
    uint8_t *pNewShowList;

    parentInfo = ldBaseGetWidgetInfoById(parentNameId);
    pNewWidget = LD_MALLOC_WIDGET_INFO(ldRadialMenu_t);
    pNewItemList=ldMalloc(sizeof(ldRadialMenuItem_t)*itemMax);
    pNewShowList=ldMalloc(sizeof (uint8_t)*itemMax);
    if ((pNewWidget != NULL)&&(pNewItemList != NULL)&&(pNewShowList!=NULL))
    {
        pNewWidget->isParentHidden=false;
        parentList = ((ldCommon_t *)parentInfo->info)->childList;
        if(((ldCommon_t *)parentInfo->info)->isHidden||((ldCommon_t *)parentInfo->info)->isParentHidden)
        {
            pNewWidget->isParentHidden=true;
        }
        pNewWidget->nameId = nameId;
        pNewWidget->childList = NULL;
        pNewWidget->widgetType = widgetTypeRadialMenu;
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
        pNewWidget->itemCount=0;
        pNewWidget->pItemList=pNewItemList;
        pNewWidget->itemMax=itemMax;
        pNewWidget->originPos.x=tResTile->tRegion.tSize.iWidth>>1;
        pNewWidget->originPos.y=tResTile->tRegion.tSize.iHeight>>1;
        pNewWidget->xAxis=xAxis;
        pNewWidget->yAxis=yAxis;
        pNewWidget->timer=0;
        pNewWidget->nowAngle=0;
        pNewWidget->offsetAngle=0;
        pNewWidget->selectItem=0;
        pNewWidget->isMove=false;
        pNewWidget->showList=pNewShowList;
        pNewWidget->isWaitInit=true;
        pNewWidget->dirtyRegionListItem.ptNext=NULL;
        pNewWidget->dirtyRegionListItem.tRegion = ldBaseGetGlobalRegion(pNewWidget,&((arm_2d_tile_t*)&pNewWidget->resource)->tRegion);
        pNewWidget->dirtyRegionListItem.bIgnore = true;
        pNewWidget->dirtyRegionListItem.bUpdated = false;
        pNewWidget->dirtyRegionState=waitChange;
        pNewWidget->dirtyRegionTemp=tResTile->tRegion;
        pNewWidget->isDirtyRegionAutoIgnore=false;

        xConnect(nameId,SIGNAL_PRESS,nameId,slotMenuSelect);
        xConnect(nameId,SIGNAL_RELEASE,nameId,slotMenuSelect);
        xConnect(nameId,SIGNAL_TOUCH_HOLD_MOVE,nameId,slotMenuSelect);
        xConnect(nameId,SIGNAL_MOVE_SPEED,nameId,slotMenuSelect);

        LOG_INFO("[radialMenu] init,id:%d\n",nameId);
    }
    else
    {
        ldFree(pNewWidget);
        ldFree(pNewItemList);
        ldFree(pNewShowList);

        LOG_ERROR("[radialMenu] init failed,id:%d\n",nameId);
    }

    return pNewWidget;
}

static void _autoScalePercent(ldRadialMenu_t *pWidget)
{
#if USE_RADIA_MENU_SCALE == 1
    for(uint8_t i=0;i<pWidget->itemCount;i++)
    {
        //计算缩放比例
        pWidget->pItemList[i].scalePercent=(pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iY)*50/pWidget->yAxis+50;

        if((pWidget->pItemList[i].scalePercent%10)>=5)
        {
            pWidget->pItemList[i].scalePercent=(pWidget->pItemList[i].scalePercent/10+1)*10;
        }
        else
        {
            pWidget->pItemList[i].scalePercent=(pWidget->pItemList[i].scalePercent/10)*10;
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

static void _autoSort(ldRadialMenu_t *pWidget)
{
    ldPoint_t globalPos=ldBaseGetGlobalPos(pWidget);
    //计算坐标
    for(uint8_t i=0;i<pWidget->itemCount;i++)
    {
        pWidget->pItemList[i].itemRegion.tLocation.iX  = pWidget->originPos.x - (arm_cos_f32(ANGLE_2_RADIAN(pWidget->pItemList[i].angle+pWidget->nowAngle)) * (pWidget->xAxis>>1))-(pWidget->pItemList[i].itemRegion.tSize.iWidth>>1);
        pWidget->pItemList[i].itemRegion.tLocation.iY  = pWidget->originPos.y + (arm_sin_f32(ANGLE_2_RADIAN(pWidget->pItemList[i].angle+pWidget->nowAngle)) * (pWidget->yAxis>>1))-(pWidget->pItemList[i].itemRegion.tSize.iHeight>>1);
#if USE_RADIA_MENU_SCALE == 1
        //计算缩放比例
        pWidget->pItemList[i].scalePercent=(pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iY)*50/pWidget->yAxis+50;
#endif
        pWidget->pItemList[i].dirtyRegionState=waitChange;
    }

    //计算排序
    for (int i = 0; i < pWidget->itemMax; i++)
    {
        pWidget->showList[i] = i;
    }
    _sortByYAxis(pWidget->pItemList, pWidget->showList, pWidget->itemCount);
}

void ldRadialMenuDirtyRegionAutoUpdate(ldRadialMenu_t* pWidget,uint8_t itemNum,arm_2d_region_t newRegion,bool isAutoIgnore)
{
    switch (pWidget->pItemList[itemNum].dirtyRegionState)
    {
    case none:
    {
        if(isAutoIgnore&&(pWidget->pItemList[itemNum].dirtyRegionListItem.bIgnore==false))
        {
            pWidget->pItemList[itemNum].dirtyRegionListItem.bIgnore=true;
        }
        break;
    }
    case waitChange://扩张到新范围
    {
        arm_2d_region_t tempRegion;
        ldPoint_t globalPos;
        globalPos=ldBaseGetGlobalPos((ldCommon_t*)pWidget);
        newRegion.tLocation.iX+=globalPos.x;
        newRegion.tLocation.iY+=globalPos.y;

        arm_2d_region_get_minimal_enclosure(&newRegion,&pWidget->pItemList[itemNum].dirtyRegionTemp,&tempRegion);

        pWidget->pItemList[itemNum].dirtyRegionListItem.tRegion=tempRegion;
        pWidget->pItemList[itemNum].dirtyRegionListItem.bIgnore=false;
        pWidget->pItemList[itemNum].dirtyRegionListItem.bUpdated=true;
        pWidget->pItemList[itemNum].dirtyRegionTemp=newRegion;
        pWidget->pItemList[itemNum].dirtyRegionState=waitUpdate;
        break;
    }
    case waitUpdate://缩小到新范围
    {
        pWidget->pItemList[itemNum].dirtyRegionListItem.tRegion=pWidget->pItemList[itemNum].dirtyRegionTemp;
        pWidget->pItemList[itemNum].dirtyRegionListItem.bIgnore=false;
        pWidget->pItemList[itemNum].dirtyRegionListItem.bUpdated=true;
        pWidget->pItemList[itemNum].dirtyRegionState=none;
        break;
    }
    default:
        break;
    }
}

void ldRadialMenuFrameStart(ldRadialMenu_t* pWidget)
{
    for(uint8_t i=0;i<pWidget->itemCount;i++)
    {
        ldRadialMenuDirtyRegionAutoUpdate(pWidget,i,pWidget->pItemList[i].itemRegion,true);
    }
}

void ldRadialMenuLoop(ldRadialMenu_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame)
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

#if USE_VIRTUAL_RESOURCE == 0
    arm_2d_tile_t tempRes = *pResTile;
#else
    arm_2d_vres_t tempRes = *((arm_2d_vres_t*)pResTile);
#endif

    arm_2d_region_t newRegion=ldBaseGetGlobalRegion((ldCommon_t*)pWidget,&pResTile->tRegion);

    arm_2d_container(pParentTile,tTarget , &newRegion)
    {
        if(pWidget->itemCount)
        {
#if MOVE_CYCLE_MS == 0
            if((bIsNewFrame)&&((pWidget->offsetAngle!=0)||(pWidget->isWaitInit)))
#else
            bool isTimeOut=false;
            if(ldTimeOut(MOVE_CYCLE_MS,&pWidget->timer,true))
            {
                isTimeOut=true;
            }
            if((bIsNewFrame)&&(isTimeOut)&&(pWidget->offsetAngle!=0))
#endif
            {
                if(pWidget->offsetAngle>0)
                {
#if SKIP_ANGLE == 0
                    pWidget->nowAngle++;
                    pWidget->offsetAngle--;
#else
                    if(pWidget->offsetAngle<SKIP_ANGLE)
                    {
                        pWidget->nowAngle=ITEM_0_ANGLE_OFFSET-pWidget->pItemList[pWidget->targetItem].angle;
                        pWidget->offsetAngle=0;
                    }
                    else
                    {
                        pWidget->nowAngle+=SKIP_ANGLE;
                        pWidget->offsetAngle-=SKIP_ANGLE;
                    }
#endif
                }
                else
                {
#if SKIP_ANGLE == 0
                    pWidget->nowAngle--;
                    pWidget->offsetAngle++;
#else
                    if(pWidget->offsetAngle>(-SKIP_ANGLE))
                    {
                        pWidget->nowAngle=ITEM_0_ANGLE_OFFSET-pWidget->pItemList[pWidget->targetItem].angle+360;
                        pWidget->offsetAngle++;
                    }
                    else
                    {
                        pWidget->nowAngle-=SKIP_ANGLE;
                        pWidget->offsetAngle+=SKIP_ANGLE;
                    }
#endif
                }

                _autoSort(pWidget);

                //旋转结束
                if((pWidget->offsetAngle==0)&&((pWidget->selectItem!=pWidget->targetItem)||(pWidget->isWaitInit)))
                {
                    pWidget->selectItem=pWidget->targetItem;
                    pWidget->nowAngle%=360;
                    pWidget->isWaitInit=false;
                    _autoScalePercent(pWidget);
                }

            }

            //刷新item
            for(uint8_t i=0;i<pWidget->itemCount;i++)
            {
//                ldRadialMenuDirtyRegionAutoUpdate(pWidget,i,pWidget->pItemList[i].itemRegion,true,bIsNewFrame);
                do {
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iX=0;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tLocation.iY=0;

                    arm_2d_tile_t tempTile = impl_child_tile(tTarget,pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iX,pWidget->pItemList[pWidget->showList[i]].itemRegion.tLocation.iY,pWidget->pItemList[pWidget->showList[i]].itemRegion.tSize.iWidth,pWidget->pItemList[pWidget->showList[i]].itemRegion.tSize.iHeight);

                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iWidth=pWidget->pItemList[pWidget->showList[i]].itemRegion.tSize.iWidth;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iHeight=pWidget->pItemList[pWidget->showList[i]].itemRegion.tSize.iHeight;
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer=(uint8_t *)pWidget->pItemList[pWidget->showList[i]].addr;

#if USE_VIRTUAL_RESOURCE == 1
                    tempRes.pTarget=pWidget->pItemList[pWidget->showList[i]].addr;
#endif
#if USE_RADIA_MENU_SCALE == 1
                    if(pWidget->pItemList[i].scalePercent>=100)
                    {
                        ldBaseImage(&tempTile,(arm_2d_tile_t*)&tempRes,pWidget->pItemList[pWidget->showList[i]].isWithMask,255);
                    }
                    else
                    {
                        ldBaseImageScale(&tempTile,(arm_2d_tile_t*)&tempRes,pWidget->pItemList[pWidget->showList[i]].isWithMask,pWidget->pItemList[i].scalePercent/100.0,&pWidget->pItemList[i].op,bIsNewFrame);
                    }
#else
                    ldBaseImage(&tempTile,(arm_2d_tile_t*)&tempRes,pWidget->pItemList[pWidget->showList[i]].isWithMask,255);
#endif
                    arm_2d_op_wait_async(NULL);
                } while (0);

//                ldRadialMenuDirtyRegionAutoUpdate(pWidget,i,pWidget->pItemList[i].itemRegion,true,bIsNewFrame);
            }
        }
    }
}

/**
 * @brief   添加项目
 * 
 * @param   pWidget         目标控件指针
 * @param   imageAddr       图片地址
 * @param   width           图片宽度
 * @param   height          图片高度
 * @param   itemSubCount    子图片数量(用于动态视频效果)
 * @param   isWithMask      图片是否带mask
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldRadialMenuAddItem(ldRadialMenu_t *pWidget,uint32_t imageAddr,uint16_t width,uint16_t height,uint8_t itemSubCount,bool isWithMask)
{
    if(pWidget==NULL)
    {
        return;
    }

    if(pWidget->itemCount<pWidget->itemMax)
    {
        pWidget->pItemList[pWidget->itemCount].addr=imageAddr;
        pWidget->pItemList[pWidget->itemCount].itemRegion.tSize.iWidth=width;
        pWidget->pItemList[pWidget->itemCount].itemRegion.tSize.iHeight=height;
        pWidget->pItemList[pWidget->itemCount].count=itemSubCount;
        pWidget->pItemList[pWidget->itemCount].isWithMask=isWithMask;

        pWidget->pItemList[pWidget->itemCount].dirtyRegionListItem.ptNext=NULL;
        pWidget->pItemList[pWidget->itemCount].dirtyRegionListItem.tRegion = (arm_2d_region_t){0};
        pWidget->pItemList[pWidget->itemCount].dirtyRegionListItem.bIgnore = false;
        pWidget->pItemList[pWidget->itemCount].dirtyRegionListItem.bUpdated = true;

        arm_2d_region_list_item_t * pDirtyRegionListItem=&pWidget->dirtyRegionListItem;
        ldBaseAddDirtyRegion(&pWidget->pItemList[pWidget->itemCount].dirtyRegionListItem,&pDirtyRegionListItem);

        pWidget->itemCount++;

        float preAngle=360.0/pWidget->itemCount;

        for(uint8_t i=0;i<pWidget->itemCount;i++)
        {
            pWidget->pItemList[i].angle=preAngle*i+ITEM_0_ANGLE_OFFSET;
        }

        //初始化数据
        _autoSort(pWidget);

        ldPoint_t globalPos=ldBaseGetGlobalPos(pWidget);
        for(uint8_t i=0;i<pWidget->itemCount;i++)
        {
            pWidget->pItemList[i].dirtyRegionState=waitChange;

            pWidget->pItemList[i].dirtyRegionTemp=pWidget->pItemList[i].itemRegion;
            pWidget->pItemList[i].dirtyRegionTemp.tLocation.iX+=globalPos.x;
            pWidget->pItemList[i].dirtyRegionTemp.tLocation.iY+=globalPos.y;

            pWidget->pItemList[i].dirtyRegionListItem.tRegion=pWidget->pItemList[i].dirtyRegionTemp;

        }
    }
}

/**
 * @brief   选中项目
 * 
 * @param   pWidget         目标控件指针
 * @param   num             项目编号，0开始
 * @author  Ou Jianbo(59935554@qq.com)
 * @date    2023-12-21
 */
void ldRadialMenuSelectItem(ldRadialMenu_t *pWidget,uint8_t num)
{
    if(pWidget==NULL)
    {
        return;
    }

    if(pWidget->selectItem==num)
    {
        return;
    }

    arm_2d_tile_t *pResTile=(arm_2d_tile_t*)&pWidget->resource;

    pWidget->targetItem=num;


    ldRadialMenuItem_t* targetItem=&pWidget->pItemList[pWidget->targetItem];
    if((targetItem->itemRegion.tLocation.iX+(targetItem->itemRegion.tSize.iWidth>>1))<(pResTile->tRegion.tSize.iWidth>>1))// left
    {
        if(pWidget->targetItem<pWidget->selectItem)
        {
            pWidget->offsetAngle=pWidget->pItemList[pWidget->selectItem].angle-pWidget->pItemList[pWidget->targetItem].angle;
        }
        else
        {
            pWidget->offsetAngle=360-pWidget->pItemList[pWidget->targetItem].angle+pWidget->pItemList[pWidget->selectItem].angle;
        }
    }
    else// right
    {
        if(pWidget->targetItem>pWidget->selectItem)
        {
            pWidget->offsetAngle=pWidget->pItemList[pWidget->targetItem].angle-pWidget->pItemList[pWidget->selectItem].angle;
        }
        else
        {
            pWidget->offsetAngle=360-pWidget->pItemList[pWidget->selectItem].angle+pWidget->pItemList[pWidget->targetItem].angle;
        }
        pWidget->offsetAngle=0-pWidget->offsetAngle;
    }

    pWidget->offsetAngle=pWidget->offsetAngle%360;
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
