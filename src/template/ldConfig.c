#include "ldConfig.h"

/**
 * @brief   获取触摸坐标
 * 
 * @param   x 返回的x坐标
 * @param   y 返回的y坐标
 * @return  true 有效触摸
 * @return  false 无效触摸
 */
bool ldCfgTouchGetPoint(int16_t *x,int16_t *y)
{
    bool touchState=false;
    int16_t rx;
    int16_t ry;
    
    //添加触摸函数
//    touchState=vtMouseGetPoint(&rx,&ry);

    if((touchState!=0)&&(((rx!=-1)&&(ry!=-1))||((rx!=0)&&(ry!=0))))
    {
        if(rx<0)
        {
            rx=0;
        }
        if(ry<0)
        {
            ry=0;
        }
        if(rx>LD_CFG_SCEEN_WIDTH)
        {
            rx=LD_CFG_SCEEN_WIDTH;
        }
        if(ry>LD_CFG_SCEEN_HEIGHT)
        {
            ry=LD_CFG_SCEEN_HEIGHT;
        }
        *x=rx;
        *y=ry;
        touchState=true;
    }
    else
    {
        touchState=false;
        *x=-1;
        *y=-1;
    }
    return touchState;
}
