#include <stdio.h>
#include <string.h>

#include <drivers/tm4c/UART.h>
#include <drivers/tm4c/PLL.h>
#include <drivers/tm4c/ADC.h>
#include <drivers/devices/nRF24L01p_mbed.h>
#include <drivers/system/time.h>
#include <drivers/system/switch.h>
#include <drivers/system/motion.h>


void printData(void)
{
  Motion_Data motion;
  ADC_Data adc;
  Switch_State switches[3];
  
  Motion_GetReadings(&motion);
  ADC_Read(&adc);
  Switch_GetStates(switches);
  
  printf("Roll: %d, Pitch: %d, Yaw: %d\r\n",motion.roll,motion.pitch,motion.yaw);
}

int main(void)
{
  PLL_Init(Bus80MHz);
  Time_Init();
  Switch_Init();
  UART1_Init(BAUD_115200);
  ADC_Init();
  Motion_Init();
  
  while(1)
  {
    //puts("Press any key to get data\r\n");
    //fgetc(0);
    for(int i = 0; i < 10; i++)
    {
      SysTick_Wait(80000);
    }
    printData();
  }
  
  return 0;
}
