#include <drivers/system/comms.h>
#include <stdio.h>

static void send(const char * buffer, int length)
{
  for(int i = length; i > 0; --i)
  {
    fputc(stdout,*buffer);
    ++buffer;
  }
}

// random header
const char header[] = {0x94, 0x26, 0xae, 0x78, 0x4d, 0xf6, 0x95, 0x05};
/*
 * Data format:
 * Header - Pos[3] - Vel[3] - Acc[3] - Orr[3] - Fingers[6]
 *
 */
void Comms_SendData(Motion_Data * motion, Fingers * fingers)
{
  static char sendBuffer[128];
  int length;



  // send our header
  send(header,sizeof(header));
  send(sendBuffer,length)
}