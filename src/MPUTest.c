#include <stdio.h>
#include <string.h>

#include <drivers/tm4c/UART.h>
#include <drivers/tm4c/PLL.h>
#include <drivers/tm4c/ADC.h>
#include <drivers/devices/nRF24L01p_mbed.h>
#include <drivers/devices/Fingers.h>
#include <drivers/system/time.h>
#include <drivers/system/switch.h>
#include <drivers/system/motion.h>


void printData(void)
{
  Motion_Data motion;
  Fingers fingers;
  ADC_Data adc;
  Switch_State switches[3];
  //Finger_GetReadings(&fingers);
  Finger_BeginReadings(&fingers, &adc);
  Motion_GetReadings(&motion);
  Switch_GetStates(switches);
  while(Finger_FinishReadings(&fingers, &adc)){};
  
  printf("Roll: %d, Pitch: %d, Yaw: %d, Finger: %d\r\n",motion.roll,motion.pitch,motion.yaw, fingers.extend[0]);
}

int main(void)
{
  PLL_Init(Bus80MHz);
  Time_Init();
  Switch_Init();
  UART1_Init(BAUD_115200);
  ADC_Init();
  Motion_Init();
  Finger_Init();
  printf("Press to tare extension...\r\n");
  fgetc(0);
  Finger_TareExtend();
  printf("Press to tare flexion...\r\n");
  fgetc(0);
  Finger_TareFlex();
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
