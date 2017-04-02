#ifndef __SWS_H__
#define __SWS_H__

#include <stdint.h>

/**
 * @file    sws.h
 * @author  Brandon Nguyen
 * @brief   This file contains prototypes and types for the Simple Windowing System
 */

typedef struct SWS_Window_str SWS_Window;
typedef struct SWS_str SWS;

/**
 * @struct  SWS_Window
 * @brief   Data structure for the window
 */
struct SWS_Window_str
{
  // Public variables
  unsigned int x;
  unsigned int y;
  unsigned int w;
	unsigned int h;

  // Private variables: do not mess with these
  // drawing callback
  void (* drawCallback)(const SWS_Window *, void *);
  void * callbackParam;
  int visible;
  int fresh;    // if it's not visible anymore, looses freshness

  // owner of this window
  SWS * owner;

  // linked list for layered drawing
  SWS_Window * prev;
  SWS_Window * next;
};


/**
 * @struct  SWS
 * @brief   Data structure for the SWS window manager
 */
struct SWS_str
{
  SWS_Window * rootWindow;
  SWS_Window * lastWindow;
};

/**
 * @enum SWS_Error
 */
typedef enum SWS_Error_enum SWS_Error;
enum SWS_Error_enum
{
  SWS_NoError = 0,
  SWS_Fail
};


/**
 * @fn    SWS_Ctor
 * @brief Construct an instance of the Simple Windowing System
 * @param[in] sws Pointer to space that an fit a SWS
 */
void SWS_Ctor( SWS * sws );

/**
 * @fn    SWS_Dtor
 * @brief Destroy an instance of the Simple Windowing System
 * @param[in] sws Pointer to a SWS
 */
void SWS_Dtor( SWS * sws );

/**
 * @fn    SWS_Draw
 * @brief Have the SWS draw the windows
 * @param[in] sws Pointer to a SWS of which to draw
 */
void SWS_Draw( SWS * sws );

/**
 * @fn    SWS_AddWindow
 * @brief Add a SWS_Window to be part of the drawing list
 * @param[in] sws Pointer to SWS to which to add the SWS_Window
 * @param[in] win Pointer to SWS_Window to add
 */
void SWS_AddWindow( SWS * sws, SWS_Window * win );

/**
 * @fn    SWS_RemoveWindow
 * @brief Remove a SWS_Window from the drawing list
 * @param[in] sws Pointer to SWS from which to remove the SWS_Window
 * @param[in] win Pointer to SWS_Window to remove
 */
void SWS_RemoveWindow( SWS * sws, SWS_Window * win );

/**
 * @fn    SWS_Window_Ctor
 * @brief Construct a SWS_Window
 *
 * @param[in] win           Pointer to space that can fit a SWS_Window
 * @param[in] x             Initial x position of the window
 * @param[in] y             Initial y position of the window
 * @param[in] width         Initial width of the window
 * @param[in] height        Initial height of the window
 * @param[in] drawCallback  Pointer to the rendering function
 * @param[in] callbackParam Pointer to the parameters for the rendering function
 */
void SWS_Window_Ctor( SWS_Window * win,
                      unsigned int x,
                      unsigned int y,
                      unsigned int width,
                      unsigned int height,
                      void (* drawCallback)(const SWS_Window *, void *),
                      void * callbackParam );

/**
 * @fn    SWS_Window_Dtor
 * @brief "Destroy" a SWS_Window and remove it from the SWS
 *
 * @param[in] win   Pointer to window destroy
 */
void SWS_Window_Dtor( SWS_Window * win );

/**
 * @fn    SWS_Window_GetVisible
 * @brief Check to see if a SWS_Window is visible
 *
 * @param[in] win   Window context
 * @return          Visibility of the Window
 */
unsigned int SWS_Window_GetVisible( const SWS_Window * win );

/**
 * @fn    SWS_Window_SetVisible
 * @brief Set visibility of a SWS_Window
 *
 * @param[in] win     Window context
 * @param[in] visible Visibility to set to
 */
void SWS_Window_SetVisible( SWS_Window * win, unsigned int visible );

/**
 * @fn    SWS_Window_GetFresh
 * @brief Get the freshness of a SWS_Window
 *
 * @param[in] win     Window context
 * @return            Freshness of a SWS_Window
 *
 * Freshness is a useful parameter incase one wants to have
 * more efficient draws, erasing and redrawing.
 * This is 0 if it's not fresh, necessitating a full redraw
 */
unsigned int SWS_Window_GetFresh( const SWS_Window * win );

// Window drawing functions
/**
 * @fn        SWS_Window_DrawPixel
 * @brief     This will fill in a pixel at this spot
 * @param[in] win   Window context
 * @param[in] x     The x coordinate (0 is the left)
 * @param[in] y     The y coordinate (0 is the top)
 * @param[in] color The color of the pixel (generically sized at 32 bits)
 */
void SWS_Window_DrawPixel( const SWS_Window * win,
                           unsigned int x,
                           unsigned int y,
                           uint32_t color );

/**
 * @fn        SWS_Window_DrawLine
 * @brief     This will draw a line between two coordinates
 * @param[in] win   Window context
 * @param[in] x0    The x coordinate paired with y0
 * @param[in] y0    The y coordinate paired with x0
 * @param[in] x1    The x coordinate paired with y1
 * @param[in] y1    The y coordinate paired with x1
 * @param[in] color The color of the line (generically sized at 32 bits)
 */
void SWS_Window_DrawLine( const SWS_Window * win,
                          unsigned int x0, unsigned int y0,
                          unsigned int x1, unsigned int y1,
                          uint32_t color );

/**
 * @fn        SWS_Window_DrawHLine
 * @brief     This will draw a horizontal line between two X coordinates
 * @param[in] win   Window context
 * @param[in] x0    The x coordinate of one end
 * @param[in] y0    The y coordinate of the line
 * @param[in] x1    The x coordinate of the other end
 * @param[in] color The color of the line (generically sized at 32 bits)
 */
void SWS_Window_DrawHLine( const SWS_Window * win,
                           unsigned int x0,
                           unsigned int y0,
                           unsigned int x1,
                           uint32_t color );

/**
 * @fn        SWS_Window_DrawVLine
 * @brief     This will draw a vertical line between two X coordinates
 * @param[in] win   Window context
 * @param[in] x0    The x coordinate of the line
 * @param[in] y0    The y coordinate of one end
 * @param[in] y1    The y coordinate of the other end
 * @param[in] color The color of the line (generically sized at 32 bits)
 */
void SWS_Window_DrawVLine( const SWS_Window * win,
                           unsigned int x0,
                           unsigned int y0,
                           unsigned int y1,
                           uint32_t color );

/**
 * @fn        SWS_Window_DrawCircle
 * @brief     This will draw a circle from the origin to the radius
 * @param[in] win   Window context
 * @param[in] x0    The x coordinate of the origin
 * @param[in] y0    The y coordinate of the origin
 * @param[in] r     The radius
 * @param[in] color The color of the circle
 */
void SWS_Window_DrawCircle( const SWS_Window * win,
                            unsigned int x0,
                            unsigned int y0,
                            unsigned int r,
                            uint32_t color );

/**
 * @fn        SWS_Window_FillRect
 * @brief     This will draw and fill a rectangle from one point
 * @param[in] win   Window context
 * @param[in] x     The x coordinate of the top left corner
 * @param[in] y     The y coordinate of the top left corner
 * @param[in] w     The width
 * @param[in] h     The height
 * @param[in] color The color of the rectangle (generically sized at 32 bits)
 */
void SWS_Window_FillRect( const SWS_Window * win,
                          unsigned int x0, unsigned int y0,
                          unsigned int w , unsigned int h ,
                          uint32_t color );

/**
 * @fn        SWS_Window_FillRectCoords
 * @brief     This will draw and fill a rectangle between two points
 * @param[in] win   Window context
 * @param[in] x0    The x coordinate of the top left corner
 * @param[in] y0    The y coordinate of the top left corner
 * @param[in] x1    The x coordinate of the bottom right corner
 * @param[in] y1    The y coordinate of the bottom right corner
 * @param[in] color The color of the rectangle (generically sized at 32 bits)
 */
void SWS_Window_FillRectCoords( const SWS_Window * win,
                                unsigned int x0, unsigned int y0,
                                unsigned int x1, unsigned int y1,
                                uint32_t color );

/**
 * @fn        SWS_Window_GetCellWidth
 * @brief     Gets the width of a character cell (including padding)
 */
unsigned int SWS_Window_GetCellWidth(void);

/**
 * @fn        SWS_Window_GetCellHeight
 * @brief     Gets the height of a character cell (including padding)
 */
unsigned int SWS_Window_GetCellHeight(void);

/**
 * @fn        SWS_Window_DrawChar
 * @brief     This will draw a character in a given color
 * @param[in] win   Window context
 * @param[in] x     x position to draw at
 * @param[in] y     y position to draw at
 * @param[in] c     Character to draw
 * @param[in] fg    Foreground (text) color
 * @param[in] bg    Background color
 * @param[in] size  Size of character
 *
 * If fg = bg, then no background color is drawn.
 * Will only draw if the entire character can fit
 * on the display
 */
void SWS_Window_DrawChar( const SWS_Window * win,
                          unsigned int x,
                          unsigned int y,
                          char c,
                          uint32_t fg,
                          uint32_t bg,
                          unsigned int size );

/**
 * @fn        SWS_Window_DrawChar
 * @brief     This will draw a character in a given color
 * @param[in] win   Window context
 * @param[in] x     x position to draw at
 * @param[in] y     y position to draw at
 * @param[in] map   Character bitmap to draw
 * @param[in] fg    Foreground (text) color
 * @param[in] bg    Background color
 * @param[in] size  Size of character
 *
 * If fg = bg, then no background color is drawn.
 * Will only draw if the entire character can fit
 * on the display
 */
void SWS_Window_DrawCharMap( const SWS_Window * win,
                             unsigned int x,
                             unsigned int y,
                             const char * map,
                             uint32_t fg,
                             uint32_t bg,
                             unsigned int size );

/**
 * @fn        SWS_Window_DrawChar
 * @brief     This will draw a string in a given color
 * @param[in] win   Window context
 * @param[in] x     x position to draw at
 * @param[in] y     y position to draw at
 * @param[in] str   String to draw
 * @param[in] fg    Foreground (text) color
 * @param[in] bg    Background color
 * @param[in] size  Size of character
 *
 * If fg = bg, then no background color is drawn
 * Will only draw if where it can fit the entire
 * character onto the display
 */
void SWS_Window_DrawString( const SWS_Window * win,
                            unsigned int x,
                            unsigned int y,
                            const char * str,
                            uint32_t fg,
                            uint32_t bg,
                            unsigned int size );
#endif  // __SWS_H__