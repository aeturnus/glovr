#ifndef __I2C0_H__
#define __I2C0_H__
/**
 * @file    I2C.h
 * @author  Brandon Nguyen
 * @brief   Low-level driver for I2C interface
 */
#include <stdint.h>

/**
 * @fn      I2C_Init
 * @brief   Initializes the I2C
 */
void I2C_Init(void);

/**
 * @fn          I2C_ReadReg
 * @brief       Reads registers from I2C slave into buffer
 * @param[out]  slave   I2C address of slave
 * @param[out]  addr    Base address of register(s) on the slave
 * @param[out]  length  How many bytes to read
 * @param[in]   buffer  Buffer to store read data
 */
int I2C_ReadReg(int8_t slave, int8_t addr, int length, uint8_t * buffer);

/**
 * @fn          I2C_WriteReg
 * @brief       Write registers from buffer to I2C slave
 * @param[out]  slave   I2C address of slave
 * @param[out]  addr    Base address of register(s) on the slave
 * @param[out]  length  How many bytes to read
 * @param[out]  buffer  Buffer to send data from
 */
int I2C_WriteReg(int8_t slave, int8_t addr, int length, const uint8_t * buffer);


/**
 * @fn          I2C_Recv
 * @brief       Receives bytes from I2C slave
 * @param[out]  slave   I2C address of slave
 * @param[out]  length  How many bytes to receive
 * @param[out]  slave   I2C address of slave
 */
int I2C_Recv(int8_t slave, int length, uint8_t * buffer);

/**
 * @fn          I2C_Send
 * @brief       Send bytes to I2C slave
 * @param[out]  slave   I2C address of slave
 * @param[out]  length  How many bytes to send
 * @param[in]   buffer  Buffer to send data from
*/
int I2C_Send(int8_t slave, int length, const uint8_t * buffer);

#endif//__I2C0_H__
