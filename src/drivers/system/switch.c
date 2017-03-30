#include <stdint.h>
#include <tm4c123gh6pm.h>
#include <drivers/system/switch.h>
#include <drivers/SysTick.h>

#define NUM_SWITCHES 5

const static uint8_t switchMask = 0x0FF >> (8 - NUM_SWITCHES);

// TODO: Fix the port
void Switch_Init(void) {
  SYSCTL_RCGCGPIO_R |= 0x02;
  while((SYSCTL_PRGPIO_R&0x02) == 0){};
  GPIO_PORTB_DIR_R &= ~switchMask;        // PB0-4 is an input
  GPIO_PORTB_AFSEL_R &= ~switchMask;      // regular port function
  GPIO_PORTB_AMSEL_R &= ~switchMask;      // disable analog on PB0-4
  GPIO_PORTB_PCTL_R &= ~(0x0FFFFFFFF >> ((8 - NUM_SWITCHES) << 2)); // PCTL GPIO on PB0-4
  GPIO_PORTB_DEN_R |= switchMask;         // PB0-4 enabled as a digital port
}

uint8_t Switch_Input(void) {
  return GPIO_PORTB_DATA_R & switchMask;
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
