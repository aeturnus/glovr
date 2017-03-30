/**
 * @file    time.c
 * @author  Brandon Nguyen
 * @brief   API for time services
 * The time is kept on a millisecond resolution
 */
#include <stdint.h>
#include <drivers/system/time.h>
#include <drivers/tm4c/SysTick.h>

static volatile uint32_t time;

static void IncrementTime(void)
{
  time = time + 1;
}

void Time_Init( void )
{
  time = 0;
  SysTick_Init(WAIT_1MS, 1, &IncrementTime);
}

uint32_t Time_GetTime( void )
{
  uint32_t now = time;
  return now;
}

uint32_t Time_Difference( uint32_t start, uint32_t end )
{
  uint32_t diff;
  if(end < start)
  {
    diff = (0xFFFFFFFF-start) + end;
  }
  else
  {
    diff = end-start;
  }
  return diff;
}
