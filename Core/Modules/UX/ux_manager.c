// ux_manager.c


#include "main.h"
#include "ux_manager.h"
#include <stdio.h>
#include "meat_temps.h"
#include "serial_user.h"
#include "ui_bitmaps.h"

// private defines


// Global Constants


// Modular Constants


// global variables
// Screens
ui_screen currentScreen;
ui_screen lastScreen;

// serial flags
extern uint8_t customUnitFlags[4];

// Display-wrapped values
// format seq (numeric): {<format string>, <error message>, <Xpos>, <Ypos>, <valid?>, <init value>}
DWfloat tempInF = {"%4.1f", "----", 24, 16, true, 150};
DWfloat tempInC = {"%4.1f", "----", 24, 16, true, 63};
DWint16_t tempCJ_F = {"%5d", "!!!!", 0, 0, true, 0};
// format seq (string): {<format string>,  <error message>, <Xpos>, <Ypos>, <valid?>, "<init value>"

// Screen titles and selections
DWfloat temp = {"%3.1f", "----", 128, 64, true, 150.0};
DWstring tempUnit = {"%s", "????", 115, 16, true, ""};
DWfloat tarTemp = {"%3.0f", "----", 128, 64, true, 0};
DWstring tarMeat = {"%s", "????", 0, 0, true, ""};
DWstring tarDone = {"%s", "????", 0, 0, true, ""};
DWstring titleCutSel = {"%11s", "????", 0, 0, true, ""};
DWfloat customOption1 = {"%3.1f", "----", 0, 0, true, 0};
DWfloat customOption2 = {"%3.1f", "----", 0, 0, true, 0};
DWfloat customOption3 = {"%3.1f", "----", 0, 0, true, 0};
DWfloat customOption4 = {"%3.1f", "----", 0, 0, true, 0};

// modular variables
char tempUnits[2][1] = {"C", "F"};
char titleChoices[2][11] = { "CUT SELECT", "TYPE SELECT"};
uint8_t meatSelected = 0;
uint8_t unitSel = 0; // 0 defaults to F, 1 for Celsius
uint8_t saveSel;
uint8_t customSel = 0;

// module prototypes
void getCorrectTar(meat_temp MEAT_TEMP);

// ***************
// Start Of Code
// ***************
// Screen switching utility that manages pre-, post-, and screen switch conditions
void SwitchScreens(ui_screen screen_no)
{
  lastScreen = currentScreen;

  
#pragma diag_suppress= Pa149
  // what must be done before current screen is switched out
  switch (lastScreen) {
  }
  
  
  // what must be done before screen is switched in
  switch (screen_no) {
  }
#pragma diag_warning= Pa149
  
  // Switch the screens
  switch (screen_no) {
  case HOME:
    // clear the screen from the previos dispayed data
    SSD1306_Clear();
    // Put up the "persistant" info (like data labels)
    SSD1306_GotoXY (42,0);
    SSD1306_Puts ("HOME", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (15, 53);
    SSD1306_Puts ("1) C/ F 2)Menu", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_DrawCircle(110, 18, 2, SSD1306_COLOR_WHITE);
    SSD1306_DrawCircle(34, 53, 1, SSD1306_COLOR_WHITE);
    SSD1306_DrawCircle(55, 53, 1, SSD1306_COLOR_WHITE);
    tempUnit.xPos = 115;
    tempUnit.yPos = 16;
    if (meatSelected) {
      SSD1306_GotoXY (0, 42);
      SSD1306_Puts ("Target Temp: ", &Font_7x10, SSD1306_COLOR_WHITE);
      getCorrectTar(targetMeat);
      tarTemp.xPos = 92;
      tarTemp.yPos = 42;
      SSD1306_DrawCircle(114, 42, 1, SSD1306_COLOR_WHITE);
      if (unitSel == 0){
        SSD1306_GotoXY (116, 42);
        SSD1306_Puts ("F", &Font_7x10, SSD1306_COLOR_WHITE);
      }
      else {
        SSD1306_GotoXY (116, 42);
        SSD1306_Puts ("C", &Font_7x10, SSD1306_COLOR_WHITE);
      }
    }
    // Set u X/Y coordinates for "live" data to be displayed on this screen
    if (unitSel == 0) {
      temp.xPos = 24;
      temp.yPos = 16;
      tempUnit.data[0] = tempUnits[1][0];
    }
    else {
      temp.xPos = 24;
      temp.yPos = 16;
      tempUnit.data[0] = tempUnits[0][0];
    }
    // Send a screen update (note this does not update the live data)
    //SSD1306_UpdateScreen();
    break;
  case MENU:
    SSD1306_Clear();
    SSD1306_GotoXY (50,0);
    SSD1306_Puts ("MENU", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 20);
    SSD1306_Puts ("Meat Type: ", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 30);
    SSD1306_Puts ("Doneness: ", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 40);
    SSD1306_Puts ("Target Temp: ", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (1, 53);
    SSD1306_Puts ("1)Sel 2)Home 3)Clr", &Font_7x10, SSD1306_COLOR_WHITE);
    if (meatSelected) {
      tarMeat.xPos = 71;
      tarMeat.yPos = 20;
      tarDone.xPos = 64;
      tarDone.yPos = 30;
      tarTemp.xPos = 84;
      tarTemp.yPos = 40;
      SSD1306_DrawCircle(114, 42, 1, SSD1306_COLOR_WHITE);
      if (unitSel == 0){
        SSD1306_GotoXY (116, 40);
        SSD1306_Puts ("F", &Font_7x10, SSD1306_COLOR_WHITE);
      }
      else {
        SSD1306_GotoXY (116, 40);
        SSD1306_Puts ("C", &Font_7x10, SSD1306_COLOR_WHITE);
      }
    }
    else {
      tarTemp.xPos = 128;
      tarTemp.yPos = 64;
    }
    break;
  case BEEF:
    SSD1306_Clear();
    SSD1306_GotoXY (3,0);
    SSD1306_Puts ("MEAT SELECT", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (34, 24);
    SSD1306_Puts ("BEEF", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_DrawBitmap(80, 24, cow, 16, 16, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 43);
    SSD1306_Puts ("1)Set 2)Nxt", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 53);
    SSD1306_Puts ("3)Bck 4)Home", &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case FISH:
    SSD1306_Clear();
    SSD1306_GotoXY (3,0);
    SSD1306_Puts ("MEAT SELECT", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (34, 24);
    SSD1306_Puts ("FISH", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_DrawBitmap(80, 24, fish_bmp, 16, 16, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 43);
    SSD1306_Puts ("1)Set 2)Nxt", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 53);
    SSD1306_Puts ("3)Bck 4)Home", &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case LAMB:
    SSD1306_Clear();
    SSD1306_GotoXY (3,0);
    SSD1306_Puts ("MEAT SELECT", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (34, 24);
    SSD1306_Puts ("LAMB", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_DrawBitmap(80, 24, lamb_bmp, 16, 16, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 43);
    SSD1306_Puts ("1)Set 2)Nxt", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 53);
    SSD1306_Puts ("3)Bck 4)Home", &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case PORK:
    SSD1306_Clear();
    SSD1306_GotoXY (3,0);
    SSD1306_Puts ("MEAT SELECT", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (34, 24);
    SSD1306_Puts ("PORK", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_DrawBitmap(80, 23, pig, 16, 16, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 43);
    SSD1306_Puts ("1)Set 2)Nxt", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 53);
    SSD1306_Puts ("3)Bck 4)Home", &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case POULTRY:
    SSD1306_Clear();
    SSD1306_GotoXY (3,0);
    SSD1306_Puts ("MEAT SELECT", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (17, 24);
    SSD1306_Puts ("POULTRY", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_DrawBitmap(96, 24, turkey, 16, 16, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 43);
    SSD1306_Puts ("1)Set 2)Nxt", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 53);
    SSD1306_Puts ("3)Bck 4)Home", &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case CUSTOM:
    SSD1306_Clear();
    SSD1306_GotoXY (3,0);
    SSD1306_Puts ("MEAT SELECT", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (23, 24);
    SSD1306_Puts ("CUSTOM", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_DrawBitmap(89, 24, wrench, 16, 16, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 43);
    SSD1306_Puts ("1)Set 2)Nxt", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (25, 53);
    SSD1306_Puts ("3)Bck 4)Home", &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case CUT_TYPE:
    saveSel = lastScreen;
    SSD1306_Clear();
 
    if (lastScreen == FISH || lastScreen == POULTRY) {
      for (int i = 0; i < 12; i++) {
        titleCutSel.data[i] = titleChoices[1][i];
      }
    }
    else {
      for (int i = 0; i < 12; i++) {
        titleCutSel.data[i] = titleChoices[0][i];
      }
    }
    
    switch (lastScreen) {
      case BEEF:
        SSD1306_GotoXY (0, 23);
        SSD1306_Puts ("1) Roasts & Steaks", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 33);
        SSD1306_Puts ("2) Ground", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 43);
        SSD1306_Puts ("3) Back", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 53);
        SSD1306_Puts ("4) Home", &Font_7x10, SSD1306_COLOR_WHITE);
        break;
      case FISH:
        SSD1306_GotoXY (0, 23);
        SSD1306_Puts ("1) Tuna & Swrdfsh", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 33);
        SSD1306_Puts ("2) Other Fish", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 43);
        SSD1306_Puts ("3) Back", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 53);
        SSD1306_Puts ("4) Home", &Font_7x10, SSD1306_COLOR_WHITE);
        break;
      case LAMB:
        SSD1306_GotoXY (0, 23);
        SSD1306_Puts ("1) Roasts & Steaks", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 33);
        SSD1306_Puts ("2) Chops", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 43);
        SSD1306_Puts ("3) Back", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 53);
        SSD1306_Puts ("4) Home", &Font_7x10, SSD1306_COLOR_WHITE);
        break;
      case PORK:
        SSD1306_GotoXY (0, 23);
        SSD1306_Puts ("1) Roasts & Chops", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 33);
        SSD1306_Puts ("2) Ribs & All else", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 43);
        SSD1306_Puts ("3) Back", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 53);
        SSD1306_Puts ("4) Home", &Font_7x10, SSD1306_COLOR_WHITE);
        break;
      case POULTRY:
        SSD1306_GotoXY (0, 23);
        SSD1306_Puts ("1) All Cuts", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 33);
        SSD1306_Puts ("2) Ground", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 43);
        SSD1306_Puts ("3) Back", &Font_7x10, SSD1306_COLOR_WHITE);
        SSD1306_GotoXY (0, 53);
        SSD1306_Puts ("4) Home", &Font_7x10, SSD1306_COLOR_WHITE);
        break;
    }
    titleCutSel.xPos = 3;
    titleCutSel.xPos = 0;
    break;
  case DONENESS0:
    SSD1306_Clear();
    SSD1306_GotoXY (20,0);
    SSD1306_Puts ("DONENESS", &Font_11x18, SSD1306_COLOR_WHITE);
    if (saveSel == PORK) {
      SSD1306_GotoXY (0, 23);
      SSD1306_Puts ("1) Medium", &Font_7x10, SSD1306_COLOR_WHITE);
      SSD1306_GotoXY (0, 33);
      SSD1306_Puts ("2) Well Done", &Font_7x10, SSD1306_COLOR_WHITE);
      SSD1306_GotoXY (0, 43);
      SSD1306_Puts ("3) Back", &Font_7x10, SSD1306_COLOR_WHITE);
      SSD1306_GotoXY (0, 53);
      SSD1306_Puts ("4) Home", &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else {
      SSD1306_GotoXY (0, 23);
      SSD1306_Puts ("1) Rare", &Font_7x10, SSD1306_COLOR_WHITE);
      SSD1306_GotoXY (0, 33);
      SSD1306_Puts ("2) Medium Rare", &Font_7x10, SSD1306_COLOR_WHITE);
      SSD1306_GotoXY (0, 43);
      SSD1306_Puts ("3) Medium", &Font_7x10, SSD1306_COLOR_WHITE);
      SSD1306_GotoXY (0, 53);
      SSD1306_Puts ("4) Next", &Font_7x10, SSD1306_COLOR_WHITE);
    }
    break;
  case DONENESS1:
    SSD1306_Clear();
    SSD1306_GotoXY (20,0);
    SSD1306_Puts ("DONENESS", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 23);
    SSD1306_Puts ("1) Medium Well", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 33);
    SSD1306_Puts ("2) Well Done", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 43);
    SSD1306_Puts ("3) Back", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 53);
    SSD1306_Puts ("4) Home", &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case CUSTOM_SEL0:
    SSD1306_Clear();
    SSD1306_GotoXY (31,0);
    SSD1306_Puts ("CUSTOM", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 23);
    SSD1306_Puts ("1) ", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 33);
    SSD1306_Puts ("2) ", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 43);
    SSD1306_Puts ("3) Next", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 53);
    SSD1306_Puts ("4) Back", &Font_7x10, SSD1306_COLOR_WHITE);
    if (customOption1.data == 0) {
      SSD1306_GotoXY (20, 23);
      SSD1306_Puts ("Not Uploaded", &Font_7x10, SSD1306_COLOR_WHITE);  
      customOption1.xPos = 128;
      customOption1.yPos = 64;
    }
    else {
      customOption1.xPos = 20;
      customOption1.yPos = 23;
    }
    if (customOption2.data == 0) {
      SSD1306_GotoXY (20, 33);
      SSD1306_Puts ("Not Uploaded", &Font_7x10, SSD1306_COLOR_WHITE);
      customOption2.xPos = 128;
      customOption2.yPos = 64;
    }
    else {
      customOption2.xPos = 20;
      customOption2.yPos = 33;
    }
    break;
  case CUSTOM_SEL1:
    SSD1306_Clear();
    SSD1306_GotoXY (31,0);
    SSD1306_Puts ("CUSTOM", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 23);
    SSD1306_Puts ("1) ", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 33);
    SSD1306_Puts ("2) ", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 43);
    SSD1306_Puts ("3) Back", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (0, 53);
    SSD1306_Puts ("4) Home", &Font_7x10, SSD1306_COLOR_WHITE);
    if (customOption3.data == 0) {
      SSD1306_GotoXY (20, 23);
      SSD1306_Puts ("Not Uploaded", &Font_7x10, SSD1306_COLOR_WHITE);  
      customOption3.xPos = 128;
      customOption3.yPos = 64;
    }
    else {
      customOption3.xPos = 20;
      customOption3.yPos = 23;
    }
    if (customOption4.data == 0) {
      SSD1306_GotoXY (20, 33);
      SSD1306_Puts ("Not Uploaded", &Font_7x10, SSD1306_COLOR_WHITE);
      customOption4.xPos = 128;
      customOption4.yPos = 64;
    }
    else {
      customOption4.xPos = 20;
      customOption4.yPos = 33;
    }
    break;
  case ERROR_SCREEN:
    SSD1306_Clear();
    SSD1306_GotoXY (36,0);
    SSD1306_Puts ("ERROR", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (8, 23);
    SSD1306_Puts ("No custom option", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (4, 33);
    SSD1306_Puts ("has been uploaded", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (18, 43);
    SSD1306_Puts ("in this slot!", &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case ERROR_SCREEN2:
    SSD1306_Clear();
    SSD1306_GotoXY (36,0);
    SSD1306_Puts ("ERROR", &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (22, 23);
    SSD1306_Puts ("Please clear", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (1, 33);
    SSD1306_Puts ("target temperature", &Font_7x10, SSD1306_COLOR_WHITE);
    SSD1306_GotoXY (8, 43);
    SSD1306_Puts ("to change units!", &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  }
  
  SSD1306_UpdateScreen(); //display

  currentScreen = screen_no;
  
#pragma diag_suppress= Pa149
  // what must be done after screen is switched in
  switch (currentScreen) {
  }
#pragma diag_warning= Pa149
  
}


//// Keyboard Processor
//
//uint8_t ProcessKeyCode (uint16_t key_code)
//{
//  switch (key_code) {
//  case 0:
//    break;
//  case 1:
//    break;
//  case 2:
//    break;
//  case 3:
//    break;
//  }
//  
//  return true;
//}


// context sensitive keyboard processor
uint8_t ProcessKeyCodeInContext (uint16_t key_code)
{
  switch (currentScreen) {
  case  HOME:
    switch (key_code) {
    case 0:
      if (meatSelected == 1 && targetMeat.temperatureF == 0) {
        SwitchScreens(ERROR_SCREEN2);
      }
      else {
        unitSel ^= 1; 
        SwitchScreens(HOME);
      }
      break;
    case 1:
      SwitchScreens(MENU);
      break;
    case 2:
      break;
    case 3:
      break;
    }
    break;
  case  MENU:
    switch (key_code) {
    case 0:
      SwitchScreens(BEEF);
      break;
    case 1:
      SwitchScreens(HOME);
      break;
    case 2: // clr
      meatSelected = 0;
      customSel = 0;
      for (int i = 0; i < 8; i++) {
        tarMeat.data[i] = 0;
      }
      for (int i = 0; i < 12; i++) {
        tarDone.data[i] = 0;
      }
      tarTemp.data = 0;
      targetMeat.temperatureF = 0;
      targetMeat.temperatureC = 0;
  
      for (int i = 0; i < 8; i++) {
        targetMeat.meatType[i] = 0;
      }
      for (int i = 0; i < 14; i++) {
        targetMeat.doneness[i] = 0;
      }
      SwitchScreens(MENU);
      break;
    case 3:
      break;
    }
    break;
  case  BEEF:
    switch (key_code) {
    case 0:
      SwitchScreens(CUT_TYPE);
      break;
    case 1:
      SwitchScreens(FISH);
      break;
    case 2:
      SwitchScreens(CUSTOM);
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  case  FISH:
    switch (key_code) {
    case 0:
      SwitchScreens(CUT_TYPE);
      break;
    case 1:
      SwitchScreens(LAMB);
      break;
    case 2:
      SwitchScreens(BEEF);
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  case  LAMB:
    switch (key_code) {
    case 0:
      SwitchScreens(CUT_TYPE);
      break;
    case 1:
      SwitchScreens(PORK);
      break;
    case 2:
      SwitchScreens(FISH);
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  case  PORK:   
    switch (key_code) {
    case 0:
      SwitchScreens(CUT_TYPE);
      break;
    case 1:
      SwitchScreens(POULTRY);
      break;
    case 2:
      SwitchScreens(LAMB);
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  case  POULTRY:   
    switch (key_code) {
    case 0:
      SwitchScreens(CUT_TYPE);
      break;
    case 1:
      SwitchScreens(CUSTOM);
      break;
    case 2:
      SwitchScreens(PORK);
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  case  CUSTOM:   
    switch (key_code) {
    case 0:
      SwitchScreens(CUSTOM_SEL0);
      break;
    case 1:
      SwitchScreens(BEEF);
      break;
    case 2:
      SwitchScreens(POULTRY);
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  case  CUT_TYPE:   
    switch (key_code) {
    case 0:
      switch(lastScreen) {
      case BEEF: 
        SwitchScreens(DONENESS0);
        break;
      case FISH:
        meatSelected = 1;
        customSel = 0;
        targetMeat = fishT_S;
        for (int i = 0; i < 8; i++) {
          tarMeat.data[i] = fishT_S.meatType[i];
        }
        for (int i = 0; i < 12; i++) {
          tarDone.data[i] = fishT_S.doneness[i];
        }
        getCorrectTar(fishT_S);
        SwitchScreens(MENU);
        break;
      case LAMB: 
        SwitchScreens(DONENESS0);
        break;
      case PORK:
        SwitchScreens(DONENESS0);
        break;
      case POULTRY:
        meatSelected = 1;
        customSel = 0;
        targetMeat = poultry;
        for (int i = 0; i < 8; i++) {
          tarMeat.data[i] = poultry.meatType[i];
        }
        for (int i = 0; i < 12; i++) {
          tarDone.data[i] = poultry.doneness[i];
        }
        getCorrectTar(poultry);
        SwitchScreens(MENU);
        break;
      }
      break;
    case 1:
        switch(lastScreen) {
        case BEEF:
          meatSelected = 1;
          customSel = 0;
          targetMeat = beefGrnd;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = beefGrnd.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = beefGrnd.doneness[i];
          }
          getCorrectTar(beefGrnd);
          SwitchScreens(MENU);
          break;
        case FISH:
          meatSelected = 1;
          customSel = 0;
          targetMeat = fish;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = fish.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = fish.doneness[i];
          }
          getCorrectTar(fish);
          SwitchScreens(MENU);
          break;
        case LAMB:
          meatSelected = 1;
          customSel = 0;
          targetMeat = lambChops;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = lambChops.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = lambChops.doneness[i];
          }
          getCorrectTar(lambChops);
          SwitchScreens(MENU);
          break;
        case PORK:
          meatSelected = 1;
          customSel = 0;
          targetMeat = porkOther;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = porkOther.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = porkOther.doneness[i];
          }
          getCorrectTar(porkOther);
          SwitchScreens(MENU);
          break;
        case POULTRY:
          meatSelected = 1;
          customSel = 0;
          targetMeat = poultryGrnd;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = poultryGrnd.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = poultryGrnd.doneness[i];
          }
          getCorrectTar(poultryGrnd);
          SwitchScreens(MENU);
          break;
        }
        break;
    case 2:
      switch(lastScreen) {
      case BEEF:
        SwitchScreens(BEEF);
        break;
      case FISH:
        SwitchScreens(FISH);
        break;
      case LAMB:
        SwitchScreens(LAMB);
        break;
      case PORK:
        SwitchScreens(PORK);
        break;
      case POULTRY:
        SwitchScreens(POULTRY);
        break;
      case CUSTOM:
        SwitchScreens(CUSTOM);
        break;
      }
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  case  DONENESS0:
    if (saveSel == PORK) {
      switch (key_code) {
      case 0:
        meatSelected = 1;
        customSel = 0;
        targetMeat = porkMed;
        for (int i = 0; i < 8; i++) {
          tarMeat.data[i] = porkMed.meatType[i];
        }
        for (int i = 0; i < 12; i++) {
          tarDone.data[i] = porkMed.doneness[i];
        }
        getCorrectTar(porkMed);
        SwitchScreens(MENU);
        break;
      case 1:
        meatSelected = 1;
        customSel = 0;
        targetMeat = porkWell;
        for (int i = 0; i < 8; i++) {
          tarMeat.data[i] = porkWell.meatType[i];
        }
        for (int i = 0; i < 12; i++) {
          tarDone.data[i] = porkWell.doneness[i];
        }
        getCorrectTar(porkWell);
        SwitchScreens(MENU);
        break;
      case 2:
        SwitchScreens(PORK);
        break;
      case 3:
        SwitchScreens(HOME);
        break;
      }
    }
    else {
      switch (key_code) {
      case 0:
        switch(saveSel) {
        case BEEF:
          meatSelected = 1;
          customSel = 0;
          targetMeat = beefRare;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = beefRare.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = beefRare.doneness[i];
          }
          getCorrectTar(beefRare);
          SwitchScreens(MENU);
          break;
        case LAMB:
          meatSelected = 1;
          customSel = 0;
          targetMeat = lambRare;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = lambRare.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = lambRare.doneness[i];
          }
          getCorrectTar(lambRare);
          SwitchScreens(MENU);
          break;
        }
        break;
      case 1:
        switch(saveSel) {
        case BEEF:
          meatSelected = 1;
          customSel = 0;
          targetMeat = beefMedRare;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = beefMedRare.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = beefMedRare.doneness[i];
          }
          getCorrectTar(beefMedRare);
          SwitchScreens(MENU);
          break;
        case LAMB:
          meatSelected = 1;
          customSel = 0;
          targetMeat = lambMedRare;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = lambMedRare.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = lambMedRare.doneness[i];
          }
          getCorrectTar(lambMedRare);
          SwitchScreens(MENU);
          break;
        }
        break;
      case 2:
        switch(saveSel) {
        case BEEF:
          meatSelected = 1;
          customSel = 0;
          targetMeat = beefMed;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = beefMed.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = beefMed.doneness[i];
          }
          getCorrectTar(beefMed);
          SwitchScreens(MENU);
          break;
        case LAMB:
          meatSelected = 1;
          customSel = 0;
          targetMeat = lambMed;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = lambMed.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = lambMed.doneness[i];
          }
          getCorrectTar(lambMed);
          SwitchScreens(MENU);
          break;
        }
        break;
      case 3:
        SwitchScreens(DONENESS1);
        break;
      }
    }
    break;
  case  DONENESS1:   
    switch (key_code) {
    case 0:
        switch(saveSel) {
        case BEEF:
          meatSelected = 1;
          customSel = 0;
          targetMeat = beefMedWell;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = beefMedWell.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = beefMedWell.doneness[i];
          }
          getCorrectTar(beefMedWell);
          SwitchScreens(MENU);
          break;
        case LAMB:
          meatSelected = 1;
          customSel = 0;
          targetMeat = lambMedWell;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = lambMedWell.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = lambMedWell.doneness[i];
          }
          getCorrectTar(lambMedWell);
          SwitchScreens(MENU);
          break;
        }
        break;
    case 1:
        switch(saveSel) {
        case BEEF:
          meatSelected = 1;
          customSel = 0;
          targetMeat = beefWell;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = beefWell.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = beefWell.doneness[i];
          }
          getCorrectTar(beefWell);
          SwitchScreens(MENU);
          break;
        case LAMB:
          meatSelected = 1;
          customSel = 0;
          targetMeat = lambWell;
          for (int i = 0; i < 8; i++) {
            tarMeat.data[i] = lambWell.meatType[i];
          }
          for (int i = 0; i < 12; i++) {
            tarDone.data[i] = lambWell.doneness[i];
          }
          getCorrectTar(lambWell);
          SwitchScreens(MENU);
          break;
        }
        break;
    case 2:
      SwitchScreens(DONENESS0);
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  case  CUSTOM_SEL0:   
    switch (key_code) {
    case 0:
      if (customOption1.data != 0) {
        switch(customUnitFlags[0]) {
        case 0:
          unitSel = 0;
          meatSelected = 1;
          customSel = 1;
          tarTemp.data = customOption1.data;
          SwitchScreens(MENU);
          break;
        case 1:
          unitSel = 1;
          customSel = 1;
          meatSelected = 1;
          tarTemp.data = customOption1.data;
          SwitchScreens(MENU);
          break;
        }
      }
      else 
        SwitchScreens(ERROR_SCREEN);
      break;
    case 1:
      if (customOption2.data != 0) {
        switch(customUnitFlags[1]) {
        case 0:
          unitSel = 0;
          meatSelected = 1;
          customSel = 1;
          tarTemp.data = customOption2.data;
          SwitchScreens(MENU);
          break;
        case 1:
          unitSel = 1;
          customSel = 1;
          meatSelected = 1;
          tarTemp.data = customOption2.data;
          SwitchScreens(MENU);
          break;
        }
      }
      else
        SwitchScreens(ERROR_SCREEN);
      break;
    case 2:
      SwitchScreens(CUSTOM_SEL1);
      break;
    case 3:
      SwitchScreens(CUSTOM);
      break;
    }
    break;
  case  CUSTOM_SEL1:   
    switch (key_code) {
    case 0:
      if (customOption3.data != 0) {
        switch(customUnitFlags[2]) {
        case 0:
          unitSel = 0;
          meatSelected = 1;
          customSel = 1;
          tarTemp.data = customOption3.data;
          SwitchScreens(MENU);
          break;
        case 1:
          unitSel = 1;
          customSel = 1;
          meatSelected = 1;
          tarTemp.data = customOption3.data;
          SwitchScreens(MENU);
          break;
        }
      }
      else
        SwitchScreens(ERROR_SCREEN);
      break;
    case 1:
      if (customOption4.data != 0) {
        switch(customUnitFlags[3]) {
        case 0:
          unitSel = 0;
          meatSelected = 1;
          customSel = 1;
          tarTemp.data = customOption4.data;
          SwitchScreens(MENU);
          break;
        case 1:
          unitSel = 1;
          customSel = 1;
          meatSelected = 1;
          tarTemp.data = customOption4.data;
          SwitchScreens(MENU);
          break;
        }
      }
      else
        SwitchScreens(ERROR_SCREEN);
      break;
    case 2:
      SwitchScreens(CUSTOM_SEL0);
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  case ERROR_SCREEN:   
    switch (key_code) {
    case 0:
      if (lastScreen == CUSTOM_SEL1) {
        SwitchScreens(CUSTOM_SEL1);
      }
      else {
        SwitchScreens(CUSTOM_SEL0);
      }
      break;
    case 1:
      if (lastScreen == CUSTOM_SEL1) {
        SwitchScreens(CUSTOM_SEL1);
      }
      else {
        SwitchScreens(CUSTOM_SEL0);
      }
      break;
    case 2:
      if (lastScreen == CUSTOM_SEL1) {
        SwitchScreens(CUSTOM_SEL1);
      }
      else {
        SwitchScreens(CUSTOM_SEL0);
      }
      break;
    case 3:
      if (lastScreen == CUSTOM_SEL1) {
        SwitchScreens(CUSTOM_SEL1);
      }
      else {
        SwitchScreens(CUSTOM_SEL0);
      }
      break;
    }
    break;
  case ERROR_SCREEN2:   
    switch (key_code) {
    case 0:
      SwitchScreens(HOME);
      break;
    case 1:
      SwitchScreens(HOME);
      break;
    case 2:
      SwitchScreens(HOME);
      break;
    case 3:
      SwitchScreens(HOME);
      break;
    }
    break;
  }
  
  return true;
}




void UpdateScreenValues(void)
{
  char displayString[25];
  
  switch (currentScreen) {
  case HOME:
    SSD1306_GotoXY (temp.xPos, temp.yPos);
    if (temp.valid) {
      sprintf(displayString, temp.format, temp.data);
      SSD1306_Puts(displayString, &Font_16x26, SSD1306_COLOR_WHITE);
    }
    else 
      SSD1306_Puts(temp.invalidMsg, &Font_16x26, SSD1306_COLOR_WHITE);

    SSD1306_GotoXY (tempUnit.xPos, tempUnit.yPos);
    if (tempUnit.valid) {
      sprintf(displayString, tempUnit.format, tempUnit.data);
      SSD1306_Puts(displayString, &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else 
      SSD1306_Puts(tempUnit.invalidMsg, &Font_7x10, SSD1306_COLOR_WHITE);
    
    SSD1306_GotoXY (tarTemp.xPos, tarTemp.yPos);
    if (tarTemp.valid) {
      sprintf(displayString, tarTemp.format, tarTemp.data);
      SSD1306_Puts(displayString, &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else 
      SSD1306_Puts(tarTemp.invalidMsg, &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case MENU:
    SSD1306_GotoXY (tarMeat.xPos, tarMeat.yPos);
    if (tarMeat.valid) {
      sprintf(displayString, tarMeat.format, tarMeat.data);
      SSD1306_Puts(displayString, &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else 
      SSD1306_Puts(tarMeat.invalidMsg, &Font_7x10, SSD1306_COLOR_WHITE);
    
    SSD1306_GotoXY (tarDone.xPos, tarDone.yPos);
    if (tarDone.valid) {
      sprintf(displayString, tarDone.format, tarDone.data);
      SSD1306_Puts(displayString, &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else
      SSD1306_Puts(tarDone.invalidMsg, &Font_7x10, SSD1306_COLOR_WHITE);
    
    SSD1306_GotoXY (tarTemp.xPos, tarTemp.yPos);
    if (tarTemp.valid) {
      sprintf(displayString, tarTemp.format, tarTemp.data);
      SSD1306_Puts(displayString, &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else
      SSD1306_Puts(tarTemp.invalidMsg, &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case BEEF:
    break;
  case FISH:
    break;
  case LAMB:
    break;
  case CUT_TYPE:
    SSD1306_GotoXY (titleCutSel.xPos, titleCutSel.yPos);
    if (titleCutSel.valid) {
      sprintf(displayString, titleCutSel.format, titleCutSel.data);
      SSD1306_Puts(displayString, &Font_11x18, SSD1306_COLOR_WHITE);
    }
    else 
      SSD1306_Puts(titleCutSel.invalidMsg, &Font_11x18, SSD1306_COLOR_WHITE);
    break;
  case CUSTOM_SEL0:
    SSD1306_GotoXY (customOption1.xPos, customOption1.yPos);
    if (customOption1.valid) {
      sprintf(displayString, customOption1.format, customOption1.data);
      SSD1306_Puts(displayString, &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else 
      SSD1306_Puts(customOption1.invalidMsg, &Font_7x10, SSD1306_COLOR_WHITE);
    
    SSD1306_GotoXY (customOption2.xPos, customOption2.yPos);
    if (customOption2.valid) {
      sprintf(displayString, customOption2.format, customOption2.data);
      SSD1306_Puts(displayString, &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else 
      SSD1306_Puts(customOption2.invalidMsg, &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  case CUSTOM_SEL1:
    SSD1306_GotoXY (customOption3.xPos, customOption3.yPos);
    if (customOption3.valid) {
      sprintf(displayString, customOption3.format, customOption3.data);
      SSD1306_Puts(displayString, &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else 
      SSD1306_Puts(customOption3.invalidMsg, &Font_7x10, SSD1306_COLOR_WHITE);
    
    SSD1306_GotoXY (customOption4.xPos, customOption4.yPos);
    if (customOption4.valid) {
      sprintf(displayString, customOption4.format, customOption4.data);
      SSD1306_Puts(displayString, &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else 
      SSD1306_Puts(customOption4.invalidMsg, &Font_7x10, SSD1306_COLOR_WHITE);
    break;
  }
  SSD1306_UpdateScreen(); //display
}

void getCorrectTar(meat_temp MEAT_TEMP)
{
  if (customSel == 0) {
    if (unitSel == 0) {
      tarTemp.data = MEAT_TEMP.temperatureF;
    }
    else {
      tarTemp.data = MEAT_TEMP.temperatureC;
    }
  }
}

uint8_t GetKeycode(void)
{
  return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
}