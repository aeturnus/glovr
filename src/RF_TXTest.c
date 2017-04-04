#include <string.h>

#include <drivers/system/time.h>
#include <drivers/tm4c/UART.h>
#include <drivers/tm4c/PLL.h>
#include <drivers/devices/nRF24L01p_mbed.h>

const char * const hello="Hello world!";
int main(void)
{
  PLL_Init(Bus80MHz);
  Time_Init();
  UART0_Init(BAUD_115200);
  nRF24L01p_Init();
  nRF24L01p_write(0,hello,strlen(hello)+1);
  
  while(1){
    UART0_OutString("hello world!\r\n");
  }
  return 0;
}