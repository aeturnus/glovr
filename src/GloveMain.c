#include <string.h>

#include <drivers/tm4c/UART.h>
#include <drivers/tm4c/PLL.h>
#include <drivers/system/hw.h>
#include <drivers/system/time.h>
#include <drivers/system/switch.h>
#include <drivers/devices/nRF24L01p_mbed.h>

static void InitializeHardware(void)
{
  Hardware_Init();
  Time_Init();
  Display_Init();
  MPU_Init();
  Fingers_Init();
  nRF24L01p_Init();
}

int main(void)
{
  InitializeHardware();
  return 0;
}