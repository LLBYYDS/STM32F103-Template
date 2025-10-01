#include "practice.h"

void Ui_Task(void)
{
    GUI_Init();
    if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY) {
        GUI_ErrorOut("Not enough memory available.");
        return;
    }

    // This is Practice Function
#if PRACTICE == 1
    TextDisplay();
#elif PRACTICE == 2
    NumberDisplay();
#elif PRACTICE == 3
    PlotRect();
    GUI_Delay(1000);
    AlphaMix();
    GUI_Delay(1000);
    PlotLine();
#elif PRACTICE == 4
    DrawBitmap();
#elif PRACTICE == 5
    Color();
#elif PRACTICE == 6
    //StorageDev();
    SegmentedStorageDev();
#elif PRACTICE == 7
    //WidgetCallbackRender();
    WMFuncUsage();
#elif PRACTICE == 8
    WM_HWIN dialog = CreateFramewin();
    GUI_Exec();
#elif PRACTICE == 9
    ButtonUsage1();
#endif
    GUI_CURSOR_Show();

    while(1)
    {
        GUI_Exec();
        osDelay(10);
    }
}

#if PRACTICE == 1
void TextDisplay()
{
    const char text[] = "This example demostrates text wrapping!";
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_SetColor(GUI_GREEN);
    GUI_DispString("Hello World!");
    GUI_SetPenSize(10);
    GUI_SetColor(GUI_RED);
    GUI_DrawLine(60, 50, 260, 130);
    GUI_DrawLine(60, 130, 260, 50);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font8x16);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_SetTextMode(GUI_TM_NORMAL);
    GUI_DispStringHCenterAt("GUI_TM_NORMAL", 160, 50);
    GUI_SetTextMode(GUI_TEXTMODE_REV);
    GUI_DispStringHCenterAt("GUI_TEXTMODE_REV", 160, 66);
    GUI_SetTextMode(GUI_TEXTMODE_NORMAL | GUI_TEXTMODE_TRANS);
    GUI_DispStringHCenterAt("GUI_TEXTMODE_NORMAL | GUI_TEXTMODE_TRANS", 160, 82);
    GUI_SetTextMode(GUI_TEXTMODE_XOR | GUI_TEXTMODE_TRANS);
    GUI_DispStringHCenterAt("GUI_TEXTMODE_XOR", 160, 98);
    GUI_SetTextMode(GUI_TEXTMODE_REV | GUI_TEXTMODE_TRANS);
    GUI_DispStringHCenterAt("GUI_TEXTMODE_REV | GUI_TEXTMODE_TRANS", 160, 114);

    GUI_SetTextMode(GUI_TEXTMODE_TRANS);

    GUI_RECT rect = { 40, 300, 100, 360 };
    GUI_WRAPMODE warp_modes[] = {
        GUI_WRAPMODE_NONE,
        GUI_WRAPMODE_WORD,
        GUI_WRAPMODE_CHAR
    };
    for (int i = 0; i < 3; i++)
    {
        GUI_SetColor(GUI_WHITE);
        GUI_FillRectEx(&rect);
        GUI_SetColor(GUI_BLACK);
        GUI_DispStringInRectWrap(text, &rect, GUI_TA_LEFT, warp_modes[i]);
        rect.x0 += 70;
        rect.x1 += 70;
    }
}

#elif PRACTICE == 2
void NumberDisplay(void)
{
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_SetColor(GUI_YELLOW);
    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("Number Display Demo", 160, 10);

    GUI_SetColor(GUI_RED);
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_SetTextAlign(GUI_TA_LEFT);
    int x = 0, y = 40;
    GUI_DispStringAt("GUI_DispDecAt", x, y);
    GUI_DispDecAt(200, x + 200, y, 3);

    GUI_DispStringAt("GUI_DispFloat", x, y + 20);
    GUI_GotoXY(x + 200, y + 20);
    GUI_DispFloat(49.35, 5);
    GUI_GotoXY(x + 250, y + 20);
    GUI_DispSFloatFix(-49.35, 6, 2);

    GUI_DispStringAt("GUI_DispBinAt", x, y + 40);
    GUI_DispBinAt(10, x + 200, y + 40, 4);

    GUI_DispStringAt("GUI_DispHex", x, y + 60);
    GUI_GotoXY(x + 200, y + 60);
    GUI_DispHex(31, 2);

    GUI_DispStringAt("GUI_DispHexAt", x, y + 80);
    GUI_DispHexAt(31, x + 200, y + 80, 2);

    GUI_DispStringAt("GUI_GetVersionString", x, y + 100);
    const char* version = GUI_GetVersionString();
    GUI_DispStringAt(version, x + 200, y + 100);
}

#elif PRACTICE == 3
void PlotRect(void)
{
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_SetColor(GUI_YELLOW);
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_DispStringHCenterAt("PlotRect", 160, 10);

    GUI_SetBkColor(GUI_YELLOW);
    GUI_ClearRect(0, 40, 100, 140);
    GUI_SetBkColor(GUI_BLUE);

    GUI_DrawGradientH(110, 40, 210, 140, 0X4117BB, 0XC6B6F5);

    GUI_DrawGradientRoundedV(220, 40, 320, 140, 10, 0X4117BB, 0XC6B6F5);

    GUI_SetPenSize(2);
    GUI_SetColor(GUI_RED);
    GUI_DrawRect(0, 150, 100, 250);

    GUI_SetBkColor(GUI_RED);
    GUI_FillRect(110, 150, 210, 250);

    GUI_FillRoundedRect(220, 150, 320, 250, 10);
}

void AlphaMix(void)
{
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_DispStringHCenterAt("Alpha Mix", 160, 10);

    GUI_DispStringHCenterAt("Alpha Blending", 160, 80);
    GUI_EnableAlpha(1);
    GUI_SetColor(GUI_RED);
    GUI_SetAlpha(40);
    GUI_FillRect(70, 30, 170, 130);
    GUI_SetAlpha(80);
    GUI_SetColor(GUI_GREEN);
    GUI_FillRect(110, 50, 210, 150);
    GUI_SetAlpha(120);
    GUI_SetColor(GUI_BLUE);
    GUI_FillRect(150, 70, 250, 170);
    GUI_EnableAlpha(0);
}

void PlotLine(void)
{
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_SetColor(GUI_YELLOW);
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_DispStringHCenterAt("Line Display", 160, 10);

    GUI_SetColor(GUI_RED);
    GUI_SetPenSize(2);
    GUI_DrawHLine(50, 60, 260);
    GUI_SetPenSize(4);
    GUI_DrawLine(60, 100, 260, 100);
    GUI_SetPenSize(1);
    GUI_SetLineStyle(GUI_LS_DASH);
    GUI_DrawLine(60, 150, 260, 150);
    GUI_SetLineStyle(GUI_LS_DASHDOT);
    GUI_DrawLine(60, 200, 260, 200);
    GUI_SetLineStyle(GUI_LS_DASHDOTDOT);
    GUI_DrawLine(60, 250, 260, 250);

    GUI_SetLineStyle(GUI_LS_SOLID);
    const GUI_POINT pos[] = { {0, 50},
                              {50, 0},
                              {100, 50},
                              {150, 0},
                              {200, 50} };
    GUI_DrawPolyLine(pos, GUI_COUNTOF(pos), 60, 300);
}

#elif PRACTICE == 4
extern const GUI_BITMAP bmkeqing;
void DrawBitmap(void)
{
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_DrawBitmap(&bmkeqing, 0, 80);
}

#elif PRACTICE == 5
typedef struct _ColorInfo
{
    unsigned char color_num;
    const char* color_name;
    GUI_COLOR     color;
}ColorInfo;

const ColorInfo color_bar[] = { {2, "Red",      GUI_RED},
                                {2, "Green",    GUI_GREEN},
                                {2, "Blue",     GUI_BLUE},
                                {2, "Yellow",   GUI_YELLOW},
                                {2, "Cyan",     GUI_CYAN},
                                {2, "Magenta",  GUI_MAGENTA},
                                {1, "Gray",     GUI_GRAY} };

void Color(void)
{
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_SetColor(GUI_YELLOW);
    GUI_DispStringHCenterAt("Color", 160, 10);

    int xsize = LCD_GetXSize();
    int ysize = LCD_GetYSize();
    int cnt = 0;
    for (int i = 0; i < GUI_COUNTOF(color_bar); i++)
    {
        cnt += color_bar[i].color_num;
    }
    int yPos = 50;
    int seg = (ysize - yPos) / cnt;

    for (int i = 0; i < GUI_COUNTOF(color_bar); i++)
    {
        if (color_bar[i].color_num == 2)
        {
            GUI_DrawGradientH(100, yPos, xsize, yPos + seg, 0x000000, color_bar[i].color);
            yPos += seg;
            GUI_DispStringAt(color_bar[i].color_name, 0, yPos);
            GUI_DrawGradientH(100, yPos, xsize, yPos + seg, 0xFFFFFF, color_bar[i].color);
            yPos += seg;
        }
        else
        {
            GUI_GRADIENT_INFO gradient[] = { {0,         0x000000},
                                            {xsize / 2,   color_bar[i].color},
                                            {xsize,     0xFFFFFF} };
            GUI_DrawGradientMH(100, yPos, yPos + seg, gradient, GUI_COUNTOF(gradient));
            yPos += seg;
            GUI_DispStringAt(color_bar[i].color_name, 0, yPos - seg / 2);
        }
    }
}

#elif PRACTICE == 6

static GUI_RECT Rect = { 30, 150, 130, 210 };

void DrawWithDelay(int ms)
{
    GUI_Delay(ms);
    GUI_SetPenSize(5);
    GUI_SetColor(GUI_RED);
    GUI_DrawLine(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
    GUI_Delay(ms);
    GUI_SetColor(GUI_GREEN);
    GUI_DrawLine(Rect.x1, Rect.y0, Rect.x0, Rect.y1);
    GUI_Delay(ms);
    GUI_SetColor(GUI_YELLOW);
    GUI_SetFont(&GUI_FontComic24B_ASCII);
    GUI_DispStringInRect("Close", &Rect, GUI_TA_HCENTER | GUI_TA_VERTICAL);
}

void StorageDev(void)
{
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetColor(GUI_YELLOW);
    GUI_DispStringHCenterAt("Storage Device", 160, 10);
    GUI_SetFont(&GUI_Font16_ASCII);
    GUI_DispStringHCenterAt("Shows the advantage of using a \nmemory device", 160, 50);

    GUI_SetFont(&GUI_Font16_ASCII);
    GUI_DispStringHCenterAt("Draws the picture\nwithout a\nmemory device", 80, 100);
    GUI_DispStringHCenterAt("Draws the picture\nusing a\nmemory device", 220, 100);

    DrawWithDelay(400);

    GUI_MEMDEV_Handle handle = GUI_MEMDEV_Create(30, 150, 130, 60);
    GUI_MEMDEV_Select(handle);
    GUI_Clear();
    DrawWithDelay(400);
    GUI_MEMDEV_CopyToLCDAt(handle, 170, 150);
}

static GUI_POINT points[] = {
    {-50, 0},
    {-10, 10},
    {0, 50},
    {10, 10},
    {50, 0},
    {10, -10},
    {0, -50},
    {-10, -10}
};

typedef struct _PolyInfo
{
    int offset;
    GUI_POINT pos[8];
}PolyInfo;

void DrawIt(void* pData)
{
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    PolyInfo* info = pData;
    GUI_SetColor(GUI_GREEN);
    GUI_FillRect(0 + info->offset, 220, 160 + info->offset, 240);
    GUI_SetColor(GUI_BLUE);
    GUI_FillPolygon(info->pos, GUI_COUNTOF(info->pos), 160, 250);
    GUI_SetColor(GUI_RED);
    GUI_FillRect(160 - info->offset, 260, 320 - info->offset, 280);
}

void SegmentedStorageDev(void)
{
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetColor(GUI_YELLOW);
    GUI_DispStringHCenterAt("Segmented Memory Device", 160, 10);
    GUI_SetFont(&GUI_Font16_ASCII);
    GUI_DispStringHCenterAt("Flicker-free animations", 160, 180);

    int swap = 0;
    PolyInfo info;
    GUI_RECT rect = { 0, 200, 320, 300 };
    while (1)
    {
        swap = ~swap;
        for (int i = 0; i < 160; i++)
        {
            info.offset = swap ? i : 160 - i;
            float angle = i * 3.1415926 / 55;
            GUI_RotatePolygon(info.pos, points, GUI_COUNTOF(points), angle);
            GUI_MEMDEV_Draw(&rect, DrawIt, &info, 0, 0);
            GUI_Delay(20);
        }
    }
}

#elif PRACTICE == 7

void BkCallback(WM_MESSAGE* pMsg)
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
    {
        GUI_SetBkColor(GUI_BLACK);
        GUI_ClearRect(0, 60, 320, 180);
        break;
    }
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
void WidgetwCallback(WM_MESSAGE* pMsg)
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
    {
        GUI_RECT rect;
        WM_GetInsideRect(&rect);
        GUI_SetBkColor(GUI_RED);
        GUI_SetColor(GUI_YELLOW);
        GUI_ClearRectEx(&rect);
        GUI_DrawRectEx(&rect);
        GUI_SetColor(GUI_BLACK);
        GUI_SetFont(&GUI_Font16_ASCII);
        GUI_DispStringHCenterAt("Foreground window", rect.x1 / 2, rect.y1 / 2);
        break;
    }
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

void WidgetMove(const char* text)
{
    if (text != NULL)
    {
        GUI_ClearRect(0, 40, 320, 60);
        GUI_DispStringHCenterAt(text, 160, 40);
    }
    WM_HWIN hwin = WM_CreateWindow(40, 60, 120, 60, WM_CF_SHOW, WidgetwCallback, 0);

    for (int i = 0; i < 30; i++)
    {
        WM_MoveWindow(hwin, 4, 2);
        GUI_Delay(10);
    }
    WM_Invalidate(WM_HBKWIN);
    WM_DeleteWindow(hwin);
    GUI_Exec();
}

void WidgetCallbackRender(void)
{
    WM_CALLBACK* bkOldCallback;
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetColor(GUI_YELLOW);
    GUI_DispStringHCenterAt("Widget Manager Render Callback", 160, 10);

    while (1)
    {
        WidgetMove("Background does not have callback");
        GUI_ClearRect(0, 40, 320, 180);
        GUI_Delay(1000);
        bkOldCallback = WM_SetCallback(WM_HBKWIN, BkCallback);
        WidgetMove("Background has callback");
        WM_SetCallback(WM_HBKWIN, bkOldCallback);
    }
}

void Window1_Callback(WM_MESSAGE* pMsg)
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
    {
        GUI_RECT rect;
        WM_GetInsideRect(&rect);
        GUI_SetBkColor(GUI_GREEN);
        GUI_SetColor(GUI_BLUE);
        GUI_ClearRectEx(&rect);
        GUI_DrawRectEx(&rect);
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font20_ASCII);
        GUI_DispStringInRect("Window 1", &rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
        break;
    }
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

void Window2_Callback(WM_MESSAGE* pMsg)
{
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
    {
        GUI_RECT rect;
        WM_GetInsideRect(&rect);
        GUI_SetBkColor(GUI_RED);
        GUI_SetColor(GUI_YELLOW);
        GUI_ClearRectEx(&rect);
        GUI_DrawRectEx(&rect);
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font20_ASCII);
        GUI_DispStringInRect("Window 2", &rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
        break;
    }
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

void WMFuncUsage(void)
{
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetFont(&GUI_Font20_ASCII);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetColor(GUI_YELLOW);
    GUI_DispStringHCenterAt("Window Manager Functions Usage", 160, 10);

    WM_HWIN hWin1 = WM_CreateWindow(40, 40, 160, 80, WM_CF_SHOW, Window1_Callback, 0);
    WM_HWIN hWin2 = WM_CreateWindow(120, 80, 160, 80, WM_CF_SHOW, Window2_Callback, 0);
    GUI_Exec();
}

#elif PRACTICE == 9
void ButtonUsage1(void)
{
    WM_HWIN hWin = CreateButtonUsageWin();
    GUI_Exec();
}

#endif

