#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "switch.h"
#include "../driverlib/SysTick.h"

#define NUM_SWITCHES 3

#define switchMask 0xE0

void Switch_Init(void) {
  SYSCTL_RCGCGPIO_R |= 0x04;            // activate Port C
  while((SYSCTL_PRGPIO_R&0x04) == 0){};
  GPIO_PORTC_LOCK_R = GPIO_LOCK_KEY;
  GPIO_PORTC_DIR_R &= ~switchMask;    // PC5-7 is an input
  GPIO_PORTC_AFSEL_R &= ~switchMask;      // regular port function
  GPIO_PORTC_AMSEL_R &= ~switchMask;      // disable analog on PC5-7
  GPIO_PORTC_PCTL_R &= ~(0xFFF00000); // PCTL GPIO on PC5-7
  GPIO_PORTC_DEN_R |= switchMask;         // PC5-7 enabled as a digital port
}

uint8_t Switch_Input(void) {
  return (GPIO_PORTC_DATA_R & switchMask) >> 5;
}

#define DELAY 160

uint8_t Switch_GetDebounce() {
  uint8_t in, old;
  uint32_t time;
  time = 1000;
  old = Switch_Input();
  while(time) {
    //SysTick_Wait(DELAY);
    in = Switch_Input();
    if(in == old) {
      time--;
    } else {
      time = 1000;
      old = in;
    }
  }
  return old;
}

void Switch_GetStatesDebounced(Switch_State *states) {
  uint8_t switchStates = Switch_GetDebounce();
  
  for(uint8_t i = 0; i < NUM_SWITCHES; i++) {
    states[i] = switchStates & (0x01 << i) ? Switch_Off : Switch_On;
  }
}

void Switch_GetStates( Switch_State * states ) {
  uint8_t switchStates = Switch_Input();
  
  for(uint8_t i = 0; i < NUM_SWITCHES; i++) {
    states[i] = (switchStates & (0x01 << i)) ? Switch_Off : Switch_On;
  }
}
