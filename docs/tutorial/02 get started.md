## 说明

例子在examples目录中，分为电脑端模拟(sdl)项目和芯片项目。需要简单体验运行效果的，可以选择sdl项目。对于新手推荐使用qt版本sdl，只需要傻瓜式安装qt（离线安装包），即可将项目运行起来。

### sdl

qt sdl 电脑端模拟运行，安装简单，小白首选。

vscode sdl 电脑端模拟运行，开放性强，大佬首选，配置复杂。

路径为[examples/sdl](../../examples/sdl)

📖 [使用说明](../../examples/sdl/README.md)

### mh2103c

该项目为ARM Cortex-M3内核，基于keil构建的例子。

移植请参考 [移植说明](./03%20porting.md)

### 其他

如果使用Cortex-M系列以外的芯片，无论是arm9、riscv都需要手动移植arm2d。

移植并不复杂，可以参照sdl项目。其中关键点是要适配math、dsp文件。
