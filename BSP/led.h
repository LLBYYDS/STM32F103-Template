/*******************************************************************************
 * File : led.h
 * Author : Bin@L
 * Created on: 2026/05/27
 ******************************************************************************/
#ifndef BSP_LED_H_
#define BSP_LED_H_

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
    E_LED_0 = 0,
    E_LED_1,
    E_LED_MAX
}E_LED_ID;

typedef enum
{
    E_LED_ON = 0,
    E_LED_OFF
}E_LED_STATE;

typedef struct
{
    GPIO_TypeDef    *port;
    uint16_t        pin;
    GPIO_PinState   onState;
    GPIO_PinState   offState;
}LED_Info_Struct;

/*******************************************************************************
 * global variable declaration
 ******************************************************************************/

/*******************************************************************************
 * global function declaration
 ******************************************************************************/
void LED_Init(void);
void LED_SetState(E_LED_ID id, E_LED_STATE state);
E_LED_STATE LED_GetState(E_LED_ID id);
void LED_Toggle(E_LED_ID id);

#endif /* BSP_LED_H_ */
