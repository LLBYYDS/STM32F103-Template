#ifndef __PRACTICE_H__
#define __PRACTICE_H__

#include "GUI.h"
#include "WM.h"

#define RECOMMENDED_MEMORY (1024L * 5)

#define PRACTICE     0
void Ui_Task(void);
#if PRACTICE == 1
void TextDisplay(void);
#elif PRACTICE == 2
void NumberDisplay(void);
#elif PRACTICE == 3
void PlotRect(void);
void AlphaMix(void);
void PlotLine(void);
#elif PRACTICE == 4
void DrawBitmap(void);
#elif PRACTICE == 5
void Color(void);
#elif PRACTICE == 6
void DrawWithDelay(int ms);
void StorageDev(void);
void DrawIt(void* pData);
void SegmentedStorageDev(void);
#elif PRACTICE == 7
// Practice 7.1
void BkCallback(WM_MESSAGE* pMsg);
void WidgetwCallback(WM_MESSAGE* pMsg);
void WidgetMove(const char* text);
void WidgetCallbackRender(void);
// Practice 7.2
void WMFuncUsage(void);
#elif PRACTICE == 8
#include "GUIBuilder/FramewinDLG.h"
#elif PRACTICE == 9
#include "GUIBuilder/ButtonUsage1.h"
void ButtonUsage1(void);
#endif

#endif /* __PRACTICE_H__ */
