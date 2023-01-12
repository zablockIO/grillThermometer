//Thermocouple.c

#include <stdint.h>
#include "thermocouple.h"

const float theoLow = 6;
const float theoHigh = 94;
const float actualLow = -6;
const float actualHigh = 92;
float slope;
float b;

int16_t tcCountsToDegC(int16_t counts) {
  int16_t i = 0;
  int16_t tempC;
  
  while ((i < 320) && (counts > tcLUT[i][1])) {
    i++;
  }
  // cels counts
  if (i != 320) {
    tempC = tcLUT[i][0];
  }
  
  else {
    tempC = 32767;
  }
  
  return tempC;
}

float calibrationEqn(int16_t temperature) {

float calibratedTemp;
  
  slope = (theoLow - theoHigh) / (actualLow - actualHigh);
  b = theoLow - slope*actualLow;

  calibratedTemp = (float)slope*temperature + b;
  
  return calibratedTemp;
}