#ifndef __SERIAL_RING_BUFFER_HAL_TYPE_H__
#define __SERIAL_RING_BUFFER_HAL_TYPE_H__

#include <stdint.h>
#include "at32f413.h"

typedef struct
{
  usart_type* usart_x;
  IRQn_Type IRQn;
  uint8_t tx_active;
} SerialRingBufferHAL_t;

#endif