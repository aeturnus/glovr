#include <stdint.h>
#include <tm4c123gh6pm.h>
#include <drivers/tm4c/I2C.h>

/*
#define I2C_MCS_ACK             0x08  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x08  // Acknowledge Data
#define I2C_MCS_ADRACK          0x04  // Acknowledge Address
#define I2C_MCS_STOP            0x04  // Generate STOP
#define I2C_MCS_START           0x02  // Generate START
#define I2C_MCS_ERROR           0x02  // Error
#define I2C_MCS_RUN             0x01  // I2C Master Enable
#define I2C_MCS_BUSY            0x01  // I2C Busy
#define I2C_MCR_MFE             0x10  // I2C Master Function Enable
*/

#define MAXRETRIES              5           // number of receive attempts before giving up
void I2C_Init(void){
  SYSCTL_RCGCI2C_R |= 0x0008;           // activate I2C3
  SYSCTL_RCGCGPIO_R |= 0x0008;          // activate port D
  while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?

  GPIO_PORTD_AFSEL_R |= 0x03;           // 3) enable alt funct on PD0,1
  GPIO_PORTD_ODR_R |= 0x02;             // 4) enable open drain on PD1 only
  GPIO_PORTD_DEN_R |= 0x03;             // 5) enable digital I/O on PD0,1
                                        // 6) configure PD0,1 as I2C
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFFFFF00)+0x00000033;
  GPIO_PORTD_AMSEL_R &= ~0x03;          // 7) disable analog functionality on PB2,3
  I2C3_MCR_R = I2C_MCR_MFE;             // 8) master function enable
  I2C3_MTPR_R = 8;                      // 9) configure for 400 kbps clock
}

int I2C_ReadReg(int8_t slave, int8_t addr, int length, uint8_t * buffer)
{
  int ret = I2C_Send(slave, 1, &addr);
  if(ret)
    return ret;
  return I2C_Recv(slave, length, buffer);
}

int I2C_WriteReg(int8_t slave, int8_t addr, int length, const uint8_t * buffer)
{
  int ret = I2C_Send(slave, 1, &addr);
  if(ret)
    return ret;
  return I2C_Send(slave, length, buffer);
}

static inline uint8_t I2C_RecvByte(uint32_t flags)
{
  I2C3_MCS_R = flags;
  while(I2C3_MCS_R&I2C_MCS_BUSY){};   // wait for transmission done
  return (I2C3_MDR_R&0xFF);
}

int I2C_Recv(int8_t slave, int length, uint8_t * buffer)
{
  int retryCounter = 1;
  
  do{
    int i = 0;

    // sendoff the slave address
    while(I2C3_MCS_R&I2C_MCS_BUSY){}; // wait for I2C ready
    I2C3_MSA_R = (slave<<1)&0xFE;     // MSA[7:1] is slave address
    I2C3_MSA_R |= 0x01;               // MSA[0] is 1 for recv

    if(length == 1)
    {
      buffer[i] = I2C_RecvByte(I2C_MCS_STOP | I2C_MCS_START | I2C_MCS_RUN);
    }
    else
    {
      buffer[i++] = I2C_RecvByte(I2C_MCS_ACK | I2C_MCS_START | I2C_MCS_RUN);

      while(--length > 1) // pre-decrement to handle the first transmission
      {
        buffer[i++] = I2C_RecvByte(I2C_MCS_ACK | I2C_MCS_RUN);
      }
      
      buffer[i] = I2C_RecvByte(I2C_MCS_STOP | I2C_MCS_RUN);
    }
    retryCounter = retryCounter + 1;
  }                                         
  while(((I2C3_MCS_R&(I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0) && (retryCounter <= MAXRETRIES));
  // repeat if error
  
  return I2C3_MCS_R&(I2C_MCS_ADRACK|I2C_MCS_ERROR); // return I2C errors
}}

// x is unused
#define I2C_CheckSendError(x) \
  if((I2C3_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0)   \
  {                                                                     \
    I2C3_MCS_R = I2C_MCS_STOP;                                           \
    return (I2C3_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));  \
  }

static inline void I2C_SendByte(uint8_t data, uint32_t flags)
{
    I2C3_MCS_R = flags;
    I2C3_MDR_R = data;
    while(I2C3_MCS_R&I2C_MCS_BUSY){};   // wait for transmission done
}

int I2C_Send(int8_t slave, int length, const uint8_t * buffer)
{
  int retryCounter = 1;
  
  int i = 0;

  // sendoff the slave address
  while(I2C3_MCS_R&I2C_MCS_BUSY){}; // wait for I2C ready
  I2C3_MSA_R = (slave<<1)&0xFE;     // MSA[7:1] is slave address
  I2C3_MSA_R &= ~0x01;              // MSA[0] is 0 for send
  
  if(length == 1)
  {
    I2C_SendByte(buffer[i], I2C_MCS_STOP | I2C_MCS_START | I2C_MCS_RUN );
    I2C_CheckSendError();
  }
  else
  {
    I2C_SendByte(buffer[i], I2C_MCS_START | I2C_MCS_RUN );
    I2C_CheckSendError();
    i++;

    while(--length > 1) // pre-decrement to handle the first transmission
    {
      I2C_SendByte(buffer[i], I2C_MCS_START | I2C_MCS_RUN );
      I2C_CheckSendError();
      i++;
    }
    
    I2C_SendByte(buffer[i], I2C_MCS_STOP | I2C_MCS_RUN );
    I2C_CheckSendError();
  }
}

#undef I2C_CheckSendError