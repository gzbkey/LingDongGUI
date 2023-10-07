# 控件列表
| 状态 | 名称 | 说明 |
| :----:| ---- | ---- |
| ✅ | window | 窗体，多用于分层 |
| ✅ | image | 图片，用工具生成图片数据，支持png格式 |
| ✅ | button | 按键 |
| ✅ | text | 文本 |
| ✅ | progress bar | 进度条，支持图片移动形成动画效果 |
| ✅ | check box | 复选框，支持自定义图片和文字显示 |
| ✅ | radia menu | 旋转菜单 |
| 🔲 | number | 数字显示 |
| 🔲 | radio button | 单选按键 |
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

# 如何测试

使用qt进行电脑模拟测试，路径为examples\qt_sdl

[详细使用说明](./examples/qt_sdl/README.md)

# 如何生产图片字模数据

tools文件夹中有工具生成数据

[详细使用说明](./tools/README.md)


# 如何添加自定义控件

1、运行python脚本，src/gui/createWidget.py，自动创控件文件

2、添加刚才新建的文件到项目中

3、修改ldCommon.h中的控件类型,ldWidgetType

4、ldGui.c中添加新控件的头文件

5、修改ldGui.c中的函数，ldGuiDelWidget，_widgetLoop

6、修改新控件的.c .h文件

# 技术交流

QQ群：187033407



