/**
 * @file    ADC.h
 * @author  Brandon Nguyen
 * @brief   Driver for the TM4C ADC
 */

#include <stdint.h>
#include <drivers/tm4c/ADC.h>
#include <tm4c123gh6pm.h>

void ADC_Init(void)
{
  // from valvanoware
  volatile uint32_t delay;                         
  SYSCTL_RCGCADC_R |= 0x00000001; // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= 0x10; // 1) activate clock for Port E
  while((SYSCTL_PRGPIO_R&0x10) == 0){};      // 2) allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x3F;      // 3) make PE0,1,2,3,4,5 input
  GPIO_PORTE_AFSEL_R |= 0x3F;     // 4) enable alternate function on PE4 PE5
  GPIO_PORTE_DEN_R &= ~0x3F;      // 5) disable digital I/O on PE4 PE5
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFF000000;
  GPIO_PORTE_AMSEL_R |= 0x3F;     // 6) enable analog functionality on PE0,1,2,3,4,5
  ADC0_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority

  ADC0_ACTSS_R &= ~0x0001;        // 10) disable sample sequencer 1
  ADC0_EMUX_R &= ~0x000F;         // 11) seq0 is software trigger
  ADC0_SSMUX0_R = 0x93210;        // 12) set channels for SS1
  ADC0_SSCTL0_R = 0x2<<21;        // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC0_IM_R &= ~0x0001;           // 14) disable SS0 interrupts
  ADC0_SAC_R = 5;                 // 32x oversampling
  ADC0_ACTSS_R |= 0x0001;         // 15) enable sample sequencer 0
}

void ADC_Read(ADC_Data * data)
{
  ADC0_PSSI_R = 0x0001;            // 1) initiate SS1
  while((ADC0_RIS_R&0x01)==0){};   // 2) wait for conversion done for 0123
  data->values[3] = ADC0_SSFIFO0_R&0xFFF; 
  data->values[2] = ADC0_SSFIFO0_R&0xFFF; 
  data->values[1] = ADC0_SSFIFO0_R&0xFFF; 
  data->values[0] = ADC0_SSFIFO0_R&0xFFF; 
  data->values[4] = ADC0_SSFIFO0_R&0xFFF;
  data->values[5] = ADC0_SSFIFO0_R&0xFFF;
  ADC0_ISC_R = 0x0001;             // 4) acknowledge completion
  
  data->flag = 1;
}

void ADC_BeginRead(ADC_Data * data)
{
  ADC0_PSSI_R = 0x0001;            // 1) initiate SS1
  data->flag = 0;
}

int ADC_FinishRead(ADC_Data * data)
{
  if((ADC0_RIS_R&0x01)==0)
  {
    return -1;
  }
  data->values[3] = ADC0_SSFIFO0_R&0xFFF; 
  data->values[2] = ADC0_SSFIFO0_R&0xFFF; 
  data->values[1] = ADC0_SSFIFO0_R&0xFFF; 
  data->values[0] = ADC0_SSFIFO0_R&0xFFF; 
  data->values[4] = ADC0_SSFIFO0_R&0xFFF;
  data->values[5] = ADC0_SSFIFO0_R&0xFFF;
  ADC0_ISC_R = 0x0001;             // 4) acknowledge completion
  data->flag = 1;
  return 0;
}