#include <stdint.h>

#include <tm4c123gh6pm.h>

#include <drivers/devices/nRF24L01p.h>

// Register mapping
#define NRF_CONFIG_A    0x00
  #define NRF_CONFIG_MASK_RX_DR   0x40
  #define NRF_CONFIG_MASK_TX_DS   0x20
  #define NRF_CONFIG_MASK_MAX_RT  0x10
  #define NRF_CONFIG_EN_CRC       0x08
  #define NRF_CONFIG_CRC0         0x04
  #define NRF_CONFIG_PWR_UP       0x02
  #define NRF_CONFIG_PRIM_RX      0x01

#define NRF_EN_AA_A     0x01
  #define NRF_EN_AA_PIPE(n) (1<<n)

#define NRF_EN_RXADDR_A 0x02
  #define NRF_EN_RXADDR_ERX_PIPE(n) (1<<n)

#define NRF_SETUP_AW_A  0x03
  #define NRF_SETUP_AW_ILLEGAL  0x0
  #define NRF_SETUP_AW_3        0x1
  #define NRF_SETUP_AW_4        0x2
  #define NRF_SETUP_AW_5        0x3

#define NRF_SETUP_RETR_A  0x04
  // gets the bits for delay: provide uS in 250 uS increments
  #define NRF_SETUP_RETR_ARD(x) (((x-250)/250)<<4)
  #define NRF_SETUP_RETR_ARC(x) (x&0xF)

#define NRF_RF_CH_A     0x05

#define NRF_RF_SETUP_A  0x06
  #define NRF_RF_SETUP_CONT_WAVE  0x80
  #define NRF_RF_SETUP_RF_DR_LOW  0x20
  #define NRF_RF_SETUP_PLL_LOCK   0x10
  #define NRF_RF_SETUP_RF_DR_HIGH 0x08
  #define NRF_RF_SETUP_RF_PWR     0x06
    #define NRF_RF_SETUP_RF_PWR_18dBm (0x0<1)
    #define NRF_RF_SETUP_RF_PWR_12dBm (0x1<1)
    #define NRF_RF_SETUP_RF_PWR_6dBm  (0x2<1)
    #define NRF_RF_SETUP_RF_PWR_0dBm  (0x3<1)

#define NRF_RF_STATUS_A 0x07
  #define NRF_RF_STATUS_RX_DR   0x40
  #define NRF_RF_STATUS_TX_DS   0x20
  #define NRF_RF_STATUS_MAX_RT  0x10
  #define NRF_RF_STATUS_RX_P_NO 0x0E
    #define NRF_RF_STATUS_PX_P_NO_RX_FIFO_EMPTY 0x7
  #define NRF_RF_STATUS_TX_FULL 0x01

#define NRF_OBSERVE_TX_A  0x08

#define NRF_RPD_A       0x09
  #define NRF_RPD       0x01

#define NRF_RX_ADDR_BASE_A    0x0A
#define NRF_RX_ADDR_PX(x)_A  (NRF_RX_ADDR_BASE+x)
#define NRF_RX_ADDR_P0_A  0x0A
#define NRF_RX_ADDR_P1_A  0x0B
#define NRF_RX_ADDR_P2_A  0x0C
#define NRF_RX_ADDR_P3_A  0x0D
#define NRF_RX_ADDR_P4_A  0x0E
#define NRF_RX_ADDR_P5_A  0x0F

#define NRF_TX_ADDR     0x10

#define NRF_RX_PW_BASE_A  0x11
#define NRF_PW_PW_PX_A(x)  (NRF_RX_PW_BASE+x)
#define NRF_RX_PW_P0_A  0x11
#define NRF_RX_PW_P1_A  0x12
#define NRF_RX_PW_P2_A  0x13
#define NRF_RX_PW_P3_A  0x14
#define NRF_RX_PW_P4_A  0x15
#define NRF_RX_PW_P5_A  0x16

#define NRF_FIFO_STATUS_A 0x17
  #define NRF_FIFO_STATUS_TX_REUSE 0x40
  #define NRF_FIFO_STATUS_TX_FULL  0x20
  #define NRF_FIFO_STATUS_TX_EMPTY 0x10
  #define NRF_FIFO_STATUS_RX_FULL  0x02
  #define NRF_FIFO_STATUS_RX_EMPTY 0x01

#define NRF_DYNPD_A     0x1C
  #define NRF_NYNPD_P(n)  (1<<n)
  #define NRF_NYNPD_P5  0x20
  #define NRF_NYNPD_P4  0x10
  #define NRF_NYNPD_P3  0x08
  #define NRF_NYNPD_P2  0x04
  #define NRF_NYNPD_P1  0x02
  #define NRF_NYNPD_P0  0x01

#define NRF_FEATURE_A   0x1D
  #define NRF_FEATURE_EN_DPL      0x4
  #define NRF_FEATURE_EN_ACK_PAY  0x2
  #define NRF_FEATURE_EN_DYN_PAY  0x1

// Commands
// Note that STATUS comes back everytime
// balance register reads with with
// an in initial read to pull out the STATUS
#define NRF_R_REGISTER_C        0x00    // OR with address
#define NRF_W_REGISTER_C        0x20    // OR with address
#define NRF_R_RX_PAYLOAD_C      0x61
#define NRF_W_TX_PAYLOAD_C      0xA0
#define NRF_FLUSH_TX_C          0xE1
#define NRF_FLUSH_RX_C          0xE2
#define NRF_REUSE_TX_PL_C       0xE3
#define NRF_R_RX_PL_WID_C       0x60
#define NRF_W_ACK_PAYLOAD_C     0xA8  // OR with pipe number
#define NRF_TX_PAYLOAD_NOACK_C  0xB0
#define NRF_NOP_C               0xFF  // Can be used to get STATUS

/* PB2 => CE
 * PB3 => CSN
 * PB5 => IRQ
 */
// bit specific addresses for the ports
#define NRF_CE_SHF  1
#define NRF_CE  (GPIO_PORTB_DATA_BITS_R | (1<<NRF_CE_SHF))
#define NRF_CSN_SHF 2
#define NRF_CSN (GPIO_PORTB_DATA_BITS_R | (1<<NRF_CSN_SHF))
static inline void port_init(void)
{
  SYSCTL_RCGCGPIO_R |= 0x02;  // turn on PB
  SYSCTL_RCGCSSI_R  |= 0x04;  // turn on SSI2
  while((SYSCTL_PRGPIO_R&0x4)==0){};  // wait for gpio
  GPIO_PORTB_AFSEL_R  |= 0xD0;  // PB7,6,4 for SSI
  GPIO_PORTB_DEN_R    |= 0xFC;  // PB7-2 digital needed
  GPIO_PORTB_ASEL_R   &= ~0xFC; // turn off analog

  // enable SSI2 on 7,6,4 GPIO on the others
  GPIO_PORTB_PCTL_R   = (GPIO_PORTD_PCTL_R&0x000000FF) | 0x22020000;

  // need to initialize tranceiver IRQ (active low) detection
  // TODO: setupt falling edge interrupt on Port B


  // need to initialize SSI2
  SSI2_CR1_R = 0x0; // turn off, master mode
  SSI2_CPSR_R = 2;  // CLK speed: divide by 2
  SSI2_CR0_R = (SSI2_CR0_R&~(0x0000FFF0));  // SCR = 0, SPH = 0, SPO = 0 Freescale
  SSI2_CR0_R |= 0x07; // DSS=8-bit data
  SSI2_DR_R = 0;      // push a zero out at the beginning
  SSI2_CR1_R |= 0x02; // enable SSI woo!
}

// synchronously send a byte, blocks
static inline void ssi_send(uint8_t data)
{
  while(!(SSI2_SR_R & SSI2_TNF)){}  // wait until TX has an opening
  SSI2_DR_R = (data&0xFF);
}

// synchronously recv a byte, blocks
static inline uint8_t ssi_recv(void)
{
  while(!(SSI2_SR_R & SSI2_RNE)){}  // wait until RX has something
  uint8_t val = SSI2_DR_R & 0xFF;
  return val;
}

static inline int get_ce(void)
{
  return (NRF_CE>>1)&0x1;
}


static inline int set_ce(int x)
{
  int old = get_ce();
  NRF_CE = (x<<1);
  return old;
}


static inline int get_csn(void)
{
  return (NRF_CSN>>1)&0x1;
}


static inline void set_csn(int x)
{
  int old = get_csn();
  NRF_CSN = (x<<2);
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


static inline void command_send(uint8_t command)
{
  ssi_send(command);
}

static inline uint8_t reg_read(uint8_t addr)
{
  command_send(NRF_R_REGISTER_C | (addr&0x1F));
  uint8_t status = ssi_recv();  // peel off the status
  uint8_t reg = ssi_recv();
  return reg;
}

// multibyte read
static inline void reg_readn(uint8_t addr, int n, uint8_t * buffer)
{
  command_send(NRF_R_REGISTER_C | (addr&0x1F));
  uint8_t status = ssi_recv();  // peel off the status
  while(n-- > 0)
  {
    *buffer++ = ssi_recv();
  }
}

static inline void reg_write(uint8_t addr, uint8_t data)
{
  command_send(NRF_W_REGISTER_C | (addr&0x1F));
  uint8_t status = ssi_recv();
  ssi_send(data);
}


void nRF24L01p_Init(void)
{
  nRF24L01p_PortInit(void);
}

void nRF24L01p_SetTX(void)
{

}

void nRF24L01p_SetRX(void)
{

}

void nRF24L01p_Send(int length, const uint8_t * buffer)
{

}

void nRF24L01p_Recv(int length, const uint8_t * buffer)
{

}

void nRF24L01p_RegisterRXHandler(void (* handlerRX)(void))
{

}

void nRF24L01p_UnregisterRXHandler(void)
{

}