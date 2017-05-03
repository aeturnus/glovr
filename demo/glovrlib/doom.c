#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <xdo.h>

#define FORWARD   "W"
#define BACKWARD  "S"
#define T_LEFT    "A"
#define T_RIGHT   "D"
#define NEXT_WEAP "I"
#define FIRE      "space"
#define USE       "K"

#define YAW_THRESH 10000
#define PITCH_THRESH 10000
#define ROLL_THRESH 10000
static xdo_t * x;
void parse(const uint8_t * buffer)
{
  int32_t orr[3];
  int32_t dorr[3];
  int8_t fing[2];
  int index = 0;
  for(int i = 0; i < 3; i++)
  {
    orr[i] = *(int32_t *)(&buffer[index]);
    index += sizeof(int32_t);
  }
  for(int i = 0; i < 3; i++)
  {
    dorr[i] = *(int32_t *)(&buffer[index]);
    index += sizeof(int32_t);
  }
  for(int i = 0; i < 2; i++)
  {
    fing[i] = *(int8_t *)(&buffer[index]);
    index += sizeof(int8_t);
  }
  static int num = 0;
  //orr[0] = -50000;
  printf("Yaw: %d, Pitch: %d, Roll %d, Thumb: %d, Index: %d\n",orr[0],orr[1],orr[2],fing[0],fing[1]);
  if(orr[2] < -PITCH_THRESH || dorr[2] < -300)
  {
    printf("[%d] Left turn! (%d)\n",num++,orr[2]);
    xdo_send_keysequence_window_down(x,CURRENTWINDOW,T_LEFT,0);
  }
  else
  {
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,T_LEFT,0);
  }

  if(orr[2] > PITCH_THRESH || dorr[2] > 300)
  {
    printf("[%d] Right turn! (%d)\n",num++,orr[2]);
    xdo_send_keysequence_window_down(x,CURRENTWINDOW,T_RIGHT,0);
  }
  else
  {
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,T_RIGHT,0);
  }

  if(orr[1] < -PITCH_THRESH || dorr[1] < -300)
  {
    printf("[%d] Forward! (%d)\n",num++,orr[1]);
    xdo_send_keysequence_window_down(x,CURRENTWINDOW,FORWARD,0);
  }
  else
  {
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,FORWARD,0);
  }

  if(orr[1] > PITCH_THRESH || dorr[1] > 300)
  {
    printf("[%d] Back! (%d)\n",num++,orr[1]);
    xdo_send_keysequence_window_down(x,CURRENTWINDOW,BACKWARD,0);
  }
  else
  {
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,BACKWARD,0);
  }

  if(fing[1] > 70)
  {
    printf("[%d] Pew pew! (%d)\n",num++,fing[1]);
    xdo_send_keysequence_window_down(x,CURRENTWINDOW,FIRE,0);
  }
  else
  {
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,FIRE,0);
  }

  if(fing[0] > 50)
  {
    printf("[%d] Unf! (%d)\n",num++,fing[0]);
    xdo_send_keysequence_window_down(x,CURRENTWINDOW,USE,0);
  }
  else
  {
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,USE,0);
  }
}

const char header[] = {0x94, 0x26, 0xae, 0x78, 0x4d, 0xf6, 0x95, 0x05};
void waitForCommand(FILE * fp)
{
  static char history[8];
  static char buffer[26];
  char c; size_t read;
  if(!memcmp(header,history,sizeof(header)))
  {
    //printf("Success!\n");
    read = fread(buffer, 1, sizeof(buffer), fp);
    parse((uint8_t *)buffer);
  }
  read = fread(&c, 1, sizeof(char), fp);
  for(int i = 0; i < 7; i++)
  {
    history[i] = history[i+1];
  }
  history[7] = c;

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

  while(1)
  {
    fflush(fp);
    waitForCommand(fp);
    //size_t read = fread(buffer, 1, sizeof(buffer), fp);
    //parse((uint8_t *)buffer);
  }

  fclose(fp);
}
