#ifndef __WRAPPERS_H__
#define __WRAPPERS_H__

void WaitForInterrupt();
void StartCritical();
void EndCritical();
void DisableInterrupts();
void EnableInterrupts();
void ContextSwitch(uint32_t * csp, uint32_t * nsp);

#endif//__WRAPPERS_H__