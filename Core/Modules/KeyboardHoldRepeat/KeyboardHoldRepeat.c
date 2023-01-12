/***********************************************************************************************************************
* File Name    : KeyboardHoldRepeat.c
* Version      : 
* Device(s)    : Keyboard Handler code
* Tool-Chain   : IAR Systems ARM
* Description  : This file implements support code for a keyCode that is a result of a keyboard or button scan
*              : 
* Creation Date: 10OCT2021
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "main.h"
#include "KeyboardHoldRepeat.h"
#include <stdio.h>
#include <string.h>



/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/


/***********************************************************************************************************************
Global/module variables
***********************************************************************************************************************/
uint8_t keyCodeDebounceCounter = KEYCODE_DEBOUNCE_TIME;
uint8_t keyCodeDebounced = false;
uint8_t keyCodeHoldCounter = KEYCODE_HOLD_TIME;
uint8_t keyCodeRepeatDelayCounter = 0;
uint8_t processKeyCode = false;
uint8_t keyCodeProcessed = false;
uint8_t useHoldRepeat = USE_HOLD_REPEAT;
uint8_t lastKeyCode = NO_KEY_PRESSED;
uint8_t firstSend = true;


/***********************************************************************************************************************
module function prototypes
***********************************************************************************************************************/
uint8_t ValidKeyCode(uint8_t _kcode);



/***********************************************************************************************************************
code start
***********************************************************************************************************************/
uint8_t DebounceKeyCode(uint8_t _kcode)
{
  if (_kcode != NO_KEY_PRESSED) {
    if (_kcode == lastKeyCode) {
      if (keyCodeDebounceCounter == 0) {
        if ( ValidKeyCode(_kcode) ) {
          if (useHoldRepeat) {
            if (firstSend == true) {
              processKeyCode = true;
              firstSend = false;
            }
            if (keyCodeHoldCounter == 0) {
              if (keyCodeRepeatDelayCounter == 0) {
                keyCodeRepeatDelayCounter = KEYCODE_REPEAT_DELAY;
                processKeyCode = true;
              }
              else {
                keyCodeRepeatDelayCounter--;
              }
            }
            else {
              keyCodeHoldCounter--;
            }
          }
          else {
            if (keyCodeProcessed == false) {
              processKeyCode = true;
            }
          }
        }
      }
      else {
        keyCodeDebounceCounter--;
      }
    }
    else {
      lastKeyCode = _kcode;
      keyCodeDebounceCounter = KEYCODE_DEBOUNCE_TIME;
    }
  }
  else {
    keyCodeDebounceCounter = KEYCODE_DEBOUNCE_TIME;
    lastKeyCode = NO_KEY_PRESSED;
    keyCodeHoldCounter = KEYCODE_HOLD_TIME;
    keyCodeRepeatDelayCounter = 0;
    keyCodeDebounced = false;
    keyCodeProcessed = false;
    firstSend = true;
  }
  
  
  
  
  uint8_t debounceDone = false;
  return debounceDone;
}