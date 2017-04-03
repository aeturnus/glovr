#ifndef __NRF24L01P_H__
#define __NRF24L01P_H__
/**
 * @file    nRF24L01p.h
 * @author  Brandon Nguyen
 * @brief   Driver for nRF24L01+
 */

#include <stdint.h>

/**
 * @fn          nRF24L01p_Init
 * @brief       Initializes the NRF24
 */
void nRF24L01p_Init(void);

/**
 * @fn          nRF24L01p_SetTX
 * @brief       Sets it as the transmitter
 */
void nRF24L01p_SetTX(void);

/**
 * @fn          nRF24L01p_SetRX
 * @brief       Sets it as the receiver
 */
void nRF24L01p_SetRX(void);

/**
 * @fn          nRF24L01p_Send
 * @brief       Sends bytes to the TX fifo
 */
void nRF24L01p_Send(int length, const uint8_t * buffer);

/**
 * @fn          nRF24L01p_Recv
 * @brief       Receive bytes from the RX fifo
 */
void nRF24L01p_Recv(int length, const uint8_t * buffer);

/**
 * @fn          nRF24L01p_RegisterHandler
 * @brief       Register a handler for receives
 * @param[in]   handlerRX The handler to be run after a receive
 */
void nRF24L01p_RegisterRXHandler(void (* handlerRX)(void));

/**
 * @fn          nRF24L01p_UnregisterHandler
 * @brief       Unregisters the RX handler
 */
void nRF24L01p_UnregisterRXHandler(void);

#endif//__NRF24L01P_H__