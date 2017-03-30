#ifndef __FINGERS_H__
#define __FINGERS_H__
/**
 * @file    Fingers.h
 * @author  Brandon Nguyen
 * @brief   Driver for the flex sensors, exposed as fingers
 */

#include <stdint.h>

typedef enum Finger_ID_enum
{
  Finger_Thumb = 0,
  Finger_Index,
  Finger_Middle,
  Finger_Ring,
  Finger_Pinky
} Finger_ID;

/**
 * @struct    Fingers
 * @brief     Holds finger data
 */
typedef struct Fingers_str
{
  uint32_t  timestamp;
  int8_t    flexion[5];   // All 5 fingers have flexion sensors
  int8_t    abduction[1]; // Only the thumb has an abduction sensor
} Fingers;

/**
 * @fn          Finger_Init
 * @brief       Initializes the Finger
 */
void Finger_Init(void);

/**
 * @fn          Finger_GetReadings
 * @brief       Gets readings from the Finger
 * @param[out]  readings  Place to write the readings to, updating its timestamp
 */
void Finger_GetReadings(Finger_Data * readings);


#endif//__FINGERS_H__