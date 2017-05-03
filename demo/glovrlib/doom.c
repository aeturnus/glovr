#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <xdo.h>

#define FORWARD_KEY   "w"
#define BACKWARD_KEY  "s"
#define T_LEFT_KEY    "a"
#define T_RIGHT_KEY   "d"
#define S_LEFT_KEY    "j"
#define S_RIGHT_KEY   "l"
#define NEXT_WEAP_KEY "i"
#define FIRE_KEY      "space"
#define USE_KEY       "k"

#define DEBUG_ENABLED

#if defined DEBUG_ENABLED
  #define DEBUG(...)  printf(__VA_ARGS__)
#else
  #define DEBUG(...)
#endif

#define YAW_THRESH 10000
#define PITCH_THRESH 10000
#define ROLL_THRESH 10000
static xdo_t * x;

typedef enum Control_e
{
  Ctrl_Forward=0,
  Ctrl_Backward,
  Ctrl_TurnLeft,
  Ctrl_TurnRight,
  Ctrl_StrafeLeft,
  Ctrl_StrafeRight,
  Ctrl_Fire,
  Ctrl_Use,
  Ctrl_NextWeapon,
  Ctrl_Num
} Control;

const char * keys[]={
  FORWARD_KEY,
  BACKWARD_KEY,
  T_LEFT_KEY,
  T_RIGHT_KEY,
  S_LEFT_KEY,
  S_RIGHT_KEY,
  FIRE_KEY,
  USE_KEY,
  NEXT_WEAP_KEY
};
static int ctrlStatus[Ctrl_Num];

static int eventCounter = 0;
static void clearKeys()
{
  for(int i=Ctrl_Num-1;i >= 0; --i)
  {
    ctrlStatus[i] = 0;
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,keys[i],0);
  }
}

static void clrKey(Control ctrl)
{
  if(ctrlStatus[ctrl] == 1)
  {
    ctrlStatus[ctrl] = 0;
    DEBUG("[%d] %s ^^^\n",eventCounter++, keys[ctrl]);
    xdo_send_keysequence_window_up(x,CURRENTWINDOW,keys[ctrl],0);
  }
}
static void setKey(Control ctrl)
{
  if(ctrlStatus[ctrl] == 0)
  {
    ctrlStatus[ctrl] = 1;
    DEBUG("[%d] %s vvv\n",eventCounter++, keys[ctrl]);
    xdo_send_keysequence_window_down(x,CURRENTWINDOW,keys[ctrl],0);
  }
}

#define YAW   0
#define PITCH 1
#define ROLL  2
typedef struct RecvData_str
{
  int32_t orr[3];
  int32_t dorr[3];
  int8_t  fing[2];
} RecvData;

typedef struct CtrlStatus_Str
{
  int isAtReady;  // "low ready"/"tactical"
  int isInteract; // interacting motion
  int move; // +1 if moving forward
  int moveTimeout; // timeout between switching states
  int turn; // +1 if turning right
  int turnTimeout; // timeout between switching states
} CtrlStatus;

#define ROT_THRESH 1250
#define FWD_THRESH 750
#define BCK_THRESH -1500
#define RGT_THRESH 750
#define LFT_THRESH -1500
#define MOV_TIME 5 // samples
void actOnData(const RecvData * data, CtrlStatus * status)
{
  DEBUG("atReady: %d, interact: %d, move: %d (%d), turn: %d (%d)\n",status->isAtReady,status->isInteract,status->move, status->moveTimeout,status->turn, status->turnTimeout);
  status->moveTimeout++;
  status->turnTimeout++;
  if(status->isAtReady)
  {
    if(data->dorr[PITCH] > ROT_THRESH && data->dorr[ROLL] < -2*ROT_THRESH)
    {
      status->isInteract = 1;
      status->isAtReady  = 0;
      clearKeys();
      setKey(Ctrl_Use);
      status->move = 0; status->turn = 0;
    }
  }

  if(!status->isAtReady)
  {
    if(status->isInteract && data->dorr[PITCH] < -ROT_THRESH && data->dorr[ROLL] > 2*ROT_THRESH)
    {
      status->isInteract = 0;
      status->isAtReady  = 1;
      clearKeys();
      status->move = 0; status->turn = 0;
    }
    else if( 75000 < data->orr[ROLL] && data->orr[ROLL] < 105000 )
    {
      status->isInteract = 0;
      status->isAtReady  = 1;
      DEBUG("At ready, by orientation\n");
      clearKeys();
      status->move = 0; status->turn = 0;
    }
  }

  //if(status->isAtReady)
  if(1)
  {
    if(data->fing[1] > 70)
    {
      setKey(Ctrl_Fire);
    }
    else
    {
      clrKey(Ctrl_Fire);
    }

    if( !(75000 < data->orr[ROLL] && data->orr[ROLL] < 105000) )
    {
      status->isInteract = 0;
      status->isAtReady  = 0;
      clearKeys();
      status->move = 0; status->turn = 0;
    }

    switch(status->move)
    {
    case -1:
      if(data->dorr[YAW] > FWD_THRESH )
      {
        if(status->moveTimeout > MOV_TIME)
        {
          status->moveTimeout = 0;
          status->move = 0;
          clrKey(Ctrl_Backward);
        }
      }
      else
      {
        setKey(Ctrl_Backward);
      }
      break;
    case 0:
      if(data->dorr[YAW] > FWD_THRESH )
      {
        if(status->moveTimeout > MOV_TIME)
        {
          status->moveTimeout = 0;
          status->move = 1;
          setKey(Ctrl_Forward);
        }
      }
      else if(data->dorr[YAW] < BCK_THRESH )
      {
        if(status->moveTimeout > MOV_TIME)
        {
          status->moveTimeout = 0;
          status->move = -1;
          setKey(Ctrl_Backward);
        }
      }
      else
      {
        clrKey(Ctrl_Backward); clrKey(Ctrl_Forward);
      }
      break;
    case 1:
      if(data->dorr[YAW] < BCK_THRESH )
      {
        if(status->moveTimeout > MOV_TIME)
        {
          status->moveTimeout = 0;
          status->move = 0;
          clrKey(Ctrl_Forward);
        }
      }
      else
      {
        setKey(Ctrl_Forward);
      }
      break;
    default:
      break;
    }

    if(20000 < data->orr[PITCH] && data->orr[PITCH] < 50000)
    {
      status->turn = 0;
      status->turnTimeout = 0;
      clrKey(Ctrl_TurnLeft);
      clrKey(Ctrl_TurnRight);
    }
    else if(data->orr[PITCH] < 20000)
    {
      status->turn = -1;
      status->turnTimeout = 0;
      setKey(Ctrl_TurnLeft);
      clrKey(Ctrl_TurnRight);
    }
    else if(data->orr[PITCH] > 50000)
    {
      status->turn = 1;
      status->turnTimeout = 0;
      clrKey(Ctrl_TurnLeft);
      setKey(Ctrl_TurnRight);
    }
    {
      switch(status->turn)
      {
      case -1:
        if(data->dorr[PITCH] > RGT_THRESH)
        {
          if(status->turnTimeout > MOV_TIME)
          {
            status->turnTimeout = 0;
            status->turn = 0;
            clrKey(Ctrl_TurnLeft);
          }
        }
        else
        {
          setKey(Ctrl_TurnLeft);
        }
        break;
      case 0:
        if(data->dorr[PITCH] > RGT_THRESH )
        {
          if(status->turnTimeout > MOV_TIME)
          {
            status->turnTimeout = 0;
            status->turn = 1;
            setKey(Ctrl_TurnRight);
          }
        }
        else if(data->dorr[PITCH] < LFT_THRESH )
        {
          if(status->turnTimeout > MOV_TIME)
          {
            status->turnTimeout = 0;
            status->turn = -1;
            setKey(Ctrl_TurnLeft);
          }
        }
        else
        {
          clrKey(Ctrl_TurnLeft); clrKey(Ctrl_TurnRight);
        }
        break;
      case 1:
        if(data->dorr[PITCH] < LFT_THRESH )
        {
          if(status->turnTimeout > MOV_TIME)
          {
            status->turnTimeout = 0;
            status->turn = 0;
            clrKey(Ctrl_TurnRight);
          }
        }
        else
        {
          setKey(Ctrl_TurnRight);
        }
        break;
      default:
        break;
      }
    }
  }
}

void parse(const uint8_t * buffer)
{
  static RecvData data;
  static CtrlStatus status;
  int index = 0;
  for(int i = 0; i < 3; i++)
  {
    data.orr[i] = *(int32_t *)(&buffer[index]);
    index += sizeof(int32_t);
  }
  for(int i = 0; i < 3; i++)
  {
    data.dorr[i] = *(int32_t *)(&buffer[index]);
    index += sizeof(int32_t);
  }
  for(int i = 0; i < 2; i++)
  {
    data.fing[i] = *(int8_t *)(&buffer[index]);
    index += sizeof(int8_t);
  }
  actOnData(&data,&status);
}

const char dataHeader[] = {0x94, 0x26, 0xae, 0x78, 0x4d, 0xf6, 0x95, 0x05};
const char offHeader[] =  {0x95, 0x27, 0xaf, 0x79, 0x4e, 0xf7, 0x96, 0x06};
void waitForCommand(FILE * fp)
{
  static char history[8];
  static char buffer[26];
  char c; size_t read;
  if(!memcmp(dataHeader,history,sizeof(dataHeader)))
  {
    //DEBUG("Data Received!\n");
    read = fread(buffer, 1, sizeof(buffer), fp);
    parse((uint8_t *)buffer);
  }
  else if(!memcmp(offHeader,history,sizeof(offHeader)))
  {
    //DEBUG("Device stopped transmitting\n");
    clearKeys();
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
