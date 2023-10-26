<h1 align="center" style="margin: 30px 0 30px; font-weight: bold;">灵动GUI</h1>
<h4 align="center">一个基于ARM-2D的GUI</h4>
<p align="center">
	<a href="https://gitee.com/gzbkey/LingDongGUI/stargazers"><img src="https://gitee.com/gzbkey/LingDongGUI/badge/star.svg"></a>
	<a href="https://gitee.com/gzbkey/LingDongGUI/members"><img src="https://gitee.com/gzbkey/LingDongGUI/badge/fork.svg"></a>
	<a><img src="https://img.shields.io/github/license/gzbkey/LingDongGUI"></a>
</p>

## 简介

* 灵动GUI是基于ARM-2D进行开发的GUI，极大降低ARM-2D的使用难度

* 支持ARM单片机、RISCV单片机

* 使用类似Qt的信号槽模式

* 界面代码和逻辑代码分离

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
| 🔲 | gauge | 仪表盘 |
| 🔲 | list | 列表 |
| 🔲 | graph | 波形图 |
| 🔲 | line edit | 编辑框 |
| 🔲 | arc bar | 圆环进度条 |
| 🔲 | date time | 日期和时间 |
| 🔲 | qr code | 二维码 |
| 🔲 | icon slider | 滑动图标 |
| 🔲 | number keyboard | 数字键盘 |
| 🔲 | qwerty keyboard | 英文键盘 |

## 如何测试

### Qt

使用qt进行电脑模拟测试，路径为[examples/qt_sdl](./examples/qt_sdl)

📖 [使用说明](./examples/qt_sdl/README.md)

### VSCode

使用vscode进行电脑模拟测试，路径为[examples/vscode_sdl](./examples/vscode_sdl)

📖 [使用说明](./examples/vscode_sdl/README.md)

## 如何生产图片字模数据

tools文件夹中有工具生成数据

📖 [使用说明](./tools/README.md)


## 如何添加自定义控件

1、运行python脚本，[src/gui/createWidget.py](./src/gui/createWidget.py)，输入控件名称后，自动创控件名称文件和自动修改代码

2、添加刚才新建的文件到项目中(新文件在./src/gui中)

3、修改新控件的.c .h文件

---
自动处理：

修改ldCommon.h中的控件类型,ldWidgetType

ldGui.c中添加新控件的头文件

修改ldGui.c中的函数，ldGuiDelWidget，_widgetLoop

---

## 技术交流

🐧 QQ群：187033407



