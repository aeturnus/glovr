#include <gtest/gtest.h>
#include <ds/queue.h>

class Queue_Test : public :: testing :: Test
{
protected:
  Queue queue;

  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }
};

#define ELEM_SIZE(x) (sizeof(x[0]))
#define ARRAY_LEN(x) (sizeof(x)/ELEM_SIZE(x))
TEST_F(Queue_Test,TestInt)
{
  int buffer[10];
  int test[] = {0,1,2,3,4,5,6,7,8,9};

  Queue queue;
  Queue_ctor(&queue, buffer, ELEM_SIZE(buffer), ARRAY_LEN(buffer));

  for(unsigned int i = 0; i < ARRAY_LEN(test); i++)
  {
    Queue_put(&queue, &test[i]);
  }

  int get;
  for(unsigned int i = 0; i < ARRAY_LEN(test); i++)
  {
    Queue_get(&queue, &get);
    ASSERT_EQ(test[i],get) << "i = " << i;
  }
}
