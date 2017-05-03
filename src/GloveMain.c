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

#define DISABLED 0
#define ENABLED  1


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
static int state = 0;
#define DISABLED_FG 0x001F
#define ENABLED_FG  0x03E0
static void ProcessInput()
{
  static Switch_State inputOld[3],input[3],keydown[3];
  Switch_GetStates(input);
  for(int i = 0; i < 3; ++i)
  {
    keydown[i] = (input[i] && !inputOld[i])? Switch_On : Switch_Off;
    inputOld[i] = input[i];
  }
  
  int cH = Display_GetCellHeight();
  if(keydown[1])
  {
    switch(state)
    {
    case DISABLED:
      Display_DrawString(0,cH*11,"Enabled ",ENABLED_FG,0x0000,1);
      state = ENABLED;
      break;
    case ENABLED:
      Display_DrawString(0,cH*11,"Disabled",DISABLED_FG,0x0000,1);      
      state = DISABLED;
      Comms_SendOff();
      break;
    }
  }
}

static void sprintfFixed(char * buffer, int32_t value)
{
  if(value < 0)
  {
    value = -value;
    sprintf(buffer,"-%d.%03d  ",value/1000,value%1000);
  }
  else
  {
    sprintf(buffer,"%d.%03d  ",value/1000,value%1000);
  }
}

static void DrawData(Motion_Data * motion, Fingers * fingers)
{
  int cW = Display_GetCellWidth();
  int cH = Display_GetCellHeight();
  int row = 1;
  char buffer[16];
  sprintfFixed(buffer,motion->yaw);
  Display_DrawString(cW*8,cH*(row++),buffer,0xFFFF,0x0000,1);
  sprintfFixed(buffer,motion->pitch);
  Display_DrawString(cW*8,cH*(row++),buffer,0xFFFF,0x0000,1);
  sprintfFixed(buffer,motion->roll);
  Display_DrawString(cW*8,cH*(row++),buffer,0xFFFF,0x0000,1);
  row++;

  sprintfFixed(buffer,motion->dyaw);
  Display_DrawString(cW*8,cH*(row++),buffer,0xFFFF,0x0000,1);
  sprintfFixed(buffer,motion->dpitch);
  Display_DrawString(cW*8,cH*(row++),buffer,0xFFFF,0x0000,1);
  sprintfFixed(buffer,motion->droll);
  Display_DrawString(cW*8,cH*(row++),buffer,0xFFFF,0x0000,1);
  
  row++;
  sprintf(buffer,"%%%02d  ",fingers->extend[Finger_Thumb]);
  Display_DrawString(cW*8,cH*(row++),buffer,0xFFFF,0x0000,1);
  sprintf(buffer,"%%%02d  ",fingers->extend[Finger_Index]);
  Display_DrawString(cW*8,cH*(row++),buffer,0xFFFF,0x0000,1);
}

static void SendData()
{
  Motion_Data motion;
  ADC_Data adc;
  Fingers fingers;
  
  Fingers_BeginReadings(&fingers,&adc);
  Motion_GetReadings(&motion);
  Fingers_FinishReadings(&fingers,&adc);
  if(state == ENABLED)
    Comms_SendData(&motion,&fingers);
  DrawData(&motion,&fingers);
}

static void InitDisplay()
{
  int cW = Display_GetCellWidth();
  int cH = Display_GetCellHeight();
  int row = 0;
  Display_DrawString(0,cH*(row++),"GloVR v0.1",0xFFFF,0x0000,1);
  Display_DrawString(0,cH*(row++),"Yaw   :",0xFFFF,0x0000,1);
  Display_DrawString(0,cH*(row++),"Pitch :",0xFFFF,0x0000,1);
  Display_DrawString(0,cH*(row++),"Roll  :",0xFFFF,0x0000,1);
  row++;
  Display_DrawString(0,cH*(row++),"dYaw  :",0xFFFF,0x0000,1);
  Display_DrawString(0,cH*(row++),"dPitch:",0xFFFF,0x0000,1);
  Display_DrawString(0,cH*(row++),"dRoll :",0xFFFF,0x0000,1);
  row++;
  Display_DrawString(0,cH*(row++),"Thumb :",0xFFFF,0x0000,1);
  Display_DrawString(0,cH*(row++),"Index :",0xFFFF,0x0000,1);
  
  Display_DrawString(0,cH*11,"Disabled ",DISABLED_FG,0x0000,1);
}

static void InitializeHardware(void)
{
  Hardware_Init();
  Time_Init();
  Display_Init();
  InitDisplay();
  Switch_Init();
  Motion_Init();
  Fingers_Init();
  Comms_Init();
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