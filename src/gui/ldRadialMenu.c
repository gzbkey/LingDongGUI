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

#define ANGLE_2_RADIAN(angle)    ((angle)*0.017453292519943f)

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

    // 查找父链表
    listInfo = ldGetWidgetInfoById(((ldCommon_t *)pWidget->parentWidget)->nameId);
    listInfo = ((ldCommon_t *)listInfo->info)->childList;

    if (listInfo != NULL)
    {
        xListInfoPrevTraverse(listInfo, pWidget, _radialMenuDel);
    }
}

static bool slotMenuSelect(xConnectInfo_t info)
{
    ldRadialMenu_t *menu;
    size_t value;
    static int16_t pressX,pressY,nowX,nowY;

    value=info.value;
    menu=ldGetWidgetById(info.receiverId);

    switch (info.signalType)
    {
    case SIGNAL_PRESS:
    {
        pressX=(value>>16)&0xFFFF;
        pressY=value&0xFFFF;
        break;
    }
    case SIGNAL_TOUCH_HOLD_MOVE:
    {
        break;
    }
    case SIGNAL_RELEASE:
    {
        if((menu->isMove==false)&&(menu->offsetAngle==0))//只允许静止状态下选择
        {
            nowX=(value>>16)&0xFFFF;
            nowY=value&0xFFFF;
            nowX-=((arm_2d_tile_t*)&menu->resource)->tRegion.tLocation.iX;
            nowY-=((arm_2d_tile_t*)&menu->resource)->tRegion.tLocation.iY;
            pressX-=((arm_2d_tile_t*)&menu->resource)->tRegion.tLocation.iX;
            pressY-=((arm_2d_tile_t*)&menu->resource)->tRegion.tLocation.iY;

            for(int8_t i=menu->itemCount-1;i>=0;i--)
            {
                if(((pressX>menu->pItemList[menu->showList[i]].pos.x)&&(pressX<(menu->pItemList[menu->showList[i]].pos.x+menu->pItemList[menu->showList[i]].size.width-1))&&(pressY>menu->pItemList[menu->showList[i]].pos.y)&&(pressY<(menu->pItemList[menu->showList[i]].pos.y+menu->pItemList[menu->showList[i]].size.height-1)))&&
                        ((nowX>menu->pItemList[menu->showList[i]].pos.x)&&(nowX<(menu->pItemList[menu->showList[i]].pos.x+menu->pItemList[menu->showList[i]].size.width-1))&&(nowY>menu->pItemList[menu->showList[i]].pos.y)&&(nowY<(menu->pItemList[menu->showList[i]].pos.y+menu->pItemList[menu->showList[i]].size.height-1))))
                {
                    ldRadialMenuSelectItem(menu,menu->showList[i]);
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
            nowX=(value>>16)&0xFFFF;
            preAngle=360.0/menu->itemCount;

            if((nowX>=preAngle)||(nowX<=(-preAngle)))
            {
                if(nowX>=360)
                {
                    nowX=359;
                }
                else
                {
                    if(nowX<=-360)
                    {
                        nowX=-359;
                    }
                }
                offsetItem=nowX/preAngle;
                menu->offsetAngle=offsetItem*preAngle;
                if(offsetItem>0)
                {
                    if((menu->selectItem-offsetItem)>=0)
                    {
                        menu->targetItem=menu->selectItem-offsetItem;
                    }
                    else
                    {
                        menu->targetItem=menu->itemCount+(menu->selectItem-offsetItem);
                    }
                }
                else
                {
                    if((menu->selectItem-offsetItem)<menu->itemCount)
                    {
                        menu->targetItem=menu->selectItem-offsetItem;
                    }
                    else
                    {
                        menu->targetItem=-offsetItem-(menu->itemCount-menu->selectItem);
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

ldRadialMenu_t *ldRadialMenuInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint16_t xAxis, uint16_t yAxis, uint8_t itemMax)
{
    ldRadialMenu_t *pNewWidget = NULL;
    xListNode *parentInfo;
    xListNode *parentList = NULL;
    arm_2d_tile_t *tResTile;
    ldRadialMenuItem_t *pNewItemList;
    uint8_t *pNewShowList;

    parentInfo = ldGetWidgetInfoById(parentNameId);
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
        pNewWidget->parentType = ((ldCommon_t *)(parentInfo->info))->widgetType;
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
        tResTile->pchBuffer = 0;
#if USE_VIRTUAL_RESOURCE == 1
        tResTile->tInfo.bVirtualResource = true;
        ((arm_2d_vres_t*)tResTile)->pTarget=0;
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
        pWidget->pItemList[i].scalePercent=(pWidget->pItemList[pWidget->showList[i]].pos.y)*50/pWidget->yAxis+50;

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
            if(arr[indexArr[j]].pos.y>arr[indexArr[j+1]].pos.y)
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
    //计算坐标
    for(uint8_t i=0;i<pWidget->itemCount;i++)
    {
        pWidget->pItemList[i].pos.x  = pWidget->originPos.x - (arm_cos_f32(ANGLE_2_RADIAN(pWidget->pItemList[i].angle+pWidget->nowAngle)) * (pWidget->xAxis>>1))-(pWidget->pItemList[i].size.width>>1);
        pWidget->pItemList[i].pos.y  = pWidget->originPos.y + (arm_sin_f32(ANGLE_2_RADIAN(pWidget->pItemList[i].angle+pWidget->nowAngle)) * (pWidget->yAxis>>1))-(pWidget->pItemList[i].size.height>>1);
#if USE_RADIA_MENU_SCALE == 1
        //计算缩放比例
        pWidget->pItemList[i].scalePercent=(pWidget->pItemList[pWidget->showList[i]].pos.y)*50/pWidget->yAxis+50;
#endif
    }

    //计算排序
    for (int i = 0; i < pWidget->itemMax; i++)
    {
        pWidget->showList[i] = i;
    }
    _sortByYAxis(pWidget->pItemList, pWidget->showList, pWidget->itemCount);
}

void ldRadialMenuLoop(ldRadialMenu_t *pWidget,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    arm_2d_tile_t *ptResTile=(arm_2d_tile_t*)&pWidget->resource;

    if (pWidget == NULL)
    {
        return;
    }

    if((pWidget->isParentHidden)||(pWidget->isHidden))
    {
        return;
    }

    arm_2d_container(ptParent,tTarget , &ptResTile->tRegion)
    {
        tTarget.tRegion.tLocation = ptResTile->tRegion.tLocation;

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
                do {
#if USE_VIRTUAL_RESOURCE == 0
                    arm_2d_tile_t tempRes = *ptResTile;
#else
                    arm_2d_vres_t tempRes = *((arm_2d_vres_t*)ptResTile);
#endif
                    arm_2d_tile_t tempTile = impl_child_tile(tTarget,pWidget->pItemList[pWidget->showList[i]].pos.x,pWidget->pItemList[pWidget->showList[i]].pos.y,pWidget->pItemList[pWidget->showList[i]].size.width,pWidget->pItemList[pWidget->showList[i]].size.height);

                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iWidth=pWidget->pItemList[pWidget->showList[i]].size.width;
                    ((arm_2d_tile_t*)&tempRes)->tRegion.tSize.iHeight=pWidget->pItemList[pWidget->showList[i]].size.height;
                    ((arm_2d_tile_t*)&tempRes)->pchBuffer=pWidget->pItemList[pWidget->showList[i]].addr;
#if USE_VIRTUAL_RESOURCE == 1
                    tempRes.pTarget=pWidget->pItemList[pWidget->showList[i]].addr;
#endif
#if USE_RADIA_MENU_SCALE == 1
                    ldBaseImageScale(&tempTile,(arm_2d_tile_t*)&tempRes,pWidget->pItemList[pWidget->showList[i]].isWithMask,pWidget->pItemList[i].scalePercent/100.0,bIsNewFrame);
#else
                    ldBaseImage(&tempTile,(arm_2d_tile_t*)&tempRes,pWidget->pItemList[pWidget->showList[i]].isWithMask,255);
#endif
                    arm_2d_op_wait_async(NULL);
                } while (0);
            }
        }
    }
}

void ldRadialMenuSetHidden(ldRadialMenu_t *pWidget,bool isHidden)
{
    ldBaseSetHidden((ldCommon_t*) pWidget,isHidden);
}

void ldRadialMenuAddItem(ldRadialMenu_t *pWidget,uint32_t imageAddr,uint16_t width,uint16_t height,uint8_t itemSubCount,bool isWithMask)
{
    if(pWidget==NULL)
    {
        return;
    }

    if(pWidget->itemCount<pWidget->itemMax)
    {
        pWidget->pItemList[pWidget->itemCount].addr=imageAddr;
        pWidget->pItemList[pWidget->itemCount].size.width=width;
        pWidget->pItemList[pWidget->itemCount].size.height=height;
        pWidget->pItemList[pWidget->itemCount].count=itemSubCount;
        pWidget->pItemList[pWidget->itemCount].isWithMask=isWithMask;

        pWidget->itemCount++;

        float preAngle=360.0/pWidget->itemCount;

        for(uint8_t i=0;i<pWidget->itemCount;i++)
        {
            pWidget->pItemList[i].angle=preAngle*i+ITEM_0_ANGLE_OFFSET;
        }
    }
}

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

    arm_2d_tile_t *ptResTile=(arm_2d_tile_t*)&pWidget->resource;

    pWidget->targetItem=num;


    ldRadialMenuItem_t* targetItem=&pWidget->pItemList[pWidget->targetItem];
    if((targetItem->pos.x+(targetItem->size.width>>1))<(ptResTile->tRegion.tSize.iWidth>>1))// left
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