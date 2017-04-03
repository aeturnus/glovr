#include <stdint.h>

#include <tm4c123gh6pm.h>

#include <drivers/devices/nRF24L01.h>

/* PB2 => CE
 * PB3 => CSN
 * PB5 => IRQ
 */
void NRF24L01_Init(void)
{
  SYSCTL_RCGCGPIO_R |= 0x02;  // turn on PB
  SYSCTL_RCGCSSI_R  |= 0x04;  // turn on SSI2
  while((SYSCTL_PRGPIO_R&0x4)==0){};  // wait for gpio
  GPIO_PORTB_AFSEL_R  |= 0xD0;  // PB7,6,4 for SSI
  GPIO_PORTB_DEN_R    |= 0xFC;  // PB7-2 digital needed
  GPIO_PORTB_ASEL_R   &= ~0xFC; // turn off analog

  // enable SSI2 on 7,6,4 GPIO on the others
  GPIO_PORTB_PCTL_R   = (GPIO_PORTD_PCTL_R&0x000000FF) | 0x22020000;

  // need to initialize GPIO for PB


  // need to initialize SSI2
  SSI2_CR1_R = 0x0; // turn off, master mode
  SSI2_CPSR_R = 2;  // CLK speed: divide by 2
  SSI2_CR0_R = (SSI2_CR0_R&~(0x0000FFF0));  // SCR = 0, SPH = 0, SPO = 0 Freescale
  SSI2_CR0_R |= 0x07; // DSS=8-bit data
  SSI2_DR_R = 0;      // push a zero out at the beginning
  SSI2_CR1_R |= 0x02; // enable SSI woo!
}

void NRF24L01_SetTX(void)
{

}

void NRF24L01_SetRX(void)
{

}

void NRF24L01_Send(int length, const uint8_t * buffer)
{

}

void NRF24L01_Recv(int length, const uint8_t * buffer)
{

}

void NRF24L01_RegisterRXHandler(void (* handlerRX)(void))
{

}

void NRF24L01_UnregisterRXHandler(void)
{

}