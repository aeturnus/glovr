//Brandon Nguyen

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <tm4c123gh6pm.h>

#define UART_FR_TXFE            0x00000080  // UART Transmit FIFO Empty
#define UART_FR_RXFF            0x00000040  // UART RXFF FIFO Full
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_FR_BUSY            0x00000008  // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_TXEN           0x00000100  // UART Enable TX
#define UART_CTL_RXEN           0x00000200  // UART Enable RX
#define UART_CTL_UARTEN         0x00000001  // UART Enable

#define UART_EIGHTH   0x0
#define UART_QUARTER  0x1
#define UART_HALF     0x2
#define UART_THREEQ   0x3
#define UART_SEIGHTH  0x4

#define BAUD_225 0
#define BAUD_450 1
#define BAUD_900 2
#define BAUD_1800 3
#define BAUD_3600 4
#define BAUD_7200 5
#define BAUD_14400 6
#define BAUD_28800 7
#define BAUD_57600 8
#define BAUD_115200 9
#define BAUD_230400 10
#define BAUD_460800 11
#define BAUD_921600 12
#define BAUD_1843200 13
#define BAUD_3686400 14

#define BAUD_2400 15
#define BAUD_9600 16
#define BAUD_19200 17


//------------UART0_Init------------
// Initialize the UART for specified baud rate (assuming 80 MHz UART clock) on UART0,
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// PC5:TX, PC4:RX
// Input: none
// Output: none
void UART0_Init(uint8_t baudRateIndex);
void UART0_InitRXInt(uint8_t trigger,uint8_t priority,void (*task)(void));
uint8_t UART0_InChar(void);
void UART0_OutChar(uint8_t data);
void UART0_OutString(uint8_t* data);

//------------UART1_Init------------
// Initialize the UART for specified baud rate (assuming 80 MHz UART clock) on UART1,
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// PC5:TX, PC4:RX
// Input: none
// Output: none
void UART1_Init(uint8_t baudRateIndex);
void UART1_InitRXInt(uint8_t trigger,uint8_t priority,void (*task)(void));
uint8_t UART1_InChar(void);
void UART1_OutChar(uint8_t data);
void UART1_OutString(uint8_t* data);


//------------UART2_Init------------
// Initialize the UART for specified baud rate (assuming 80 MHz UART clock) on UART2,
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// PD7:TX, PD6:TX
// Input: none
// Output: none
void UART2_Init(uint8_t baudRateIndex);
void UART2_InitRXInt(uint8_t trigger,uint8_t priority,void (*task)(void));
uint8_t UART2_InChar(void);
void UART2_OutChar(uint8_t data);
void UART2_OutString(uint8_t* data);

/////////////////////////////////////
///////////////UART 3////////////////
/////////////////////////////////////
//------------UART3_Init------------
// Initialize the UART for specified baud rate (assuming 80 MHz UART clock) on UART3,
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// PC7:TX, PC6:TX
// Input: none
// Output: none
void UART3_Init(uint8_t baudRateIndex);
void UART3_InitRXInt(uint8_t trigger,uint8_t priority,void (*task)(void));
uint8_t UART3_InChar(void);
void UART3_OutChar(uint8_t data);
void UART3_OutString(uint8_t* data);

//Little endian transmission
//0xABCD->  A  B  C  D-> FIFO
void UART_OutByte(uint8_t module,uint8_t data);
void UART_Out2Bytes(uint8_t module,uint16_t data);
void UART_Out4Bytes(uint8_t module,uint32_t data);

#endif
