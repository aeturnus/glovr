#ifndef __NRF24L01_H__
#define __NRF24L01_H__
/**
 * @file    NRF24L01.h
 * @author  Brandon Nguyen
 * @brief   Driver for nRF24L01+
 */

#include <stdint.h>

/**
 * @fn          NRF24L01_Init
 * @brief       Initializes the NRF24
 */
void NRF24L01_Init(void);

/**
 * @fn          NRF24L01_SetTX
 * @brief       Sets it as the transmitter
 */
void NRF24L01_SetTX(void);

/**
 * @fn          NRF24L01_SetRX
 * @brief       Sets it as the receiver
 */
void NRF24L01_SetRX(void);

/**
 * @fn          NRF24L01_Send
 * @brief       Sends bytes to the TX fifo
 */
void NRF24L01_Send(int length, const uint8_t * buffer);

/**
 * @fn          NRF24L01_Recv
 * @brief       Receive bytes from the RX fifo
 */
void NRF24L01_Recv(int length, const uint8_t * buffer);

/**
 * @fn          NRF24L01_RegisterHandler
 * @brief       Register a handler for receives
 * @param[in]   handlerRX The handler to be run after a receive
 */
void NRF24L01_RegisterRXHandler(void (* handlerRX)(void));

/**
 * @fn          NRF24L01_UnregisterHandler
 * @brief       Unregisters the RX handler
 */
void NRF24L01_UnregisterRXHandler(void);

#endif//__NRF24L01_H__