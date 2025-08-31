#ifndef DELAY_H
#define DELAY_H

#include "stm32f1xx_hal.h"

void delay_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif
