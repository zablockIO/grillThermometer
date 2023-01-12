// ux_manager.h



#ifndef UX_MGR
#define UX_MGR

// includes
#include <stdint.h>
#include "ssd1306.h"


// typedefs
typedef enum  Screens_{
  HOME,
  MENU,
  BEEF,
  FISH,
  LAMB,
  PORK,
  POULTRY,
  CUSTOM,
  CUT_TYPE,
  DONENESS0,
  DONENESS1,
  CUSTOM_SEL0,
  CUSTOM_SEL1,
  ERROR_SCREEN,
  ERROR_SCREEN2,
//  NUMBER_OF_SCREENS
} ui_screen;


typedef struct DWuint8_t_
{
  char format[10];
  char invalidMsg[4];
  uint16_t xPos;
  uint16_t yPos;
  uint8_t valid;
  uint8_t data;
} DWuint8_t;


typedef struct DWint8_t_
{
  char format[10];
  char invalidMsg[4];
  uint16_t xPos;
  uint16_t yPos;
  uint8_t valid;
  int8_t data;
} DWint8_t;


typedef struct DWuint16_t_
{
  char format[10];
  char invalidMsg[4];
  uint16_t xPos;
  uint16_t yPos;
  uint8_t valid;
  uint16_t data;
} DWuint16_t;


typedef struct DWint16_t_
{
  char format[10];
  char invalidMsg[4];
  uint16_t xPos;
  uint16_t yPos;
  uint8_t valid;
  int16_t data;
} DWint16_t;


typedef struct DWstring_
{
  char format[10];
  char invalidMsg[4];
  uint16_t xPos;
  uint16_t yPos;
  uint8_t valid;
  char data[26];
} DWstring;


typedef struct DWfloat_
{
  char format[10];
  char invalidMsg[4];
  uint16_t xPos;
  uint16_t yPos;
  uint8_t valid;
  float data;
} DWfloat;


// Global variables
// live screen data variables
extern ui_screen screenNumber;
extern uint8_t unitSel;

// display data
extern DWfloat temp;
extern DWstring tempUnit;
extern DWfloat tarTemp;
extern DWfloat customOption1;
extern DWfloat customOption2;
extern DWfloat customOption3;
extern DWfloat customOption4;
// Global Constants


// exposed function prototypes
void SwitchScreens(ui_screen screen_no);
//void PrintTest(char * formater, uint16_t variable, uint8_t position);
//uint8_t ProcessKeyCode (uint16_t key_code);
uint8_t ProcessKeyCodeInContext (uint16_t key_code);
void UpdateScreenValues(void);
uint8_t GetKeycode(void);

#endif