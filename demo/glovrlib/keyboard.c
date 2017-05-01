#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include <xdo.h>

static xdo_t * x;
void parse(const uint8_t * buffer)
{
  int32_t pos[3];
  int32_t orr[3];
  int8_t fing[6];
  int index = 0;
  for(int i = 0; i < 3; i++)
  {
    pos[i] = *(int32_t *)(&buffer[index]);
    index += sizeof(int32_t);
  }
  for(int i = 0; i < 3; i++)
  {
    orr[i] = *(int32_t *)(&buffer[index]);
    index += sizeof(int32_t);
  }
  for(int i = 0; i < 6; i++)
  {
    fing[i] = *(int8_t *)(&buffer[index]);
    index += sizeof(int8_t);
  }
  static int num = 0;
  //orr[0] = -50000;
  printf("Yaw: %d, Pitch: %d, Roll %d\n",orr[0],orr[1],orr[2]);
  if(orr[0] < -15000)
  {
    printf("[%d] Left turn! (%d)\n",num++,orr[2]);
    xdo_send_keysequence_window_down(x,CURRENTWINDOW,"Left",0);
    //xdo_move_mouse_relative_to_window(x,-100,0,CURRENTWINDOW);
  }
  else
  {
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,"Left",0);
  }

  if(orr[0] > 15000)
  {
    printf("[%d] Right turn! (%d)\n",num++,orr[2]);
    xdo_send_keysequence_window_down(x,CURRENTWINDOW,"Right",0);
    //xdo_move_mouse_relative_to_window(x,100,0,CURRENTWINDOW);
  }
  else
  {
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,"Right",0);
  }
}


int main(int argc, char * argv[])
{
  if(argc < 2)
  {
    fprintf(stderr,"No device file provided\n");
    return -1;
  }

  x = xdo_new(":0.0");

  char cmdBuff[128];
  const char * usbPath = argv[1];
  sprintf(cmdBuff,"stty -F %s 115200 sane",usbPath);
  system(cmdBuff);
  sprintf(cmdBuff,"stty -F %s 115200 raw",usbPath);
  system(cmdBuff);
  FILE * fp;
  fp = fopen(usbPath,"rwb");
  if(!fp)
  {
    fprintf(stderr,"Could not open %s\n",usbPath);
    return -1;
  }
  setvbuf(fp, 0x0,_IONBF, 0);

  char buffer[30];
  while(1)
  {
    fflush(fp);
    size_t read = fread(buffer, 1, sizeof(buffer), fp);
    parse((uint8_t *)buffer);
  }

  fclose(fp);
}
