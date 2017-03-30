#ifndef __TIME_H__
#define __TIME_H__
/**
 * @file    time.h
 * @author  Brandon Nguyen
 * @brief   API for time services
 * The time is kept on a millisecond resolution
 */
#include <stdint.h>

/**
 * @fn      Time_Init
 * @brief   Initializes the Time
 */
void Time_Init( void );

/**
 * @fn          Time_GetTime
 * @brief       Acquires the time
 * @param[out]  timePtr Copies system time to the timePtr
 */
uint32_t Time_GetTime( void );

/**
 * @fn              Time_TimeDifference
 * @brief           Get the difference between times
 * @param[in] start The earlier time
 * @param[in] end   The later time
 * @return          The difference between end and start: returns end-start,
 *                  normalized for overflows
 */
uint32_t Time_Difference( uint32_t start, uint32_t end );

#endif//__TIME_H__

