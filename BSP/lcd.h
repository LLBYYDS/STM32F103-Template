#ifndef LCD_H__
#define LCD_H__

#include "main.h"
#include "stm32f1xx_hal.h"

#define LCD_FSMC_NEX         4
#define LCD_FSMC_AX          10

typedef struct
{
    uint16_t width;     /* LCD 宽度 */
    uint16_t height;    /* LCD 高度 */
    uint16_t id;        /* LCD ID */
    uint8_t dir;        /* 横屏还是竖屏控制：0，竖屏；1，横屏 */
    uint16_t wramcmd;   /* 开始写gram指令 */
    uint16_t setxcmd;   /* 设置x坐标指令 */
    uint16_t setycmd;   /* 设置y坐标指令 */
} _lcd_dev;

extern _lcd_dev lcddev;

typedef struct
{
    volatile uint16_t LCD_REG; /* 寄存器地址 */
    volatile uint16_t LCD_RAM; /* 数据地址 */
} LCD_TypeDef;


#define LCD_BASE        (uint32_t)((0X60000000 + (0X4000000 * (LCD_FSMC_NEX - 1))) | (((1 << LCD_FSMC_AX) * 2) -2))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

#define LCD_BL(x)   do{ x ? \
                      HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET); \
                     }while(0)

/******************************************************************************************/
/* 函数声明 */
void lcd_wr_data(volatile uint16_t data);            /* LCD写数据 */
void lcd_wr_regno(volatile uint16_t regno);          /* LCD写寄存器编号/地址 */
void lcd_write_reg(uint16_t regno, uint16_t data);   /* LCD写寄存器的值 */

void lcd_init(void);                        /* 初始化LCD */
void lcd_display_on(void);                  /* 开显示 */
void lcd_display_off(void);                 /* 关显示 */
void lcd_scan_dir(uint8_t dir);             /* 设置屏扫描方向 */
void lcd_display_dir(uint8_t dir);          /* 设置屏幕显示方向 */

void lcd_write_ram_prepare(void);               /* 准备些GRAM */
void lcd_set_cursor(uint16_t x, uint16_t y);    /* 设置光标 */
void lcd_clear(uint16_t color);     /* LCD清屏 */
uint16_t lcd_read_point(uint16_t x, uint16_t y);
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);

void nt35310_reginit(void);

#endif
