// serial_user.c

#include "serial.h"
#include "serial_user.h"
#include "ASCII_numbers.h"
#include "main.h"
#include "ux_manager.h"
#include "stdio.h"


uint8_t packetBuffer[16];
uint8_t inPacket = false;
uint8_t nextPacketChar = 0;
uint8_t processPacket = false;
uint16_t sel;
uint16_t customTemp;
uint8_t customUnitFlags[4] = {0, 0, 0, 0};

extern uint16_t serialValue;

// temp data 
char tempStr[5];
char tarTempStr[5];
char option1Str[4];
char option2Str[4];
char option3Str[4];
char option4Str[4];

// function to process the input buffer
uint8_t ProcessReceiveBuffer(void)
{
//  SendString((char const *)&rxBuffer[nextSerialRx2Proc], 1, StripZeros, NoAddCRLF);
  if (rxBuffer[nextSerialRx2Proc] == '$') {
    inPacket = true;
    packetBuffer[0] = rxBuffer[nextSerialRx2Proc];
    nextPacketChar = 1;
  }
  else {
    if (inPacket == true) {
      if (((rxBuffer[nextSerialRx2Proc] >= '0') && (rxBuffer[nextSerialRx2Proc] <= '9')) || 
          ((rxBuffer[nextSerialRx2Proc] >= 'r') && (rxBuffer[nextSerialRx2Proc] <= 'v')) ||
          ((rxBuffer[nextSerialRx2Proc] >= 'R') && (rxBuffer[nextSerialRx2Proc] <= 'V')) ||
          (rxBuffer[nextSerialRx2Proc] >= '\n') || (rxBuffer[nextSerialRx2Proc] <= '\r')) {
        
            packetBuffer[nextPacketChar++] = rxBuffer[nextSerialRx2Proc];

            if (rxBuffer[nextSerialRx2Proc] == '\n') {
              processPacket = true;
              inPacket = false;
            }
          }
      else {
        inPacket = false;
      }
    }
  }
  
  
  if (++nextSerialRx2Proc >= RX_BUFFER_SIZE) {
    nextSerialRx2Proc = 0;
  }
  return 0;

}


uint8_t ProcessPacket(void)
{
  switch (packetBuffer[1]) {
  // list of commands
  // each command has intentional fallthru for upper/lower case
  case 't':     // t = return current temp
  case 'T':     
    sprintf(tempStr, "%f", temp.data);
    SendString(tempStr, 5, StripZeros, NoAddCRLF);
    SendString(tempUnit.data, 1, StripZeros, AddCRLF);
    break;
  case 'x':     // x = returns target temp
  case 'X':
    if (tarTemp.data == 0) {
      SendString("Target Temp Not Set", 20, StripZeros, AddCRLF);
    }
    else {
      sprintf(tarTempStr, "%f", tarTemp.data);
      SendString(tarTempStr, 5, StripZeros, NoAddCRLF);
      SendString(tempUnit.data, 1, StripZeros, AddCRLF);
    }
    break;
  case 'l':     // l = lists custom option
  case 'L':
    ConvertASCII2UINT16((char const *)&packetBuffer[2], 1, '\n', &sel);
    switch(sel) {
    case 1:
      sprintf(option1Str, "%f", customOption1.data);
      SendString("1) ", 3, StripZeros, NoAddCRLF);
      SendString(option1Str, 5, StripZeros, NoAddCRLF);
      sel = 0;
      if (customUnitFlags[0] == 1) {
        SendString("C", 1, StripZeros, AddCRLF);
      }
      else {
        SendString("F", 1, StripZeros, AddCRLF);
      }
      break;
    case 2:
      sprintf(option2Str, "%f", customOption2.data);
      SendString("2) ", 3, StripZeros, NoAddCRLF);
      SendString(option2Str, 5, StripZeros, NoAddCRLF);
      sel = 0;
      if (customUnitFlags[1] == 1) {
        SendString("C", 1, StripZeros, AddCRLF);
      }
      else {
        SendString("F", 1, StripZeros, AddCRLF);
      }
      break;
     case 3:
      sprintf(option3Str, "%f", customOption3.data);
      SendString("3) ", 3, StripZeros, NoAddCRLF);
      SendString(option3Str, 5, StripZeros, NoAddCRLF);
      sel = 0;
      if (customUnitFlags[2] == 1) {
        SendString("C", 1, StripZeros, AddCRLF);
      }
      else {
        SendString("F", 1, StripZeros, AddCRLF);
      }
      break;
    case 4:
      sprintf(option4Str, "%f", customOption4.data);
      SendString("4) ", 3, StripZeros, NoAddCRLF);
      SendString(option4Str, 5, StripZeros, NoAddCRLF);
      sel = 0;
      if (customUnitFlags[3] == 1) {
        SendString("C", 1, StripZeros, AddCRLF);
      }
      else {
        SendString("F", 1, StripZeros, AddCRLF);
      }
      break;
    default:
      sprintf(option1Str, "%f", customOption1.data);
      SendString("1) ", 3, StripZeros, NoAddCRLF);
      SendString(option1Str, 5, StripZeros, NoAddCRLF);
      if (customUnitFlags[0] == 1) {
        SendString("C", 1, StripZeros, AddCRLF);
      }
      else {
        SendString("F", 1, StripZeros, AddCRLF);
      }
      sprintf(option2Str, "%f", customOption2.data);
      SendString("2) ", 3, StripZeros, NoAddCRLF);
      SendString(option2Str, 5, StripZeros, NoAddCRLF);
      if (customUnitFlags[1] == 1) {
        SendString("C", 1, StripZeros, AddCRLF);
      }
      else {
        SendString("F", 1, StripZeros, AddCRLF);
      }
      sprintf(option3Str, "%f", customOption3.data);
      SendString("3) ", 3, StripZeros, NoAddCRLF);
      SendString(option3Str, 5, StripZeros, NoAddCRLF);
      if (customUnitFlags[2] == 1) {
        SendString("C", 1, StripZeros, AddCRLF);
      }
      else {
        SendString("F", 1, StripZeros, AddCRLF);
      }
      sprintf(option4Str, "%f", customOption4.data);
      SendString("4) ", 3, StripZeros, NoAddCRLF);
      SendString(option4Str, 5, StripZeros, NoAddCRLF);
      if (customUnitFlags[3] == 1) {
        SendString("C", 1, StripZeros, AddCRLF);
      }
      else {
        SendString("F", 1, StripZeros, AddCRLF);
      }      
      sel = 0;
      break;
    }
    break;
  case 'c':     // u = upload Celsius custom option
  case 'C':
    ConvertASCII2UINT16((char const *)&packetBuffer[2], 1, '\n', &sel);
    switch(sel) {
    case 1:
      ConvertASCII2UINT16((char const *)&packetBuffer[3], 3, '\n', &customTemp);
      customOption1.data = (float)customTemp;
      customUnitFlags[0] = 1;
      sel = 0;
      break;
    case 2:
      ConvertASCII2UINT16((char const *)&packetBuffer[3], 3, '\n', &customTemp);
      customOption2.data = (float)customTemp;
      customUnitFlags[1] = 1;
      sel = 0;
      break;
    case 3:
      ConvertASCII2UINT16((char const *)&packetBuffer[3], 3, '\n', &customTemp);
      customOption3.data = (float)customTemp;
      customUnitFlags[2] = 1;
      sel = 0;
      break;
    case 4:
      ConvertASCII2UINT16((char const *)&packetBuffer[3], 3, '\n', &customTemp);
      customOption4.data = (float)customTemp;
      customUnitFlags[3] = 1;
      sel = 0;
      break;
    default:
      SendString("Not a valid option.", 20, StripZeros, AddCRLF);
      break;
    }
    break;
  case 'f':     // f = upload Farenheit custom option
  case 'F':
    ConvertASCII2UINT16((char const *)&packetBuffer[2], 1, '\n', &sel);
    switch(sel) {
    case 1:
      ConvertASCII2UINT16((char const *)&packetBuffer[3], 3, '\n', &customTemp);
      customOption1.data = (float)customTemp;
      customUnitFlags[0] = 0;
      sel = 0;
      break;
    case 2:
      ConvertASCII2UINT16((char const *)&packetBuffer[3], 3, '\n', &customTemp);
      customOption2.data = (float)customTemp;
      customUnitFlags[1] = 0;
      sel = 0;
      break;
    case 3:
      ConvertASCII2UINT16((char const *)&packetBuffer[3], 3, '\n', &customTemp);
      customOption3.data = (float)customTemp;
      customUnitFlags[2] = 0;
      sel = 0;
      break;
    case 4:
      ConvertASCII2UINT16((char const *)&packetBuffer[3], 3, '\n', &customTemp);
      customOption4.data = (float)customTemp;
      customUnitFlags[3] = 0;
      sel = 0;
      break;
    default:
      SendString("Not a valid option.", 20, StripZeros, AddCRLF);
      sel = 0;
      break;
    }
    break;
  }
  processPacket = false;

  return 0;
}


