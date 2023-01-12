// Serial.h


#ifndef _SERIAL_H_
#define _SERIAL_H_


// Copy the following into the serial interrupt handler in IRQn 0
// (not as a comment, of course)
//  HANDLE_INPUT_BUFFER

//  HANDLE_OUTPUT_BUFFER

//  HANDLE_ERRORS


// Includes
#include "main.h"
#include <stdint.h>
#include "stm32g0xx_hal.h"
#include <stm32g0xx_ll_usart.h>

// typedefs, structs, and enums
typedef enum _strip_zeros_ {
  StripZeros,
  NoStripZeros
} strip_zeros;

typedef enum _add_crlf_ {
  AddCRLF,
  NoAddCRLF
} add_CRLF;

// defines
#define TX_BUFFER_SIZE 50
#define RX_BUFFER_SIZE 50



// private modular variables


// public variables
extern uint8_t rxBuffer[];
extern uint8_t txBuffer[];
extern uint8_t nextSerialTxOut;
extern uint8_t nextSerialTxIn;
extern uint8_t nextSerialRxIn;
extern uint8_t nextSerialRx2Proc;

// public function prototypes
uint8_t SendString(const char * _msg, uint16_t _len, strip_zeros _supressZeros, add_CRLF _add_crlf);
uint16_t CheckBuffer(void);



// MACROS for Serial Buffer handling
#define HANDLE_INPUT_BUFFER                                                     \
  if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))   \
  {                                                                             \
    rxBuffer[nextSerialRxIn] =  LL_USART_ReceiveData8(USART1);                  \
    if (++nextSerialRxIn >= RX_BUFFER_SIZE) nextSerialRxIn = 0;                 \
  }

#define HANDLE_OUTPUT_BUFFER                                                    \
  if(LL_USART_IsEnabledIT_TXE(USART1) && LL_USART_IsActiveFlag_TXE(USART1))     \
  {                                                                             \
    if (nextSerialTxOut != nextSerialTxIn) {                                    \
      LL_USART_TransmitData8(USART1, txBuffer[nextSerialTxOut]);                \
      if (++nextSerialTxOut >= TX_BUFFER_SIZE) nextSerialTxOut = 0;             \
    }                                                                           \
    else LL_USART_DisableIT_TXE(USART1);                                        \
  }                                                                             \
  if(LL_USART_IsEnabledIT_TC(USART1) && LL_USART_IsActiveFlag_TC(USART1))       \
  {                                                                             \
    LL_USART_ClearFlag_TC(USART1);                                              \
    /*UART_CharTransmitComplete_Callback(); */                                  \
  }

#define HANDLE_ERRORS                                                           \
  if(LL_USART_IsEnabledIT_ERROR(USART1) && LL_USART_IsActiveFlag_NE(USART1))    \
  {                                                                             \
    /* Call Error function */                                                   \
    /*UART_Error_Callback();*/                                                  \
  }


#endif
