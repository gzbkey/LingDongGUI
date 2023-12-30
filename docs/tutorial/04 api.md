# Arc
### 简述
圆环控件
### 函数列表
* ldArc_t *ldArcInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint32_t srcQuarterAddr,uint32_t maskQuarterAddr,ldColor parentColor);
* void ldArcSetBgAngle(ldArc_t *pWidget,float bgStart,float bgEnd);
* void ldArcSetFgAngle(ldArc_t *pWidget,float fgEnd);
* void ldArcSetRotationAngle(ldArc_t *pWidget,float rotationAngle);
* void ldArcSetColor(ldArc_t *pWidget,ldColor bgColor,ldColor fgColor);
### 信号列表
### 函数说明
#### ldArcInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldArc_t *ldArcInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint32_t srcQuarterAddr,uint32_t maskQuarterAddr,ldColor parentColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    圆环控件的初始化函数 <br>    圆环素材尺寸建议为单数的像素点，<br>    例如101x101的圆环，裁剪51x51的左上角图片作为素材        </td>
    </tr>
    <tr>
        <td rowspan="9">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>srcQuarterAddr</td>
        <td>圆环素材左上角(四分之一),mask A8</td>
    </tr>
    <tr>
        <td>maskQuarterAddr</td>
        <td>圆环素材左上角(四分之一),mask A8，遮挡区域比src的稍大</td>
    </tr>
    <tr>
        <td>parentColor</td>
        <td>背景颜色</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldArc_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldArcSetBgAngle
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldArcSetBgAngle(ldArc_t *pWidget,float bgStart,float bgEnd);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设定底层圆环角度范围        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgStart</td>
        <td>底层圆环起始角度</td>
    </tr>
    <tr>
        <td>bgEnd</td>
        <td>底层圆环结束角度</td>
    </tr>
</table>
<br>

#### ldArcSetFgAngle
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldArcSetFgAngle(ldArc_t *pWidget,float fgEnd);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设定顶层圆环角度范围        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>fgEnd</td>
        <td>顶层圆环结束角度(1-359)<br>                         必须在底层圆环角度范围内<br>                         设定为0则屏蔽顶层圆环</td>
    </tr>
</table>
<br>

#### ldArcSetRotationAngle
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldArcSetRotationAngle(ldArc_t *pWidget,float rotationAngle);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设定圆环整体旋转角度        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>rotationAngle</td>
        <td>旋转角度，0-359</td>
    </tr>
</table>
<br>

#### ldArcSetColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldArcSetColor(ldArc_t *pWidget,ldColor bgColor,ldColor fgColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设定圆环颜色        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>底层颜色</td>
    </tr>
    <tr>
        <td>fgColor</td>
        <td>顶层颜色</td>
    </tr>
</table>
<br>

---
# Button
### 简述
button widget
### 函数列表
* ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
* void ldButtonSetText(ldButton_t* pWidget,uint8_t *pStr);
* void ldButtonSetColor(ldButton_t* pWidget,ldColor releaseColor,ldColor pressColor);
* void ldButtonSetImage(ldButton_t* pWidget,uint32_t releaseImgAddr,bool isReleaseMask,uint32_t pressImgAddr,bool isPressMask);
* void ldButtonSetSelectImage(ldButton_t* pWidget,uint32_t selectMaskAddr,ldColor selectColor);
* void ldButtonSetTextColor(ldButton_t* pWidget,ldColor charColor);
* void ldButtonSetTransparent(ldButton_t* pWidget,bool isTransparent);
* void ldButtonSetRoundCorner(ldButton_t* pWidget,bool isCorner);
* void ldButtonSetSelect(ldButton_t* pWidget,bool isSelected);
* void ldButtonSetAlign(ldButton_t *pWidget,uint8_t align);
* void ldButtonSetFont(ldButton_t *pWidget,ldFontDict_t *pFontDict);
### 信号列表
* SIGNAL_PRESS
* SIGNAL_RELEASE
### 函数说明
#### ldButtonInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldButton_t* ldButtonInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    button初始化函数        </td>
    </tr>
    <tr>
        <td rowspan="6">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldButton_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldButtonSetText
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetText(ldButton_t* pWidget,uint8_t *pStr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置按键显示文本        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pStr</td>
        <td>字符串指针</td>
    </tr>
</table>
<br>

#### ldButtonSetColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetColor(ldButton_t* pWidget,ldColor releaseColor,ldColor pressColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置按键颜色，设置该函数后，图片设置无效        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>releaseColor</td>
        <td>松开显示的颜色</td>
    </tr>
    <tr>
        <td>pressColor</td>
        <td>按下显示的颜色</td>
    </tr>
</table>
<br>

#### ldButtonSetImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetImage(ldButton_t* pWidget,uint32_t releaseImgAddr,bool isReleaseMask,uint32_t pressImgAddr,bool isPressMask);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置按键图片，设置该函数后，颜色设置无效        </td>
    </tr>
    <tr>
        <td rowspan="5">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>releaseImgAddr</td>
        <td>松开显示的图片</td>
    </tr>
    <tr>
        <td>isReleaseMask</td>
        <td>松开显示的图片是否带透明度(蒙版)</td>
    </tr>
    <tr>
        <td>pressImgAddr</td>
        <td>按下显示的图片</td>
    </tr>
    <tr>
        <td>isPressMask</td>
        <td>按下显示的图片是否带透明度(蒙版)</td>
    </tr>
</table>
<br>

#### ldButtonSetSelectImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetSelectImage(ldButton_t* pWidget,uint32_t selectMaskAddr,ldColor selectColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    选中按键的选中框(图片)        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>selectMaskAddr</td>
        <td>选择效果显示的图片(蒙版)</td>
    </tr>
    <tr>
        <td>selectColor</td>
        <td>显示的颜色</td>
    </tr>
</table>
<br>

#### ldButtonSetTextColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetTextColor(ldButton_t* pWidget,ldColor charColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置文字颜色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>charColor</td>
        <td>文字颜色</td>
    </tr>
</table>
<br>

#### ldButtonSetTransparent
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetTransparent(ldButton_t* pWidget,bool isTransparent);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    按键设置为透明，则不显示，但按下有按键效果 <br>    例如触摸某图片的左右两侧后，会切换图片，<br>    则可以使用功能两个透明按键放置图片顶层的两侧，<br>    即可实现该功能        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isTransparent</td>
        <td>true=透明 false=不透明</td>
    </tr>
</table>
<br>

#### ldButtonSetRoundCorner
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetRoundCorner(ldButton_t* pWidget,bool isCorner);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    按键实现圆角显示效果        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isCorner</td>
        <td>true=圆角 false=方角</td>
    </tr>
</table>
<br>

#### ldButtonSetSelect
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetSelect(ldButton_t* pWidget,bool isSelected);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    选择按键，触发选中的显示效果        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isSelected</td>
        <td>true=选中 false=不选中</td>
    </tr>
</table>
<br>

#### ldButtonSetAlign
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetAlign(ldButton_t *pWidget,uint8_t align);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    文本的对齐方式        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>align</td>
        <td>LD_ALIGN_CENTER<br>                         LD_ALIGN_TOP<br>                         LD_ALIGN_BOTTOM<br>                         LD_ALIGN_LEFT<br>                         LD_ALIGN_RIGHT</td>
    </tr>
</table>
<br>

#### ldButtonSetFont
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonSetFont(ldButton_t *pWidget,ldFontDict_t *pFontDict);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置字体        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体指针</td>
    </tr>
</table>
<br>

---
# CheckBox
### 简述
复选框 + 单选功能，支持自定义图片和文字显示
### 函数列表
* ldCheckBox_t *ldCheckBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
* void ldCheckBoxSetColor(ldCheckBox_t* pWidget,ldColor bgColor,ldColor fgColor);
* void ldCheckBoxSetImage(ldCheckBox_t* pWidget,uint16_t boxWidth,uint32_t uncheckedImgAddr,bool isUncheckedMask,uint32_t checkedImgAddr,bool isCheckedMask);
* void ldCheckBoxSetText(ldCheckBox_t* pWidget,ldFontDict_t *pFontDict,uint8_t *pStr);
* void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* pWidget,uint8_t num);
* void ldCheckBoxSetCorner(ldCheckBox_t* pWidget,bool isCorner);
* void ldCheckBoxSetCharColor(ldCheckBox_t* pWidget,ldColor charColor);
### 信号列表
### 函数说明
#### ldCheckBoxInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldCheckBox_t *ldCheckBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    check                box初始化函数        </td>
    </tr>
    <tr>
        <td rowspan="6">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldCheckBox_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldCheckBoxSetColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldCheckBoxSetColor(ldCheckBox_t* pWidget,ldColor bgColor,ldColor fgColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设定颜色        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>背景颜色</td>
    </tr>
    <tr>
        <td>fgColor</td>
        <td>前景颜色</td>
    </tr>
</table>
<br>

#### ldCheckBoxSetImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldCheckBoxSetImage(ldCheckBox_t* pWidget,uint16_t boxWidth,uint32_t uncheckedImgAddr,bool isUncheckedMask,uint32_t checkedImgAddr,bool isCheckedMask);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设定图片，只能设定方型图片        </td>
    </tr>
    <tr>
        <td rowspan="6">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>boxWidth</td>
        <td>图片宽度</td>
    </tr>
    <tr>
        <td>uncheckedImgAddr</td>
        <td>未选中的显示图片</td>
    </tr>
    <tr>
        <td>isUncheckedMask</td>
        <td>未选中图片是否带透明度</td>
    </tr>
    <tr>
        <td>checkedImgAddr</td>
        <td>选中的显示图片</td>
    </tr>
    <tr>
        <td>isCheckedMask</td>
        <td>选中图片是否带透明度</td>
    </tr>
</table>
<br>

#### ldCheckBoxSetText
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldCheckBoxSetText(ldCheckBox_t* pWidget,ldFontDict_t *pFontDict,uint8_t *pStr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置显示文字        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pStr</td>
        <td>需要显示的字符串</td>
    </tr>
</table>
<br>

#### ldCheckBoxSetRadioButtonGroup
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* pWidget,uint8_t num);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    单选功能设定为同一组 <br>    实现同一组的radio button自动单选        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>num</td>
        <td>组号 0-255</td>
    </tr>
</table>
<br>

#### ldCheckBoxSetCorner
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldCheckBoxSetCorner(ldCheckBox_t* pWidget,bool isCorner);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    实现圆角显示效果        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isCorner</td>
        <td>true=圆角 false=方角</td>
    </tr>
</table>
<br>

#### ldCheckBoxSetCharColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldCheckBoxSetCharColor(ldCheckBox_t* pWidget,ldColor charColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置文本颜色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>charColor</td>
        <td>文本颜色</td>
    </tr>
</table>
<br>

---
# ComboBox
### 简述
下拉框控件
### 函数列表
* ldComboBox_t *ldComboBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t* pFontDict,uint8_t itemMax);
* void ldComboBoxAddItem(ldComboBox_t* pWidget,uint8_t *pStr);
* void ldComboBoxSetCorner(ldComboBox_t* pWidget,bool isCorner);
### 信号列表
### 函数说明
#### ldComboBoxInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldComboBox_t *ldComboBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t* pFontDict,uint8_t itemMax);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    下拉框控件初始化        </td>
    </tr>
    <tr>
        <td rowspan="8">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体指针</td>
    </tr>
    <tr>
        <td>itemMax</td>
        <td>项目最大数量</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldComboBox_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldComboBoxAddItem
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldComboBoxAddItem(ldComboBox_t* pWidget,uint8_t *pStr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    添加项目        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pStr</td>
        <td>项目显示字符串</td>
    </tr>
</table>
<br>

#### ldComboBoxSetCorner
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldComboBoxSetCorner(ldComboBox_t* pWidget,bool isCorner);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    实现圆角显示效果        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isCorner</td>
        <td>true=圆角 false=方角</td>
    </tr>
</table>
<br>

---
# Common
### 简述
通用函数文件
### 函数列表
* void ldBaseDrawLine0(arm_2d_tile_t *pTile,int16_t x0 , int16_t y0 , int16_t x1 , int16_t y1,ldColor color);
### 信号列表
### 函数说明
#### ldBaseDrawLine0
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldBaseDrawLine0(arm_2d_tile_t *pTile,int16_t x0 , int16_t y0 , int16_t x1 , int16_t y1,ldColor color);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    吴小林抗锯齿直线算法        </td>
    </tr>
    <tr>
        <td rowspan="6">参数</td>
        <td>pTile</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>x0</td>
        <td>直线起始坐标x</td>
    </tr>
    <tr>
        <td>y0</td>
        <td>直线起始坐标y</td>
    </tr>
    <tr>
        <td>x1</td>
        <td>直线结束坐标x</td>
    </tr>
    <tr>
        <td>y1</td>
        <td>直线结束坐标y</td>
    </tr>
    <tr>
        <td>color</td>
        <td>直线颜色</td>
    </tr>
</table>
<br>

---
# DateTime
### 简述
date time widget
### 函数列表
* ldDateTime_t *ldDateTimeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t* pFontDict);
* void ldDateTimeSetTransparent(ldDateTime_t* pWidget,bool isTransparent);
* void ldDateTimeSetFormat(ldDateTime_t* pWidget,uint8_t *pStr);
* void ldDateTimeSetTextColor(ldDateTime_t* pWidget,ldColor charColor);
* void ldDateTimeSetAlign(ldDateTime_t *pWidget,uint8_t align);
* void ldDateTimeSetBgColor(ldDateTime_t *pWidget, ldColor bgColor);
* void ldDateTimeSetOpacity(ldDateTime_t *pWidget, uint8_t opacity);
* void ldDateTimeSetDate(ldDateTime_t *pWidget, uint16_t year, uint8_t month, uint8_t day);
* void ldDateTimeSetTime(ldDateTime_t *pWidget, uint8_t hour, uint8_t minute, uint8_t second);
### 信号列表
### 函数说明
#### ldDateTimeInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldDateTime_t *ldDateTimeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t* pFontDict);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    日期时间控件初始化        </td>
    </tr>
    <tr>
        <td rowspan="7">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体指针</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldDateTime_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldDateTimeSetTransparent
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldDateTimeSetTransparent(ldDateTime_t* pWidget,bool isTransparent);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    控件设置透明        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isTransparent</td>
        <td>true=透明 false=不透明</td>
    </tr>
</table>
<br>

#### ldDateTimeSetFormat
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldDateTimeSetFormat(ldDateTime_t* pWidget,uint8_t *pStr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置时间日期格式        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pStr</td>
        <td>年:yyyy 月:mm 日:dd 时:hh 分:nn 秒:ss<br>                         例子1: yyyy-mm-dd hh:nn:ss<br>                         例子2: yyyy年mm月dd日 hh时nn分ss秒</td>
    </tr>
</table>
<br>

#### ldDateTimeSetTextColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldDateTimeSetTextColor(ldDateTime_t* pWidget,ldColor charColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置文本颜色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>charColor</td>
        <td>文本颜色</td>
    </tr>
</table>
<br>

#### ldDateTimeSetAlign
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldDateTimeSetAlign(ldDateTime_t *pWidget,uint8_t align);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    文本的对齐方式        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>align</td>
        <td>LD_ALIGN_CENTER<br>                         LD_ALIGN_TOP<br>                         LD_ALIGN_BOTTOM<br>                         LD_ALIGN_LEFT<br>                         LD_ALIGN_RIGHT</td>
    </tr>
</table>
<br>

#### ldDateTimeSetBgColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldDateTimeSetBgColor(ldDateTime_t *pWidget, ldColor bgColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置底色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>底色</td>
    </tr>
</table>
<br>

#### ldDateTimeSetOpacity
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldDateTimeSetOpacity(ldDateTime_t *pWidget, uint8_t opacity);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置不透明度        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>opacity</td>
        <td>0(透明)-255(不透明)</td>
    </tr>
</table>
<br>

#### ldDateTimeSetDate
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldDateTimeSetDate(ldDateTime_t *pWidget, uint16_t year, uint8_t month, uint8_t day);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置日期        </td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>year</td>
        <td>年</td>
    </tr>
    <tr>
        <td>month</td>
        <td>月</td>
    </tr>
    <tr>
        <td>day</td>
        <td>日</td>
    </tr>
</table>
<br>

#### ldDateTimeSetTime
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldDateTimeSetTime(ldDateTime_t *pWidget, uint8_t hour, uint8_t minute, uint8_t second);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置时间        </td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>hour</td>
        <td>时</td>
    </tr>
    <tr>
        <td>minute</td>
        <td>分</td>
    </tr>
    <tr>
        <td>second</td>
        <td>秒</td>
    </tr>
</table>
<br>

---
# Gauge
### 简述
仪表盘控件
### 函数列表
* ldGauge_t *ldGaugeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint32_t bgImgAddr,bool isBgMask);
* void ldGaugeSetPointerImage(ldGauge_t *pWidget,uint32_t pointerImgAddr,int16_t pointerWidth,int16_t pointerHeight,int16_t pointerOriginOffsetX,int16_t pointerOriginOffsetY);
* void ldGaugeSetCenterOffset(ldGauge_t *pWidget, int16_t centreOffsetX, int16_t centreOffsetY);
* void ldGaugeSetAngle(ldGauge_t *pWidget, float angle);
* void ldGaugeSetPointerImageType(ldGauge_t *pWidget,gaugePointerType_t pointerImgType,ldColor keyingOrMaskColor);
### 信号列表
### 函数说明
#### ldGaugeInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldGauge_t *ldGaugeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint32_t bgImgAddr,bool isBgMask);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    gauge初始化函数        </td>
    </tr>
    <tr>
        <td rowspan="6">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldGauge_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldGaugeSetPointerImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGaugeSetPointerImage(ldGauge_t *pWidget,uint32_t pointerImgAddr,int16_t pointerWidth,int16_t pointerHeight,int16_t pointerOriginOffsetX,int16_t pointerOriginOffsetY);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置指针图片        </td>
    </tr>
    <tr>
        <td rowspan="6">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pointerImgAddr</td>
        <td>指针图片地址</td>
    </tr>
    <tr>
        <td>pointerWidth</td>
        <td>指针宽度</td>
    </tr>
    <tr>
        <td>pointerHeight</td>
        <td>指针高度</td>
    </tr>
    <tr>
        <td>pointerOriginOffsetX</td>
        <td>指针原点坐标x轴(相对本指针图片)</td>
    </tr>
    <tr>
        <td>pointerOriginOffsetY</td>
        <td>指针原点坐标y轴(相对本指针图片)</td>
    </tr>
</table>
<br>

#### ldGaugeSetCenterOffset
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGaugeSetCenterOffset(ldGauge_t *pWidget, int16_t centreOffsetX, int16_t centreOffsetY);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置仪表盘的旋转中心点        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>centreOffsetX</td>
        <td>中心点坐标x轴</td>
    </tr>
    <tr>
        <td>centreOffsetY</td>
        <td>中心点坐标y轴</td>
    </tr>
</table>
<br>

#### ldGaugeSetAngle
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGaugeSetAngle(ldGauge_t *pWidget, float angle);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    仪表盘的指针显示角度        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>angle</td>
        <td>旋转角度，小数点后1位有效</td>
    </tr>
</table>
<br>

#### ldGaugeSetPointerImageType
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGaugeSetPointerImageType(ldGauge_t *pWidget,gaugePointerType_t pointerImgType,ldColor keyingOrMaskColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    指针图片的素材类型        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pointerImgType</td>
        <td>nomal,withMask,onlyMask,keying</td>
    </tr>
    <tr>
        <td>keyingOrMaskColor</td>
        <td>颜色</td>
    </tr>
</table>
<br>

---
# Graph
### 简述
曲线图控件
### 函数列表
* ldGraph_t *ldGraphInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height ,uint8_t seriesMax);
* void ldGraphSetPointImageMask(ldGraph_t *pWidget,uint32_t addr,int16_t width);
* int8_t ldGraphAddSeries(ldGraph_t *pWidget,ldColor seriesColor,uint8_t lineSize,uint16_t valueCountMax);
* void ldGraphSetValue(ldGraph_t *pWidget,uint8_t seriesNum,uint16_t valueNum,uint16_t value);
* void ldGraphSetAxis(ldGraph_t *pWidget,uint16_t xAxis,uint16_t yAxis,uint16_t xAxisOffset);
* void ldGraphSetFrameSpace(ldGraph_t *pWidget,uint8_t frameSpace,bool isCorner);
* void ldGraphSetGridOffset(ldGraph_t *pWidget,uint8_t gridOffset);
* void ldGraphMoveAdd(ldGraph_t *pWidget,uint8_t seriesNum,uint16_t newValue);
### 信号列表
### 函数说明
#### ldGraphInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldGraph_t *ldGraphInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height ,uint8_t seriesMax);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    曲线图初始化        </td>
    </tr>
    <tr>
        <td rowspan="7">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>seriesMax</td>
        <td>曲线数量最大值</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldGraph_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldGraphSetPointImageMask
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGraphSetPointImageMask(ldGraph_t *pWidget,uint32_t addr,int16_t width);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置圆点mask图片        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>addr</td>
        <td>mask图片地址</td>
    </tr>
    <tr>
        <td>width</td>
        <td>图片宽度</td>
    </tr>
</table>
<br>

#### ldGraphAddSeries
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">int8_t ldGraphAddSeries(ldGraph_t *pWidget,ldColor seriesColor,uint8_t lineSize,uint16_t valueCountMax);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    添加曲线缓存        </td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>seriesColor</td>
        <td>曲线颜色</td>
    </tr>
    <tr>
        <td>lineSize</td>
        <td>线条大小：0=无线条，1-255</td>
    </tr>
    <tr>
        <td>valueCountMax</td>
        <td>储存数据数量，一般为:x轴最大值/x轴间隔</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>int8_t</td>
        <td></td>
    </tr>
</table>
<br>

#### ldGraphSetValue
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGraphSetValue(ldGraph_t *pWidget,uint8_t seriesNum,uint16_t valueNum,uint16_t value);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置曲线数据        </td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>seriesNum</td>
        <td>曲线序号</td>
    </tr>
    <tr>
        <td>valueNum</td>
        <td>数据序号</td>
    </tr>
    <tr>
        <td>value</td>
        <td>数据值</td>
    </tr>
</table>
<br>

#### ldGraphSetAxis
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGraphSetAxis(ldGraph_t *pWidget,uint16_t xAxis,uint16_t yAxis,uint16_t xAxisOffset);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置波形参考线        </td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>xAxis</td>
        <td>x轴数据最大值</td>
    </tr>
    <tr>
        <td>yAxis</td>
        <td>y轴数据最大值</td>
    </tr>
    <tr>
        <td>xAxisOffset</td>
        <td>x轴数据间隔</td>
    </tr>
</table>
<br>

#### ldGraphSetFrameSpace
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGraphSetFrameSpace(ldGraph_t *pWidget,uint8_t frameSpace,bool isCorner);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置边框间距        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>frameSpace</td>
        <td>间距值</td>
    </tr>
    <tr>
        <td>isCorner</td>
        <td>是否圆角</td>
    </tr>
</table>
<br>

#### ldGraphSetGridOffset
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGraphSetGridOffset(ldGraph_t *pWidget,uint8_t gridOffset);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置栅格间隔        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>gridOffset</td>
        <td>栅格间距</td>
    </tr>
</table>
<br>

#### ldGraphMoveAdd
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGraphMoveAdd(ldGraph_t *pWidget,uint8_t seriesNum,uint16_t newValue);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    将最新数据放到最后， <br>    数据达到最大值后，自动向后移动数据，自动丢弃最前边的数据<br>    实现波形自动刷新        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>seriesNum</td>
        <td>曲线号码</td>
    </tr>
    <tr>
        <td>newValue</td>
        <td>数据值</td>
    </tr>
</table>
<br>

---
# IconSlider
### 简述
icon slider widget
### 函数列表
* ldIconSlider_t *ldIconSliderInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,int16_t iconWidth,uint8_t iconSpace,uint8_t columnCount,uint8_t rowCount,uint8_t pageMax,ldFontDict_t* pFontDict);
* void ldIconSliderAddIcon(ldIconSlider_t *pWidget,uint32_t imageAddr,uint8_t* pNameStr);
* void ldIconSliderSetHorizontalScroll(ldIconSlider_t *pWidget,bool isHorizontal);
* void ldIconSliderSetSpeed(ldIconSlider_t *pWidget, uint8_t speed);
### 信号列表
* SIGNAL_CLICKED_ITEM
### 函数说明
#### ldIconSliderInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldIconSlider_t *ldIconSliderInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,int16_t iconWidth,uint8_t iconSpace,uint8_t columnCount,uint8_t rowCount,uint8_t pageMax,ldFontDict_t* pFontDict);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    图标滑动控件初始化        </td>
    </tr>
    <tr>
        <td rowspan="12">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>iconWidth</td>
        <td>图标宽度</td>
    </tr>
    <tr>
        <td>iconSpace</td>
        <td>图标间距</td>
    </tr>
    <tr>
        <td>columnCount</td>
        <td>列数</td>
    </tr>
    <tr>
        <td>rowCount</td>
        <td>行数</td>
    </tr>
    <tr>
        <td>pageMax</td>
        <td>页数</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体指针</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldIconSlider_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldIconSliderAddIcon
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldIconSliderAddIcon(ldIconSlider_t *pWidget,uint32_t imageAddr,uint8_t* pNameStr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    添加图标        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>imageAddr</td>
        <td>图片地址</td>
    </tr>
    <tr>
        <td>pNameStr</td>
        <td>图标名称字符串指针</td>
    </tr>
</table>
<br>

#### ldIconSliderSetHorizontalScroll
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldIconSliderSetHorizontalScroll(ldIconSlider_t *pWidget,bool isHorizontal);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    滑动方向        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isHorizontal</td>
        <td>true=水平滑动 false=纵向滑动</td>
    </tr>
</table>
<br>

#### ldIconSliderSetSpeed
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldIconSliderSetSpeed(ldIconSlider_t *pWidget, uint8_t speed);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置移动速度        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>speed</td>
        <td>速度值，最小值:1,最大值：控件宽度或者高度</td>
    </tr>
</table>
<br>

---
# Image
### 简述
image widget background、window、image共用控件
### 函数列表
* ldImage_t *ldImageInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint32_t imageAddr, bool isWithMask);
* void ldImageSetBgColor(ldImage_t *pWidget,ldColor bgColor);
* void ldImageSetOpacity(ldImage_t *pWidget, uint8_t opacity);
* void ldImageSetImage(ldImage_t *pWidget, uint32_t imageAddr, bool isWithMask);
### 信号列表
### 函数说明
#### ldImageInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldImage_t *ldImageInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint32_t imageAddr, bool isWithMask);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    图片初始化        </td>
    </tr>
    <tr>
        <td rowspan="8">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>imageAddr</td>
        <td>图片地址</td>
    </tr>
    <tr>
        <td>isWithMask</td>
        <td>图片是否带mask</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldImage_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldImageSetBgColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldImageSetBgColor(ldImage_t *pWidget,ldColor bgColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置背景色，window专用        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>背景颜色</td>
    </tr>
</table>
<br>

#### ldImageSetOpacity
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldImageSetOpacity(ldImage_t *pWidget, uint8_t opacity);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置不透明度        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>opacity</td>
        <td>0(透明)-255(不透明)</td>
    </tr>
</table>
<br>

#### ldImageSetImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldImageSetImage(ldImage_t *pWidget, uint32_t imageAddr, bool isWithMask);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置图片        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>imageAddr</td>
        <td>图片地址</td>
    </tr>
    <tr>
        <td>isWithMask</td>
        <td>是否带mask</td>
    </tr>
</table>
<br>

---
# Keyboard
### 简述
键盘控件，可以切换数字键盘和字母键盘
### 函数列表
* ldKeyboard_t *ldKeyboardInit(uint16_t nameId,ldFontDict_t *pFontDict);
### 信号列表
* SIGNAL_INPUT_ASCII
### 函数说明
#### ldKeyboardInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldKeyboard_t *ldKeyboardInit(uint16_t nameId,ldFontDict_t *pFontDict);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    键盘初始化        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>nameId</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体指针</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldKeyboard_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

---
# Label
### 简述
label widget
### 函数列表
* ldLabel_t *ldLabelInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict);
* void ldLabelSetTransparent(ldLabel_t* pWidget,bool isTransparent);
* void ldLabelSetText(ldLabel_t* pWidget,uint8_t *pStr);
* void ldLabelSetTextColor(ldLabel_t* pWidget,ldColor charColor);
* void ldLabelSetAlign(ldLabel_t *pWidget,uint8_t align);
* void ldLabelSetBgImage(ldLabel_t *pWidget, uint32_t imageAddr);
* void ldLabelSetBgColor(ldLabel_t *pWidget, ldColor bgColor);
* void ldLabelSetOpacity(ldLabel_t *pWidget, uint8_t opacity);
### 信号列表
### 函数说明
#### ldLabelInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldLabel_t *ldLabelInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    标签文本初始化        </td>
    </tr>
    <tr>
        <td rowspan="7">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体指针</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldLabel_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldLabelSetTransparent
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLabelSetTransparent(ldLabel_t* pWidget,bool isTransparent);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置透明        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isTransparent</td>
        <td>true=透明 false=不透明</td>
    </tr>
</table>
<br>

#### ldLabelSetText
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLabelSetText(ldLabel_t* pWidget,uint8_t *pStr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置文本        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pStr</td>
        <td>文本指针</td>
    </tr>
</table>
<br>

#### ldLabelSetTextColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLabelSetTextColor(ldLabel_t* pWidget,ldColor charColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置文本颜色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>charColor</td>
        <td>文本颜色</td>
    </tr>
</table>
<br>

#### ldLabelSetAlign
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLabelSetAlign(ldLabel_t *pWidget,uint8_t align);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    文本的对齐方式        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>align</td>
        <td>LD_ALIGN_CENTER<br>                         LD_ALIGN_TOP<br>                         LD_ALIGN_BOTTOM<br>                         LD_ALIGN_LEFT<br>                         LD_ALIGN_RIGHT</td>
    </tr>
</table>
<br>

#### ldLabelSetBgImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLabelSetBgImage(ldLabel_t *pWidget, uint32_t imageAddr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置背景图片        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>imageAddr</td>
        <td>图片地址</td>
    </tr>
</table>
<br>

#### ldLabelSetBgColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLabelSetBgColor(ldLabel_t *pWidget, ldColor bgColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置背景颜色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>背景颜色</td>
    </tr>
</table>
<br>

#### ldLabelSetOpacity
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLabelSetOpacity(ldLabel_t *pWidget, uint8_t opacity);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置不透明度        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>opacity</td>
        <td>0(透明)-255(不透明)</td>
    </tr>
</table>
<br>

---
# LineEdit
### 简述
输入框控件
### 函数列表
* ldLineEdit_t *ldLineEditInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t *pFontDict,uint8_t textMax);
* void ldLineEditSetText(ldLineEdit_t* pWidget,uint8_t *pText);
* void ldLineEditSetKeyboard(ldLineEdit_t* pWidget,uint16_t kbNameId);
* void ldLineEditSetType(ldLineEdit_t* pWidget,ldEditType_t editType);
### 信号列表
### 函数说明
#### ldLineEditInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldLineEdit_t *ldLineEditInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t *pFontDict,uint8_t textMax);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    输入框控件初始化        </td>
    </tr>
    <tr>
        <td rowspan="8">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体指针</td>
    </tr>
    <tr>
        <td>textMax</td>
        <td>字符串最大长度 0-255</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldLineEdit_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldLineEditSetText
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLineEditSetText(ldLineEdit_t* pWidget,uint8_t *pText);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置显示文本        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pText</td>
        <td>字符串指针</td>
    </tr>
</table>
<br>

#### ldLineEditSetKeyboard
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLineEditSetKeyboard(ldLineEdit_t* pWidget,uint16_t kbNameId);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    关联键盘        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>kbNameId</td>
        <td>目标键盘id</td>
    </tr>
</table>
<br>

#### ldLineEditSetType
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLineEditSetType(ldLineEdit_t* pWidget,ldEditType_t editType);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置输入框的数据类型        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>editType</td>
        <td>typeString、typeInt、typeFloat</td>
    </tr>
</table>
<br>

---
# ProgressBar
### 简述
progress bar widget
### 函数列表
* ldProgressBar_t *ldProgressBarInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);
* void ldProgressBarSetPercent(ldProgressBar_t *pWidget,float percent);
* void ldProgressBarSetBgImage(ldProgressBar_t *pWidget,uint32_t bgAddr,uint16_t bgWidth,bool isMove);
* void ldProgressBarSetFgImage(ldProgressBar_t *pWidget,uint32_t fgAddr,uint16_t fgWidth,bool isMove);
* void ldProgressBarSetFrameImage(ldProgressBar_t *pWidget,uint32_t frameAddr,uint16_t frameWidth);
* void ldProgressBarSetColor(ldProgressBar_t *pWidget,ldColor bgColor,ldColor fgColor,ldColor frameColor);
### 信号列表
### 函数说明
#### ldProgressBarInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldProgressBar_t *ldProgressBarInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    进度条初始化        </td>
    </tr>
    <tr>
        <td rowspan="6">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldProgressBar_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldProgressBarSetPercent
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldProgressBarSetPercent(ldProgressBar_t *pWidget,float percent);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置百分比        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>percent</td>
        <td>百分比</td>
    </tr>
</table>
<br>

#### ldProgressBarSetBgImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldProgressBarSetBgImage(ldProgressBar_t *pWidget,uint32_t bgAddr,uint16_t bgWidth,bool isMove);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置背景图片        </td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgAddr</td>
        <td>图片地址</td>
    </tr>
    <tr>
        <td>bgWidth</td>
        <td>图片宽度</td>
    </tr>
    <tr>
        <td>isMove</td>
        <td>图片是否滚动</td>
    </tr>
</table>
<br>

#### ldProgressBarSetFgImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldProgressBarSetFgImage(ldProgressBar_t *pWidget,uint32_t fgAddr,uint16_t fgWidth,bool isMove);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置前景图片        </td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>fgAddr</td>
        <td>图片地址</td>
    </tr>
    <tr>
        <td>fgWidth</td>
        <td>图片宽度</td>
    </tr>
    <tr>
        <td>isMove</td>
        <td>图片是否滚动</td>
    </tr>
</table>
<br>

#### ldProgressBarSetFrameImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldProgressBarSetFrameImage(ldProgressBar_t *pWidget,uint32_t frameAddr,uint16_t frameWidth);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置边框图片，计为最前边的遮挡图片        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>frameAddr</td>
        <td>图片地址</td>
    </tr>
    <tr>
        <td>frameWidth</td>
        <td>图片宽度</td>
    </tr>
</table>
<br>

#### ldProgressBarSetColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldProgressBarSetColor(ldProgressBar_t *pWidget,ldColor bgColor,ldColor fgColor,ldColor frameColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置颜色        </td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>背景颜色</td>
    </tr>
    <tr>
        <td>fgColor</td>
        <td>前景颜色</td>
    </tr>
    <tr>
        <td>frameColor</td>
        <td>边框颜色</td>
    </tr>
</table>
<br>

---
# QRCode
### 简述
qr code widget
### 函数列表
* ldQRCode_t *ldQRCodeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t* qrText, ldColor qrColor, ldColor bgColor, ldQRCodeEcc_t qrEcc, uint8_t qrMaxVersion, uint8_t qrZoom);
* void ldQRCodeSetText(ldQRCode_t *pWidget, uint8_t *pNewText);
### 信号列表
### 函数说明
#### ldQRCodeInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldQRCode_t *ldQRCodeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t* qrText, ldColor qrColor, ldColor bgColor, ldQRCodeEcc_t qrEcc, uint8_t qrMaxVersion, uint8_t qrZoom);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    二维码初始化函数        </td>
    </tr>
    <tr>
        <td rowspan="12">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>qrText</td>
        <td>二维码的内容文本</td>
    </tr>
    <tr>
        <td>qrColor</td>
        <td>二维码颜色</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>底色</td>
    </tr>
    <tr>
        <td>qrEcc</td>
        <td>eccLow 容忍错误7%<br>                         eccMedium 容忍错误15%<br>                         eccQuartile 容忍错误25%<br>                         eccHigh 容忍错误30%</td>
    </tr>
    <tr>
        <td>qrMaxVersion</td>
        <td>二维码版本，0-40，版本越高，尺寸越大</td>
    </tr>
    <tr>
        <td>qrZoom</td>
        <td>放大二维码倍数，1-255</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldQRCode_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldQRCodeSetText
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldQRCodeSetText(ldQRCode_t *pWidget, uint8_t *pNewText);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置二维码文本        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pNewText</td>
        <td>文本指针</td>
    </tr>
</table>
<br>

---
# RadialMenu
### 简述
radial menu widget
### 函数列表
* ldRadialMenu_t *ldRadialMenuInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint16_t xAxis, uint16_t yAxis, uint8_t itemMax);
* void ldRadialMenuAddItem(ldRadialMenu_t *pWidget,uint32_t imageAddr,uint16_t width,uint16_t height,uint8_t itemSubCount,bool isWithMask);
* void ldRadialMenuSelectItem(ldRadialMenu_t *pWidget,uint8_t num);
### 信号列表
### 函数说明
#### ldRadialMenuInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldRadialMenu_t *ldRadialMenuInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint16_t xAxis, uint16_t yAxis, uint8_t itemMax);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    旋转菜单初始化        </td>
    </tr>
    <tr>
        <td rowspan="9">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>xAxis</td>
        <td>x轴长度</td>
    </tr>
    <tr>
        <td>yAxis</td>
        <td>y轴长度</td>
    </tr>
    <tr>
        <td>itemMax</td>
        <td>项目数量最大值</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldRadialMenu_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldRadialMenuAddItem
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldRadialMenuAddItem(ldRadialMenu_t *pWidget,uint32_t imageAddr,uint16_t width,uint16_t height,uint8_t itemSubCount,bool isWithMask);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    添加项目        </td>
    </tr>
    <tr>
        <td rowspan="6">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>imageAddr</td>
        <td>图片地址</td>
    </tr>
    <tr>
        <td>width</td>
        <td>图片宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>图片高度</td>
    </tr>
    <tr>
        <td>itemSubCount</td>
        <td>子图片数量(用于动态视频效果)</td>
    </tr>
    <tr>
        <td>isWithMask</td>
        <td>图片是否带mask</td>
    </tr>
</table>
<br>

#### ldRadialMenuSelectItem
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldRadialMenuSelectItem(ldRadialMenu_t *pWidget,uint8_t num);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    选中项目        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>num</td>
        <td>项目编号，0开始</td>
    </tr>
</table>
<br>

---
# ScrollSelecter
### 简述
scroll selecter widget
### 函数列表
* ldScrollSelecter_t *ldScrollSelecterInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict, uint8_t itemMax);
* void ldScrollSelecterAddItem(ldScrollSelecter_t* pWidget,uint8_t *pStr);
* void ldScrollSelecterSetTextColor(ldScrollSelecter_t* pWidget,ldColor charColor);
* void ldScrollSelecterSetBgColor(ldScrollSelecter_t* pWidget,ldColor bgColor);
* void ldScrollSelecterSetBgImage(ldScrollSelecter_t* pWidget,uint32_t imgAddr);
* void ldScrollSelecterSetTransparent(ldScrollSelecter_t* pWidget,bool isTransparent);
* void ldScrollSelecterSetOpacity(ldScrollSelecter_t *pWidget, uint8_t opacity);
* void ldScrollSelecterSetSpeed(ldScrollSelecter_t *pWidget, uint8_t speed);
* void ldScrollSelecterSetItem(ldScrollSelecter_t *pWidget, uint8_t itemNum);
* void ldScrollSelecterSetAlign(ldScrollSelecter_t *pWidget,uint8_t align);
### 信号列表
### 函数说明
#### ldScrollSelecterInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldScrollSelecter_t *ldScrollSelecterInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict, uint8_t itemMax);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    滚动选择器初始化        </td>
    </tr>
    <tr>
        <td rowspan="8">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体指针</td>
    </tr>
    <tr>
        <td>itemMax</td>
        <td>项目数量最大值</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldScrollSelecter_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldScrollSelecterAddItem
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldScrollSelecterAddItem(ldScrollSelecter_t* pWidget,uint8_t *pStr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    添加项目内容        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pStr</td>
        <td>项目显示的字符串</td>
    </tr>
</table>
<br>

#### ldScrollSelecterSetTextColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldScrollSelecterSetTextColor(ldScrollSelecter_t* pWidget,ldColor charColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置文本颜色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>charColor</td>
        <td>文本颜色</td>
    </tr>
</table>
<br>

#### ldScrollSelecterSetBgColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldScrollSelecterSetBgColor(ldScrollSelecter_t* pWidget,ldColor bgColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置背景颜色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>背景颜色</td>
    </tr>
</table>
<br>

#### ldScrollSelecterSetBgImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldScrollSelecterSetBgImage(ldScrollSelecter_t* pWidget,uint32_t imgAddr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置背景图片        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>imgAddr</td>
        <td>背景图片地址</td>
    </tr>
</table>
<br>

#### ldScrollSelecterSetTransparent
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldScrollSelecterSetTransparent(ldScrollSelecter_t* pWidget,bool isTransparent);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置为背景透明        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isTransparent</td>
        <td>true=透明 false=不透明</td>
    </tr>
</table>
<br>

#### ldScrollSelecterSetOpacity
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldScrollSelecterSetOpacity(ldScrollSelecter_t *pWidget, uint8_t opacity);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置不透明度        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>opacity</td>
        <td>不透明度 0-255</td>
    </tr>
</table>
<br>

#### ldScrollSelecterSetSpeed
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldScrollSelecterSetSpeed(ldScrollSelecter_t *pWidget, uint8_t speed);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置移动速度        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>speed</td>
        <td>速度值，最小值:1,最大值：控件高度</td>
    </tr>
</table>
<br>

#### ldScrollSelecterSetItem
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldScrollSelecterSetItem(ldScrollSelecter_t *pWidget, uint8_t itemNum);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    选中项目        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>itemNum</td>
        <td>项目编号，0开始</td>
    </tr>
</table>
<br>

#### ldScrollSelecterSetAlign
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldScrollSelecterSetAlign(ldScrollSelecter_t *pWidget,uint8_t align);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置文字对齐方式        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>align</td>
        <td>LD_ALIGN_CENTER<br>                         LD_ALIGN_TOP<br>                         LD_ALIGN_BOTTOM<br>                         LD_ALIGN_LEFT<br>                         LD_ALIGN_RIGHT</td>
    </tr>
</table>
<br>

---
# Table
### 简述
表格控件
### 函数列表
* uint8_t ldTableCurrentColumn(ldTable_t *pWidget);
* uint8_t ldTableCurrentRow(ldTable_t *pWidget);
* ldTableItem_t *ldTableCurrentItem(ldTable_t *pWidget);
* ldTableItem_t *ldTableItem(ldTable_t *pWidget,uint8_t row, uint8_t column);
* ldTableItem_t *ldTableItemAt(ldTable_t *pWidget,int16_t x,int16_t y);
* ldTable_t *ldTableInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t rowCount, uint8_t columnCount, uint8_t itemSpace);
* void ldTableSetItemWidth(ldTable_t *pWidget,uint8_t column,int16_t width);
* void ldTableSetItemHeight(ldTable_t *pWidget,uint8_t row,int16_t height);
* void ldTableSetItemText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);
* void ldTableSetItemStaticText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);
* void ldTableSetItemColor(ldTable_t *pWidget,uint8_t row,uint8_t column,ldColor textColor,ldColor bgColor);
* void ldTableSetBgColor(ldTable_t *pWidget,ldColor bgColor);
* void ldTableSetItemAlign(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t align);
* void ldTableSetItemImage(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uint32_t imgAddr);
* void ldTableSetItemButton(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uint32_t releaseImgAddr,uint32_t pressImgAddr,bool isCheckable);
### 信号列表
### 函数说明
#### ldTableCurrentColumn
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">uint8_t ldTableCurrentColumn(ldTable_t *pWidget);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    获取选中列的编号        </td>
    </tr>
    <tr>
        <td rowspan="1">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>uint8_t</td>
        <td>列号</td>
    </tr>
</table>
<br>

#### ldTableCurrentRow
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">uint8_t ldTableCurrentRow(ldTable_t *pWidget);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    获取选中行的编号        </td>
    </tr>
    <tr>
        <td rowspan="1">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>uint8_t</td>
        <td>行号</td>
    </tr>
</table>
<br>

#### ldTableCurrentItem
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldTableItem_t *ldTableCurrentItem(ldTable_t *pWidget);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    获取选中行的项目        </td>
    </tr>
    <tr>
        <td rowspan="1">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldTableItem_t*</td>
        <td>返回目标item指针</td>
    </tr>
</table>
<br>

#### ldTableItem
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldTableItem_t *ldTableItem(ldTable_t *pWidget,uint8_t row, uint8_t column);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    获取指定的项目        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>row</td>
        <td>行号</td>
    </tr>
    <tr>
        <td>column</td>
        <td>列号</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldTableItem_t*</td>
        <td>返回目标item指针</td>
    </tr>
</table>
<br>

#### ldTableItemAt
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldTableItem_t *ldTableItemAt(ldTable_t *pWidget,int16_t x,int16_t y);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    获取指定坐标的项目        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>x</td>
        <td>全局坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>全局坐标y轴</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldTableItem_t*</td>
        <td>返回目标item指针</td>
    </tr>
</table>
<br>

#### ldTableInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldTable_t *ldTableInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t rowCount, uint8_t columnCount, uint8_t itemSpace);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    表格初始化        </td>
    </tr>
    <tr>
        <td rowspan="9">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>rowCount</td>
        <td>行数</td>
    </tr>
    <tr>
        <td>columnCount</td>
        <td>列数</td>
    </tr>
    <tr>
        <td>itemSpace</td>
        <td>间隔</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldTable_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldTableSetItemWidth
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableSetItemWidth(ldTable_t *pWidget,uint8_t column,int16_t width);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置指定列的宽度        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>column</td>
        <td>列</td>
    </tr>
    <tr>
        <td>width</td>
        <td>宽度</td>
    </tr>
</table>
<br>

#### ldTableSetItemHeight
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableSetItemHeight(ldTable_t *pWidget,uint8_t row,int16_t height);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置指定行的高度        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>row</td>
        <td>行</td>
    </tr>
    <tr>
        <td>height</td>
        <td>高度</td>
    </tr>
</table>
<br>

#### ldTableSetItemText
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableSetItemText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置项目的文本        </td>
    </tr>
    <tr>
        <td rowspan="5">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>row</td>
        <td>行</td>
    </tr>
    <tr>
        <td>column</td>
        <td>列</td>
    </tr>
    <tr>
        <td>pText</td>
        <td>文本内容</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体</td>
    </tr>
</table>
<br>

#### ldTableSetItemStaticText
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableSetItemStaticText(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t *pText,ldFontDict_t* pFontDict);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置项目的静态文本，不可变更的文本内容，不消耗内存        </td>
    </tr>
    <tr>
        <td rowspan="5">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>row</td>
        <td>行</td>
    </tr>
    <tr>
        <td>column</td>
        <td>列</td>
    </tr>
    <tr>
        <td>pText</td>
        <td>文本内容</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体</td>
    </tr>
</table>
<br>

#### ldTableSetItemColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableSetItemColor(ldTable_t *pWidget,uint8_t row,uint8_t column,ldColor textColor,ldColor bgColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置项目颜色        </td>
    </tr>
    <tr>
        <td rowspan="5">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>row</td>
        <td>行</td>
    </tr>
    <tr>
        <td>column</td>
        <td>列</td>
    </tr>
    <tr>
        <td>textColor</td>
        <td>文字颜色</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>文字背景颜色</td>
    </tr>
</table>
<br>

#### ldTableSetBgColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableSetBgColor(ldTable_t *pWidget,ldColor bgColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置表格底色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>bgColor</td>
        <td>底色</td>
    </tr>
</table>
<br>

#### ldTableSetItemAlign
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableSetItemAlign(ldTable_t *pWidget,uint8_t row,uint8_t column,uint8_t align);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置指定项目文本对齐方式        </td>
    </tr>
    <tr>
        <td rowspan="4">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>row</td>
        <td>行号</td>
    </tr>
    <tr>
        <td>column</td>
        <td>列号</td>
    </tr>
    <tr>
        <td>align</td>
        <td>对齐方式</td>
    </tr>
</table>
<br>

#### ldTableSetItemImage
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableSetItemImage(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uint32_t imgAddr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置项目图片        </td>
    </tr>
    <tr>
        <td rowspan="8">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>row</td>
        <td>行号</td>
    </tr>
    <tr>
        <td>column</td>
        <td>列号</td>
    </tr>
    <tr>
        <td>x</td>
        <td>图片在项目中的x轴坐标</td>
    </tr>
    <tr>
        <td>y</td>
        <td>图片在项目中的y轴坐标</td>
    </tr>
    <tr>
        <td>width</td>
        <td>图片宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>图片高度</td>
    </tr>
    <tr>
        <td>imgAddr</td>
        <td>图片地址</td>
    </tr>
</table>
<br>

#### ldTableSetItemButton
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableSetItemButton(ldTable_t *pWidget,uint8_t row,uint8_t column,int16_t x,int16_t y,int16_t width,int16_t height,uint32_t releaseImgAddr,uint32_t pressImgAddr,bool isCheckable);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置项目按键图片        </td>
    </tr>
    <tr>
        <td rowspan="10">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>row</td>
        <td>行号</td>
    </tr>
    <tr>
        <td>column</td>
        <td>列号</td>
    </tr>
    <tr>
        <td>x</td>
        <td>图片在项目中的x轴坐标</td>
    </tr>
    <tr>
        <td>y</td>
        <td>图片在项目中的y轴坐标</td>
    </tr>
    <tr>
        <td>width</td>
        <td>图片宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>图片高度</td>
    </tr>
    <tr>
        <td>releaseImgAddr</td>
        <td>释放的图片地址</td>
    </tr>
    <tr>
        <td>pressImgAddr</td>
        <td>按下的图片地址</td>
    </tr>
    <tr>
        <td>isCheckable</td>
        <td>是否为开关型按键</td>
    </tr>
</table>
<br>

---
# Text
### 简述
text widget
### 函数列表
* ldText_t *ldTextInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict);
* void ldTextSetTransparent(ldText_t* pWidget,bool isTransparent);
* void ldTextSetText(ldText_t* pWidget,uint8_t *pStr);
* void ldTextSetTextColor(ldText_t* pWidget,ldColor charColor);
* void ldTextSetAlign(ldText_t *pWidget,uint8_t align);
* void ldTextScrollSeek(ldText_t *pWidget,int16_t offset);
* void ldTextScrollMove(ldText_t *pWidget, int8_t moveValue);
### 信号列表
### 函数说明
#### ldTextInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldText_t *ldTextInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height, ldFontDict_t *pFontDict);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    文本控件初始化函数        </td>
    </tr>
    <tr>
        <td rowspan="7">参数</td>
        <td>nameId</td>
        <td>新控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>pFontDict</td>
        <td>字体指针</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldText_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldTextSetTransparent
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTextSetTransparent(ldText_t* pWidget,bool isTransparent);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置背景透明        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>isTransparent</td>
        <td>true=透明 false=不透明</td>
    </tr>
</table>
<br>

#### ldTextSetText
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTextSetText(ldText_t* pWidget,uint8_t *pStr);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置文本        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pStr</td>
        <td>字符串指针</td>
    </tr>
</table>
<br>

#### ldTextSetTextColor
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTextSetTextColor(ldText_t* pWidget,ldColor charColor);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设置文本颜色        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>charColor</td>
        <td>文本颜色</td>
    </tr>
</table>
<br>

#### ldTextSetAlign
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTextSetAlign(ldText_t *pWidget,uint8_t align);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    文本的对齐方式        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>align</td>
        <td>LD_ALIGN_CENTER<br>                         LD_ALIGN_TOP<br>                         LD_ALIGN_BOTTOM<br>                         LD_ALIGN_LEFT<br>                         LD_ALIGN_RIGHT</td>
    </tr>
</table>
<br>

#### ldTextScrollSeek
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTextScrollSeek(ldText_t *pWidget,int16_t offset);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设定文本显示位置绝对值        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>offset</td>
        <td>位置绝对值</td>
    </tr>
</table>
<br>

#### ldTextScrollMove
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTextScrollMove(ldText_t *pWidget, int8_t moveValue);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    设定文本显示偏移值（相对值）        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>moveValue</td>
        <td>偏移相对值</td>
    </tr>
</table>
<br>

---
# Window
### 简述
window widget 窗体控件可包含其他的控件，实现复合型控件或者控件组合模块
### 函数列表
* ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
* void ldWindowSetTransparent(ldWindow_t *pWidget,bool isTransparent);
### 信号列表
### 函数说明
#### ldWindowInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldWindow_t* ldWindowInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    window初始化函数        </td>
    </tr>
    <tr>
        <td rowspan="6">参数</td>
        <td>nameId</td>
        <td>控件id</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
    </tr>
    <tr>
        <td>x</td>
        <td>相对坐标x轴</td>
    </tr>
    <tr>
        <td>y</td>
        <td>相对坐标y轴</td>
    </tr>
    <tr>
        <td>width</td>
        <td>控件宽度</td>
    </tr>
    <tr>
        <td>height</td>
        <td>控件高度</td>
    </tr>
    <tr>
        <td>返回</td>
        <td>ldWindow_t*</td>
        <td>新控件指针</td>
    </tr>
</table>
<br>

#### ldWindowSetTransparent
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldWindowSetTransparent(ldWindow_t *pWidget,bool isTransparent);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    window控件设置透明        </td>
    </tr>
    <tr>
        <td rowspan="2">参数</td>
        <td>pWidget</td>
        <td>window控件指针</td>
    </tr>
    <tr>
        <td>isTransparent</td>
        <td>true=透明 false=不透明</td>
    </tr>
</table>
<br>

---
