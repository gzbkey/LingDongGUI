#ifndef _LD_CHECKBOX_H_
#define _LD_CHECKBOX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    bool isChecked:1;
    bool isCorner:1;
    bool isUncheckedMask:1;
    bool isCheckedMask:1;
    bool isRadioButton:1;
    ldColor bgColor;
    ldColor fgColor;
    ldColor textColor;
    uint32_t uncheckedImgAddr;
    uint32_t checkedImgAddr;
    uint16_t boxWidth;
    uint8_t radioButtonGroup;
    ldChar_t *pTextInfo;
}ldCheckBox_t;

ldCheckBox_t* ldCheckBoxInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldCheckBoxLoop(ldCheckBox_t *pWidget,const arm_2d_tile_t *pParentTile,bool bIsNewFrame);
void ldCheckBoxDel(ldCheckBox_t *pWidget);
void ldCheckBoxSetColor(ldCheckBox_t* pWidget,ldColor bgColor,ldColor fgColor);
void ldCheckBoxSetImage(ldCheckBox_t* pWidget,uint16_t boxWidth,uint32_t uncheckedImgAddr,bool isUncheckedMask,uint32_t checkedImgAddr,bool isCheckedMask);
void ldCheckBoxSetFont(ldCheckBox_t *pWidget,ldFontDict_t *pFontDict);
void ldCheckBoxSetText(ldCheckBox_t* pWidget,uint8_t *pStr);
void ldCheckBoxSetRadioButtonGroup(ldCheckBox_t* pWidget,uint8_t num);
void ldCheckBoxSetCorner(ldCheckBox_t* pWidget,bool isCorner);

#define ldCheckBoxSetHidden        ldBaseSetHidden
#define ldCheckBoxMove             ldBaseMove



#ifdef __cplusplus
}
#endif

#endif //_LD_CHECKBOX_H_
