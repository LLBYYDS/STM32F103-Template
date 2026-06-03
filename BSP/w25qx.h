/*******************************************************************************
 * File : w25qx.h
 * Author : Bin@L
 * Created on: 2026/06/03
 ******************************************************************************/

#ifndef W25QX_H_
#define W25QX_H_

/*******************************************************************************
 * include
 ******************************************************************************/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "usart.h"
/*******************************************************************************
 * global type define
 ******************************************************************************/
typedef struct
{
    uint8_t manufacturerID;
    uint8_t memoryType;
    uint8_t memoryDensity;
}W25QX_DeviceInfo_Struct;

/* 指令表 */
#define FLASH_WriteEnable           0x06 
#define FLASH_WriteDisable          0x04 
#define FLASH_ReadStatusReg1        0x05 
#define FLASH_ReadStatusReg2        0x35 
#define FLASH_ReadStatusReg3        0x15 
#define FLASH_WriteStatusReg1       0x01 
#define FLASH_WriteStatusReg2       0x31 
#define FLASH_WriteStatusReg3       0x11 
#define FLASH_ReadData              0x03 
#define FLASH_FastReadData          0x0B 
#define FLASH_FastReadDual          0x3B 
#define FLASH_FastReadQuad          0xEB  
#define FLASH_PageProgram           0x02 
#define FLASH_PageProgramQuad       0x32 
#define FLASH_BlockErase            0xD8 
#define FLASH_SectorErase           0x20 
#define FLASH_ChipErase             0xC7 
#define FLASH_PowerDown             0xB9 
#define FLASH_ReleasePowerDown      0xAB 
#define FLASH_DeviceID              0xAB 
#define FLASH_ManufactDeviceID      0x90 
#define FLASH_JedecDeviceID         0x9F 
#define FLASH_Enable4ByteAddr       0xB7
#define FLASH_Exit4ByteAddr         0xE9
#define FLASH_SetReadParam          0xC0 
#define FLASH_EnterQPIMode          0x38
#define FLASH_ExitQPIMode           0xFF

#define DEVICE_MANUFACTURER         0X52 // NM25Q128芯片的厂商ID
#define DEVICE_MEMORY_Type          0x21 // 芯片类型 
#define DEVICE_MEMORY_DENSITY       0x18 // 芯片容量
// NM25Q128是一款大容量 SPI FLASH产品，其容量为 16M
// 将 16M字节的容量分为 256个块（ Block 每个块大小为 64K字节，每个块又分为 16个扇区（ Sector）
// 每一个扇区 16页，每页 256个字节，即每个扇区 4K个字节
#define NM25Q128_SECTOR_SIZE       0x1000 // 4k
#define NM25Q128_PAGE_SIZE         0x100  // 256

#define NORFLASH_CS(x)  do { x ? \
                            HAL_GPIO_WritePin(SPI2_F_CS_GPIO_Port, SPI2_F_CS_Pin, GPIO_PIN_SET) : \
                            HAL_GPIO_WritePin(SPI2_F_CS_GPIO_Port, SPI2_F_CS_Pin, GPIO_PIN_RESET); \
                        } while(0)

#define W25QX_TIMEOUT 1000

/*******************************************************************************
 * global variable declaration
 ******************************************************************************/
extern SPI_HandleTypeDef hspi2;

/*******************************************************************************
 * global function declaration
 ******************************************************************************/
void W25Qx_Init(void);
void W25QX_WriteEnable(void);
void W25QX_WriteDisable(void);
W25QX_DeviceInfo_Struct W25Qx_GetDeviceInfo(void);
uint8_t W25Qx_ReadRegister(uint8_t reg);
void W25Qx_WriteRegister(uint8_t reg, uint8_t data);
void W25Qx_EraseChip(void);
void W25QX_EraseSector(uint32_t sectorAddr);
void W25QX_WriteBytes(uint32_t writeAddr, uint8_t* pData, uint32_t size);
void W25QX_ReadBytes(uint32_t readAddr, uint8_t* pData, uint32_t size);

#endif /* W25QX_H_ */

