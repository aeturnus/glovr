#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <stdint.h>
/**
 * @file    queue.h
 * @author  Brandon Nguyen
 * @brief   Header for queue data structure
 */

typedef enum QueueStatus_enum
{
  QueueOk=0,
  QueueEmpty,
  QueueFull
} QueueStatus;

typedef struct Queue_str
{
  uint8_t * buffer;
  int element_size;
  int buffer_size;

  QueueStatus status;
  int enq_i;
  int deq_i;
} Queue;


/**
 * @fn  Queue_ctor
 * @brief Constructs the queue struct
 *
 * @param queue         Pointer to an allocated Queue
 * @param buffer        The buffer of data to hold the queue: this should be of the
 *                      data-type of your choosing
 * @param element_size  The sizeof() of each data elements
 * @param buffer_size   The how many elements can be stored in the buffer
 */
void Queue_ctor( Queue * queue, void * buffer, int element_size, int buffer_size );

/**
 * @fn Queue_put
 * @brief Adds an element to the queue
 *
 * @param queue Pointer to the queue
 * @param data  The pointer to the data to write
 * @return  QueueOk if successfully queued
 */
QueueStatus Queue_put( Queue * queue, const void * data );

/**
 * @fn Queue_get
 * @brief Removes an element to the queue
 *
 * @param queue Pointer to the queue
 * @param data  The pointer to the data to be written to
 * @return  Status of operation
 */
QueueStatus Queue_get( Queue * queue, void * data );


typedef struct Queue8_str
{
  uint8_t * buffer;
  int buffer_size;

  QueueStatus status;
  int enq_i;
  int deq_i;
} Queue8;

typedef struct Queue16_str
{
  uint16_t * buffer;
  int buffer_size;

  QueueStatus status;
  int enq_i;
  int deq_i;
} Queue16;

typedef struct Queue32_str
{
  uint32_t * buffer;
  int buffer_size;

  QueueStatus status;
  int enq_i;
  int deq_i;
} Queue32;



void Queue8_ctor( Queue8 * queue, uint8_t * buffer, int buffer_size );
QueueStatus Queue8_put( Queue8 * queue, const uint8_t * data );
QueueStatus Queue8_get( Queue8 * queue, uint8_t * data );

#endif//__QUEUE_H__
