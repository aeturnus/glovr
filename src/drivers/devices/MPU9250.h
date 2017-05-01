#ifndef __MPU9250_H__
#define __MPU9250_H__
/**
 * @file    MPU9250.h
 * @author  Brandon Nguyen
 * @brief   Driver for MPU-9250
 */

#include <stdint.h>

/**
 * @struct    MPU_Data
 * @brief     Holds readings from the MPU
 */
typedef struct MPU_Data_str
{
  uint32_t timestamp;

  int16_t gyr_x;
  int16_t gyr_y;
  int16_t gyr_z;

  int16_t acc_x;
  int16_t acc_y;
  int16_t acc_z;

  int16_t mag_x;
  int16_t mag_y;
  int16_t mag_z;
} MPU_Data;

/**
 * @fn          MPU_Init
 * @brief       Initializes the MPU
 */
void MPU_Init(void);

/**
 * @fn          MPU_GetReadings
 * @brief       Gets readings from the MPU
 * @param[out]  readings  Place to write the readings to, updating its timestamp
 */
void MPU_GetReadings(MPU_Data * readings);

/**
 * @fn          MPU_Tare
 * @brief       Set software offsets for readings
 */
void MPU_Tare(int tareAcc, int tareGyr);

#endif//__MPU9250_H__