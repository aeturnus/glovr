#ifndef __SWITCH_H__
#define __SWITCH_H__
/**
 * @file    switch.h
 * @author  Brandon Nguyen
 * @brief   API for getting hardware input
 */
 
// must be a number from 0 to 8 inclusive
typedef enum Switch_State_enum
{
  Switch_Off,
  Switch_On,
  Switch_DoesNotExist
} Switch_State;

/**
 * @fn    Switch_Init
 * @brief Initializes Switch hardware drivers
 */
void Switch_Init( void );

/**
 * @fn          Switch_GetStatesDebounced
 * @brief       Gets the states of the switches after software debouncing
 * @param[out]  states Buffer of 5 states
 */
void Switch_GetStatesDebounced( Switch_State * states );

/**
 * @fn          Switch_GetStates
 * @brief       Gets the states of the switches
 * @param[out]  states Buffer of 5 states
 */
void Switch_GetStates( Switch_State * states );

#endif//__SWITCH_H__
