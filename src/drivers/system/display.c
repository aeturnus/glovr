#include <stdint.h>
#include <drivers/devices/ST7735.h>
#include <drivers/system/display.h>


void Display_Init(void)
{
  ST7735_InitR(INITR_REDTAB);
  ST7735_FillRect(0,0,ST7735_TFTWIDTH,ST7735_TFTHEIGHT,0x0000);
}

#define DISPLAY_HEIGHT ST7735_TFTHEIGHT
#define DISPLAY_WIDTH  ST7735_TFTWIDTH

unsigned int Display_GetWidth(void)
{
  return ST7735_TFTWIDTH;
}

unsigned int Display_GetHeight(void)
{
  return ST7735_TFTHEIGHT;
}

void Display_DrawPixel( unsigned int x, unsigned int y, uint32_t color )
{
  ST7735_DrawPixel( x, y, color );
}

void Display_DrawLine( unsigned int x0, unsigned int y0,
                       unsigned int x1, unsigned int y1,
                       uint32_t color )
{
  ST7735_DrawLine(x0, y0, x1, y1, color);
}

void Display_DrawHLine( unsigned int x0,
                        unsigned int y0,
                        unsigned int x1,
                        uint32_t color )
{
  if( x1 >= x0 )
  {
    ST7735_DrawFastHLine( x0, y0, x1-x0, color );
  }
  else
  {
    ST7735_DrawFastHLine( x1, y0, x0-x1, color );
  }
}

void Display_DrawVLine( unsigned int x0,
                        unsigned int y0,
                        unsigned int y1,
                        uint32_t color )
{
  if( y1 >= y0 )
  {
    ST7735_DrawFastVLine( x0, y0, y1-y0, color );
  }
  else
  {
    ST7735_DrawFastVLine( x0, y1, y0-y1, color );
  }
}

void Display_DrawCircle( unsigned int x0,
                         unsigned int y0,
                         unsigned int r,
                         uint32_t color )
{
  unsigned int x = r;
  unsigned int y = 0;
  int err = 0;
  while( x >= y )
  {
    Display_DrawPixel(x0 + x, y0 + y, color);
    Display_DrawPixel(x0 + y, y0 + x, color);
    Display_DrawPixel(x0 - y, y0 + x, color);
    Display_DrawPixel(x0 - x, y0 + y, color);
    Display_DrawPixel(x0 - x, y0 - y, color);
    Display_DrawPixel(x0 - y, y0 - x, color);
    Display_DrawPixel(x0 + y, y0 - x, color);
    Display_DrawPixel(x0 + x, y0 - y, color);
    if( err <= 0 )
    {
      y += 1;
      err += y<<1 + 1;
    }
    if( err > 0 )
    {
      x -= 1;
      err -= x<<1 + 1;
    }
  }
}

void Display_FillRect( unsigned int x0, unsigned int y0,
                       unsigned int w , unsigned int h ,
                       uint32_t color )
{
  ST7735_FillRect( x0, y0, w, h, color );
}

void Display_FillRectCoords( unsigned int x0, unsigned int y0,
                             unsigned int x1, unsigned int y1,
                             uint32_t color )
{
  // normalize coordinates
  if( x0 > x1 )
  {
    unsigned int swapTemp = x1;
    x1 = x0;
    x0 = swapTemp;
  }
  if( y0 > y1 )
  {
    unsigned int swapTemp = y1;
    y1 = y0;
    y0 = swapTemp;
  }
  ST7735_FillRect( x0, y0, x1-x0, y1-y0, color );
}

#define DISPLAY_CELL_WIDTH  6
#define DISPLAY_CELL_HEIGHT 10
unsigned int Display_GetCellWidth(void)
{
  return DISPLAY_CELL_WIDTH;
}

unsigned int Display_GetCellHeight(void)
{
  return DISPLAY_CELL_HEIGHT;
}

void Display_DrawChar( unsigned int x,
                       unsigned int y,
                       char c,
                       uint32_t fg,
                       uint32_t bg,
                       unsigned int size )
{
  ST7735_DrawCharS(x,y,c,fg,bg,size);
  // compensate for gaps
  if( fg != bg )
  {
    ST7735_FillRect(x,y+DISPLAY_CELL_HEIGHT-2,DISPLAY_CELL_WIDTH,2,bg);
  }
}

void Display_DrawCharMap( unsigned int x,
                          unsigned int y,
                          const char * map,
                          uint32_t fg,
                          uint32_t bg,
                          unsigned int size )
{
  ST7735_DrawCharMap(x,y,map,fg,bg,size);
  // compensate for gaps
  if( fg != bg )
  {
    ST7735_FillRect(x,y+DISPLAY_CELL_HEIGHT-2,DISPLAY_CELL_WIDTH,2,bg);
  }
}

void Display_DrawString( unsigned int x,
                         unsigned int y,
                         const char * str,
                         uint32_t fg,
                         uint32_t bg,
                         unsigned int size )
{
  // Have our own custom processing
  unsigned int currentX = x;  // current positions
  unsigned int currentY = y;
  /*
  while(*str)
  {
    Display_DrawChar(currentX,currentY,*str,fg,bg,size);
    currentX += DISPLAY_CELL_WIDTH;
    str++;
  }
  */
  ///*
  // For some reason this doesn't work?
  for( char c = *str;
       c != '\0' && 
        currentY <= size*(DISPLAY_HEIGHT-DISPLAY_CELL_HEIGHT);
       c = *++str )
  {
    // newline detection
    if( c == '\n' )
    {
      currentY += size*DISPLAY_CELL_HEIGHT;  // new row
      currentX = x;                     // reset column
    }
    // skip until we can print on a new line
    else if( currentX > DISPLAY_WIDTH-DISPLAY_CELL_WIDTH*size )
    {
      continue;
    }
    else
    {
      Display_DrawChar(currentX,currentY,c,fg,bg,size);
      currentX += size*DISPLAY_CELL_WIDTH;
    }
  }
  //*/
}

#undef DISPLAY_HEIGHT
#undef DISPLAY_WIDTH

#undef DISPLAY_CELL_WIDTH
#undef DISPLAY_CELL_HEIGHT