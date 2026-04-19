#include "ldConfig.h"
#include "arm_2d_types.h"
#include "ldMem.h"
#include "ldBase.h"
#if defined(__PERF_COUNTER__)
#include "perf_counter.h"
#endif

__WEAK void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode)
{
    (void)chCond;
    (void)chLine;
    (void)wErrCode;

    while(1) {
    }
}

/**
 * @brief   Get touch coordinates 
 * 
 * @param   x Returned x coordinate
 * @param   y Returned y coordinate
 * @return  true Valid touch
 * @return  false Invalid touch
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
        if(rx>LD_CFG_SCREEN_WIDTH)
        {
            rx=LD_CFG_SCREEN_WIDTH;
        }
        if(ry>LD_CFG_SCREEN_HEIGHT)
        {
            ry=LD_CFG_SCREEN_HEIGHT;
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

#if USE_VIRTUAL_RESOURCE == 1
/**
 * @brief   Read external storage data
 * 
 * @param   pObj
 * @param   pBuffer         buffer pointer
 * @param   pAddress        offset address
 * @param   nSizeInByte     data size
 * @return  无
 */
void __disp_adapter0_vres_read_memory( intptr_t pObj,
                                       void *pBuffer,
                                       uintptr_t pAddress,
                                       size_t nSizeInByte)
{
    (void)pObj;
//    norRead(pBuffer,pAddress,nSizeInByte);
}

uintptr_t __disp_adapter0_vres_get_asset_address(
    uintptr_t pObj,
    arm_2d_vres_t *ptVRES)
{
    (void)ptVRES;
    return pObj;
}
#endif

void Disp0_DrawBitmap (uint32_t x,uint32_t y,uint32_t width,uint32_t height,const uint8_t *bitmap)
{
    //对接屏幕驱动的彩色填充函数
    //参考1
    //函数原型 void lcd_colorFill(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t *color)
    //填写 lcd_colorFill(x,y,x+width-1,y+height-1,(uint16_t *)bitmap);
    //参考2
    //函数原型 void lcd_colorFill(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t *color)
    //填写 lcd_colorFill(x,y,width,height,(uint16_t *)bitmap);
}

#if 0
void __disp_adapter0_request_async_flushing(
        void *pTarget,
        bool bIsNewFrame,
        int16_t iX,
        int16_t iY,
        int16_t iWidth,
        int16_t iHeight,
        const COLOUR_INT *pBuffer)
{
    VT_Fill_Multiple_Colors(iX, iY,iX+iWidth-1,iY+iHeight-1,(color_typedef*) pBuffer);
    s_bRequestAsyncFlush = true;
}
#endif

int64_t arm_2d_helper_get_system_timestamp(void)
{
#if defined (__riscv)
    return (int64_t)clock();
#elif defined (__arm__)
#if defined(__PERF_COUNTER__)
    return get_system_ticks();
#else
    return 0;
#endif
#elif defined(_POSIX_VERSION) || defined(CLOCK_REALTIME) || defined(__APPLE__)
    struct timespec timestamp;
    clock_gettime(CLOCK_REALTIME, &timestamp);
    static bool waitInit=true;
    if(waitInit)
    {
        waitInit=false;
        tzset();
    }
    return 1000000ul * (timestamp.tv_sec - timezone) + timestamp.tv_nsec / 1000ul;
#endif
}

uint32_t arm_2d_helper_get_reference_clock_frequency(void)
{
#if defined (__riscv)
    return CLOCKS_PER_SEC;
#elif defined (__arm__)
#if defined(__PERF_COUNTER__) && __PER_COUNTER_VER__ >= 20300
    extern uint32_t perfc_port_get_system_timer_freq(void);
    return perfc_port_get_system_timer_freq();
#else
    extern uint32_t SystemCoreClock;
    return SystemCoreClock;
#endif
#elif defined(_POSIX_VERSION) || defined(CLOCK_MONOTONIC) || defined(__APPLE__)
    return 1000000ul;
#endif
}

#if ( !__x86_64__ && !__i386__ && !__APPLE__ )

void *__arm_2d_allocate_scratch_memory( uint32_t wSize,
                                        uint_fast8_t nAlign,
                                        arm_2d_mem_type_t tType)
{
    ARM_2D_UNUSED(nAlign);
    ARM_2D_UNUSED(tType);

    /* ensure nAlign is 2^n */
    assert((((~nAlign) + 1) & nAlign) == nAlign);

    void *pBuff = ldCalloc(1,wSize);
    assert(0 == ((uintptr_t)pBuff & (nAlign - 1)));

    return pBuff;
}

void __arm_2d_free_scratch_memory( arm_2d_mem_type_t tType,
                                   void *pBuff)
{
    ARM_2D_UNUSED(tType);

    ldFree(pBuff);
}

#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__

void * __disp_adapter0_aligned_malloc(size_t nSize, size_t nAlign)
{
    ARM_2D_UNUSED(nAlign);

    /* ensure nAlign is 2^n */
    assert((((~nAlign) + 1) & nAlign) == nAlign);

    void * pMem = ldCalloc(1,nSize);
    assert( 0 == ((uintptr_t)pMem & (nAlign - 1)));
    return pMem;
}

void __disp_adapter0_free(void *pMem)
{
    if (NULL != pMem) {
        ldFree(pMem);
    }
}

#endif

#endif
