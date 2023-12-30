#ifndef _LD_USER_H_
#define _LD_USER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldGui.h"
#include "image.h"

#include "ldButton.h"
#include "ldWindow.h"
#include "ldText.h"
#include "ldProgressBar.h"
#include "ldRadialMenu.h"
#include "ldCheckBox.h"
#include "ldLabel.h"
#include "ldScrollSelecter.h"
#include "ldDateTime.h"
#include "ldIconSlider.h"
#include "ldGauge.h"
#include "ldQRCode.h"
#include "ldTable.h"
#include "ldKeyboard.h"
#include "ldLineEdit.h"
#include "ldGraph.h"
#include "ldComboBox.h"
#include "ldArc.h"

#define LD_PAGE_MAX              (2)

extern void (*ldUserPageInitFunc[LD_PAGE_MAX])(void);
extern void (*ldUserPageLoopFunc[LD_PAGE_MAX])(void);
extern void (*ldUserPageQuitFunc[LD_PAGE_MAX])(void);


#ifdef __cplusplus
}
#endif

#endif //_LD_USER_H_
