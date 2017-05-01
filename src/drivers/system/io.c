#include <stdio.h>

#include <drivers/tm4c/UART.h>

int fputc(int character, FILE * stream)
{
  UART1_OutChar(character);
  return 1;
}

int fgetc(FILE * stream)
{
  char c = UART1_InChar();
  //UART1_OutChar(c);
  return c;
}

int ferror(FILE * stream)
{
  return 0;
}