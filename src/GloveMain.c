#include <string.h>

#include <drivers/tm4c/Wrappers.h>
#include <drivers/tm4c/Timer0.h>

#include <drivers/devices/Fingers.h>

#include <drivers/system/hw.h>
#include <drivers/system/time.h>
#include <drivers/system/switch.h>
#include <drivers/system/display.h>
#include <drivers/system/motion.h>
#include <drivers/system/comms.h>


static int count = 0;
static int sent = 0;
static int misses = 0;  // debugging purposes
static void setSent()
{
  DisableInterrupts();
  sent = 1;
  EnableInterrupts();
}
static void clrSent()
{
  DisableInterrupts();
  sent = 0;
  count = 0;
  EnableInterrupts();
}

#define PERIOD_MS 100
static void periodicTimerTask()
{
  if(!sent)
  {
    ++count;
    if(count >= PERIOD_MS)
    {
      setSent();
    }
  }
  return;
}

static void ProcessInput()
{
  static Switch_State inputOld[3],input[3],keydown[3];
  static int state = 0;
  Switch_GetStates(input);
  for(int i = 0; i < 3; ++i)
  {
    keydown[i] = (input[i] && !inputOld[i])? Switch_On : Switch_Off;
    inputOld[i] = input[i];
  }
  
  if(keydown[0])
  {
    switch(state)
    {
    case 0:
      Fingers_TareExtend();
      state++;
      break;
    case 1:
      Fingers_TareFlex();
      state++;
      break;
    case 2:
      Fingers_TareAbduct();
      state++;
      break;
    case 3:
      Fingers_TareAdduct();
      state = 0;
      break;
    }
  }
}

static void SendData()
{
  Motion_Data motion;
  ADC_Data adc;
  Fingers fingers;
  
  Fingers_BeginReadings(&fingers,&adc);
  Motion_GetReadings(&motion);
  Comms_SendData(&motion,&fingers);
  Fingers_FinishReadings(&fingers,&adc);
}

static void InitializeHardware(void)
{
  Hardware_Init();
  Time_Init();
  Display_Init();
  Motion_Init();
  Fingers_Init();
  Timer0_Init(&periodicTimerTask,80000,2);
}

static int flag = 0;

int main(void)
{
  InitializeHardware();
  while(1)
  {
    WaitForInterrupt();
    if(setSent)
    {
      ProcessInput();
      SendData();
      clrSent();
    }
  }
  return 0;
}