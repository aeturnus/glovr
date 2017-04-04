#include <stdio.h>

#include <drivers/tm4c/UART.h>

int fputc(int character, FILE * stream)
{
  UART0_OutChar(character);
  return 1;
}

int fgetc(FILE * stream)
{
  char c = UART0_InChar();
  UART0_OutChar(c);
  return c;
}

int ferror(FILE * stream)
{
  return 0;
}