#ifndef __MOTION_H__
#define __MOTION_H__
/**
 * @file    motion.h
 * @author  Brandon Nguyen
 * @brief   High level motion driver
 */

#include <stdint.h>


typedef struct Motion_Data_str
{
  uint32_t timestamp;

  // position
  int32_t x;
  int32_t y;
  int32_t z;

  // orientation
  int32_t yaw;
  int32_t pitch;
  int32_t roll;
} Motion_Data;

/**
 * @fn    Motion_Init
 * @brief Initializes components of the motion driver
 */
void Motion_Init(void);

/**
 * @fn                Motion_GetReadings
 * @brief             Gets the readings of the motion driver
 * @param[out]  data  Pointer with which to write data to
 *
 *
 */
void Motion_GetReadings(Motion_Data * data);

/**
 * @fn                Motion_Tare
 * @brief             Zeros the motion driver at current point and orientation
 */
void Motion_Tare(void);

/**
 * @fn                Motion_TarePosition
 * @brief             Zeros the position of the motion driver
 */
void Motion_TarePosition(void);

/**
 * @fn                Motion_TareOrientation
 * @brief             Zeros the orientation of the motion driver
 */
void Motion_TareOrientation(void);


#endif//__MOTION_H__
