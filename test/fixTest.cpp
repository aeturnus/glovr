#include <gtest/gtest.h>
#include <fixed/types.h>

class Fixed_Test: public :: testing :: Test
{
protected:
  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {
  }
};

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))
TEST_F(Fixed_Test,TestAdd)
{
  int testX[] =   {5, -5, -5};
  int testY[] =   {10,10, -10};
  int result[] =  {15,5,-15};
  fix16_16 x;
  fix16_16 y;
  for(int i = 0; i < ARRAY_LEN(testX); i++)
  {
    x = decTo16_16(testX[i]);
    y = decTo16_16(testY[i]);
    ASSERT_EQ(decTo16_16(result[i]), add16_16(x,y)) << "i = " << i << ", x = " << x << ", y = " << y;
  }
}

TEST_F(Fixed_Test,TestMul)
{
  int testX[] =   {5, -5, -5};
  int testY[] =   {10,10, -10};
  int result[] =  {50,-50,50};
  fix16_16 x;
  fix16_16 y;
  for(int i = 0; i < ARRAY_LEN(testX); i++)
  {
    x = decTo16_16(testX[i]);
    y = decTo16_16(testY[i]);
    ASSERT_EQ(decTo16_16(result[i]), mul16_16(x,y)) << "i = " << i << ", x = " << x << ", y = " << y;
  }
}
