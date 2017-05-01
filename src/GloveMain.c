#include <string.h>

#include <drivers/tm4c/UART.h>
#include <drivers/tm4c/PLL.h>

#include <drivers/devices/Fingers.h>

#include <drivers/system/hw.h>
#include <drivers/system/time.h>
#include <drivers/system/switch.h>
#include <drivers/system/display.h>
#include <drivers/system/motion.h>

void periodicTimerTask()
{
  return;
}

static void InitializeHardware(void)
{
  Hardware_Init();
  Time_Init();
  Display_Init();
  Motion_Init();
  Fingers_Init();
}

static int flag = 0;

int main(void)
{
  InitializeHardware();
  return 0;
}