#include <drivers/system/comms.h>
#include <drivers/tm4c/UART.h>
#include <stdio.h>

void Comms_Init()
{
  UART1_Init(BAUD_115200);
}

static void send(const char * buffer, int length)
{
  for(int i = length; i > 0; --i)
  {
    fputc(*buffer,stdout);
    ++buffer;
  }
}

// variations of a random header
const char dataHeader[] = {0x94, 0x26, 0xae, 0x78, 0x4d, 0xf6, 0x95, 0x05};
const char offHeader[] = {0x95, 0x27, 0xaf, 0x79, 0x4e, 0xf7, 0x96, 0x06};
/*
 * Data format:
 * Header - Orr[3] - dOrr[3] - Fingers[2] (thumb, index)
 * 8 - 12 - 12 - 2
 */
#define DATA_SIZE 26
void Comms_SendData(Motion_Data * motion, Fingers * fingers)
{
  static char sendBuffer[128];
  int length;

  int32_t orr[3];
  
  orr[0] = motion->yaw; orr[1] = motion->pitch; orr[2] = motion->roll;
  void * buff = sendBuffer;
  for(int i = 0; i < 3; i++)
  {
    *(int32_t *)buff = orr[i];
    buff = ((char *)buff) + sizeof(int32_t);
  }
  
  orr[0] = motion->dyaw; orr[1] = motion->dpitch; orr[2] = motion->droll;
  for(int i = 0; i < 3; i++)
  {
    *(int32_t *)buff = orr[i];
    buff = ((char *)buff) + sizeof(int32_t);
  }

  *(int8_t *)buff = fingers->extend[Finger_Thumb];
  buff = ((char *)buff) + sizeof(int8_t);
  *(int8_t *)buff = fingers->extend[Finger_Index];
  buff = ((char *)buff) + sizeof(int8_t);


  // send our header and data off
  send(dataHeader,sizeof(dataHeader));
  send(sendBuffer,DATA_SIZE);
}

void Comms_SendOff(void)
{
  send(offHeader,sizeof(offHeader));
}