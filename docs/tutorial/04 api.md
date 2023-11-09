## Button
### 简述
button widget
### 函数列表
### 信号列表
### 函数说明
---
## CheckBox
### 简述
check box widget
### 函数列表
### 信号列表
### 函数说明
---
## Common
### 简述
通用函数文件
### 函数列表
### 信号列表
### 函数说明
---
## DateTime
### 简述
date time widget
### 函数列表
### 信号列表
### 函数说明
---
## IconSlider
### 简述
icon slider widget
### 函数列表
### 信号列表
### 函数说明
---
## Image
### 简述
image widget
### 函数列表
### 信号列表
### 函数说明
---
## Label
### 简述
label widget
### 函数列表
### 信号列表
### 函数说明
---
## ProgressBar
### 简述
progress bar widget
### 函数列表
### 信号列表
### 函数说明
---
## RadialMenu
### 简述
radial menu widget
### 函数列表
### 信号列表
### 函数说明
---
## ScrollSelecter
### 简述
scroll selecter widget
### 函数列表
### 信号列表
### 函数说明
---
## Text
### 简述
text widget
### 函数列表
### 信号列表
### 函数说明
---
## Window
### 简述
window widget 窗体控件可包含其他的控件，实现复合型控件或者控件组合模块
### 函数列表
* ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
* void ldWindowSetTransparent(ldWindow_t *pWidget,bool isTransparent);
### 信号列表
### 函数说明
##### ldWindowInit
```c
原型
    ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
说明
    window初始化函数

参数
    nameId              控件id
    parentNameId        父控件id
    x                   相对坐标x轴
    y                   相对坐标y轴
    width               控件宽度
    height              控件高度

返回
    ldWindow_t*         新控件指针
```

##### ldWindowSetTransparent
```c
原型
    void ldWindowSetTransparent(ldWindow_t *pWidget,bool isTransparent);
说明
    window控件设置透明

参数
    pWidget             window控件指针
    isTransparent       true=透明 false=不透明

```

---
