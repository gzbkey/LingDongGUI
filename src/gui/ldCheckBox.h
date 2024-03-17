#ifndef _LD_CHECKBOX_H_
#define _LD_CHECKBOX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isChecked:1;
    bool isCorner:1;
    bool isWithUncheckedMask:1;
    bool isWithCheckedMask:1;
    bool isRadioButton:1;
    ldColor bgColor;
    ldColor fgColor;
    ldColor charColor;
    uintptr_t uncheckedImgAddr;
    uintptr_t checkedImgAddr;
    uint16_t boxWidth;
    uint8_t radioButtonGroup;
    ldFontDict_t* pFontDict;
    uint8_t* pStr;
    uint8_t align:4;
}ldCheckBox_t;

ldCheckBox_t* ldCheckBoxInit(arm_2d_scene_t *pScene,uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldCheckBoxFrameUpdate(ldCheckBox_t* pWidget);
void ldCheckBoxLoop(arm_2d_scene_t *pScene,ldCheckBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldCheckBoxDel(ldCheckBox_t *pWidget);
void ldCheckBoxSetColor(ldCheckBox_t* pWidget,ldColor bgColor,ldColor fgColor);
void ldCheckBoxSetImage(ldCheckBox_t* pWidget,uint16_t boxWidth,uintptr_t uncheckedImgAddr,bool isUncheckedMask,uintptr_t checkedImgAddr,bool isCheckedMask);
void ldCheckBoxSetText(ldCheckBox_t* pWidget,ldFontDict_t *pFontDict,uint8_t *pStr);
void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* pWidget,uint8_t num);
void ldCheckBoxSetCorner(ldCheckBox_t* pWidget,bool isCorner);
void ldCheckBoxSetCharColor(ldCheckBox_t* pWidget,ldColor charColor);

#define ldCheckBoxSetHidden        ldBaseSetHidden
#define ldCheckBoxMove             ldBaseMove



#ifdef __cplusplus
}
#endif

#endif //_LD_CHECKBOX_H_
