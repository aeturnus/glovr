#include <drivers/system/comms.h>
#include <stdio.h>

static void send(const char * buffer, int length)
{
  for(int i = length; i > 0; --i)
  {
    fputc(*buffer,stdout);
    ++buffer;
  }
}

// random header
const char header[] = {0x94, 0x26, 0xae, 0x78, 0x4d, 0xf6, 0x95, 0x05};
/*
 * Data format:
 * Header - Orr[3] - Fingers[6]
 * 8 - 12 - 6
 */
#define DATA_SIZE 18
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

  for(int i = 0; i < NUM_EXTEND; i++)
  {
    *(int8_t *)buff = fingers->extend[i];
    buff = ((char *)buff) + sizeof(int8_t);
  }

  for(int i = 0; i < NUM_ABDUCT; i++)
  {
    *(int8_t *)buff = fingers->abduct[i];
    buff = ((char *)buff) + sizeof(int8_t);
  }

  // send our header and data off
  send(header,sizeof(header));
  send(sendBuffer,DATA_SIZE);
}