## 如何添加自定义控件

1、运行python脚本，[src/gui/createWidget.py](../../src/gui/createWidget.py)，输入控件名称后，自动创建控件文件和自动修改代码，例如输入checkBox，则新建ldCheckBox.c、ldCheckBox.h

2、添加刚才新建的.c .h文件到项目中(新文件在./src/gui中)

3、修改新控件的.c .h文件

---
自动处理：

修改ldCommon.h中的控件类型,ldWidgetType

ldGui.c中添加新控件的头文件

修改ldGui.c中的函数，ldGuiDelWidget，_widgetLoop

---