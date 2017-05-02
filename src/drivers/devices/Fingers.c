#include <stdint.h>
#include <drivers/devices/Fingers.h>
#include <drivers/system/time.h>
#include <drivers/tm4c/ADC.h>

typedef struct Interp_str
{
  int16_t max;
  int16_t min;
  // perform interpolation:
  // (100-0)/(max-min) * adc + offset
  // calculate these
  int32_t num;
  int32_t den;
  int32_t offset;
} Interp;

static void calcInterp(Interp * interp, int16_t max, int16_t min)
{
  interp->max = max;
  interp->min = min;
  interp->num = 100;
  interp->den = max-min;
  interp->offset = -interp->num * min / interp->den;
}

static int32_t performInterp(Interp * interp, int16_t value)
{
  int32_t output = interp->num * value / interp->den + interp->offset;
  return output;
}

static int8_t getFingerVal(Interp * interp, int16_t adc)
{
  int32_t output = performInterp(interp, adc);
  if(output > 99)
    output = 99;
  if(output < 0)
    output = 0;
  return output & 0xFF;
}

static Interp extendInterp[NUM_EXTEND];
static Interp abductInterp[NUM_ABDUCT];

void Fingers_Init(void)
{
  ADC_Init();
}


void Fingers_TareExtend(void)
{
  ADC_Data data;
  ADC_Read(&data);
  for(int i = 0; i < NUM_EXTEND; i++)
  {
    int16_t max = extendInterp[i].max;
    int16_t min = data.values[i];
    calcInterp(&extendInterp[i], max, min);
  }
}

void Fingers_TareFlex(void)
{
  ADC_Data data;
  ADC_Read(&data);
  for(int i = 0; i < NUM_EXTEND; i++)
  {
    int16_t min = extendInterp[i].min;
    int16_t max = data.values[i];
    calcInterp(&extendInterp[i], max, min);
  }
}

void Fingers_TareAbduct(void)
{
  ADC_Data data;
  ADC_Read(&data);
  for(int i = 0; i < NUM_ABDUCT; i++)
  {
    int16_t max = abductInterp[i].max;
    int16_t min = data.values[i+5];
    calcInterp(&abductInterp[i], max, min);
  }
}

void Fingers_TareAdduct(void)
{
  ADC_Data data;
  ADC_Read(&data);
  for(int i = 0; i < NUM_ABDUCT; i++)
  {
    int16_t min = abductInterp[i].min;
    int16_t max = data.values[i+5];
    calcInterp(&abductInterp[i], max, min);
  }
}

static void calculateReadings(Fingers * readings, ADC_Data * data)
{
  Interp interp;
  calcInterp(&interp,3000,1000);
  for(int i = 0; i < NUM_EXTEND; i++)
  {
    int8_t output = getFingerVal(&interp,data->values[i]);
    readings->extend[i] = output;
  }
  /*
  for(int i = 0; i < NUM_EXTEND; i++)
  {
    int8_t output = getFingerVal(&extendInterp[i],data->values[i]);
    readings->extend[i] = output;
  }
  for(int i = 0; i < NUM_ABDUCT; i++)
  {
    int8_t output = getFingerVal(&abductInterp[i],data->values[i]);
    readings->abduct[i] = output;
  }
  */

  readings->timestamp = Time_GetTime();
}

void Fingers_GetReadings(Fingers * readings)
{
  ADC_Data data;
  ADC_Read(&data);
  calculateReadings(readings, &data);
}


void Fingers_BeginReadings(Fingers * readings, ADC_Data * data)
{
  ADC_BeginRead(data);
}

int  Fingers_FinishReadings(Fingers * readings, ADC_Data * data)
{
  if(ADC_FinishRead(data))
    return -1;
  calculateReadings(readings, data);
  return 0;
}