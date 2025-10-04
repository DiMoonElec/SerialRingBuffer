#ifndef __SERIAL_RING_BUFFER_HAL_TYPE_H__
#define __SERIAL_RING_BUFFER_HAL_TYPE_H__

#include <stdint.h>
#include "at32f435_437.h"

typedef struct
{
  usart_type* usart_x;
  IRQn_Type IRQn;
} SerialRingBufferHAL_t;

#endif