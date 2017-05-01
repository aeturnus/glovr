#include <stdio.h>
#include <string.h>

#include <drivers/tm4c/UART.h>
#include <drivers/tm4c/PLL.h>
#include <drivers/tm4c/ADC.h>
#include <drivers/devices/nRF24L01p_mbed.h>
#include <drivers/system/time.h>
#include <drivers/system/switch.h>
#include <drivers/devices/MPU9250.h>


char sendBuffer[30];
void printData(void)
{
  // bytes of transmission:
  // 3*4 bytes for position, 3*4 bytes for orientation, 6*1 byte for fingers
  // = 12 + 12 + 6 = 30 bytes
  int32_t pos[3] = {40000, 50000, -30000};
  int32_t orr[3] = {170000, -170000, 45000};
  int8_t fing[6] = {10, 20, 30, 40, 50, 60};
  void * buffer = sendBuffer;
  for(int i = 0; i < 3; i++)
  {
    *(uint32_t *)buffer = pos[i];
    buffer = (uint8_t *)buffer + 4;
  }
  for(int i = 0; i < 3; i++)
  {
    *(uint32_t *)buffer = orr[i];
    buffer = (uint8_t *)buffer + 4;
  }
  for(int i = 0; i < 6; i++)
  {
    *(uint8_t *)buffer = fing[i];
    buffer = (uint8_t *)buffer + 1;
  }
  for(int i = 0; i < 30; i++)
  {
    fputc(sendBuffer[i]&0xFF,stdout);
  }

  /*
  int index = 0;
  for(int i = 0; i < 3; i++)
  {
    int32_t num = pos[i];
    sendBuffer[index++] = (num >> 24) & 0xFF;
    sendBuffer[index++] = (num >> 16) & 0xFF;
    sendBuffer[index++] = (num >> 8)  & 0xFF;
    sendBuffer[index++] = (num >> 0)  & 0xFF;
  }
  for(int i = 0; i < 3; i++)
  {
    int32_t num = orr[i];
    sendBuffer[index++] = (num >> 24) & 0xFF;
    sendBuffer[index++] = (num >> 16) & 0xFF;
    sendBuffer[index++] = (num >> 8)  & 0xFF;
    sendBuffer[index++] = (num >> 0)  & 0xFF;
  }
  for(int i = 0; i < 6; i++)
  {
    sendBuffer[index++] = fing[i]&0xFF;
  }
  for(int i = 0; i < 30; i++)
  {
    fputc(sendBuffer[i]&0xFF,stdout);
  }
  */
}

int main(void)
{
  PLL_Init(Bus80MHz);
  Time_Init();
  Switch_Init();
  UART1_Init(BAUD_115200);
  ADC_Init();
  MPU_Init();
  
  char buffer[32];
  while(1)
  {
    //puts("Press any key to get data\r\n");
    //fgets(buffer,sizeof(buffer),stdin);
    //if(!strcmp(buffer,"POLL"))
    //{
    char c;
    fread(&c,1,1,stdin);
    if(c == 10)
      printData();
    //}
  }
  
  return 0;
}
