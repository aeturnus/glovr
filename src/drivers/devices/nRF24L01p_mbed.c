/**
 * @file nRF24L01P_mbed.c
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

/**
 * Includes
 */
#include <drivers/devices/nRF24L01p_mbed.h>
#include <tm4c123gh6pm.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Defines
 *
 * (Note that all defines here start with an underscore, e.g. '_NRF24L01P_MODE_UNKNOWN',
 *  and are local to this library.  The defines in the nRF24L01P.h file do not start
 *  with the underscore, and can be used by code to access this library.)
 */

typedef enum {
    _NRF24L01P_MODE_UNKNOWN,
    _NRF24L01P_MODE_POWER_DOWN,
    _NRF24L01P_MODE_STANDBY,
    _NRF24L01P_MODE_RX,
    _NRF24L01P_MODE_TX,
} nRF24L01P_Mode_Type;

/*
 * The following FIFOs are present in nRF24L01+:
 *   TX three level, 32 byte FIFO
 *   RX three level, 32 byte FIFO
 */
#define _NRF24L01P_TX_FIFO_COUNT   3
#define _NRF24L01P_RX_FIFO_COUNT   3

#define _NRF24L01P_TX_FIFO_SIZE   32
#define _NRF24L01P_RX_FIFO_SIZE   32

#define _NRF24L01P_SPI_MAX_DATA_RATE     10000000

#define _NRF24L01P_SPI_CMD_RD_REG            0x00
#define _NRF24L01P_SPI_CMD_WR_REG            0x20
#define _NRF24L01P_SPI_CMD_RD_RX_PAYLOAD     0x61   
#define _NRF24L01P_SPI_CMD_WR_TX_PAYLOAD     0xa0
#define _NRF24L01P_SPI_CMD_FLUSH_TX          0xe1
#define _NRF24L01P_SPI_CMD_FLUSH_RX          0xe2
#define _NRF24L01P_SPI_CMD_REUSE_TX_PL       0xe3
#define _NRF24L01P_SPI_CMD_R_RX_PL_WID       0x60
#define _NRF24L01P_SPI_CMD_W_ACK_PAYLOAD     0xa8
#define _NRF24L01P_SPI_CMD_W_TX_PYLD_NO_ACK  0xb0
#define _NRF24L01P_SPI_CMD_NOP               0xff


#define _NRF24L01P_REG_CONFIG                0x00
#define _NRF24L01P_REG_EN_AA                 0x01
#define _NRF24L01P_REG_EN_RXADDR             0x02
#define _NRF24L01P_REG_SETUP_AW              0x03
#define _NRF24L01P_REG_SETUP_RETR            0x04
#define _NRF24L01P_REG_RF_CH                 0x05
#define _NRF24L01P_REG_RF_SETUP              0x06
#define _NRF24L01P_REG_STATUS                0x07
#define _NRF24L01P_REG_OBSERVE_TX            0x08
#define _NRF24L01P_REG_RPD                   0x09
#define _NRF24L01P_REG_RX_ADDR_P0            0x0a
#define _NRF24L01P_REG_RX_ADDR_P1            0x0b
#define _NRF24L01P_REG_RX_ADDR_P2            0x0c
#define _NRF24L01P_REG_RX_ADDR_P3            0x0d
#define _NRF24L01P_REG_RX_ADDR_P4            0x0e
#define _NRF24L01P_REG_RX_ADDR_P5            0x0f
#define _NRF24L01P_REG_TX_ADDR               0x10
#define _NRF24L01P_REG_RX_PW_P0              0x11
#define _NRF24L01P_REG_RX_PW_P1              0x12
#define _NRF24L01P_REG_RX_PW_P2              0x13
#define _NRF24L01P_REG_RX_PW_P3              0x14
#define _NRF24L01P_REG_RX_PW_P4              0x15
#define _NRF24L01P_REG_RX_PW_P5              0x16
#define _NRF24L01P_REG_FIFO_STATUS           0x17
#define _NRF24L01P_REG_DYNPD                 0x1c
#define _NRF24L01P_REG_FEATURE               0x1d

#define _NRF24L01P_REG_ADDRESS_MASK          0x1f

// CONFIG register:
#define _NRF24L01P_CONFIG_PRIM_RX        (1<<0)
#define _NRF24L01P_CONFIG_PWR_UP         (1<<1)
#define _NRF24L01P_CONFIG_CRC0           (1<<2)
#define _NRF24L01P_CONFIG_EN_CRC         (1<<3)
#define _NRF24L01P_CONFIG_MASK_MAX_RT    (1<<4)
#define _NRF24L01P_CONFIG_MASK_TX_DS     (1<<5)
#define _NRF24L01P_CONFIG_MASK_RX_DR     (1<<6)

#define _NRF24L01P_CONFIG_CRC_MASK       (_NRF24L01P_CONFIG_EN_CRC|_NRF24L01P_CONFIG_CRC0)
#define _NRF24L01P_CONFIG_CRC_NONE       (0)
#define _NRF24L01P_CONFIG_CRC_8BIT       (_NRF24L01P_CONFIG_EN_CRC)
#define _NRF24L01P_CONFIG_CRC_16BIT      (_NRF24L01P_CONFIG_EN_CRC|_NRF24L01P_CONFIG_CRC0)

// EN_AA register:
#define _NRF24L01P_EN_AA_NONE            0

// EN_RXADDR register:
#define _NRF24L01P_EN_RXADDR_NONE        0

// SETUP_AW register:
#define _NRF24L01P_SETUP_AW_AW_MASK      (0x3<<0)
#define _NRF24L01P_SETUP_AW_AW_3BYTE     (0x1<<0)
#define _NRF24L01P_SETUP_AW_AW_4BYTE     (0x2<<0)
#define _NRF24L01P_SETUP_AW_AW_5BYTE     (0x3<<0)

// SETUP_RETR register:
#define _NRF24L01P_SETUP_RETR_NONE       0

// RF_SETUP register:
#define _NRF24L01P_RF_SETUP_RF_PWR_MASK          (0x3<<1)
#define _NRF24L01P_RF_SETUP_RF_PWR_0DBM          (0x3<<1)
#define _NRF24L01P_RF_SETUP_RF_PWR_MINUS_6DBM    (0x2<<1)
#define _NRF24L01P_RF_SETUP_RF_PWR_MINUS_12DBM   (0x1<<1)
#define _NRF24L01P_RF_SETUP_RF_PWR_MINUS_18DBM   (0x0<<1)

#define _NRF24L01P_RF_SETUP_RF_DR_HIGH_BIT       (1 << 3)
#define _NRF24L01P_RF_SETUP_RF_DR_LOW_BIT        (1 << 5)
#define _NRF24L01P_RF_SETUP_RF_DR_MASK           (_NRF24L01P_RF_SETUP_RF_DR_LOW_BIT|_NRF24L01P_RF_SETUP_RF_DR_HIGH_BIT)
#define _NRF24L01P_RF_SETUP_RF_DR_250KBPS        (_NRF24L01P_RF_SETUP_RF_DR_LOW_BIT)
#define _NRF24L01P_RF_SETUP_RF_DR_1MBPS          (0)
#define _NRF24L01P_RF_SETUP_RF_DR_2MBPS          (_NRF24L01P_RF_SETUP_RF_DR_HIGH_BIT)

// STATUS register:
#define _NRF24L01P_STATUS_TX_FULL        (1<<0)
#define _NRF24L01P_STATUS_RX_P_NO        (0x7<<1)
#define _NRF24L01P_STATUS_MAX_RT         (1<<4)
#define _NRF24L01P_STATUS_TX_DS          (1<<5)
#define _NRF24L01P_STATUS_RX_DR          (1<<6)

// RX_PW_P0..RX_PW_P5 registers:
#define _NRF24L01P_RX_PW_Px_MASK         0x3F

#define _NRF24L01P_TIMING_Tundef2pd_us     100000   // 100mS
#define _NRF24L01P_TIMING_Tstby2a_us          130   // 130uS
#define _NRF24L01P_TIMING_Thce_us              10   //  10uS
#define _NRF24L01P_TIMING_Tpd2stby_us        4500   // 4.5mS worst case
#define _NRF24L01P_TIMING_Tpece2csn_us          4   //   4uS


/* PB2 => CE
 * PB3 => CSN
 * PB5 => IRQ
 */
// bit specific addresses for the ports
#define NRF_CE_SHF  2
#define NRF_CE  (*((volatile uint32_t *)((uint32_t)GPIO_PORTB_DATA_BITS_R | (1<<(NRF_CE_SHF+2)))))
//#define NRF_CE  (*((volatile uint32_t *)(0x40005000 | 0x10)))
#define NRF_CSN_SHF 3
#define NRF_CSN (*((volatile uint32_t *)((uint32_t)GPIO_PORTB_DATA_BITS_R | (1<<(NRF_CSN_SHF+2)))))
//#define NRF_CSN (*((volatile uint32_t *)(0x40005000 | 0x20)))
static inline void port_init(void)
{
  SYSCTL_RCGCGPIO_R |= 0x02;  // turn on PB
  SYSCTL_RCGCSSI_R  |= 0x04;  // turn on SSI2
  while((SYSCTL_PRGPIO_R&0x2)==0){};  // wait for gpio
  GPIO_PORTB_AFSEL_R  |= 0xD0;  // PB7,6,4 for SSI
  GPIO_PORTB_DEN_R    |= 0xFC;  // PB7-2 digital needed
  GPIO_PORTB_DIR_R    |= 0x0C;  // PB2,3 are output
  GPIO_PORTB_AMSEL_R  &= ~0xFC; // turn off analog

  // enable SSI2 on 7,6,4 GPIO on the others
  GPIO_PORTB_PCTL_R   = (GPIO_PORTB_PCTL_R&0x000000FF) | 0x22020000;

  // need to initialize tranceiver IRQ (active low) detection
  // TODO: setupt falling edge interrupt on Port B


  // need to initialize SSI2
  SSI2_CR1_R = 0x0; // turn off, master mode
  SSI2_CPSR_R = 2;  // CLK speed: divide by 2
  SSI2_CR0_R = (SSI2_CR0_R&~(0x0000FFF0));  // SCR = 0, SPH = 0, SPO = 0 Freescale
  SSI2_CR0_R |= 0x07; // DSS=8-bit data
  SSI2_CR1_R |= 0x02; // enable SSI woo!
}

// synchronously send a byte, blocks
static inline void ssi_send(uint8_t data)
{
  while(!(SSI2_SR_R & SSI_SR_TNF)){}  // wait until TX has an opening
  SSI2_DR_R = (data&0xFF);
}

// synchronously recv a byte, blocks
static inline uint8_t ssi_recv(void)
{
  while(!(SSI2_SR_R & SSI_SR_RNE)){}  // wait until RX has something
  uint8_t val = SSI2_DR_R & 0xFF;
  return val;
}

static inline int get_ce(void)
{
  return (NRF_CE>>NRF_CE_SHF)&0x1;
}


static inline int set_ce(int x)
{
  int old = get_ce();
  NRF_CE = (x<<NRF_CE_SHF);
  return old;
}


static inline int get_csn(void)
{
  return (NRF_CSN>>NRF_CSN_SHF)&0x1;
}


static inline int set_csn(int x)
{
  int old = get_csn();
  NRF_CSN = (x<<NRF_CSN_SHF);
  return old;
}


// above is the MCU specific code
// below is the generic interface

static inline int disable(void)
{
  return set_ce(0);
}

static inline int enable(void)
{
  return set_ce(1);
}

static inline int select(void)
{
  return set_csn(0);
}

static inline int deselect(void)
{
  return set_csn(1);
}

static void wait_us( int val )
{
  //TODO:
  while(val-- > 0)
  {
    for(volatile int i = 0; i < 0xFF; i++)
    {
    }
  }
}

/**
 * Get the contents of an addressable register.
 *
 * @param regAddress address of the register
 * @return the contents of the register
 */
static int nRF24L01p_getRegister(int regAddress);

/**
 * Set the contents of an addressable register.
 *
 * @param regAddress address of the register
 * @param regData data to write to the register
 */
static void nRF24L01p_setRegister(int regAddress, int regData);

/**
 * Get the contents of the status register.
 *
 * @return the contents of the status register
 */
static int nRF24L01p_getStatusRegister(void);

static int mode;

/**
 * Methods
 */
void nRF24L01p_Init()
{
    mode = _NRF24L01P_MODE_UNKNOWN;

    port_init();
    disable();
    deselect();

    wait_us(_NRF24L01P_TIMING_Tundef2pd_us);    // Wait for Power-on reset

    nRF24L01p_setRegister(_NRF24L01P_REG_CONFIG, 0); // Power Down

    nRF24L01p_setRegister(_NRF24L01P_REG_STATUS, _NRF24L01P_STATUS_MAX_RT|_NRF24L01P_STATUS_TX_DS|_NRF24L01P_STATUS_RX_DR);   // Clear any pending interrupts

    //
    // Setup default configuration
    //
    nRF24L01p_disableAllRxPipes();
    nRF24L01p_setRfFrequency(DEFAULT_NRF24L01P_RF_FREQUENCY);
    nRF24L01p_setRfOutputPower(DEFAULT_NRF24L01P_TX_PWR);
    nRF24L01p_setAirDataRate(DEFAULT_NRF24L01P_DATARATE);
    nRF24L01p_setCrcWidth(DEFAULT_NRF24L01P_CRC);
    nRF24L01p_setTxAddress(DEFAULT_NRF24L01P_ADDRESS,DEFAULT_NRF24L01P_ADDRESS_WIDTH);
    nRF24L01p_setRxAddress(DEFAULT_NRF24L01P_ADDRESS,DEFAULT_NRF24L01P_ADDRESS_WIDTH,NRF24L01P_PIPE_P0);
    nRF24L01p_disableAutoAcknowledge();
    nRF24L01p_disableAutoRetransmit();
    nRF24L01p_setTransferSize(DEFAULT_NRF24L01P_TRANSFER_SIZE,NRF24L01P_PIPE_P0);

    mode = _NRF24L01P_MODE_POWER_DOWN;

}


void nRF24L01p_powerUp(void) {

    int config = nRF24L01p_getRegister(_NRF24L01P_REG_CONFIG);

    config |= _NRF24L01P_CONFIG_PWR_UP;

    nRF24L01p_setRegister(_NRF24L01P_REG_CONFIG, config);

    // Wait until the nRF24L01+ powers up
    wait_us( _NRF24L01P_TIMING_Tpd2stby_us );

    mode = _NRF24L01P_MODE_STANDBY;

}


void nRF24L01p_powerDown(void) {

    int config = nRF24L01p_getRegister(_NRF24L01P_REG_CONFIG);

    config &= ~_NRF24L01P_CONFIG_PWR_UP;

    nRF24L01p_setRegister(_NRF24L01P_REG_CONFIG, config);

    // Wait until the nRF24L01+ powers down
    wait_us( _NRF24L01P_TIMING_Tpd2stby_us );    // This *may* not be necessary (no timing is shown in the Datasheet), but just to be safe

    mode = _NRF24L01P_MODE_POWER_DOWN;

}


void nRF24L01p_setReceiveMode(void) {

    if ( _NRF24L01P_MODE_POWER_DOWN == mode ) nRF24L01p_powerUp();

    int config = nRF24L01p_getRegister(_NRF24L01P_REG_CONFIG);

    config |= _NRF24L01P_CONFIG_PRIM_RX;

    nRF24L01p_setRegister(_NRF24L01P_REG_CONFIG, config);

    mode = _NRF24L01P_MODE_RX;

}


void nRF24L01p_setTransmitMode(void) {

    if ( _NRF24L01P_MODE_POWER_DOWN == mode ) nRF24L01p_powerUp();

    int config = nRF24L01p_getRegister(_NRF24L01P_REG_CONFIG);

    config &= ~_NRF24L01P_CONFIG_PRIM_RX;

    nRF24L01p_setRegister(_NRF24L01P_REG_CONFIG, config);

    mode = _NRF24L01P_MODE_TX;

}


void nRF24L01p_setRfFrequency(int frequency) {

    if ( ( frequency < NRF24L01P_MIN_RF_FREQUENCY ) || ( frequency > NRF24L01P_MAX_RF_FREQUENCY ) ) {

        //error( "nRF24L01P: Invalid RF Frequency setting %d\r\n", frequency );
        return;

    }

    int channel = ( frequency - NRF24L01P_MIN_RF_FREQUENCY ) & 0x7F;

    nRF24L01p_setRegister(_NRF24L01P_REG_RF_CH, channel);

}


int nRF24L01p_getRfFrequency(void) {

    int channel = nRF24L01p_getRegister(_NRF24L01P_REG_RF_CH) & 0x7F;

    return ( channel + NRF24L01P_MIN_RF_FREQUENCY );

}


void nRF24L01p_setRfOutputPower(int power) {

    int rfSetup = nRF24L01p_getRegister(_NRF24L01P_REG_RF_SETUP) & ~_NRF24L01P_RF_SETUP_RF_PWR_MASK;

    switch ( power ) {

        case NRF24L01P_TX_PWR_ZERO_DB:
            rfSetup |= _NRF24L01P_RF_SETUP_RF_PWR_0DBM;
            break;

        case NRF24L01P_TX_PWR_MINUS_6_DB:
            rfSetup |= _NRF24L01P_RF_SETUP_RF_PWR_MINUS_6DBM;
            break;

        case NRF24L01P_TX_PWR_MINUS_12_DB:
            rfSetup |= _NRF24L01P_RF_SETUP_RF_PWR_MINUS_12DBM;
            break;

        case NRF24L01P_TX_PWR_MINUS_18_DB:
            rfSetup |= _NRF24L01P_RF_SETUP_RF_PWR_MINUS_18DBM;
            break;

        default:
            //error( "nRF24L01P: Invalid RF Output Power setting %d\r\n", power );
            return;

    }

    nRF24L01p_setRegister(_NRF24L01P_REG_RF_SETUP, rfSetup);

}


int nRF24L01p_getRfOutputPower(void) {

    int rfPwr = nRF24L01p_getRegister(_NRF24L01P_REG_RF_SETUP) & _NRF24L01P_RF_SETUP_RF_PWR_MASK;

    switch ( rfPwr ) {

        case _NRF24L01P_RF_SETUP_RF_PWR_0DBM:
            return NRF24L01P_TX_PWR_ZERO_DB;

        case _NRF24L01P_RF_SETUP_RF_PWR_MINUS_6DBM:
            return NRF24L01P_TX_PWR_MINUS_6_DB;

        case _NRF24L01P_RF_SETUP_RF_PWR_MINUS_12DBM:
            return NRF24L01P_TX_PWR_MINUS_12_DB;

        case _NRF24L01P_RF_SETUP_RF_PWR_MINUS_18DBM:
            return NRF24L01P_TX_PWR_MINUS_18_DB;

        default:
            //error( "nRF24L01P: Unknown RF Output Power value %d\r\n", rfPwr );
            return 0;

    }
}


void nRF24L01p_setAirDataRate(int rate) {

    int rfSetup = nRF24L01p_getRegister(_NRF24L01P_REG_RF_SETUP) & ~_NRF24L01P_RF_SETUP_RF_DR_MASK;

    switch ( rate ) {

        case NRF24L01P_DATARATE_250_KBPS:
            rfSetup |= _NRF24L01P_RF_SETUP_RF_DR_250KBPS;
            break;

        case NRF24L01P_DATARATE_1_MBPS:
            rfSetup |= _NRF24L01P_RF_SETUP_RF_DR_1MBPS;
            break;

        case NRF24L01P_DATARATE_2_MBPS:
            rfSetup |= _NRF24L01P_RF_SETUP_RF_DR_2MBPS;
            break;

        default:
            //error( "nRF24L01P: Invalid Air Data Rate setting %d\r\n", rate );
            return;

    }

    nRF24L01p_setRegister(_NRF24L01P_REG_RF_SETUP, rfSetup);

}


int nRF24L01p_getAirDataRate(void) {

    int rfDataRate = nRF24L01p_getRegister(_NRF24L01P_REG_RF_SETUP) & _NRF24L01P_RF_SETUP_RF_DR_MASK;

    switch ( rfDataRate ) {

        case _NRF24L01P_RF_SETUP_RF_DR_250KBPS:
            return NRF24L01P_DATARATE_250_KBPS;

        case _NRF24L01P_RF_SETUP_RF_DR_1MBPS:
            return NRF24L01P_DATARATE_1_MBPS;

        case _NRF24L01P_RF_SETUP_RF_DR_2MBPS:
            return NRF24L01P_DATARATE_2_MBPS;

        default:
            //error( "nRF24L01P: Unknown Air Data Rate value %d\r\n", rfDataRate );
            return 0;

    }
}


void nRF24L01p_setCrcWidth(int width) {

    int config = nRF24L01p_getRegister(_NRF24L01P_REG_CONFIG) & ~_NRF24L01P_CONFIG_CRC_MASK;

    switch ( width ) {

        case NRF24L01P_CRC_NONE:
            config |= _NRF24L01P_CONFIG_CRC_NONE;
            break;

        case NRF24L01P_CRC_8_BIT:
            config |= _NRF24L01P_CONFIG_CRC_8BIT;
            break;

        case NRF24L01P_CRC_16_BIT:
            config |= _NRF24L01P_CONFIG_CRC_16BIT;
            break;

        default:
            //error( "nRF24L01P: Invalid CRC Width setting %d\r\n", width );
            return;

    }

    nRF24L01p_setRegister(_NRF24L01P_REG_CONFIG, config);

}


int nRF24L01p_getCrcWidth(void) {

    int crcWidth = nRF24L01p_getRegister(_NRF24L01P_REG_CONFIG) & _NRF24L01P_CONFIG_CRC_MASK;

    switch ( crcWidth ) {

        case _NRF24L01P_CONFIG_CRC_NONE:
            return NRF24L01P_CRC_NONE;

        case _NRF24L01P_CONFIG_CRC_8BIT:
            return NRF24L01P_CRC_8_BIT;

        case _NRF24L01P_CONFIG_CRC_16BIT:
            return NRF24L01P_CRC_16_BIT;

        default:
            //error( "nRF24L01P: Unknown CRC Width value %d\r\n", crcWidth );
            return 0;

    }
}


void nRF24L01p_setTransferSize(int size, int pipe) {

    if ( ( pipe < NRF24L01P_PIPE_P0 ) || ( pipe > NRF24L01P_PIPE_P5 ) ) {

        //error( "nRF24L01P: Invalid Transfer Size pipe number %d\r\n", pipe );
        return;

    }

    if ( ( size < 0 ) || ( size > _NRF24L01P_RX_FIFO_SIZE ) ) {

        //error( "nRF24L01P: Invalid Transfer Size setting %d\r\n", size );
        return;

    }

    int rxPwPxRegister = _NRF24L01P_REG_RX_PW_P0 + ( pipe - NRF24L01P_PIPE_P0 );

    nRF24L01p_setRegister(rxPwPxRegister, ( size & _NRF24L01P_RX_PW_Px_MASK ) );

}


int nRF24L01p_getTransferSize(int pipe) {

    if ( ( pipe < NRF24L01P_PIPE_P0 ) || ( pipe > NRF24L01P_PIPE_P5 ) ) {

        //error( "nRF24L01P: Invalid Transfer Size pipe number %d\r\n", pipe );
        return 0;

    }

    int rxPwPxRegister = _NRF24L01P_REG_RX_PW_P0 + ( pipe - NRF24L01P_PIPE_P0 );

    int size = nRF24L01p_getRegister(rxPwPxRegister);
    
    return ( size & _NRF24L01P_RX_PW_Px_MASK );

}


void nRF24L01p_disableAllRxPipes(void) {

    nRF24L01p_setRegister(_NRF24L01P_REG_EN_RXADDR, _NRF24L01P_EN_RXADDR_NONE);

}


void nRF24L01p_disableAutoAcknowledge(void) {

    nRF24L01p_setRegister(_NRF24L01P_REG_EN_AA, _NRF24L01P_EN_AA_NONE);

}


void nRF24L01p_enableAutoAcknowledge(int pipe) {

    if ( ( pipe < NRF24L01P_PIPE_P0 ) || ( pipe > NRF24L01P_PIPE_P5 ) ) {

        //error( "nRF24L01P: Invalid Enable AutoAcknowledge pipe number %d\r\n", pipe );
        return;

    }

    int enAA = nRF24L01p_getRegister(_NRF24L01P_REG_EN_AA);

    enAA |= ( 1 << (pipe - NRF24L01P_PIPE_P0) );

    nRF24L01p_setRegister(_NRF24L01P_REG_EN_AA, enAA);

}


void nRF24L01p_disableAutoRetransmit(void) {

    nRF24L01p_setRegister(_NRF24L01P_REG_SETUP_RETR, _NRF24L01P_SETUP_RETR_NONE);

}

void nRF24L01p_setRxAddress(unsigned long long address, int width, int pipe) {

    if ( ( pipe < NRF24L01P_PIPE_P0 ) || ( pipe > NRF24L01P_PIPE_P5 ) ) {

        //error( "nRF24L01P: Invalid setRxAddress pipe number %d\r\n", pipe );
        return;

    }

    if ( ( pipe == NRF24L01P_PIPE_P0 ) || ( pipe == NRF24L01P_PIPE_P1 ) ) {

        int setupAw = nRF24L01p_getRegister(_NRF24L01P_REG_SETUP_AW) & ~_NRF24L01P_SETUP_AW_AW_MASK;
    
        switch ( width ) {
    
            case 3:
                setupAw |= _NRF24L01P_SETUP_AW_AW_3BYTE;
                break;
    
            case 4:
                setupAw |= _NRF24L01P_SETUP_AW_AW_4BYTE;
                break;
    
            case 5:
                setupAw |= _NRF24L01P_SETUP_AW_AW_5BYTE;
                break;
    
            default:
                //error( "nRF24L01P: Invalid setRxAddress width setting %d\r\n", width );
                return;
    
        }
    
        nRF24L01p_setRegister(_NRF24L01P_REG_SETUP_AW, setupAw);

    } else {
    
        width = 1;
    
    }

    int rxAddrPxRegister = _NRF24L01P_REG_RX_ADDR_P0 + ( pipe - NRF24L01P_PIPE_P0 );

    int cn = (_NRF24L01P_SPI_CMD_WR_REG | (rxAddrPxRegister & _NRF24L01P_REG_ADDRESS_MASK));

    select();
    ssi_send(cn);
    int status = ssi_recv();

    while ( width-- > 0 ) {

        //
        // LSByte first
        //
        ssi_send(address & 0xFF);
        address >>= 8;

    }
    deselect();

    int enRxAddr = nRF24L01p_getRegister(_NRF24L01P_REG_EN_RXADDR);

    enRxAddr |= (1 << ( pipe - NRF24L01P_PIPE_P0 ) );

    nRF24L01p_setRegister(_NRF24L01P_REG_EN_RXADDR, enRxAddr);
}

/*
 * This version of setRxAddress is just a wrapper for the version that takes 'long long's,
 *  in case the main code doesn't want to deal with long long's.
 */
void nRF24L01p_setRxAddressSplit(unsigned long msb_address, unsigned long lsb_address, int width, int pipe) {

    unsigned long long address = ( ( (unsigned long long) msb_address ) << 32 ) | ( ( (unsigned long long) lsb_address ) << 0 );

    nRF24L01p_setRxAddress(address, width, pipe);

}


/*
 * This version of setTxAddress is just a wrapper for the version that takes 'long long's,
 *  in case the main code doesn't want to deal with long long's.
 */
void nRF24L01p_setTxAddressSplit(unsigned long msb_address, unsigned long lsb_address, int width) {

    unsigned long long address = ( ( (unsigned long long) msb_address ) << 32 ) | ( ( (unsigned long long) lsb_address ) << 0 );

    nRF24L01p_setTxAddress(address, width);

}


void nRF24L01p_setTxAddress(unsigned long long address, int width) {

    int setupAw = nRF24L01p_getRegister(_NRF24L01P_REG_SETUP_AW) & ~_NRF24L01P_SETUP_AW_AW_MASK;

    switch ( width ) {

        case 3:
            setupAw |= _NRF24L01P_SETUP_AW_AW_3BYTE;
            break;

        case 4:
            setupAw |= _NRF24L01P_SETUP_AW_AW_4BYTE;
            break;

        case 5:
            setupAw |= _NRF24L01P_SETUP_AW_AW_5BYTE;
            break;

        default:
            //error( "nRF24L01P: Invalid setTxAddress width setting %d\r\n", width );
            return;

    }

    nRF24L01p_setRegister(_NRF24L01P_REG_SETUP_AW, setupAw);

    int cn = (_NRF24L01P_SPI_CMD_WR_REG | (_NRF24L01P_REG_TX_ADDR & _NRF24L01P_REG_ADDRESS_MASK));

    select();
    ssi_send(cn);
    int status = ssi_recv();

    while ( width-- > 0 ) {

        //
        // LSByte first
        //
        ssi_send(address & 0xFF);
        address >>= 8;

    }
    deselect();

}


unsigned long long nRF24L01p_getRxAddress(int pipe) {

    if ( ( pipe < NRF24L01P_PIPE_P0 ) || ( pipe > NRF24L01P_PIPE_P5 ) ) {

        //error( "nRF24L01P: Invalid setRxAddress pipe number %d\r\n", pipe );
        return 0;

    }

    int width;

    if ( ( pipe == NRF24L01P_PIPE_P0 ) || ( pipe == NRF24L01P_PIPE_P1 ) ) {

        int setupAw = nRF24L01p_getRegister(_NRF24L01P_REG_SETUP_AW) & _NRF24L01P_SETUP_AW_AW_MASK;

        switch ( setupAw ) {

            case _NRF24L01P_SETUP_AW_AW_3BYTE:
                width = 3;
                break;

            case _NRF24L01P_SETUP_AW_AW_4BYTE:
                width = 4;
                break;

            case _NRF24L01P_SETUP_AW_AW_5BYTE:
                width = 5;
                break;

            default:
                //error( "nRF24L01P: Unknown getRxAddress width value %d\r\n", setupAw );
                return 0;

        }

    } else {

        width = 1;

    }

    int rxAddrPxRegister = _NRF24L01P_REG_RX_ADDR_P0 + ( pipe - NRF24L01P_PIPE_P0 );

    int cn = (_NRF24L01P_SPI_CMD_RD_REG | (rxAddrPxRegister & _NRF24L01P_REG_ADDRESS_MASK));

    unsigned long long address = 0;

    select();
    ssi_send(cn);
    int status = ssi_recv();

    for ( int i=0; i<width; i++ ) {

        //
        // LSByte first
        //
        ssi_send(_NRF24L01P_SPI_CMD_NOP);
        address |= ( ( (unsigned long long)( ssi_recv() & 0xFF ) ) << (i*8) );

    }
    deselect();

    if ( !( ( pipe == NRF24L01P_PIPE_P0 ) || ( pipe == NRF24L01P_PIPE_P1 ) ) ) {

        address |= ( nRF24L01p_getRxAddress(NRF24L01P_PIPE_P1) & ~((unsigned long long) 0xFF) );

    }

    return address;

}

    
unsigned long long nRF24L01p_getTxAddress(void) {

    int setupAw = nRF24L01p_getRegister(_NRF24L01P_REG_SETUP_AW) & _NRF24L01P_SETUP_AW_AW_MASK;

    int width;

    switch ( setupAw ) {

        case _NRF24L01P_SETUP_AW_AW_3BYTE:
            width = 3;
            break;

        case _NRF24L01P_SETUP_AW_AW_4BYTE:
            width = 4;
            break;

        case _NRF24L01P_SETUP_AW_AW_5BYTE:
            width = 5;
            break;

        default:
            //error( "nRF24L01P: Unknown getTxAddress width value %d\r\n", setupAw );
            return 0;

    }

    int cn = (_NRF24L01P_SPI_CMD_RD_REG | (_NRF24L01P_REG_TX_ADDR & _NRF24L01P_REG_ADDRESS_MASK));

    unsigned long long address = 0;

    select();
    ssi_send(cn);
    int status = ssi_recv();

    for ( int i=0; i<width; i++ ) {

        //
        // LSByte first
        //
        ssi_send(_NRF24L01P_SPI_CMD_NOP);
        address |= ( ( (unsigned long long)( ssi_recv() & 0xFF ) ) << (i*8) );

    }
    deselect();

    return address;
}


int nRF24L01p_readable(int pipe) {

    if ( ( pipe < NRF24L01P_PIPE_P0 ) || ( pipe > NRF24L01P_PIPE_P5 ) ) {

        //error( "nRF24L01P: Invalid readable pipe number %d\r\n", pipe );
        return 0;

    }

    int status = nRF24L01p_getStatusRegister();

    return ( ( status & _NRF24L01P_STATUS_RX_DR ) && ( ( ( status & _NRF24L01P_STATUS_RX_P_NO ) >> 1 ) == ( pipe & 0x7 ) ) );

}


int nRF24L01p_write(int pipe, const char *data, int count) {

    // Note: the pipe number is ignored in a Transmit / write

    //
    // Save the CE state
    //
    int originalCe = disable();

    if ( count <= 0 ) return 0;

    if ( count > _NRF24L01P_TX_FIFO_SIZE ) count = _NRF24L01P_TX_FIFO_SIZE;

    // Clear the Status bit
    nRF24L01p_setRegister(_NRF24L01P_REG_STATUS, _NRF24L01P_STATUS_TX_DS);
    
    select();
    ssi_send(_NRF24L01P_SPI_CMD_WR_TX_PAYLOAD);
    int status = ssi_recv();

    for ( int i = 0; i < count; i++ ) {

        ssi_send(*data++);

    }
    deselect();

    int originalMode = mode;
    nRF24L01p_setTransmitMode();

    enable();
    wait_us(_NRF24L01P_TIMING_Thce_us);
    disable();

    while ( !( nRF24L01p_getStatusRegister() & _NRF24L01P_STATUS_TX_DS ) ) {

        // Wait for the transfer to complete

    }

    // Clear the Status bit
    nRF24L01p_setRegister(_NRF24L01P_REG_STATUS, _NRF24L01P_STATUS_TX_DS);

    if ( originalMode == _NRF24L01P_MODE_RX ) {

        nRF24L01p_setReceiveMode();

    }

    set_ce(originalCe);
    wait_us( _NRF24L01P_TIMING_Tpece2csn_us );

    return count;

}


int nRF24L01p_read(int pipe, char *data, int count) {

    if ( ( pipe < NRF24L01P_PIPE_P0 ) || ( pipe > NRF24L01P_PIPE_P5 ) ) {

        //error( "nRF24L01P: Invalid read pipe number %d\r\n", pipe );
        return -1;

    }

    if ( count <= 0 ) return 0;

    if ( count > _NRF24L01P_RX_FIFO_SIZE ) count = _NRF24L01P_RX_FIFO_SIZE;

    if ( nRF24L01p_readable(pipe) ) {

        select();
        ssi_send(_NRF24L01P_SPI_CMD_R_RX_PL_WID);
        int status = ssi_recv();
        ssi_send(_NRF24L01P_SPI_CMD_NOP);
        int rxPayloadWidth = ssi_recv();
        deselect();       

        if ( ( rxPayloadWidth < 0 ) || ( rxPayloadWidth > _NRF24L01P_RX_FIFO_SIZE ) ) {
    
            // Received payload error: need to flush the FIFO

            select();
            ssi_send(_NRF24L01P_SPI_CMD_FLUSH_RX);
            int status = ssi_recv();
    
            ssi_send(_NRF24L01P_SPI_CMD_NOP);
            int rxPayloadWidth = ssi_recv();
            deselect();
            
            //
            // At this point, we should retry the reception,
            //  but for now we'll just fall through...
            //

        } else {

            if ( rxPayloadWidth < count ) count = rxPayloadWidth;

            select();
            ssi_send(_NRF24L01P_SPI_CMD_RD_RX_PAYLOAD);
            int status = ssi_recv();
        
            for ( int i = 0; i < count; i++ ) {
        
                ssi_send(_NRF24L01P_SPI_CMD_NOP);
                *data++ = ssi_recv();
        
            }
            deselect();

            // Clear the Status bit
            nRF24L01p_setRegister(_NRF24L01P_REG_STATUS, _NRF24L01P_STATUS_RX_DR);

            return count;

        }

    } else {

        //
        // What should we do if there is no 'readable' data?
        //  We could wait for data to arrive, but for now, we'll
        //  just return with no data.
        //
        return 0;

    }

    //
    // We get here because an error condition occured;
    //  We could wait for data to arrive, but for now, we'll
    //  just return with no data.
    //
    return -1;

}

void nRF24L01p_setRegister(int regAddress, int regData) {

    //
    // Save the CE state
    //
    int originalCe = disable();

    int cn = (_NRF24L01P_SPI_CMD_WR_REG | (regAddress & _NRF24L01P_REG_ADDRESS_MASK));

    select();
    ssi_send(cn);
    int status = ssi_recv();

    ssi_send(regData & 0xFF);
    deselect();

    set_ce(originalCe);
    wait_us( _NRF24L01P_TIMING_Tpece2csn_us );

}


int nRF24L01p_getRegister(int regAddress) {

    int cn = (_NRF24L01P_SPI_CMD_RD_REG | (regAddress & _NRF24L01P_REG_ADDRESS_MASK));

    select();

    ssi_send(cn);
    int status = ssi_recv();

    ssi_send(_NRF24L01P_SPI_CMD_NOP);
    int dn = ssi_recv();

    deselect();

    return dn;

}

int nRF24L01p_getStatusRegister(void) {

    select();
    ssi_send(_NRF24L01P_SPI_CMD_NOP);
    int status = ssi_recv();
    deselect();
    return status;
}
