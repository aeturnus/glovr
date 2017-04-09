#ifndef __FIXED_TYPES_H__
#define __FIXED_TYPES_H__
/**
 * @file    types.h
 * @author  Brandon Nguyen
 * @brief   Fixed point types
 */

#include <stdint.h>

typedef int32_t fix16_16;

#define MAX_16_16 0x7FFFFFFF
#define MIN_16_16 0x80000000

static inline fix16_16 sat16_16(int64_t x)
{
  if( x > MAX_16_16 ) return MAX_16_16;
  else if( x < -MIN_16_16 ) return -MIN_16_16;
  return  (fix16_16)x;
  }

static inline fix16_16 add16_16(fix16_16 x, fix16_16 y)
{
  return x + y;
}

static inline fix16_16 sadd16_16(fix16_16 x, fix16_16 y)
{
  int64_t temp = (int64_t)x + (int64_t)y;
  return sat16_16(temp);
}

static inline fix16_16 sub16_16(fix16_16 x, fix16_16 y)
{
  return x - y;
}

static inline fix16_16 ssub16_16(fix16_16 x, fix16_16 y)
{
  int64_t temp = (int64_t)x - (int64_t)y;
  return sat16_16(temp);
}

static inline fix16_16 mul16_16(fix16_16 x, fix16_16 y)
{
  int64_t temp = (int64_t)x * (int64_t)y;
  temp += 1<<15;  // round up the bit at the end
  temp >>= 16;
  return (fix16_16)temp;
}

static inline fix16_16 smul16_16(fix16_16 x, fix16_16 y)
{
  int64_t temp = (int64_t)x * (int64_t)y;
  temp += 1<<15;  // round up the bit at the end
  temp >>= 16;
  return sat16_16(temp);
}

static inline fix16_16 div16_16(fix16_16 x, fix16_16 y)
{
  int64_t temp = (int64_t)x << 16;
  // rounding
  if( (x >= 0 && y >= 0) || (x < 0 && y < 0) )
    temp += (y>>1);
  else
    temp -= (y>>1);
  temp /= y;
  return (int16_t)temp;
}

#define decTo16_16(i) ((i) * 0x00010000)

#endif//__FIXED_TYPES_H__
