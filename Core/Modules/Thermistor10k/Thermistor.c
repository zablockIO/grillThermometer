//Thermistor.c

#include <stdint.h>
#include "thermistor.h"


uint16_t breakpointsX[7] = {3975, 3801, 3133, 1424, 816, 475, 260};
float MandB[2][7] = {
  {0, -0.086207, -0.037425, -0.023406, -0.032895, -0.05571, -0.101523},
  {0, 302.67241, 117.25299, 73.329432, 86.842105, 105.45961, 126.39594}
};


int16_t ThermCounts2DegC(uint16_t counts)
{
  int8_t i;
  uint8_t found = 7;
  int16_t tempC;
  
  for (i=0; i<6; i++) {
    if (counts > breakpointsX[i]) {
      found = i;
      i = 7;
    }
  }
    
  if ( (found != 0) && (found != 7) ) {
    tempC = (int16_t)(MandB[0][found] * counts + MandB[1][found] + 0.5);
  }
  else {
    tempC = 10000; // this indicates an error: outside of table values
  }

  return tempC;
}

int16_t C2F(int16_t tempc, tenths_units is_tenths)
{
  int16_t tempF;
  
  if (tempc > 1000) tempF = 10000;
  else if (is_tenths == IS_IN_TENTHS) tempF = (int16_t) ((tempc*9/5) + 320);
  else tempF = (int16_t) ((tempc*9/5) + 32);
  return tempF;
}
