/*******************************************************************************
 * File : at24c02.h
 * Author : Bin@L
 * Created on: 2026/06/02
 ******************************************************************************/

#ifndef BSP_AT24C02_H_
#define BSP_AT24C02_H_

/*******************************************************************************
 * include
 ******************************************************************************/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "delay.h"
#include "cmsis_os.h"
/*******************************************************************************
 * global type define
 ******************************************************************************/
#define AT24C02_ADDR_W 0xA0
#define AT24C02_ADDR_R 0xA1

#define I2C_SCL(x) do { x ? \
                        HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET) : \
                        HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET); \
                    } while(0)

#define I2C_SDA(x) do { x ? \
                        HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET) : \
                        HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_RESET); \
                    } while(0)

#define I2C_SDA_READ() HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin)

/*******************************************************************************
 * global variable declaration
 ******************************************************************************/

/*******************************************************************************
 * global function declaration
 ******************************************************************************/
void AT24C02_WriteOneByte(uint8_t addr, uint8_t data);
uint8_t AT24C02_ReadOneByte(uint8_t addr);
void AT24C02_WriteMultiBytes(uint8_t addr, uint8_t *data, uint8_t len);
void AT24C02_ReadMultiBytes(uint8_t addr, uint8_t *data, uint8_t len);

#endif /* BSP_AT24C02_H_ */

