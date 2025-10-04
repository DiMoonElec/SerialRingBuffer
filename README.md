# SerialRingBuffer

Простая и переносимая библиотека для работы с UART с использованием кольцевых буферов.  
Позволяет организовать приём и передачу данных по прерываниям без блокировок, поддерживает несколько экземпляров драйвера одновременно.  

Автор: **Sivokon Dmitriy aka DiMoon Electronics**  
Лицензия **BSD 2-Clause License**  

## Структура проекта
```include/SerialRingBuffer/public-api.h``` - Публичный API для использования сторонним кодом  
```src/serial_ring_buffer.c``` - Платформонезависимая часть  
```port/<platform_name>/inc``` - Заголовки HAL-части для конкретной платформы  
```port/<platform_name>/src``` - Реализация HAL для конкретной платформы  


**Для работы** библиотека использует [RingFIFO](https://github.com/DiMoonElec/RingFIFO.git) как submodule.  
```RingFIFO``` необходимо установить так, чтобы API был доступен по пути `RingFIFO/public-api.h`.

## Идея

```serial_ring_buffer.c``` содержит всю логику драйвера (работа с кольцевыми буферами, обработчики приёма/передачи).

HAL-часть (```port/<platform>```) отвечает только за работу с конкретным железом (USART, NVIC и т.п.).

Весь пользовательский код работает только через ```SerialRingBuffer/public-api.h```.

Таким образом, можно легко перенести библиотеку на другой МК, написав свою HAL-часть.

## Подключение к проекту

Добавляем ```RingFIFO```
```bash
git submodule add https://github.com/DiMoonElec/RingFIFO.git source/external/RingFIFO
```

Добавляем ```SerialRingBuffer```
```bash
git submodule add https://github.com/DiMoonElec/SerialRingBuffer.git source/external/SerialRingBuffer
```

Добавляем пути до h-файлов в настройки проекта
```
$PROJ_DIR$\external\RingFIFO\include\
$PROJ_DIR$\external\SerialRingBuffer\include\
$PROJ_DIR$\external\SerialRingBuffer\port\at32f435_437\inc\
```

Добавляем c-файлы библиотек в проект для компиляции совместно с остальным проектом.

## Пример использования

Упрощенный пример. Тут не показаны функции инициализации периферии, отображено только взаимодействие с библиотекой.

```c
#include "SerialRingBuffer/public-api.h"
#include "serial_ring_buffer_hal.h"

/******************************************************************************/
/********************************* UART2 **************************************/
/******************************************************************************/

#define UART2_RING_BUFFER_SIZE 16

/* Экземпляр драйвера для UART2.
 * Здесь указываем аппаратно-зависимые параметры (USART и номер IRQ). */
SerialRingBuffer_t SerialBuffer_UART2 =
{
  .hal =
  {
    .IRQn    = USART2_IRQn,
    .usart_x = USART2
  }
};

/* Буферы приёма и передачи */
static uint8_t uart2_rx_ring_buffer[UART2_RING_BUFFER_SIZE];
static uint8_t uart2_tx_ring_buffer[UART2_RING_BUFFER_SIZE];

/******************************************************************************/

void main(void)
{
  /* Настройка тактирования, NVIC и USART */
  system_clock_config();
  periph_clock_config();
  nvic_config();
  usart2_init();

  /* Инициализация HAL-части драйвера (включаем прерывания и USART) */
  SerialRingBufferHAL_Init(&SerialBuffer_UART2);

  /* Инициализация платформонезависимой части (кольцевые буферы) */
  SerialRingBuffer_Init(&SerialBuffer_UART2,
                        uart2_rx_ring_buffer, UART2_RING_BUFFER_SIZE,
                        uart2_tx_ring_buffer, UART2_RING_BUFFER_SIZE);

  while (1)
  {
    /* Читаем байт из приёмного буфера */
    int c = SerialRingBuffer_Getc(&SerialBuffer_UART2);

    if (c >= 0)
    {
      /* Если байт получен — отправляем его обратно в UART */
      SerialRingBuffer_Putc(&SerialBuffer_UART2, (uint8_t)c);
    }
  }
}
/* Прерывание от USART2 */
void USART2_IRQHandler(void)
{
  /* Уведомляем SerialRingBufferHAL о возникновении прерывания от USART2 */
  SerialRingBufferHAL_IRQHandler(&SerialBuffer_UART2);
}
```