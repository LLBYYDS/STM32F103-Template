#ifndef XPT2046_H
#define XPT2046_H

#include "main.h"
#include "stm32f1xx_hal.h"

#define INVALIDE_POS    0xFFFF
#define XPT2046_X_MAX   31320   // 最大物理x坐标
#define XPT2046_Y_MAX   31320   // 最大物理y坐标
#define XPT2046_X_SCALE 93.1    // x坐标比例
#define XPT2046_Y_SCALE 62.5    // y坐标比例
typedef struct {
    uint16_t real_x; // 物理x坐标
    uint16_t real_y; // 物理y坐标
    uint16_t logic_x; // 逻辑x坐标
    uint16_t logic_y; // 逻辑y坐标
} TOUCH_POS;

extern TOUCH_POS g_touch_pos;

void XPT2046_WriteByte(uint8_t cmd);
uint16_t XPT2046_ReadData(uint8_t cmd);
void XPT2046_GetXY(void);

#endif /* XPT2046_H */
