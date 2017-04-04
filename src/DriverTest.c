#include <stdio.h>
#include <string.h>

#include <drivers/tm4c/UART.h>
#include <drivers/tm4c/PLL.h>
#include <drivers/tm4c/ADC.h>
#include <drivers/devices/nRF24L01p_mbed.h>
#include <drivers/system/time.h>
#include <drivers/system/switch.h>
#include <drivers/devices/MPU9250.h>


void printData(void)
{
  MPU_Data mpu;
  ADC_Data adc;
  Switch_State switches[3];
  
  MPU_GetReadings(&mpu);
  ADC_Read(&adc);
  Switch_GetStates(switches);
  
  printf("Dump:\r\nAcc:\r\n    x: %d\r\n    y: %d\r\n    z: %d\r\nGyr:\r\n    x: %d\r\n    y: %d\r\n    z: %d\r\n"
  "ADC:\r\n[0]: %d\r\n[1]: %d\r\n[2]: %d\r\n[3]: %d\r\n[4]: %d\r\n[5]: %d\r\nSwitch 0: %s\r\nSwitch 1: %s\r\nSwitch 2: %s\r\n",
    mpu.acc_x,mpu.acc_y,mpu.acc_z,mpu.gyr_x,mpu.gyr_y,mpu.gyr_z,
    adc.values[0],adc.values[1],adc.values[2],adc.values[3],adc.values[4],adc.values[5],
    switches[0]?"ON":"OFF",switches[1]?"ON":"OFF",switches[2]?"ON":"OFF");
}

int main(void)
{
  PLL_Init(Bus80MHz);
  Time_Init();
  Switch_Init();
  UART1_Init(BAUD_115200);
  ADC_Init();
  MPU_Init();
  
  while(1)
  {
    puts("Press any key to get data\r\n");
    fgetc(0);
    printData();
  }
  
  return 0;
}
