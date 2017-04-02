#ifndef __HW_H__
#define __HW_H__

/**
 * @fn    Hardware_Init
 * @brief Initializes base systems such as clock frequency
 *        and the 
 */
void Hardware_Init(void);

/**
 * @fn    Hardware_Standby
 * @brief Puts the system in a low power-state until an interrupt occurs
 */
void Hardware_Standby(void);

#endif//__HW_H__
