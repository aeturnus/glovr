//Brandon Nguyen

#include <stdint.h>
#include <tm4c123gh6pm.h>
#include <drivers/UART.h>

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

//uint32_t UART1_ErrorCounter = 0;


// IBRD = int(80,000,000 / (16 * 115200)) = int(43.40277)
// FBRD = int(.40277 * 64 +.5) = int(26.2777792)
const uint32_t baudI[]={22222,11111,5556,2778,1389,694,347,174,87,43,22,11,5,3,1,2083,521,260};
const uint32_t baudF[]={15,8,36,50,57,29,15,40,52,26,45,55,28,46,23,22,54,27};


/////////////////////////////////////
///////////////UART 0////////////////
/////////////////////////////////////
static void (*UART0_Task)(void);   //Function pointer for UART0 handler
//------------UART0_Init------------
// Initialize the UART for specified baud rate (assuming 80 MHz UART clock) on UART0,
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// PA1:TX, PA0:RX
// Input: none
// Output: none
void UART0_Init(uint8_t baudRateIndex){
  SYSCTL_RCGCUART_R |= 0x01;            // activate UART0
  SYSCTL_RCGCGPIO_R |= 0x01;            // activate port A
  while((SYSCTL_RCGCUART_R&0x01) == 0);
  while((SYSCTL_RCGCGPIO_R&0x01) == 0);
  UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  
  UART0_IBRD_R = baudI[baudRateIndex];  // IBRD = int(80,000,000 / (16 * 115200)) = int(43.40277)
  UART0_FBRD_R = baudF[baudRateIndex];  // FBRD = int(.40277 * 64 +.5) = int(26.2777792)
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0_CTL_R |= UART_CTL_TXEN|UART_CTL_RXEN|UART_CTL_UARTEN;       // enable UART w/ TX and RX
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
                                        // configure PA1-0 as UART0
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)|0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA1-0
}

/* TODO: implement properly
void UART0_InitRXInt(uint8_t trigger,uint8_t priority,void (*task)(void))
{
  //Set the UART0 task
  UART0_Task = task;
  //Init the Rx interrupt
  UART0_IM_R |= 0x10;                                   //Arm RXRIS
  UART0_IFLS_R = (UART0_IFLS_R & ~0x38)|(trigger<<3);   //Set 5,4,3 for fullness
  NVIC_PRI1_R = (NVIC_PRI1_R&~0xE00000)|(priority<<21); //Set priority
  NVIC_EN0_R |= (0x1<<6);                                   //Enable int 6 in NVIC->interrupt 6
}
*/

void UART0_Handler()
{
  UART0_Task();             //Dat automatic dereference
}

uint8_t UART0_InChar()
{
  while((UART0_FR_R&UART_FR_RXFE)!=0){}
  return((uint8_t)(UART0_DR_R&0xFF));
}

void UART0_OutChar(uint8_t data)
{
  while((UART0_FR_R&UART_FR_TXFF)!=0){}
  UART0_DR_R = (data&0xFF);
}

void UART0_OutString(uint8_t* data)
{
  while(*data)
  {
    UART0_OutChar(*data);
    data++;
  }
}

/////////////////////////////////////
///////////////UART 1////////////////
/////////////////////////////////////
static void (*UART1_Task)(void);   //Function pointer for UART1 handler
//------------UART1_Init------------
// Initialize the UART for specified baud rate (assuming 80 MHz UART clock) on UART1,
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// PB1:TX, PB0:RX
// Input: none
// Output: none
void UART1_Init(uint8_t baudRateIndex){
  SYSCTL_RCGCUART_R |= 0x02;            // activate UART1
  SYSCTL_RCGCGPIO_R |= 0x02;            // activate port B
  while((SYSCTL_RCGCUART_R&0x02) == 0);
  while((SYSCTL_RCGCGPIO_R&0x02) == 0);
  UART1_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  
  UART1_IBRD_R = baudI[baudRateIndex];  // IBRD = int(80,000,000 / (16 * 115200)) = int(43.40277)
  UART1_FBRD_R = baudF[baudRateIndex];  // FBRD = int(.40277 * 64 +.5) = int(26.2777792)
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART1_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART1_CTL_R |= UART_CTL_TXEN|UART_CTL_RXEN|UART_CTL_UARTEN;       // enable UART w/ TX and RX
  GPIO_PORTB_AFSEL_R |= 0x03;           // enable alt funct on PB1-0
  GPIO_PORTB_DEN_R |= 0x03;             // enable digital I/O on PB1-0
                                        // configure PB1-0 as UART1
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFFFF00)|0x00000011;
  GPIO_PORTB_AMSEL_R &= ~0x03;          // disable analog functionality on PB1-0
}

void UART1_InitRXInt(uint8_t trigger,uint8_t priority,void (*task)(void))
{
  //Set the UART1 task
  UART1_Task = task;
  //Init the Rx interrupt
  UART1_IM_R |= 0x10;                                   //Arm RXRIS
  UART1_IFLS_R = (UART1_IFLS_R & ~0x38)|(trigger<<3);   //Set 5,4,3 for fullness
  NVIC_PRI1_R = (NVIC_PRI1_R&~0xE00000)|(priority<<21); //Set priority
  NVIC_EN0_R |= (0x1<<6);                                   //Enable int 6 in NVIC->interrupt 6
}
void UART1_Handler()
{
  UART1_Task();             //Dat automatic dereference
}

uint8_t UART1_InChar()
{
  while((UART1_FR_R&UART_FR_RXFE)!=0){}
  return((uint8_t)(UART1_DR_R&0xFF));
}

void UART1_OutChar(uint8_t data)
{
  while((UART1_FR_R&UART_FR_TXFF)!=0){}
  UART1_DR_R = (data&0xFF);
}

void UART1_OutString(uint8_t* data)
{
  while(*data)
  {
    UART1_OutChar(*data);
    data++;
  }
}

/////////////////////////////////////
///////////////UART 2////////////////
/////////////////////////////////////
static void (*UART2_Task)(void);   //Function pointer for UART1 handler
//------------UART2_Init------------
// Initialize the UART for specified baud rate (assuming 80 MHz UART clock) on UART2,
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// PD7:TX, PD6:TX
// Input: none
// Output: none
void UART2_Init(uint8_t baudRateIndex){
  SYSCTL_RCGCUART_R |= 0x04;            // activate UART2
  SYSCTL_RCGCGPIO_R |= 0x08;            // activate port D
  while((SYSCTL_RCGCUART_R&0x04) == 0);
  while((SYSCTL_RCGCGPIO_R&0x08) == 0);
  UART2_CTL_R &= ~UART_CTL_UARTEN;      // disable UART

  UART2_IBRD_R = baudI[baudRateIndex];  // IBRD = int(80,000,000 / (16 * 115200)) = int(43.40277)
  UART2_FBRD_R = baudF[baudRateIndex];  // FBRD = int(.40277 * 64 +.5) = int(26.2777792)
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART2_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART2_CTL_R |= UART_CTL_TXEN|UART_CTL_RXEN|UART_CTL_UARTEN;       // enable UART w/ TX and RX

  GPIO_PORTD_LOCK_R = 0x4C4F434B;       //Unlock PD7
  GPIO_PORTD_CR_R = 0xFF;               

  GPIO_PORTD_AFSEL_R |= 0xC0;           // enable alt funct on PD7-6
  GPIO_PORTD_DEN_R |= 0xC0;             // enable digital I/O on PD7-6
                                        // configure PD7-6 as UART2
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0x00FFFFFF)|0x11000000;
  GPIO_PORTD_AMSEL_R &= ~0xC0;          // disable analog functionality on PD7-6
}

void UART2_InitRXInt(uint8_t trigger,uint8_t priority,void (*task)(void))
{
  //Set the UART2 task
  UART2_Task = task;
  //Init the Rx interrupt
  UART2_IM_R |= 0x10;                                   //Arm RXRIS
  UART2_IFLS_R = (UART2_IFLS_R & ~0x38)|(trigger<<3);   //Set 5,4,3 for fullness
  NVIC_PRI8_R = (NVIC_PRI8_R&~0x00E000)|(priority<<13); //Set priority
  NVIC_EN1_R |= (0x1<<1);                                   //Enable int 33 in NVIC->interrupt 33 (33-31-1=1) -1 for 32 being b0
}

void UART2_Handler()
{
  UART2_Task();             //Dat automatic dereference
}

uint8_t UART2_InChar()
{
  while((UART2_FR_R&UART_FR_RXFE)!=0){}
  return((uint8_t)(UART2_DR_R&0xFF));
}

void UART2_OutChar(uint8_t data)
{
  while((UART2_FR_R&UART_FR_TXFF)!=0){}
  UART2_DR_R = (data&0xFF);
}

void UART2_OutString(uint8_t* data)
{
  while(*data)
  {
    UART2_OutChar(*data);
    data++;
  }
}



/////////////////////////////////////
///////////////UART 3////////////////
/////////////////////////////////////
static void (*UART3_Task)(void);   //Function pointer for UART1 handler
//------------UART3_Init------------
// Initialize the UART for specified baud rate (assuming 80 MHz UART clock) on UART3,
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// PC7:TX, PC6:TX
// Input: none
// Output: none
void UART3_Init(uint8_t baudRateIndex){
  SYSCTL_RCGCUART_R |= 0x08;            // activate UART3
  SYSCTL_RCGCGPIO_R |= 0x04;            // activate port C
  while((SYSCTL_RCGCUART_R&0x08) == 0);
  while((SYSCTL_RCGCGPIO_R&0x04) == 0);
  UART3_CTL_R &= ~UART_CTL_UARTEN;      // disable UART

  UART3_IBRD_R = baudI[baudRateIndex];  // IBRD = int(80,000,000 / (16 * 115200)) = int(43.40277)
  UART3_FBRD_R = baudF[baudRateIndex];  // FBRD = int(.40277 * 64 +.5) = int(26.2777792)
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART3_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART3_CTL_R |= UART_CTL_TXEN|UART_CTL_RXEN|UART_CTL_UARTEN;       // enable UART w/ TX and RX

  GPIO_PORTC_AFSEL_R |= 0xC0;           // enable alt funct on PC7-6
  GPIO_PORTC_DEN_R |= 0xC0;             // enable digital I/O on PC7-6
                                        // configure PC7-6 as UART3
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0x00FFFFFF)|0x11000000;
  GPIO_PORTC_AMSEL_R &= ~0xC0;          // disable analog functionality on PC7-6
}

void UART3_InitRXInt(uint8_t trigger,uint8_t priority,void (*task)(void))
{
  //Set the UART3 task
  UART3_Task = task;
  //Init the Rx interrupt
  UART3_IM_R |= 0x10;                                   //Arm RXRIS
  UART3_IFLS_R = (UART3_IFLS_R & ~0x38)|(trigger<<3);   //Set 5,4,3 for fullness
  NVIC_PRI14_R = (NVIC_PRI14_R&~0x0E0000)|(priority<<21); //Set priority
  NVIC_EN1_R |= (0x1<<27);                                   //Enable int 59 in NVIC->interrupt 59 (59-31-1=27) -1 for 32 being b0
}

void UART3_Handler()
{
  UART3_Task();             //Dat automatic dereference
}

uint8_t UART3_InChar()
{
  while((UART3_FR_R&UART_FR_RXFE)!=0){}
  return((uint8_t)(UART3_DR_R&0xFF));
}

void UART3_OutChar(uint8_t data)
{
  while((UART3_FR_R&UART_FR_TXFF)!=0){}
  UART3_DR_R = (data&0xFF);
}

void UART3_OutString(uint8_t* data)
{
  while(*data)
  {
    UART3_OutChar(*data);
    data++;
  }
}



void UART_OutByte(uint8_t module,uint8_t data)
{
  switch(module)
  {
    case 0:
      UART0_OutChar(data);
      break;
    case 1:
      UART1_OutChar(data);
      break;
    case 2:
      UART2_OutChar(data);
      break;
    case 3:
      UART3_OutChar(data);
      break;
    default:
      break;
  }
}

void UART_Out2Bytes(uint8_t module,uint16_t data)
{
  UART_OutByte(module,(data>>0)&0xFF);
  UART_OutByte(module,(data>>8)&0xFF);
}

void UART_Out4Bytes(uint8_t module,uint32_t data)
{
  UART_OutByte(module,(data>>0)&0xFF);
  UART_OutByte(module,(data>>8)&0xFF);
  UART_OutByte(module,(data>>16)&0xFF);
  UART_OutByte(module,(data>>24)&0xFF);
}
