#include <stdint.h>
#include <drivers/devices/Fingers.h>
#include <drivers/system/time.h>
#include <drivers/tm4c/ADC.h>

void Finger_Init(void)
{
  ADC_Init();
}


void Finger_TareExtend(void)
{

}

void Finger_TareFlex(void)
{

}

void Finger_TareAbduct(void)
{

}

void Finger_TareAdduct(void)
{
  
}


void Finger_GetReadings(Finger_Data * readings)
{

}


#endif//__FINGERS_H__