#include <stdint.h>
#include <drivers/devices/MPU9250.h>
#include <drivers/tm4c/I2C.h>
#include <drivers/tm4c/SysTick.h>
#include <drivers/system/time.h>

#define MPU9250_ADDR      0x68
#define MPU9250_PWR1      107
#define MPU9250_REGBASE   59
void MPU_Init(void)
{
  I2C_Init();
  uint8_t on = 0x0;
  I2C_WriteReg(MPU9250_ADDR,MPU9250_PWR1,1,&on);
}

static int16_t offsets[6] = {0,0,0,0,0,0};
static uint8_t buffer[14];
void MPU_GetReadings(MPU_Data * readings)
{
  I2C_ReadReg(MPU9250_ADDR,MPU9250_REGBASE,14,buffer);
  readings->acc_x = (int16_t)( (buffer[0] << 8) | buffer[1] ) + offsets[0];
  readings->acc_y = (int16_t)( (buffer[2] << 8) | buffer[3] ) + offsets[1];
  readings->acc_z = (int16_t)( (buffer[4] << 8) | buffer[5] ) + offsets[2];

  readings->gyr_x = (int16_t)( (buffer[ 8] << 8) | buffer[ 9] ) + offsets[3];
  readings->gyr_y = (int16_t)( (buffer[10] << 8) | buffer[11] ) + offsets[4];
  readings->gyr_z = (int16_t)( (buffer[12] << 8) | buffer[13] ) + offsets[5];
  
  readings->timestamp = Time_GetTime();
}

#define MPU_TARE_AVG 128
void MPU_Tare(int tareAcc, int tareGyr)
{
  int32_t sums[6];
  for(int i = 0; i < MPU_TARE_AVG; i++)
  {
    I2C_ReadReg(MPU9250_ADDR,MPU9250_REGBASE,14,buffer);
    sums[0] = (int16_t)( (buffer[0] << 8) | buffer[1] );
    sums[1] = (int16_t)( (buffer[2] << 8) | buffer[3] );
    sums[2] = (int16_t)( (buffer[4] << 8) | buffer[5] );

    sums[3] = (int16_t)( (buffer[ 8] << 8) | buffer[ 9] );
    sums[4] = (int16_t)( (buffer[10] << 8) | buffer[11] );
    sums[5] = (int16_t)( (buffer[12] << 8) | buffer[13] );
    SysTick_Wait(80000);
  }
  if(tareAcc)
  {
    for(int i = 0; i < 3; i++)
    {
      offsets[i] = sums[i]/MPU_TARE_AVG;
    }
  }
  if(tareGyr)
  {
    for(int i = 3; i < 6; i++)
    {
      offsets[i] = sums[i]/MPU_TARE_AVG;
    }
  }
}