/**
 * @file nRF24L01P.h
 *
 * @author Owen Edwards, adapted by Brandon Nguyen
 * 
 * @section LICENSE
 *
 * Copyright (c) 2010 Owen Edwards
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * @section DESCRIPTION
 *
 * nRF24L01+ Single Chip 2.4GHz Transceiver from Nordic Semiconductor.
 *
 * Datasheet:
 *
 * http://www.nordicsemi.no/files/Product/data_sheet/nRF24L01P_Product_Specification_1_0.pdf
 */

#ifndef __NRF24L01P_H__
#define __NRF24L01P_H__

/**
 * Defines
 */
#define NRF24L01P_TX_PWR_ZERO_DB         0
#define NRF24L01P_TX_PWR_MINUS_6_DB     -6
#define NRF24L01P_TX_PWR_MINUS_12_DB   -12
#define NRF24L01P_TX_PWR_MINUS_18_DB   -18

#define NRF24L01P_DATARATE_250_KBPS    250
#define NRF24L01P_DATARATE_1_MBPS     1000
#define NRF24L01P_DATARATE_2_MBPS     2000

#define NRF24L01P_CRC_NONE               0
#define NRF24L01P_CRC_8_BIT              8
#define NRF24L01P_CRC_16_BIT            16

#define NRF24L01P_MIN_RF_FREQUENCY    2400
#define NRF24L01P_MAX_RF_FREQUENCY    2525

#define NRF24L01P_PIPE_P0                0
#define NRF24L01P_PIPE_P1                1
#define NRF24L01P_PIPE_P2                2
#define NRF24L01P_PIPE_P3                3
#define NRF24L01P_PIPE_P4                4
#define NRF24L01P_PIPE_P5                5

/**
* Default setup for the nRF24L01+, based on the Sparkfun "Nordic Serial Interface Board"
*  for evaluation (http://www.sparkfun.com/products/9019)
*/
#define DEFAULT_NRF24L01P_ADDRESS       ((unsigned long long) 0xE7E7E7E7E7 )
#define DEFAULT_NRF24L01P_ADDRESS_WIDTH  5
#define DEFAULT_NRF24L01P_CRC            NRF24L01P_CRC_8_BIT
#define DEFAULT_NRF24L01P_RF_FREQUENCY  (NRF24L01P_MIN_RF_FREQUENCY + 2)
#define DEFAULT_NRF24L01P_DATARATE       NRF24L01P_DATARATE_1_MBPS
#define DEFAULT_NRF24L01P_TX_PWR         NRF24L01P_TX_PWR_ZERO_DB
#define DEFAULT_NRF24L01P_TRANSFER_SIZE  4

/**
 * nRF24L01+ Single Chip 2.4GHz Transceiver from Nordic Semiconductor.
 */

/**
 * @fn nRF24L01p_Init
 *
 * Initializes it
 */
void nRF24L01p_Init(void);

/**
 * Set the RF frequency.
 *
 * @param frequency the frequency of RF transmission in MHz (2400..2525).
 */
void nRF24L01p_setRfFrequency(int frequency);

/**
 * Get the RF frequency.
 *
 * @return the frequency of RF transmission in MHz (2400..2525).
 */
int nRF24L01p_getRfFrequency(void);

/**
 * Set the RF output power.
 *
 * @param power the RF output power in dBm (0, -6, -12 or -18).
 */
void nRF24L01p_setRfOutputPower(int power );

/**
 * Get the RF output power.
 *
 * @return the RF output power in dBm (0, -6, -12 or -18).
 */
int nRF24L01p_getRfOutputPower(void);

/**
 * Set the Air data rate.
 *
 * @param rate the air data rate in kbps (250, 1M or 2M).
 */
void nRF24L01p_setAirDataRate(int rate);

/**
 * Get the Air data rate.
 *
 * @return the air data rate in kbps (250, 1M or 2M).
 */
int nRF24L01p_getAirDataRate(void);

/**
 * Set the CRC width.
 *
 * @param width the number of bits for the CRC (0, 8 or 16).
 */
void nRF24L01p_setCrcWidth(int width);

/**
 * Get the CRC width.
 *
 * @return the number of bits for the CRC (0, 8 or 16).
 */
int nRF24L01p_getCrcWidth(void);

/**
 * Set the Receive address.
 *
 * @param address address associated with the particular pipe
 * @param width width of the address in bytes (3..5)
 * @param pipe pipe to associate the address with (0..5, default 0)
 *
 * Note that Pipes 0 & 1 have 3, 4 or 5 byte addresses,
 *  while Pipes 2..5 only use the lowest byte (bits 7..0) of the
 *  address provided here, and use 2, 3 or 4 bytes from Pipe 1's address.
 *  The width parameter is ignored for Pipes 2..5.
 */
void nRF24L01p_setRxAddress(unsigned long long address, int width, int pipe);

void nRF24L01p_setRxAddressSplit(unsigned long msb_address, unsigned long lsb_address, int width, int pipe);

/**
 * Set the Transmit address.
 *
 * @param address address for transmission
 * @param width width of the address in bytes (3..5)
 *
 * Note that the address width is shared with the Receive pipes,
 *  so a change to that address width affect transmissions.
 */
void nRF24L01p_setTxAddress(unsigned long long address, int width);

void nRF24L01p_setTxAddressSplit(unsigned long msb_address, unsigned long lsb_address, int width);

/**
 * Get the Receive address.
 *
 * @param pipe pipe to get the address from (0..5, default 0)
 * @return the address associated with the particular pipe
 */
unsigned long long nRF24L01p_getRxAddress(int pipe);

/**
 * Get the Transmit address.
 *
 * @return address address for transmission
 */
unsigned long long nRF24L01p_getTxAddress(void);

/**
 * Set the transfer size.
 *
 * @param size the size of the transfer, in bytes (1..32)
 * @param pipe pipe for the transfer (0..5, default 0)
 */
void nRF24L01p_setTransferSize(int size, int pipe);

/**
 * Get the transfer size.
 *
 * @return the size of the transfer, in bytes (1..32).
 */
int nRF24L01p_getTransferSize(int pipe);


/**
 * Get the RPD (Received Power Detector) state.
 *
 * @return true if the received power exceeded -64dBm
 */
int nRF24L01p_getRPD(void);

/**
 * Put the nRF24L01+ into Receive mode
 */
void nRF24L01p_setReceiveMode(void);

/**
 * Put the nRF24L01+ into Transmit mode
 */
void nRF24L01p_setTransmitMode(void);

/**
 * Power up the nRF24L01+ into Standby mode
 */
void nRF24L01p_powerUp(void);

/**
 * Power down the nRF24L01+ into Power Down mode
 */
void nRF24L01p_powerDown(void);

/**
 * Enable the nRF24L01+ to Receive or Transmit (using the CE pin)
 */
void nRF24L01p_enable(void);

/**
 * Disable the nRF24L01+ to Receive or Transmit (using the CE pin)
 */
void nRF24L01p_disable(void);

/**
 * Transmit data
 *
 * @param pipe is ignored (included for consistency with file write routine)
 * @param data pointer to an array of bytes to write
 * @param count the number of bytes to send (1..32)
 * @return the number of bytes actually written, or -1 for an error
 */
int nRF24L01p_write(int pipe, const char *data, int count);

/**
 * Receive data
 *
 * @param pipe the receive pipe to get data from
 * @param data pointer to an array of bytes to store the received data
 * @param count the number of bytes to receive (1..32)
 * @return the number of bytes actually received, 0 if none are received, or -1 for an error
 */
int nRF24L01p_read(int pipe, char *data, int count);

/**
 * Determine if there is data available to read
 *
 * @param pipe the receive pipe to check for data
 * @return true if the is data waiting in the given pipe
 */
int nRF24L01p_readable(int pipe);

/**
 * Disable all receive pipes
 *
 * Note: receive pipes are enabled when their address is set.
 */
void nRF24L01p_disableAllRxPipes(void);

/**
 * Disable AutoAcknowledge function
 */
void nRF24L01p_disableAutoAcknowledge(void);

/**
 * Enable AutoAcknowledge function
 *
 * @param pipe the receive pipe
 */
void nRF24L01p_enableAutoAcknowledge(int pipe);

/**
 * Disable AutoRetransmit function
 */
void nRF24L01p_disableAutoRetransmit(void);

/**
 * Enable AutoRetransmit function
 *
 * @param delay the delay between restransmits, in uS (250uS..4000uS)
 * @param count number of retransmits before generating an error (1..15)
 */
void nRF24L01p_enableAutoRetransmit(int delay, int count);

#endif /* __NRF24L01P_H__ */
