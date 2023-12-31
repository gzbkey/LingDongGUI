<h1 align="center" style="margin: 30px 0 30px; font-weight: bold;">灵动GUI</h1>
<h4 align="center">一个基于ARM-2D的GUI</h4>
<p align="center">
	<a href="https://gitee.com/gzbkey/LingDongGUI/stargazers"><img src="https://gitee.com/gzbkey/LingDongGUI/badge/star.svg"></a>
	<a href="https://gitee.com/gzbkey/LingDongGUI/members"><img src="https://gitee.com/gzbkey/LingDongGUI/badge/fork.svg"></a>
	<a><img src="https://img.shields.io/github/license/gzbkey/LingDongGUI"></a>
	<a><img src="https://img.shields.io/badge/architecture-ARM%20|%20RISC%20V-blue"></a>	
</p>

## 特性

1. **各种芯片通用**
    * 支持ARM单片机、RISC-V单片机

2. **简单易用**
    * 对ARM-2D进行二次封装，极大降低ARM-2D的使用难度
    * 使用类似Qt的信号槽模式，轻松实现多对多触发
    * 界面代码和功能代码分离，逻辑清晰

3. **自由扩展控件**
    * 使用脚本自动生成新控件框架，只需实现显示效果

4. **内置便捷功能**
    * 软件定时器
    * 多功能按键，支持实体按键和触摸按键
    * 队列
    * 彩色log打印

|ℹ️ Note|
|:----|
|可以通过模拟器在PC上运行，解除了只能在Cortex-M系列单片机上运行的限制，理论上可以在任意支持C语言的平台运行。|

## 源码

🏠️主仓库: https://gitee.com/gzbkey/LingDongGUI

🏠️镜像仓库: https://github.com/gzbkey/LingDongGUI


## 控件列表

| 状态 | 名称 | 说明 |
| :----:| ---- | ---- |
| ✅ | window | 窗体，多用于分层 |
| ✅ | image | 图片，用工具生成图片数据，支持png格式 |
| ✅ | button | 按键 |
| ✅ | text | 文本 |
| ✅ | progress bar | 进度条，支持图片移动形成动画效果 |
| ✅ | check box + radio button| 复选框 + 单选功能，支持自定义图片和文字显示 |
| ✅ | radia menu | 旋转菜单 |
| ✅ | label | 简单文本显示 |
| ✅ | scroll selecter | 滚动选择器 |
| ✅ | date time | 日期和时间 |
| ✅ | icon slider | 滑动图标 |
| ✅ | gauge | 仪表盘 |
| ✅ | qr code | 二维码 |
| ✅ | table | 表格 |
| ✅ | keyboard | 键盘 |
| ✅ | line edit | 编辑框 |
| ✅ | graph | 波形图 |
| ✅ | combo box | 下拉框 |
| ✅ | arc bar | 圆环进度条 |

## 文件结构
|名称|类型|说明|
|---|---|---|
|docs|文件夹|详细说明文档|
|examples|文件夹|例子项目所在文件夹|
|src|文件夹|源码文件|
|tools|文件夹|配套使用的相关工具|
|README|.md|项目简介文档|
|LICENSE|License|Apache 2.0开源协议文件|

## 教程

📖 [详细教程(目录)](./docs/tutorial)

* 🏷️[介绍](./docs/tutorial/01%20introduction.md)
* 🚀[开始使用](./docs/tutorial/02%20get%20started.md)
* ⚙️[移植](./docs/tutorial/03%20porting.md)
* 🔗[函数接口](./docs/tutorial/04%20api.md)
* 🛠️[开发](./docs/tutorial/05%20development.md)

## 技术交流

🐧 QQ群：187033407



