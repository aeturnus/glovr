#ifndef __FINGERS_H__
#define __FINGERS_H__
/**
 * @file    Fingers.h
 * @author  Brandon Nguyen
 * @brief   Driver for the flex sensors, exposed as fingers
 */

#include <stdint.h>
#include <drivers/tm4c/ADC.h>

#define NUM_EXTEND 5
#define NUM_ABDUCT 1

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
  int8_t    extend[5];   // All 5 fingers have flexion sensors
  int8_t    abduct[1]; // Only the thumb has an abduction sensor
} Fingers;

/**
 * @fn          Finger_Init
 * @brief       Initializes the Finger
 */
void Fingers_Init(void);

void Fingers_TareExtend(void);

void Fingers_TareFlex(void);

void Fingers_TareAbduct(void);

void Fingers_TareAdduct(void);

/**
 * @fn          Finger_GetReadings
 * @brief       Gets readings from the Finger
 * @param[out]  readings  Place to write the readings to, updating its timestamp
 */
void Fingers_GetReadings(Fingers * readings);

void Fingers_BeginReadings(Fingers * readings, ADC_Data * data);
int  Fingers_FinishReadings(Fingers * readings, ADC_Data * data);

#endif//__FINGERS_H__