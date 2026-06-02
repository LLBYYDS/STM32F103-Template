/*******************************************************************************
 * File : beep.h
 * Author : Bin@L
 * Created on: 2026/05/27
 ******************************************************************************/
#ifndef BSP_BEEP_H_
#define BSP_BEEP_H_

/*******************************************************************************
 * include
 ******************************************************************************/
#include "main.h"
#include "stm32f1xx_hal.h"

/*******************************************************************************
 * global type define
 ******************************************************************************/

#define BEEP_PIN_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define BEEP_ON() do { \
                    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);\
                    } while(0)
#define BEEP_OFF() do { \
                    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);\
                    } while(0)

#define BEEP_TOGGLE() do { \
                    HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);\
                    } while(0)
#define BEEP_IS_ON() (HAL_GPIO_ReadPin(BEEP_GPIO_Port, BEEP_Pin) == GPIO_PIN_SET)

/*******************************************************************************
 * global variable declaration
 ******************************************************************************/

/*******************************************************************************
 * global function declaration
 ******************************************************************************/
void Beep_Init(void);

#endif /* BSP_BEEP_H_ */
