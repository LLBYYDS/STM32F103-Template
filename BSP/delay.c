#include "delay.h"
#include "stm32f1xx.h"

static uint32_t fac_us = 0;
static uint32_t fac_ms = 0;

void delay_init(void)
{
    fac_us = SystemCoreClock / 1000000U;   /* 1 µs 需要的时钟数 */
    fac_ms = SystemCoreClock / 1000U;      /* 1 ms 需要的时钟数 */
}

void delay_us(uint32_t us)
{
    uint32_t reload = us * fac_us;
    if (reload > SysTick_LOAD_RELOAD_Msk)  /* 超过 24 位最大值 */
        reload = SysTick_LOAD_RELOAD_Msk;

    SysTick->LOAD  = reload - 1;
    SysTick->VAL   = 0;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |   /* 用 HCLK */
                     SysTick_CTRL_ENABLE_Msk;        /* 启动 */

    /* 等待 COUNTFLAG 置位 */
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
        ;

    SysTick->CTRL = 0;  /* 关闭 */
    SysTick->VAL  = 0;
}

void delay_ms(uint32_t ms)
{
    /* 最大只能延时 SysTick_LOAD_RELOAD_Msk / fac_ms 毫秒 */
    while (ms--)
        delay_us(1000);
}
