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
  Queue_ctor_raw(&queue, buffer, ELEM_SIZE(buffer), ARRAY_LEN(buffer));

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

typedef struct TestStruct_str
{
  int real;
  int comp;
  char hash;
}TestStruct;
TEST_F(Queue_Test,TestStruct)
{
  TestStruct buffer[10];
  TestStruct test[] = {
                        {5,6,0}, {2,0,1}, {8,3,2},
                        {9,9,9}, {0,0,0}, {3,9,3},
                        {2,3,6}, {2310,3429,23}, {-1023,32389,-120},
                      };

  Queue queue;
  Queue_ctor_raw(&queue, buffer, ELEM_SIZE(buffer), ARRAY_LEN(buffer));

  for(unsigned int i = 0; i < ARRAY_LEN(test); i++)
  {
    Queue_put(&queue, &test[i]);
  }

  TestStruct get;
  for(unsigned int i = 0; i < ARRAY_LEN(test); i++)
  {
    Queue_get(&queue, &get);
    ASSERT_EQ(test[i].real,get.real) << "i = " << i;
    ASSERT_EQ(test[i].comp,get.comp) << "i = " << i;
    ASSERT_EQ(test[i].hash,get.hash) << "i = " << i;
  }
}

TEST_F(Queue_Test,TestCtorMacro)
{
  int buffer[10];
  int test[] = {0,1,2,3,4,5,6,7,8,9};

  Queue queue;
  Queue_ctor(&queue, buffer);
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

template_QueueT(int);
template_QueueT(char);
TEST_F(Queue_Test,TestIntTyped)
{
  int buffer[10];
  int test[] = {0,1,2,3,4,5,6,7,8,9};

  QueueT(int) queue;
  QueueT_ctor(int,&queue,buffer);
  for(unsigned int i = 0; i < ARRAY_LEN(test); i++)
  {
    QueueT_put_int(&queue,&test[i]);
  }
  int get;
  for(unsigned int i = 0; i < ARRAY_LEN(test); i++)
  {
    QueueT_get_int(&queue,&get);
    ASSERT_EQ(test[i],get) << "i = " << i;
  }
}
