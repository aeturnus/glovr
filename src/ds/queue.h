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
 * @fn  Queue_ctor_raw
 * @brief Constructs the queue struct.
 *
 * @param queue         Pointer to an allocated Queue
 * @param buffer        The buffer of data to hold the queue: this should be of the
 *                      data-type of your choosing
 * @param element_size  The sizeof() of each data elements
 * @param buffer_size   The how many elements can be stored in the buffer
 */
void Queue_ctor_raw( Queue * queue, void * buffer, int element_size, int buffer_size );

/*
 * An abstracted version of Queue_ctor_raw, which uses a macro to get the sizes
 */
#define Queue_ctor(queue, buffer) Queue_ctor_raw(queue,buffer,sizeof(buffer[0]),sizeof(buffer)/sizeof(buffer[0]))

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

// Templated implementation
#define template_QueueT(type)\
  \
  typedef struct QueueT_##type_str QueueT_##type;\
  typedef struct\
  {\
    QueueStatus (* getStatus)(const QueueT_##type *);\
    QueueStatus (* put)(QueueT_##type *, const type *);\
    QueueStatus (* get)(QueueT_##type *, type *);\
  } QueueT_vtable_##type;\
  struct QueueT_##type_str\
  {\
    type * buffer;\
    int buffer_size;\
    QueueStatus status;\
    int enq_i;\
    int deq_i;\
    \
    QueueT_vtable_##type vtable;\
  };\
  QueueStatus QueueT_getStatus_##type( const QueueT_##type * queue )\
  {\
    return queue->status;\
  }\
  QueueStatus QueueT_put_##type( QueueT_##type * queue, const type * data )\
  {\
    if(queue->status == QueueFull)\
    {\
      return QueueFull;\
    }\
    queue->buffer[queue->enq_i] = *data;\
    queue->enq_i = (queue->enq_i+1)%queue->buffer_size;\
    if(queue->enq_i == queue->deq_i)\
    {\
      queue->status = QueueFull;\
    }\
    else\
    {\
      queue->status = QueueOk;\
    }\
    return QueueOk;\
  }\
  QueueStatus QueueT_get_##type( QueueT_##type * queue, type * data )\
  {\
    if(queue->status == QueueEmpty)\
    {\
      return QueueEmpty;\
    }\
    *data = queue->buffer[queue->deq_i];\
    queue->deq_i = (queue->deq_i+1)%queue->buffer_size;\
    if(queue->deq_i == queue->enq_i)\
    {\
      queue->status = QueueEmpty;\
    }\
    else\
    {\
      queue->status = QueueOk;\
    }\
    return QueueOk;\
  }\
  \
  QueueT_vtable_##type QueueT_vtable_##type_table =\
  {\
    &QueueT_getStatus_##type,\
    &QueueT_put_##type,\
    &QueueT_get_##type\
  };\
  \
  void QueueT_ctor_##type( QueueT_##type * queue, type * buffer, int buffer_size )\
  {\
    queue->buffer = buffer;\
    queue->buffer_size = buffer_size;\
    queue->status = QueueEmpty;\
    queue->enq_i = 0;\
    queue->deq_i = 0;\
    queue->vtable = QueueT_vtable_##type_table;\
  }\



#define QueueT(type) QueueT_##type
#define QueueT_ctor(type,queue,buffer)\
  QueueT_ctor_##type(queue,buffer,sizeof(buffer)/sizeof(buffer[0]))

#define QueueT_getStatus(queue,data)\
  (queue)->vtable.getStatus(queue,data)

#define QueueT_put(queue,data)\
  (queue)->vtable.put(queue,data)

#define QueueT_get(queue,data)\
  (queue)->vtable.get(queue,data)

#endif//__QUEUE_H__
