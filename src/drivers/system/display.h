#ifndef __DISPLAY_H__
#define __DISPLAY_H__
/**
 * @file    display.h
 * @author  Brandon Nguyen
 * @brief   Generic interface for displays
 * Provides a simple, low level graphics interface
 */

#include <stdint.h>

/**
 * @fn          Display_Init
 * @brief       Initializes the display
 */
void Display_Init(void);

/**
 * @fn          Display_GetWidth
 * @brief       Gets the width
 * @param[out]  The width
 */
unsigned int Display_GetWidth(void);

/**
 * @fn          Display_GetHeight
 * @brief       Gets the height
 * @param[out]  The height
 */
unsigned int Display_GetHeight(void);

/**
 * @fn        Display_DrawPixel
 * @brief     This will fill in a pixel at this spot
 * @param[in] x     The x coordinate (0 is the left)
 * @param[in] y     The y coordinate (0 is the top)
 * @param[in] color The color of the pixel (generically sized at 32 bits)
 */
void Display_DrawPixel( unsigned int x, unsigned int y, uint32_t color );

/**
 * @fn        Display_DrawLine
 * @brief     This will draw a line between two coordinates
 * @param[in] x0    The x coordinate paired with y0
 * @param[in] y0    The y coordinate paired with x0
 * @param[in] x1    The x coordinate paired with y1
 * @param[in] y1    The y coordinate paired with x1
 * @param[in] color The color of the line (generically sized at 32 bits)
 */
void Display_DrawLine( unsigned int x0, unsigned int y0,
                       unsigned int x1, unsigned int y1,
                       uint32_t color );

/**
 * @fn        Display_DrawHLine
 * @brief     This will draw a horizontal line between two X coordinates
 * @param[in] x0    The x coordinate of one end
 * @param[in] y0    The y coordinate of the line
 * @param[in] x1    The x coordinate of the other end
 * @param[in] color The color of the line (generically sized at 32 bits)
 */
void Display_DrawHLine( unsigned int x0,
                        unsigned int y0,
                        unsigned int x1,
                        uint32_t color );

/**
 * @fn        Display_DrawVLine
 * @brief     This will draw a vertical line between two X coordinates
 * @param[in] x0    The x coordinate of the line
 * @param[in] y0    The y coordinate of one end
 * @param[in] y1    The y coordinate of the other end
 * @param[in] color The color of the line (generically sized at 32 bits)
 */
void Display_DrawVLine( unsigned int x0,
                        unsigned int y0,
                        unsigned int y1,
                        uint32_t color );

/**
 * @fn        Display_DrawCircle
 * @brief     This will draw a circle from the origin to the radius
 * @param[in] x0    The x coordinate of the origin
 * @param[in] y0    The y coordinate of the origin
 * @param[in] r     The radius
 * @param[in] color The color of the circle
 */
void Display_DrawCircle( unsigned int x0,
                         unsigned int y0,
                         unsigned int r,
                         uint32_t color );

/**
 * @fn        Display_FillRect
 * @brief     This will draw and fill a rectangle from one point
 * @param[in] x     The x coordinate of the top left corner
 * @param[in] y     The y coordinate of the top left corner
 * @param[in] w     The width
 * @param[in] h     The height
 * @param[in] color The color of the rectangle (generically sized at 32 bits)
 */
void Display_FillRect( unsigned int x0, unsigned int y0,
                       unsigned int w , unsigned int h ,
                       uint32_t color );

/**
 * @fn        Display_FillRectCoords
 * @brief     This will draw and fill a rectangle between two points
 * @param[in] x0    The x coordinate of the top left corner
 * @param[in] y0    The y coordinate of the top left corner
 * @param[in] x1    The x coordinate of the bottom right corner
 * @param[in] y1    The y coordinate of the bottom right corner
 * @param[in] color The color of the rectangle (generically sized at 32 bits)
 */
void Display_FillRectCoords( unsigned int x0, unsigned int y0,
                             unsigned int x1, unsigned int y1,
                             uint32_t color );

/**
 * @fn        Display_GetCellWidth
 * @brief     Gets the width of a character cell (including padding)
 */
unsigned int Display_GetCellWidth(void);

/**
 * @fn        Display_GetCellHeight
 * @brief     Gets the height of a character cell (including padding)
 */
unsigned int Display_GetCellHeight(void);

/**
 * @fn        Display_DrawChar
 * @brief     This will draw a character in a given color
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
void Display_DrawChar( unsigned int x,
                       unsigned int y,
                       char c,
                       uint32_t fg,
                       uint32_t bg,
                       unsigned int size );

/**
 * @fn        Display_DrawCharMap
 * @brief     This will draw a character in a given color
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
void Display_DrawCharMap( unsigned int x,
                          unsigned int y,
                          const char * map,
                          uint32_t fg,
                          uint32_t bg,
                          unsigned int size );

/**
 * @fn        Display_DrawChar
 * @brief     This will draw a string in a given color
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
void Display_DrawString( unsigned int x,
                         unsigned int y,
                         const char * str,
                         uint32_t fg,
                         uint32_t bg,
                         unsigned int size );

#endif  // __DISPLAY_H__
