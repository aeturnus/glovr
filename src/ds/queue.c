#include <ds/queue.h>

void Queue_ctor( Queue * queue, void * buffer, int element_size, int buffer_size )
{
  queue->buffer = (uint8_t *) buffer;
  queue->element_size = element_size;
  queue->buffer_size = buffer_size;
  queue->status = QueueEmpty;
  queue->enq_i = 0;
  queue->deq_i = 0;
}

//#define INLINE_TRANSFER 1
#ifndef INLINE_TRANSFER
static inline void transfer(uint8_t * write_head, const uint8_t * read_head, int element_size)
{
  unsigned int i = element_size;
  for(; i > sizeof(uint32_t)-1; i -= sizeof(uint32_t))
  {
    *(uint32_t *)write_head = *(const uint32_t *)read_head;
    write_head += sizeof(uint32_t);
    read_head += sizeof(uint32_t);
    //(uint32_t *)write_head = ((uint32_t *)write_head) + 1;
    //(const uint32_t *)read_head = ((const uint32_t *)read_head) + 1;
  }
  for(; i > 0; --i)
  {
    *write_head = *read_head;
    ++write_head;
    ++read_head;
  }
}
#else
#define transfer(write_head,read_head,element_size) \
  unsigned int i = element_size;\
  for(; i > sizeof(uint32_t)-1; i -= sizeof(uint32_t))\
  {\
    *(uint32_t *)write_head = *(const uint32_t *)read_head;\
    write_head += sizeof(uint32_t);\
    read_head += sizeof(uint32_t);\
  }\
  for(; i > 0; --i)\
  {\
    *write_head = *read_head;\
    ++write_head;\
    ++read_head;\
  }
#endif

QueueStatus Queue_put( Queue * queue, const void * data )
{
  if(queue->status == QueueFull)
  {
    return QueueFull;
  }
  // copy bytes from cast_data into the buffer
  const uint8_t * read_head = (const uint8_t *) data;
  uint8_t * write_head = &(queue->buffer[queue->enq_i * queue->element_size]);

  transfer(write_head,read_head,queue->element_size);

  queue->enq_i = (queue->enq_i+1)%queue->buffer_size;
  if(queue->enq_i == queue->deq_i)
  {
    queue->status = QueueFull;
  }
  else
  {
    queue->status = QueueOk;
  }
  return QueueOk;
}

QueueStatus Queue_get( Queue * queue, void * data )
{
  if(queue->status == QueueEmpty)
  {
    return QueueEmpty;
  }
  // copy bytes from buffer into cast_data
  const uint8_t * read_head = &(queue->buffer[queue->deq_i * queue->element_size]);
  uint8_t * write_head = (uint8_t *) data;

  transfer(write_head,read_head,queue->element_size);

  queue->deq_i = (queue->deq_i+1)%queue->buffer_size;
  if(queue->deq_i == queue->enq_i)
  {
    queue->status = QueueEmpty;
  }
  else
  {
    queue->status = QueueOk;
  }
  return QueueOk;
}

#ifdef INLINE_TRANSFER
#undef transfer
#undef INLINE_TRANSFER
#endif


void Queue8_ctor( Queue8 * queue, uint8_t * buffer, int buffer_size )
{
  queue->buffer = (uint8_t *) buffer;
  queue->buffer_size = buffer_size;
  queue->status = QueueEmpty;
  queue->enq_i = 0;
  queue->deq_i = 0;
}

QueueStatus Queue8_put( Queue8 * queue, const uint8_t * data )
{
  if(queue->status == QueueFull)
  {
    return QueueFull;
  }
  // copy bytes from cast_data into the buffer
  queue->buffer[queue->enq_i] = *data;
  queue->enq_i = (queue->enq_i+1)%queue->buffer_size;
  if(queue->enq_i == queue->deq_i)
  {
    queue->status = QueueFull;
  }
  else
  {
    queue->status = QueueOk;
  }
  return QueueOk;
}

QueueStatus Queue8_get( Queue8 * queue, uint8_t * data )
{
  if(queue->status == QueueEmpty)
  {
    return QueueEmpty;
  }
  // copy bytes from buffer into cast_data
  *data = queue->buffer[queue->deq_i];
  queue->deq_i = (queue->deq_i+1)%queue->buffer_size;
  if(queue->deq_i == queue->enq_i)
  {
    queue->status = QueueEmpty;
  }
  else
  {
    queue->status = QueueOk;
  }
  return QueueOk;
}
