#ifndef _LD_TEMPLATE_H_
#define _LD_TEMPLATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"
#include "ldConfig.h"

typedef struct {
    LD_COMMON_ATTRIBUTES;
    // add other variable
}ldTemplate_t;

ldTemplate_t* ldTemplateInit(uint16_t nameId, uint16_t parentNameId, int16_t x,int16_t y,int16_t width,int16_t height);
void ldTemplateLoop(ldTemplate_t *widget,const arm_2d_tile_t *ptParent,bool bIsNewFrame);
void ldTemplateDel(ldTemplate_t *widget);



#ifdef __cplusplus
}
#endif

#endif //_LD_TEMPLATE_H_
