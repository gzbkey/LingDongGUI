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
* void ldButtonLoop(ldButton_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
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

#### ldButtonLoop
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldButtonLoop(ldButton_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    按键循环处理函数        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pParentTile</td>
        <td>父控件tile对象</td>
    </tr>
    <tr>
        <td>bIsNewFrame</td>
        <td>新的一帧开始标志</td>
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
* void ldCheckBoxLoop(ldCheckBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
* void ldCheckBoxSetColor(ldCheckBox_t* pWidget,ldColor bgColor,ldColor fgColor);
* void ldCheckBoxSetImage(ldCheckBox_t* pWidget,uint16_t boxWidth,uint32_t uncheckedImgAddr,bool isUncheckedMask,uint32_t checkedImgAddr,bool isCheckedMask);
* void ldCheckBoxSetFont(ldCheckBox_t *pWidget,ldFontDict_t *pFontDict);
* void ldCheckBoxSetText(ldCheckBox_t* pWidget,uint8_t *pStr);
* void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* pWidget,uint8_t num);
* void ldCheckBoxSetCorner(ldCheckBox_t* pWidget,bool isCorner);
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

#### ldCheckBoxLoop
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldCheckBoxLoop(ldCheckBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    check                box循环处理函数        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pParentTile</td>
        <td>父控件tile对象</td>
    </tr>
    <tr>
        <td>bIsNewFrame</td>
        <td>新的一帧开始标志</td>
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

#### ldCheckBoxSetFont
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldCheckBoxSetFont(ldCheckBox_t *pWidget,ldFontDict_t *pFontDict);</code></pre>
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
        <td>字体</td>
    </tr>
</table>
<br>

#### ldCheckBoxSetText
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldCheckBoxSetText(ldCheckBox_t* pWidget,uint8_t *pStr);</code></pre>
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

---
# Common
### 简述
通用函数文件
### 函数列表
### 信号列表
### 函数说明
---
# DateTime
### 简述
date time widget
### 函数列表
### 信号列表
### 函数说明
---
# gauge
### 简述
仪表盘控件
### 函数列表
* ldGauge_t *ldGaugeInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,uint32_t bgImgAddr,bool isBgMask);
* void ldGaugeLoop(ldGauge_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
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

#### ldGaugeLoop
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldGaugeLoop(ldGauge_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    gauge循环处理函数        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pParentTile</td>
        <td>父控件tile对象</td>
    </tr>
    <tr>
        <td>bIsNewFrame</td>
        <td>新的一帧开始标志</td>
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
# IconSlider
### 简述
icon slider widget
### 函数列表
### 信号列表
* SIGNAL_CLICKED_ITEM
### 函数说明
---
# Image
### 简述
image widget
### 函数列表
### 信号列表
### 函数说明
---
# keyboard
### 简述
键盘控件，可以切换数字键盘和字母键盘
### 函数列表
* ldKeyboard_t *ldKeyboardInit(uint16_t nameId, uint16_t parentNameId,ldFontDict_t *pFontDict);
* void ldKeyboardLoop(ldKeyboard_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
### 信号列表
* SIGNAL_INPUT_ASCII
### 函数说明
#### ldKeyboardInit
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">ldKeyboard_t *ldKeyboardInit(uint16_t nameId, uint16_t parentNameId,ldFontDict_t *pFontDict);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    键盘初始化        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>nameId</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>parentNameId</td>
        <td>父控件id</td>
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

#### ldKeyboardLoop
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldKeyboardLoop(ldKeyboard_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    键盘显示处理函数        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pParentTile</td>
        <td>父控件tile对象</td>
    </tr>
    <tr>
        <td>bIsNewFrame</td>
        <td>新的一帧开始标志</td>
    </tr>
</table>
<br>

---
# Label
### 简述
label widget
### 函数列表
### 信号列表
### 函数说明
---
# ineEdit
### 简述
输入框控件
### 函数列表
* ldLineEdit_t *ldLineEditInit(uint16_t nameId, uint16_t parentNameId, int16_t x, int16_t y, int16_t width, int16_t height,ldFontDict_t *pFontDict,uint8_t textMax);
* void ldLineEditLoop(ldLineEdit_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
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

#### ldLineEditLoop
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldLineEditLoop(ldLineEdit_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    输入框显示处理        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pParentTile</td>
        <td>父控件tile对象</td>
    </tr>
    <tr>
        <td>bIsNewFrame</td>
        <td>新的一帧开始标志</td>
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
### 信号列表
### 函数说明
---
# QRCode
### 简述
qr code widget
### 函数列表
### 信号列表
### 函数说明
---
# RadialMenu
### 简述
radial menu widget
### 函数列表
### 信号列表
### 函数说明
---
# ScrollSelecter
### 简述
scroll selecter widget
### 函数列表
### 信号列表
### 函数说明
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
* void ldTableLoop(ldTable_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
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

#### ldTableLoop
<table>
    <tr>
        <td>函数</td>
        <td colspan="2">
            <pre><code class="language-c">void ldTableLoop(ldTable_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);</code></pre>
        </td>
    </tr>
    <tr>
        <td>说明</td>
        <td colspan="2">
    表格显示处理        </td>
    </tr>
    <tr>
        <td rowspan="3">参数</td>
        <td>pWidget</td>
        <td>目标控件指针</td>
    </tr>
    <tr>
        <td>pParentTile</td>
        <td>父控件tile对象</td>
    </tr>
    <tr>
        <td>bIsNewFrame</td>
        <td>新的一帧开始标志</td>
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
### 信号列表
### 函数说明
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
