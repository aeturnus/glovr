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

QueueStatus Queue_put( Queue * queue, const void * data )
{
  if(queue->status == QueueFull)
  {
    return QueueFull;
  }
  // copy bytes from cast_data into the buffer
  const uint8_t * read_head = (const uint8_t *) data;
  uint8_t * write_head = &(queue->buffer[queue->enq_i]);
  for(int i = queue->element_size; i > 0; --i)
  {
    *write_head = *read_head;
    ++write_head;
    ++read_head;
  }

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
  const uint8_t * read_head = &(queue->buffer[queue->deq_i]);
  uint8_t * write_head = (uint8_t *) data;
  for(int i = queue->element_size; i > 0; --i)
  {
    *write_head = *read_head;
    ++write_head;
    ++read_head;
  }

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
