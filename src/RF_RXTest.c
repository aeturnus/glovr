#include <string.h>

#include <drivers/tm4c/UART.h>
#include <drivers/tm4c/PLL.h>
#include <drivers/devices/nRF24L01p_mbed.h>
#include <drivers/system/hw.h>

char buffer[32];
int main(void)
{
  PLL_Init(Bus80MHz);
  Time_Init();
  UART0_Init(BAUD_115200);
  UART0_OutString("Receiver: \r\n");
  nRF24L01p_Init();
  
  nRF24L01p_setTransferSize(32,0);
  nRF24L01p_setReceiveMode();
  nRF24L01p_enable();
  int thing;
  while(1)
  {
    while(!nRF24L01p_readable(0)){}
    if(nRF24L01p_read(0,buffer,32) > 0)
    {
      UART0_OutString("Received: ");
      for(int i = 0; i < 1; i++){
        UART0_OutString(buffer);
        UART0_OutString("\r\n");
      }
    }
  }
  return 0;
}