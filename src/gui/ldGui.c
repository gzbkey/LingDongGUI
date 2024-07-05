#include "ldGui.h"

void ldGuiDraw(arm_2d_scene_t *ptScene,
               const arm_2d_tile_t *ptTile,
               bool bIsNewFrame)
{
    arm_ctrl_enum(ldWidgetNode, ptItem, PREORDER_TRAVERSAL) {

((ldBase_t*)ptItem)->pFunc->show(ptScene,ptItem,ptTile,bIsNewFrame);
    }
}







