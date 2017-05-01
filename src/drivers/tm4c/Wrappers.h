#ifndef __WRAPPERS_H__
#define __WRAPPERS_H__
#include <stdint.h>
void WaitForInterrupt(void);
void StartCritical(void);
void EndCritical(void);
void DisableInterrupts(void);
void EnableInterrupts(void);
void ContextSwitch(uint32_t * csp, uint32_t * nsp);

#endif//__WRAPPERS_H__