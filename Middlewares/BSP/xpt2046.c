#include "xpt2046.h"
#include "delay.h"
#include "lcd.h"

TOUCH_POS g_touch_pos = {INVALIDE_POS, INVALIDE_POS, INVALIDE_POS, INVALIDE_POS};

void XPT2046_WriteByte(uint8_t cmd)
{
    HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_RESET);
    for(uint8_t i=0; i<8; i++)
    {
        HAL_GPIO_WritePin(T_CLK_GPIO_Port, T_CLK_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(T_MOSI_GPIO_Port, T_MOSI_Pin, (cmd & (0x80 >> i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(T_CLK_GPIO_Port, T_CLK_Pin, GPIO_PIN_SET);
    }
}

uint16_t XPT2046_ReadData(uint8_t cmd)
{
    uint16_t data = 0;
    XPT2046_WriteByte(cmd);
    for (uint32_t i = 0; i < 576; i++)    // 576 ¡Ö 72 * 8
        __NOP();
    HAL_GPIO_WritePin(T_CLK_GPIO_Port, T_CLK_Pin, GPIO_PIN_RESET);
    for(int i=0; i<16; i++)
    {
        HAL_GPIO_WritePin(T_CLK_GPIO_Port, T_CLK_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(T_CLK_GPIO_Port, T_CLK_Pin, GPIO_PIN_SET);
        data <<= 1;
        if(HAL_GPIO_ReadPin(T_MISO_GPIO_Port, T_MISO_Pin) == GPIO_PIN_SET)
        {
            data |= 0x01;
        }
    }
    return data;
}

void XPT2046_GetXY(void)
{
    if(HAL_GPIO_ReadPin(T_PEN_GPIO_Port, T_PEN_Pin) == GPIO_PIN_RESET)
    {
        g_touch_pos.real_x = XPT2046_ReadData(0xD0);
        g_touch_pos.real_y = XPT2046_ReadData(0x90);
        g_touch_pos.logic_x = (31320 - g_touch_pos.real_x) / 93.1;
        g_touch_pos.logic_y = (31320 - g_touch_pos.real_y) / 62.5;
    }
}
