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



