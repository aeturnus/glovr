#include <stdint.h>
#include <drivers/devices/MPU9250.h>
#include <drivers/tm4c/I2C.h>

#define MPU9250_ADDR      0x68
#define MPU9250_PWR1      107
#define MPU9250_REGBASE   59
void MPU_Init(void)
{
  I2C_Init();
  uint8_t on = 0x0;
  I2C_WriteReg(MPU9250_ADDR,MPU9250_PWR1,1,&on);
}

static uint8_t buffer[14];
void MPU_GetReadings(MPU_Data * readings)
{
  I2C_ReadReg(MPU9250_ADDR,MPU9250_REGBASE,14,buffer);
  readings->acc_x = (int16_t)( (buffer[0] << 8) | buffer[1] );
  readings->acc_y = (int16_t)( (buffer[2] << 8) | buffer[3] );
  readings->acc_z = (int16_t)( (buffer[4] << 8) | buffer[5] );

  readings->gyr_x = (int16_t)( (buffer[ 8] << 8) | buffer[ 9] );
  readings->gyr_y = (int16_t)( (buffer[10] << 8) | buffer[11] );
  readings->gyr_z = (int16_t)( (buffer[12] << 8) | buffer[13] );
}
