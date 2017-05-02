#ifndef __COMMS_H__
#define __COMMS_H__
#include <drivers/system/motion.h>
#include <drivers/devices/Fingers.h>

void Comms_SendData(Motion_Data * motion, Fingers * fingers);

#endif//__COMMS_H__