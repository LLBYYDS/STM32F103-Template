/*******************************************************************************
 * File : led.c
 * Author : Bin@L
 * Created on: 2026/05/27
 ******************************************************************************/


/*******************************************************************************
 * include
 ******************************************************************************/
#include "led.h"

/*******************************************************************************
 * static function declaration
 ******************************************************************************/

/*******************************************************************************
 * static variable define
 ******************************************************************************/

/*******************************************************************************
 * extern variable and declaratipn
 ******************************************************************************/

/*******************************************************************************
 * global variable define
 ******************************************************************************/
static LED_Info_Struct LED_Info[E_LED_MAX] =
{
    /* LED: LED0, 低电平亮 */
    {
        .port = LED0_GPIO_Port,
        .pin = LED0_Pin,
        .onState = GPIO_PIN_RESET,
        .offState = GPIO_PIN_SET,
    },
    /* LED: LED1, 低电平亮 */
    {
        .port = LED1_GPIO_Port,
        .pin = LED1_Pin,
        .onState = GPIO_PIN_RESET,
        .offState = GPIO_PIN_SET,
    },
};

/*******************************************************************************
 * global function define
 ******************************************************************************/


/*******************************************************************************
 * static function define
 ******************************************************************************/

/*
 * @brief  初始化LED
 * @param  none
 * @retval none
 */
void LED_Init(void)
{
    // 开启LED时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef ledGpioInitStruct = {0};
    ledGpioInitStruct.Mode = GPIO_MODE_INPUT;
    for(int i = 0; i < E_LED_MAX; i++)
    {
        ledGpioInitStruct.Pin = LED_Info[i].pin;
        ledGpioInitStruct.Pull = (LED_Info[i].onState == GPIO_PIN_RESET) ? GPIO_PULLUP : GPIO_PULLDOWN;
        HAL_GPIO_Init(LED_Info[i].port, &ledGpioInitStruct);
    }
}

void LED_SetState(E_LED_ID id, E_LED_STATE state)
{
    GPIO_PinState pinState = (E_LED_ON == state) ? LED_Info[id].onState : LED_Info[id].offState;
    HAL_GPIO_WritePin(LED_Info[id].port, LED_Info[id].pin, pinState);
}

E_LED_STATE LED_GetState(E_LED_ID id)
{
    GPIO_PinState pinState = HAL_GPIO_ReadPin(LED_Info[id].port, LED_Info[id].pin);
    return (pinState == LED_Info[id].onState) ? E_LED_ON : E_LED_OFF;
}

void LED_Toggle(E_LED_ID id)
{
    HAL_GPIO_TogglePin(LED_Info[id].port, LED_Info[id].pin);
}
