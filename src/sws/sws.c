#include <stddef.h>

#include <drivers/system/display.h>
#include "sws.h"

void SWS_Ctor( SWS * sws )
{
  sws->rootWindow = NULL;
}

void SWS_Dtor( SWS * sws )
{
  sws->rootWindow = NULL;
}

void SWS_Draw( SWS * sws )
{
  // draw windows in order from back to front layer
  SWS_Window * currWin = sws->rootWindow;
  while(currWin)
  {
    // use each window's draw routine
    if( currWin->visible )
    {
      currWin->drawCallback( currWin, currWin->callbackParam );
      currWin->fresh = 1; // freshly drawn!
    }
    currWin = currWin->next;
  }
}

void SWS_AddWindow( SWS * sws, SWS_Window * win )
{
  if( !sws->rootWindow )
  {
    // make new window root if there are no windows
    sws->rootWindow = win;
    sws->lastWindow = win;
    win->prev = NULL;
    win->next = NULL;

    win->owner = sws;
  }
  else
  {
    // else put it on top
    win->prev = sws->lastWindow;
    win->next = NULL;
    sws->lastWindow->next = win;
    sws->lastWindow = win;

    win->owner = sws;
  }
  win->fresh = 0;
}


void SWS_RemoveWindow( SWS * sws, SWS_Window * win )
{
  // leaving sws for future development when we can support multiple owners
  sws = win->owner; // explicitly handle it
  if( !sws )
  {
    return;
  }

  // link the previous and next windows
  if( win->prev )
  {
    win->prev->next = win->next;
  }
  else
  {
    sws->rootWindow = win->next;
  }

  if( win->next )
  {
    win->next->prev = win->prev;
  }
  else
  {
    sws->lastWindow = win->prev;
  }

  win->owner = NULL;
  win->prev  = NULL;
  win->next  = NULL;
}


void SWS_Window_Ctor( SWS_Window * win,
                      unsigned int x,
                      unsigned int y,
                      unsigned int width,
                      unsigned int height,
                      void (* drawCallback)(const SWS_Window *, void *),
                      void * callbackParam )
{
  win->x = x;
  win->y = y;
  win->w  = width;
  win->h = height;
  win->drawCallback  = drawCallback;
  win->callbackParam = callbackParam;
  win->visible = 1;

  win->owner = NULL;
  win->prev  = NULL;
  win->next  = NULL;
}


void SWS_Window_Dtor( SWS_Window * win )
{
  // nothing to do
  win->owner = NULL;
  win->prev  = NULL;
  win->next  = NULL;
}


unsigned int SWS_Window_GetVisible( const SWS_Window * win )
{
  return win->visible;
}

void SWS_Window_SetVisible( SWS_Window * win, unsigned int visible )
{
  if( visible )
  {
    win->visible = visible;
    // no need to check if it wasn't visible:
    // if it wasn't it would not have been fresh anyway
  }
  else
  {
    win->fresh = 0;
    win->visible = 0;
  }
}

unsigned int SWS_Window_GetFresh( const SWS_Window * win )
{
  return win->fresh;
}

void SWS_Window_DrawPixel( const SWS_Window * win,
                           unsigned int x,
                           unsigned int y,
                           uint32_t color )
{
  Display_DrawPixel( win->x + x, win->y + y, color );
}

void SWS_Window_DrawLine( const SWS_Window * win,
                          unsigned int x0, unsigned int y0,
                          unsigned int x1, unsigned int y1,
                          uint32_t color )
{

  Display_DrawLine( win->x + x0, win->y + y0,
                    win->x + x1, win->y + y1, color );
}

void SWS_Window_DrawHLine( const SWS_Window * win,
                           unsigned int x0,
                           unsigned int y0,
                           unsigned int x1,
                           uint32_t color )
{
  Display_DrawHLine( win->x + x0, win->y + y0, win->x + x1, color );
}


void SWS_Window_DrawVLine( const SWS_Window * win,
                           unsigned int x0,
                           unsigned int y0,
                           unsigned int y1,
                           uint32_t color )
{
  Display_DrawHLine( win->x + x0, win->y + y0, win->y + y1, color );
}

void SWS_Window_DrawCircle( const SWS_Window * win,
                            unsigned int x0,
                            unsigned int y0,
                            unsigned int r,
                            uint32_t color )
{
  Display_DrawCircle( win->x + x0, win->y + y0, r, color );
}


void SWS_Window_FillRect( const SWS_Window * win,
                          unsigned int x0, unsigned int y0,
                          unsigned int w , unsigned int h ,
                          uint32_t color )
{
  Display_FillRect( win->x + x0, win->y + y0, w, h, color );
}

void SWS_Window_FillRectCoords( const SWS_Window * win,
                                unsigned int x0, unsigned int y0,
                                unsigned int x1, unsigned int y1,
                                uint32_t color )
{
  Display_FillRect( win->x + x0, win->y + y0,
                    win->x + x1, win->y + y1,
                    color );
}

unsigned int SWS_Window_GetCellWidth(void)
{
  return Display_GetCellWidth();
}


unsigned int SWS_Window_GetCellHeight(void)
{
  return Display_GetCellHeight();
}

void SWS_Window_DrawChar( const SWS_Window * win,
                          unsigned int x,
                          unsigned int y,
                          char c,
                          uint32_t fg,
                          uint32_t bg,
                          unsigned int size )
{
  Display_DrawChar( win->x + x, win->y + y, c, fg, bg, size );
}

void SWS_Window_DrawCharMap( const SWS_Window * win,
                             unsigned int x,
                             unsigned int y,
                             const char * map,
                             uint32_t fg,
                             uint32_t bg,
                             unsigned int size )
{
  Display_DrawCharMap( win->x + x, win->y + y, map, fg, bg, size );
}


void SWS_Window_DrawString( const SWS_Window * win,
                            unsigned int x,
                            unsigned int y,
                            const char * str,
                            uint32_t fg,
                            uint32_t bg,
                            unsigned int size )
{
  Display_DrawString( win->x + x, win->y + y, str, fg, bg, size );
}