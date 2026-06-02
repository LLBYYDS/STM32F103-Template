/*******************************************************************************
 * File : key
 * Author : Bin@L
 * Created on: 2026/05/27
 ******************************************************************************/


/*******************************************************************************
 * include
 ******************************************************************************/
#include "key.h"

/*******************************************************************************
 * static function declaration
 ******************************************************************************/

/*******************************************************************************
 * static variable define
 ******************************************************************************/
static Key_Event s_key_event[KEY_ID_MAX] = {KEY_EVENT_NONE};

/*******************************************************************************
 * extern variable and declaratipn
 ******************************************************************************/

/*******************************************************************************
 * global variable define
 ******************************************************************************/
static Key_Info_Struct Key_Info[KEY_ID_MAX] =
{
    /* KEY: WK_UP, 高电平有效 */
    {
        .port = WK_UP_GPIO_Port,
        .pin = WK_UP_Pin,
        .activeLevel = GPIO_PIN_SET,
        .debounceCnt = 0,
        .pressCnt = 0,
        .state = KEY_STATE_IDLE,
        .longPressFlag = 0
    },
    /* KEY: KEY0, 低电平有效 */
    {
        .port = KEY0_GPIO_Port,
        .pin = KEY0_Pin,
        .activeLevel = GPIO_PIN_RESET,
        .debounceCnt = 0,
        .pressCnt = 0,
        .state = KEY_STATE_IDLE,
        .longPressFlag = 0
    },
    /* KEY: KEY1, 低电平有效 */
    {
        .port = KEY1_GPIO_Port,
        .pin = KEY1_Pin,
        .activeLevel = GPIO_PIN_RESET,
        .debounceCnt = 0,
        .pressCnt = 0,
        .state = KEY_STATE_IDLE,
        .longPressFlag = 0
    }
};

/*******************************************************************************
 * global function define
 ******************************************************************************/
/**
 * @brief 按键初始化
 * 
 * @param 无
 * @return 无
 */
void KEY_Init(void)
{
    // 使能 KEY引脚时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef keyGpioInitStruct;
    keyGpioInitStruct.Mode = GPIO_MODE_INPUT;
    keyGpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    for(int id = 0; id < KEY_ID_MAX; id++)
    {
        keyGpioInitStruct.Pull = (Key_Info[id].activeLevel == GPIO_PIN_SET) ? GPIO_PULLDOWN : GPIO_PULLUP;
        keyGpioInitStruct.Pin = Key_Info[id].pin;
        HAL_GPIO_Init(Key_Info[id].port, &keyGpioInitStruct);
    }
}

/**
 * @brief 扫描单个按键
 * 
 * @param key_id 按键 ID
 * @return 按键事件
 */
Key_Event KEY_Scan(Key_ID key_id)
{
    Key_Info_Struct* key = &Key_Info[key_id];
    GPIO_PinState pin_level = HAL_GPIO_ReadPin(key->port, key->pin);
    Key_Event event = KEY_EVENT_NONE;

    if (pin_level == key->activeLevel)
    {
        if (key->state == KEY_STATE_IDLE)
        {
            key->state = KEY_STATE_DEBOUNCE;
            key->debounceCnt = 0;
        }

        if (key->state == KEY_STATE_DEBOUNCE)
        {
            key->debounceCnt += KEY_SCAN_INTERVAL;
            if (key->debounceCnt >= KEY_DEBOUNCE_TIME)
            {
                key->state = KEY_STATE_CONFIRMED;
                key->pressCnt = 0;
            }
        }
        else if (key->state == KEY_STATE_CONFIRMED)
        {
            key->pressCnt += KEY_SCAN_INTERVAL;
            if (key->pressCnt >= KEY_LONG_TIME)
            {
                key->state = KEY_STATE_LONG_PRESS;
                key->longPressFlag = 1;
                event = KEY_EVENT_LONG_PRESS;
                s_key_event[key_id] = event;
            }
        }
        else if (key->state == KEY_STATE_LONG_PRESS)
        {
            key->pressCnt += KEY_SCAN_INTERVAL;
        }
    }
    else
    {
        if (key->state == KEY_STATE_CONFIRMED)
        {
            if (key->pressCnt < KEY_LONG_TIME && key->pressCnt >= KEY_DEBOUNCE_TIME)
            {
                event = KEY_EVENT_SHORT_PRESS;
                s_key_event[key_id] = event;
            }
        }
        else if (key->state == KEY_STATE_LONG_PRESS)
        {
            event = KEY_EVENT_LONG_RELEASE;
            s_key_event[key_id] = event;
        }

        key->state = KEY_STATE_IDLE;
        key->debounceCnt = 0;
        key->pressCnt = 0;
        key->longPressFlag = 0;
    }

    return event;
}

/**
 * @brief 获取按键事件 (非阻塞)
 * 
 * @param key_id 按键 ID
 * @return 按键事件
 */
Key_Event KEY_GetEvent(Key_ID key_id)
{
    Key_Event event = s_key_event[key_id];
    s_key_event[key_id] = KEY_EVENT_NONE;
    return event;
}

/**
 * @brief 按键处理函数 (需周期性调用)
 * 
 * 在定时器或主循环中周期性调用，建议 10ms 调用一次
 */
void KEY_Process(void)
{
    for (uint8_t i = 0; i < KEY_ID_MAX; i++)
    {
        KEY_Scan((Key_ID)i);
    }
}

/*******************************************************************************
 * static function define
 ******************************************************************************/

