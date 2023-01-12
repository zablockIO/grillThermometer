// Thermistor.h


typedef enum tenths_ {
  IS_IN_TENTHS,
  ISNT_IN_TENTHS
} tenths_units;



// prototypes

int16_t ThermCounts2DegC(uint16_t counts);
int16_t C2F(int16_t tempc, tenths_units is_tenths);
