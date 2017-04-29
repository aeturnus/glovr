#include <drivers/tm4c/Wrappers.h>
#include <drivers/tm4c/PLL.h>

void Hardware_Init(void)
{
  PLL_Init(Bus80MHz);
}

void Hardware_Standby(void)
{
  WaitForInterrupt();
}

#endif//__HW_H__
