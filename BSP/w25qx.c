/*******************************************************************************
 * File : w25qx.c
 * Author : Bin@L
 * Created on: 2026/06/03
 ******************************************************************************/

/*******************************************************************************
 * include
 ******************************************************************************/
#include "w25qx.h"
#include <string.h>
/*******************************************************************************
 * static function declaration
 ******************************************************************************/
static uint8_t SPI_WirteRead(uint8_t txData);
static void W25Qx_WaitBusy(void);
static void W25Qx_SendAddress(uint32_t addr);
static void W25Qx_WritePage(uint32_t writeAddr, uint8_t* pData, uint32_t size);
static void W25Qx_WriteSector(uint32_t writeAddr, uint8_t* pData, uint32_t size);

/*******************************************************************************
 * static variable define
 ******************************************************************************/


/*******************************************************************************
 * extern variable and declaratipn
 ******************************************************************************/
static W25QX_DeviceInfo_Struct W25Qx_DeviceInfo;

/*******************************************************************************
 * global function define
 ******************************************************************************/
void W25Qx_Init(void)
{
    NORFLASH_CS(0);
    SPI_WirteRead(FLASH_JedecDeviceID);
    W25Qx_DeviceInfo.manufacturerID = SPI_WirteRead(0xFF);
    W25Qx_DeviceInfo.memoryType = SPI_WirteRead(0xFF);
    W25Qx_DeviceInfo.memoryDensity = SPI_WirteRead(0xFF);
    NORFLASH_CS(1);
}

void W25QX_WriteEnable(void)
{
    NORFLASH_CS(0);
    SPI_WirteRead(FLASH_WriteEnable);
    W25Qx_WaitBusy();
    NORFLASH_CS(1);
}

void W25QX_WriteDisable(void)
{
    NORFLASH_CS(0);
    SPI_WirteRead(FLASH_WriteDisable);
    NORFLASH_CS(1);
}

W25QX_DeviceInfo_Struct W25Qx_GetDeviceInfo(void)
{
    return W25Qx_DeviceInfo;
}

uint8_t W25Qx_ReadRegister(uint8_t reg)
{
    uint8_t cmd = 0, data = 0;
    
    switch(reg)
    {
        case 1:
            cmd = FLASH_ReadStatusReg1;
            break;
        case 2:
            cmd = FLASH_ReadStatusReg2;
            break;
        case 3:
            cmd = FLASH_ReadStatusReg3;
            break;
        default:
            cmd = FLASH_ReadStatusReg1;
            break;
    }
    NORFLASH_CS(0);
    SPI_WirteRead(cmd);
    data = SPI_WirteRead(0xFF);
    NORFLASH_CS(1);
    return data;
}


void W25Qx_WriteRegister(uint8_t reg, uint8_t data)
{
    uint8_t cmd = 0; 
    switch(reg)
    {
        case 1:
            cmd = FLASH_WriteStatusReg1;
            break;
        case 2:
            cmd = FLASH_WriteStatusReg2;
            break;
        case 3:
            cmd = FLASH_WriteStatusReg3;
            break;
        default:
            cmd = FLASH_ReadStatusReg1;
            break;
    }
    NORFLASH_CS(0);
    SPI_WirteRead(cmd);
    SPI_WirteRead(data);
    NORFLASH_CS(1);
}

void W25Qx_EraseChip(void)
{
    W25QX_WriteEnable();
    NORFLASH_CS(0);
    SPI_WirteRead(FLASH_ChipErase);
    W25Qx_WaitBusy();
    NORFLASH_CS(1);
}

void W25QX_EraseSector(uint32_t sectorAddr)
{
    uint32_t addr = sectorAddr * NM25Q128_SECTOR_SIZE;
    W25QX_WriteEnable();
    NORFLASH_CS(0);
    SPI_WirteRead(FLASH_SectorErase);
    W25Qx_SendAddress(addr);
    NORFLASH_CS(1);
    W25Qx_WaitBusy();
}

uint8_t g_NorFlashBuf[NM25Q128_SECTOR_SIZE] = {0xFF};
void W25QX_WriteBytes(uint32_t writeAddr, uint8_t* pData, uint32_t size)
{
    uint32_t i = 0;
    uint32_t secPos = writeAddr / NM25Q128_SECTOR_SIZE;
    uint32_t secOffset = writeAddr % NM25Q128_SECTOR_SIZE;
    uint32_t secRemain = NM25Q128_SECTOR_SIZE - secOffset;
    if(size <= secRemain)
    {
        secRemain = size;
    }

    while(1)
    {
        memset(g_NorFlashBuf, 0xFF, NM25Q128_SECTOR_SIZE);
        W25QX_ReadBytes(secPos*NM25Q128_SECTOR_SIZE, g_NorFlashBuf, NM25Q128_SECTOR_SIZE);
        for(i = 0; i<secRemain; i++)
        {
            if(0xFF != g_NorFlashBuf[secOffset + i])
            {
                break;
            }
        }
        if(i < secRemain)
        {
            W25QX_EraseSector(secPos);
            for(i=0; i<secRemain; i++)
            {
                g_NorFlashBuf[secOffset + i] = pData[i];
            }
            W25Qx_WriteSector(secPos * NM25Q128_SECTOR_SIZE, g_NorFlashBuf, NM25Q128_SECTOR_SIZE);
        }
        else
        {
            W25Qx_WriteSector(writeAddr, pData, secRemain);
        }
        if(size == secRemain)
        {
            break;
        }
        else
        {
            writeAddr += secRemain;
            pData += secRemain;
            size -= secRemain;
            secPos++;
            secOffset = 0;
            if(size < NM25Q128_SECTOR_SIZE)
            {
                secRemain = size;
            }
            else
            {
                secRemain = NM25Q128_SECTOR_SIZE;
            }
        }
    }
}
void W25QX_ReadBytes(uint32_t readAddr, uint8_t* pData, uint32_t size)
{
    NORFLASH_CS(0);
    SPI_WirteRead(FLASH_ReadData);
    W25Qx_SendAddress(readAddr);
    for(uint32_t i = 0; i < size; i++)
    {
        pData[i] = SPI_WirteRead(0xFF);
    }
    NORFLASH_CS(1);
}

/*******************************************************************************
 * static function define
 ******************************************************************************/
static uint8_t SPI_WirteRead(uint8_t txData)
{
    uint8_t rxData = 0;
    HAL_SPI_TransmitReceive(&hspi2, &txData, &rxData, 1, W25QX_TIMEOUT);
    return rxData;
}

static void W25Qx_WaitBusy(void)
{
    while((W25Qx_ReadRegister(1) & 0x01) == 0x01);
}

static void W25Qx_SendAddress(uint32_t addr)
{
    SPI_WirteRead((uint8_t)(addr >> 16));
    SPI_WirteRead((uint8_t)(addr >> 8));
    SPI_WirteRead((uint8_t)addr);
}

static void W25Qx_WritePage(uint32_t writeAddr, uint8_t* pData, uint32_t size)
{
    uint16_t i;

    W25QX_WriteEnable();

    NORFLASH_CS(0);
    SPI_WirteRead(FLASH_PageProgram);
    W25Qx_SendAddress(writeAddr);

    for(i=0;i<size;i++)
    {
        SPI_WirteRead(pData[i]);
    }
    
    NORFLASH_CS(1);
    W25Qx_WaitBusy();
}

static void W25Qx_WriteSector(uint32_t writeAddr, uint8_t* pData, uint32_t size)
{
    uint16_t pageremain;
    pageremain = NM25Q128_PAGE_SIZE - writeAddr % NM25Q128_PAGE_SIZE; 

    if (size <= pageremain)  
    {
        pageremain = size;
    }

    while (1)
    {
        W25Qx_WritePage(writeAddr, pData, pageremain);

        if (size == pageremain) 
        {
            break;
        }
        else     
        {
            writeAddr += pageremain;
            pData += pageremain;
            size -= pageremain;

            if (size > NM25Q128_PAGE_SIZE)
            {
                pageremain = NM25Q128_PAGE_SIZE;
            }
            else
            {
                pageremain = size;
            }
        }
    }
}

