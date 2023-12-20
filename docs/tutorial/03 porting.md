## 移植

本文档以单片机为标准，说明移植过程

## Keil

安装好MDK-ARM，这里使用的版本是5.38。建议使用最新版本

### 移植arm-2d准备
 * 安装arm-2d的pack，最新版本[下载](https://github.com/ARM-software/Arm-2D/releases)
 * 安装perf_counter的pack，最新版本[下载](https://github.com/GorgonMeducer/perf_counter/releases)
 * 准备带屏幕的开发板，可以正常显示图片的keil项目(lcd_project)
 * 屏幕接口
    ```c 
    void Disp0_DrawBitmap (uint32_t x, 
                           uint32_t y, 
                           uint32_t width, 
                           uint32_t height, 
                           const uint8_t *bitmap)
    ```
 * 下载ldgui源码
🏠️主仓库: https://gitee.com/gzbkey/LingDongGUI
🏠️镜像仓库: https://github.com/gzbkey/LingDongGUI

|ℹ️ 关于github下载慢的问题|
|:----|
|推荐使用Watt Toolkit加速|

### 先让arm-2d跑起来

1. 在lcd_project中加入arm-2d、perf_counter、DSP、CMSIS，keil中选择Project -> Manage -> Run-Time Environment

    ![keilPackSelect](../tutorial/images/03/arm2d%20Manage%20Run-Time%20Environment.png)

2. 选择ac6编译器，并且选择gnu11

    ![ac6Setting](../tutorial/images/03/ac6%20setting.png)

3. 如果使用ac5编译器，则需要选择c99和gnu支持，但是不建议使用ac5

    ![ac5Setting](../tutorial/images/03/ac5%20setting.png)

4. 确保keil的CMSIS版本不得低于5.7.0，查看方式，Project -> Manage -> Select Software Packs

    ![cmsisVersion](../tutorial/images/03/cmsis%20version.png)

5. 树目录中的Acceleration，找到arm_2d_disp_adapter_0.h。

    编辑器的左下角选择 Configuration Wizard，进入图形配置界面，根据实际情况配置

    颜色位数（Screen Colour Depth）

    横向分辨率（Width of the screen）

    纵向分辨率（Height of the Screen）

    部分刷新缓冲块的宽度（Width of the PFB Block），配置为width/2

    部分刷新缓冲块的高度（Height of the PFB Block），配置为8

6. 树目录中的Acceleration，找到arm_2d_disp_adapter_0.h。
    
    编辑器的左下角选择 Configuration Wizard，进入图形配置界面，配置Extra下的colour depth、width和height

7. 使用microLib需要添加__aeabi_assert

    不使用microLib需要关闭半主机模式，并自定义c库
    ```c 
    #include "stdio.h"
    #include "ctype.h"
    #include "stdlib.h"

    #if defined(__MICROLIB)
    void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
    {
        ARM_2D_UNUSED(chCond);
        ARM_2D_UNUSED(chLine);
        ARM_2D_UNUSED(wErrCode);
        while(1) {
            __NOP();
        }
    }
    #else

    #if (__ARMCC_VERSION >= 6010050)
    __asm(".global __use_no_semihosting\n\t");
    __asm(".global __ARM_use_no_argv\n\t");
    #else
    #pragma import(__use_no_semihosting)

    struct __FILE
    {
        int handle;
        /* Whatever you require here. If the only file you are using is */
        /* standard output using printf() for debugging, no file handling */
        /* is required. */
    };
    #endif


    typedef int FILEHANDLE;
    FILEHANDLE _sys_open(const char *name,int openmode)
    {
     return 0;
    }
    int _sys_close(FILEHANDLE fh)
    {
        return 0;
    }
    int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
    {
        return 0;
    }
    int _sys_read(FILEHANDLE fh, unsigned char*buf, unsigned len, int mode)
    {
        return 0;
    }

    int _sys_istty(FILEHANDLE fh)
    {
        return 0;
    }
    int _sys_seek(FILEHANDLE fh, long pos)
    {
        return 0;
    }

    int _sys_ensure(FILEHANDLE fh)
    {
        return 0;
    }

    long _sys_flen(FILEHANDLE fh)
    {
        return 0;
    }
    void _sys_exit(int status)
    {
        //while(1);
    }
    int _sys_tmpnam(char *name, int fileno, unsigned maxlength)
    {
        return 0;
    }

    void _ttywrch(int ch)
    {
    }
    int remove(const char *filename)
    {
        return 0;
    }
    char *_sys_command_string(char *cmd, int len)
    {
     return NULL;
    }

    void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
    {
        (void)chCond;
        (void)chLine;
        (void)wErrCode;
        
        while(1) {
        }
    }

    #endif

    ```

8. 
7. main文件加入如下代码
    ```c 
    #include "arm_2d.h"
    #include "arm_2d_disp_adapters.h"
    #include "perf_counter.h"

    __attribute__((used))
    void SysTick_Handler(void)
    {
    }

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

    int main(void) 
    {
        system_init();     // 包括 LCD 在内的系统初始化

        init_cycle_counter(false);

        arm_irq_safe
        {
            arm_2d_init(); // 初始化 arm-2d
        }

        disp_adapter0_init();
        while(1)
        {
            disp_adapter0_task();
        }
    }
    ```

3. 运行效果

   ![arm2d-demo](../tutorial/images/03/arm2d%20demo.gif)

### 加入ldgui

1. 将ldgui的c文件添加到项目，添加头文件路径。其中_ldTemplate.c为模板文件，请勿添加到项目中。

2. keil中选择Project -> Manage -> Run-Time Environment，Acceleration - Arm-2D Helper中，Scene设置为0

3. arm_2d_disp_adapter_0.h中添加ldgui配置头文件
    ```c
    #include "ldConfig.h" 
    ```

4. 新建ldConfig.c、ldConfig.h、ldUser.c、ldUser.h

5. xLog.h关闭打印功能
    ```c
    #define SET_LOG_LEVEL            LOG_LEVEL_NONE
    ```