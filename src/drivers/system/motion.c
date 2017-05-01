#include <drivers/devices/MPU9250.h>
#include <drivers/system/time.h>
#include <drivers/system/motion.h>

#include <math.h>
void Motion_Init(void)
{
  MPU_Init();
}


static float offPitch, offRoll, offYaw = 0.0f;
static float pitch,roll,yaw;
static int32_t oldTime = 0;
static Motion_Data oldData;
#define M_PI 3.1459265359
#define ACCEL_SEN 8192.0
#define GYRO_SEN  (32768.0f/250.0f)
//#define GYRO_SEN  32.768
//#define GYRO_SEN  65.536
void Motion_GetReadings(Motion_Data * data)
{
  int32_t time = Time_GetTime();
  if(time == oldTime)
  {
    *data = oldData;
    return;
  }

  int32_t dt = time - oldTime;
  MPU_Data mpu;
  static int16_t filter[6][32];
  static int16_t result[6];
  static int index = 0;
  MPU_GetReadings(&mpu);
  filter[0][index] = mpu.acc_x;
  filter[1][index] = mpu.acc_y;
  filter[2][index] = mpu.acc_z;
  filter[3][index] = mpu.gyr_x;
  filter[4][index] = mpu.gyr_y;
  filter[5][index] = mpu.gyr_z;
  for(int r = 0; r < 6; r++)
  {
    int32_t sum = 0;
    for(int c = 0; c < 32; c++)
    {
      sum += filter[r][c];
    }
    result[r] = sum/32;
  }
  mpu.acc_x = result[0];
  mpu.acc_y = result[1];
  mpu.acc_z = result[2];
  mpu.gyr_x = result[3];
  mpu.gyr_y = result[4];
  mpu.gyr_z = result[5];
  index = (index+1)&0x1F;
  float pitchA, rollA, yawA;
  pitch += (((float)mpu.gyr_x / GYRO_SEN) * dt)/1000 + offPitch;
  roll  -= (((float)mpu.gyr_y / GYRO_SEN) * dt)/1000  + offRoll;
  yaw   -= (((float)mpu.gyr_z / GYRO_SEN) * dt)/1000  + offYaw;

  int forceMagApprox = abs(mpu.acc_x) + abs(mpu.acc_y) + abs(mpu.acc_z);
  if(forceMagApprox > 8192 && forceMagApprox < 32768)
  {
    pitchA = atan2f((float)mpu.acc_x, (float)mpu.acc_z) * 180 / M_PI;
    rollA  = atan2f((float)mpu.acc_y, (float)mpu.acc_z) * 180 / M_PI;
    
    pitch = pitch * .80 + pitchA * .20;
    roll = roll * .80 + rollA * .20;
  }
  if(roll > 180.0)
    roll = 180.0;
  if(roll < -180.0)
    roll = -180.0;
  if(pitch > 180.0)
    pitch = 180.0;
  if(pitch< -180.0)
    pitch = -180.0;
  if(yaw > 180.0)
    yaw = 180.0;
  if(yaw < -180.0)
    yaw = -180.0;
  int32_t pitchF, rollF, yawF;
  pitchF = (int32_t) (pitch * 1000);
  rollF = (int32_t) (roll * 1000);
  yawF = (int32_t) (yaw * 1000);
  data->roll = pitchF;
  data->pitch = rollF;
  data->yaw = yawF;

  oldTime = time;
  oldData = *data;
}
void Motion_Tare(void)
{

}
void Motion_TarePosition(void)
{

}
void Motion_TareOrientation(void)
{
  offPitch = -pitch;
  offRoll = -roll;
  offYaw= -yaw;
}