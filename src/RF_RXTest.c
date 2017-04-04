#include <string.h>

#include <drivers/tm4c/UART.h>
#include <drivers/tm4c/PLL.h>
#include <drivers/devices/nRF24L01p_mbed.h>
#include <drivers/system/hw.h>

char buffer[32];
int main(void)
{
  PLL_Init(Bus80MHz);
  UART0_Init(BAUD_115200);
  nRF24L01p_Init();
  
  nRF24L01p_setReceiveMode();
  nRF24L01p_enable();
  while(!nRF24L01p_readable(0)){}
  nRF24L01p_read(0,buffer,32);
  UART0_OutString("Received: \r\n");
  for(int i = 0; i < 2; i++){
    UART0_OutString(buffer);
  }
  return 0;
}