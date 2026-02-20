#include "key.h"
#include "usart.h"
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case WK_UP_Pin:
        if(HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin) == GPIO_PIN_SET)
        {
            
        }
        break;
    case KEY0_Pin:
        if(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET)
        {
            
        }
        break;
    case KEY1_Pin:
        if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
        {
            
        }
        break;
    default:
        break;
    }
}
