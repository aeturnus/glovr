#ifndef __FILTER_H__
#define __FILTER_H__
/**
 * @file    filters.h
 * @author  Brandon Nguyen
 * @brief   Digital filters
 */
#include <fixed/types.h>

/**
 * @struct  CyclicBuffer
 * @brief   A cyclic buffer for shifting values
 */
typedef struct CyclicBuffer_str
{
  fixed16_16 * buffer;
  int baseI;
  int size;
} CyclicBuffer;

/**
 * @struct  FIR
 * @brief   FIR. Hope that you like your group delay.
 */
typedef struct FIR_str
{
  int order;
  const fixed16_16 * b;
  CyclicBuffer x;
  fixed16_16   y;
} FIR;

/**
 * @struct  DF1
 * @brief   Direct Form 1 filter. Poor choice.
 */
typedef struct DF1_str
{
  int order;
  const fixed16_16 * b;
  const fixed16_16 * a;
  CyclicBuffer x;
  CyclicBuffer y;
} DF1;

/**
 * @struct  DF2
 * @brief   Direct Form 2 filter. Arguably better choice.
 */
typedef struct DF1_str
{
  int order;
  const fixed16_16 * b;
  const fixed16_16 * a;
  CyclicBuffer v;
  fixed16_16 y;
  int n;
} DF2;

/**
 * @struct  Biquad
 * @brief   1337 tier 2nd order filter.
 */
typedef struct Biquad_str
{
  const fixed16_16 * b; // should be 3
  const fixed16_16 * a; // should be 3
  fixed16_16 g;         // gain
  fixed16_16 x[3];
  int x_base;
  fixed16_16 y[3];
  int y_base;
} Biquad;
typedef Biquad SOS; // SOS == Biquad

/**
 * @struct  CSOS
 * @brief   Cascaded Second Order Sections/Cascaded Biquads.
 *          <insert expanding mind meme here>
 */
typedef struct CSOS_str
{
  int order;
  const fixed16_16 ** b;
  const fixed16_16 ** a;
  const fixed16_16 *  g;
  CyclicBuffer x;
  CyclicBuffer y;
} CSOS;

/**
 * @fn    FIR_ctor
 * @brief Construct the FIR filter
 * @param filter    The pointer to allocated space for the filter
 * @param order     What order the filter.
 * @param b         The feedback coefficients.    Length: order+1
 * @param x_buffer  Buffer for input values.      Length: order+1
 */
void FIR_ctor( FIR * filter, int order,
               const fixed16_16 * b fixed16_16 * x_buffer );
/**
 * @fn    FIR_input
 * @brief Pass an input to the filter
 * @param filter  Pointer to the filter
 * @param input   Input value to pass in
 */
void FIR_input( FIR * filter, fixed16_16 input );

/**
 * @fn    FIR_input
 * @brief Pass an input to the filter
 * @param filter  Pointer to the filter
 * @return        Current output of the filter
 */
fixed16_16 FIR_output( const FIR * filter );



/**
 * @fn    DF1_ctor
 * @brief Construct the DF1 filter
 * @param filter    The pointer to allocated space for the filter
 * @param order     What order the filter.
 * @param b         The feedback coefficients.    Length: order+1
 * @param a         The feedforward coefficients. Length: order+1
 * @param x_buffer  Buffer for input values.      Length: order+1
 * @param y_buffer  Buffer for output values.     Length: order+1
 */
void DF1_ctor( DF1 * filter, int order,
               const fixed16_16 * b, const fixed16_16 * a,
               fixed16_16 * x_buffer, fixed16_16 * y_buffer );
/**
 * @fn    DF1_input
 * @brief Pass an input to the filter
 * @param filter  Pointer to the filter
 * @param input   Input value to pass in
 */
void DF1_input( DF1 * filter, fixed16_16 input );

/**
 * @fn    DF1_input
 * @brief Pass an input to the filter
 * @param filter  Pointer to the filter
 * @return        Current output of the filter
 */
fixed16_16 DF1_output( const DF1 * filter );

/**
 * @fn    DF2_ctor
 * @brief Construct the DF2 filter
 * @param filter    The pointer to allocated space for the filter
 * @param order     What order the filter.
 * @param b         The feedback coefficients.      Length: order+1
 * @param a         The feedforward coefficients.   Length: order+1
 * @param v_buffer  Buffer for intermediate values. Length: order+1
 */
void DF2_ctor( DF2 * filter, int order,
               const fixed16_16 * b, const fixed16_16 * a,
               fixed16_16 * v_buffer);
/**
 * @fn    DF2_input
 * @brief Pass an input to the filter
 * @param filter  Pointer to the filter
 * @param input   Input value to pass in
 */
void DF2_input( DF2 * filter, fixed16_16 input );

/**
 * @fn    DF2_input
 * @brief Pass an input to the filter
 * @param filter  Pointer to the filter
 * @return        Current output of the filter
 */
fixed16_16 DF2_output( const DF2 * filter );

/**
 * @fn    Biquad_ctor
 * @brief Construct the Biquad filter
 * @param filter    The pointer to allocated space for the filter
 * @param b         The feedback coefficients.      Length: 3
 * @param a         The feedforward coefficients.   Length: 3
 */
void Biquad_ctor( Biquad * filter, const fixed16_16 * b, const fixed16_16 * a );

/**
 * @fn    Biquad_input
 * @brief Pass an input to the filter
 * @param filter  Pointer to the filter
 * @param input   Input value to pass in
 */
void Biquad_input( Biquad * filter, fixed16_16 input );

/**
 * @fn    Biquad_input
 * @brief Pass an input to the filter
 * @param filter  Pointer to the filter
 * @return        Current output of the filter
 */
fixed16_16 Biquad_output( const Biquad * filter );

#endif//__FILTER_H__
