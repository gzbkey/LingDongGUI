## 说明

例子在examples目录中，分为电脑端模拟(sdl)项目和芯片项目。需要简单体验运行效果的，可以选择sdl项目。对于新手推荐使用qt版本sdl，只需要傻瓜式安装qt（离线安装包），即可将项目运行起来。

### sdl

qt sdl 电脑端模拟运行，安装简单，小白首选。

vscode sdl 电脑端模拟运行，开放性强，大佬首选，配置复杂。

路径为[examples/sdl](../../examples/sdl)

📖 [使用说明](../../examples/sdl/README.md)

### arm mcu

即为Cortex-M系列单片机。

一般使用keil项目，必须先确保已经配置好arm-2d项目，并且能运行arm-2d的默认demo。

移植请参考 [移植说明](./03%20porting.md)

ldgui的keil pack，勾选对应example

配置uiDemo.h

参考以下代码运行demo
~~~c
    #include "uiDemo.h"

    int main(void)
    {
        sysInit();

        LD_ADD_PAGE_DEMO;

        arm_irq_safe {
            arm_2d_init();
        }

        disp_adapter0_init();

        arm_2d_scene0_init(&DISP0_ADAPTER);

        while(1)
        {
            disp_adapter0_task();
        }
    }
~~~

### 其他

如果使用Cortex-M系列以外的芯片，无论是arm9、riscv都需要手动移植arm2d。

移植并不复杂，可以参照sdl项目。其中关键点是要适配math、dsp文件。

### 如何生产图片字模数据

tools文件夹中有工具生成数据

📖 [使用说明](../../tools/README.md)