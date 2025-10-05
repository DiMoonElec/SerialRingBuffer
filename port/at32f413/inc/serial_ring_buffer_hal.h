#ifndef __SERIAL_RING_BUFFER_HAL_H__
#define __SERIAL_RING_BUFFER_HAL_H__

#include "SerialRingBuffer/public-api.h"

void SerialRingBufferHAL_Init(SerialRingBuffer_t *instance);
void SerialRingBufferHAL_SerialEnable(SerialRingBuffer_t *instance);

void SerialRingBufferHAL_EnterTxCritical(SerialRingBuffer_t *instance);
void SerialRingBufferHAL_ExitTxCritical(SerialRingBuffer_t *instance);
void SerialRingBufferHAL_EnterRxCritical(SerialRingBuffer_t *instance);
void SerialRingBufferHAL_ExitRxCritical(SerialRingBuffer_t *instance);

void SerialRingBufferHAL_TxEnable(SerialRingBuffer_t *instance);
int SerialRingBufferHAL_TransferComplete(SerialRingBuffer_t *instance);

void SerialRingBufferHAL_IRQHandler(SerialRingBuffer_t *instance);

#endif

