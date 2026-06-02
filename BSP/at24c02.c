/*******************************************************************************
 * File : at24c02.c
 * Author : Bin@L
 * Created on: 2026/06/02
 ******************************************************************************/

/*******************************************************************************
 * include
 ******************************************************************************/
#include "at24c02.h"
#include "cmsis_os.h"
/*******************************************************************************
 * static function declaration
 ******************************************************************************/
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_ACK(void);
static void I2C_NACK(void);
static uint8_t I2C_WaitAck(void);
static void I2C_SendOneByte(uint8_t dat);
static uint8_t I2C_ReadOneByte(void);
static void I2C_Delay(void);

/*******************************************************************************
 * static variable define
 ******************************************************************************/


/*******************************************************************************
 * extern variable and declaratipn
 ******************************************************************************/

/*******************************************************************************
 * global function define
 ******************************************************************************/
/**
 * @brief 向AT24C02 EEPROM写入一个字节数据
 * 
 * @param addr 要写入数据的内存地址
 * @param data 要写入的数据字节
 * @return void
 */
void AT24C02_WriteOneByte(uint8_t addr, uint8_t data)
{
    // 发起I2C起始信号，开始通信
    I2C_Start();

    // 发送AT24C02的写设备地址并等待应答
    I2C_SendOneByte(AT24C02_ADDR_W);
    I2C_WaitAck();

    // 发送目标存储地址并等待应答
    I2C_SendOneByte(addr);
    I2C_WaitAck();

    // 发送要写入的数据并等待应答
    I2C_SendOneByte(data);
    I2C_WaitAck();

    // 发起I2C停止信号，结束通信
    I2C_Stop();
}

/**
 * @brief 从AT24C02 EEPROM中读取一个字节的数据
 * 
 * @param addr 要读取数据的内存地址
 * @return uint8_t 读取到的数据字节
 */
uint8_t AT24C02_ReadOneByte(uint8_t addr)
{
    uint8_t data;

    // 第一阶段：写入目标地址（伪写操作）
    I2C_Start();
    I2C_SendOneByte(AT24C02_ADDR_W);
    I2C_WaitAck();
    I2C_SendOneByte(addr);
    I2C_WaitAck();
    I2C_Stop();

    // 第二阶段：发送读命令并读取数据
    I2C_Start();
    I2C_SendOneByte(AT24C02_ADDR_R);
    I2C_WaitAck();
    data = I2C_ReadOneByte();
    
    // 发送NACK表示最后一个字节，随后停止通信
    I2C_NACK();
    I2C_Stop();
    
    return data;
}

/**
 * @brief 向AT24C02 EEPROM连续写入多个字节数据
 * 
 * @param addr 起始写入地址
 * @param data 指向待写入数据缓冲区的指针
 * @param len 需要写入的数据长度（字节数）
 * @return 无返回值
 */
void AT24C02_WriteMultiBytes(uint8_t addr, uint8_t *data, uint8_t len)
{
    // 循环逐字节写入数据，每写入一个字节后延时以确保EEPROM完成内部写入周期
    while(len--)
    {
        AT24C02_WriteOneByte(addr, *data);
        data++;
        addr++;
        osDelay(5);
    }
}

/**
 * @brief 从AT24C02 EEPROM中连续读取多个字节数据
 * 
 * @param addr 起始读取地址
 * @param data 指向用于存储读取数据的缓冲区的指针
 * @param len 需要读取的字节数量
 */
void AT24C02_ReadMultiBytes(uint8_t addr, uint8_t *data, uint8_t len)
{
    // 循环读取指定长度的数据，每次读取后递增地址和数据指针
    while(len--)
    {
        *data = AT24C02_ReadOneByte(addr);
        data++;
        addr++;
    }
}

/*******************************************************************************
 * static function define
 ******************************************************************************/
/**
 * @brief 产生I2C起始信号
 * 
 * @note 该函数无参数，无返回值
 */
static void I2C_Start(void)
{
    /* 确保在SDA高电平时SCL产生下降沿，以符合I2C起始条件时序 */
    I2C_SDA(1);
    I2C_SCL(1);
    I2C_Delay();
    I2C_SDA(0);
    I2C_Delay();
    I2C_SCL(0);
}

/**
 * @brief 产生I2C总线的停止信号
 * 
 * @note 该函数无参数，无返回值
 */
static void I2C_Stop(void)
{
    /* 在SCL高电平期间，SDA由低变高，产生停止条件 */
    I2C_SDA(0);
    I2C_SCL(1);
    I2C_Delay();
    I2C_SDA(1);
}

/**
 * @brief  I2C主机发送应答信号(ACK)
 * 
 * @note   该函数用于在I2C通信中，主机向从机发送一个低电平应答信号。
 *         时序遵循标准I2C协议：在SCL为低时拉低SDA，然后产生一个SCL高脉冲，
 *         最后释放SDA和SCL线以准备下一次数据传输。
 * 
 * @param  无
 * 
 * @return 无
 */
static void I2C_ACK(void)
{
    /* 拉低SDA线，表示应答(ACK) */
    I2C_SDA(0);
    
    /* 产生SCL高电平脉冲，让从机读取ACK信号 */
    I2C_SCL(1);
    I2C_Delay();
    
    /* 拉低SCL，完成当前位传输 */
    I2C_SCL(0);
    
    /* 释放SDA线，准备后续操作 */
    I2C_SDA(1);
}

/**
 * @brief  I2C总线发送非应答信号(NACK)
 * @note   该函数用于在I2C通信中向主机或从机发送非应答信号，通常表示接收结束或错误状态。
 *         时序说明：
 *         1. 拉高SDA线，表示非应答电平
 *         2. 拉高SCL线，产生时钟脉冲
 *         3. 延时以确保信号稳定
 *         4. 拉低SCL线，完成一个时钟周期
 * @param  无
 * @retval 无
 */
static void I2C_NACK(void)
{
    /* 设置SDA为高电平，表示NACK */
    I2C_SDA(1);
    I2C_SCL(1);
    I2C_Delay();
    I2C_SCL(0);
}

/**
 * @brief 等待I2C从机应答信号
 * 
 * @note 该函数在发送一个字节数据后调用，用于检测从机是否返回ACK信号。
 *       若超时未收到ACK，则强制停止I2C总线并返回错误状态。
 * 
 * @param 无
 * 
 * @return uint8_t 
 *         - 0: 成功接收到从机ACK信号
 *         - 1: 等待超时，未收到ACK信号（发生错误）
 */
static uint8_t I2C_WaitAck(void)
{
    uint16_t timeout = 1000;
    
    // 释放SDA线并产生第9个时钟脉冲以读取ACK信号
    I2C_SDA(1);
    I2C_Delay();
    I2C_SCL(1);
    I2C_Delay();
    
    // 循环等待从机拉低SDA线表示ACK，若超时则执行错误处理
    while(I2C_SDA_READ())
    {
        if(--timeout == 0)
        {
            I2C_SCL(0);
            I2C_Stop();
            return 1;
        }
    }
    
    // 拉低SCL线，完成ACK时钟周期
    I2C_SCL(0);
    I2C_Delay();
    return 0;
}


/**
 * @brief 通过I2C总线发送一个字节的数据
 * 
 * @param dat 要发送的8位数据
 * @return 无返回值
 */
static void I2C_SendOneByte(uint8_t dat)
{
    uint8_t i;

    /* 逐位发送数据，高位在前 */
    for(i = 0; i < 8; i++)
    {
        I2C_SDA(dat & 0x80);
        dat <<= 1;
        I2C_SCL(1);
        I2C_Delay();
        I2C_SCL(0);
        I2C_Delay();
    }
}


/**
 * @brief  从I2C总线上读取一个字节数据
 * @note   该函数通过软件模拟I2C时序，高位在前（MSB First）
 * @param  无
 * @retval uint8_t 读取到的8位数据
 */
static uint8_t I2C_ReadOneByte(void)
{
    uint8_t i, dat = 0;
    // 释放SDA线，准备接收数据
    I2C_SDA(1);
    // 循环读取8位数据
    for(i = 0; i < 8; i++)
    {
        // 拉高SCL，使SDA上的数据有效
        I2C_SCL(1);
        // 数据左移一位，为新读入的位腾出空间
        dat <<= 1;
        // 读取SDA电平并存入最低位
        dat |= I2C_SDA_READ();
        I2C_Delay();
        // 拉低SCL，允许SDA变化以准备下一位数据
        I2C_SCL(0);
        I2C_Delay();
    }
    return dat;
}

/**
 * @brief  I2C通信延时函数
 * @note   用于产生I2C时序所需的微小延时，当前固定延时2微秒
 * @param  无
 * @retval 无
 */
static void I2C_Delay(void)
{
    delay_us(2);
}


