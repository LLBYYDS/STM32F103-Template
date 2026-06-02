/*******************************************************************************
 * File : key.h
 * Author : Bin@L
 * Created on: 2026/05/27
 ******************************************************************************/
#ifndef BSP_KEY_H_
#define BSP_KEY_H_

/*******************************************************************************
 * include
 ******************************************************************************/
#include "main.h"
#include "stm32f1xx_hal.h"

/*******************************************************************************
 * global type define
 ******************************************************************************/
typedef enum
{
    KEY_EVENT_NONE = 0,         /* 无事件 */
    KEY_EVENT_SHORT_PRESS,      /* 短按 */
    KEY_EVENT_LONG_PRESS,       /* 长按 */
    KEY_EVENT_LONG_RELEASE      /* 长按释放 */
} Key_Event;

typedef enum
{
    KEY_WK_UP = 0,                  /* 按钮WK_UP */
    KEY_0,                      /* 按钮KEY0 */
    KEY_1,                      /* 按钮KEY1 */
    KEY_ID_MAX                  /* 按键总数 */
} Key_ID;

typedef struct
{
    GPIO_TypeDef* port;         /* GPIO 端口 */
    uint16_t pin;               /* GPIO 引脚 */
    GPIO_PinState activeLevel;  /* 有效电平 */
    uint16_t debounceCnt;       /* 消抖计数 */
    uint16_t pressCnt;          /* 按下时长计数 */
    uint8_t state;              /* 当前状态 */
    uint8_t longPressFlag;      /* 长按标志 */
} Key_Info_Struct;

/* 按键配置参数 */
#define KEY_DEBOUNCE_TIME       20      /* 消抖时间 (ms) */
#define KEY_LONG_TIME           1000    /* 长按阈值 (ms) */
#define KEY_SCAN_INTERVAL       10      /* 扫描周期 (ms) */

/* 按键状态机状态 */
#define KEY_STATE_IDLE          0       /* 空闲状态 */
#define KEY_STATE_DEBOUNCE      1       /* 消抖状态 */
#define KEY_STATE_CONFIRMED     2       /* 确认按下 */
#define KEY_STATE_LONG_PRESS    3       /* 长按状态 */

/*******************************************************************************
 * global variable declaration
 ******************************************************************************/

/*******************************************************************************
 * global function declaration
 ******************************************************************************/
void KEY_Init(void);
Key_Event KEY_Scan(Key_ID key_id);
Key_Event KEY_GetEvent(Key_ID key_id);
void KEY_Process(void);


#endif /* BSP_KEY_H_ */
