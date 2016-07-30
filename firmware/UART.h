#ifndef UART_H__
#define UART_H__

#include <stdint.h>
#include "UART_drvr_2313a.h"

void UART_init(void);
void UART_transmit(uint8_t *data_to_tx_ptr, uint8_t len);
void UART_transmit_byte(uint8_t data_to_tx);
void UART_receive(uint8_t *receive_buffer_ptr, uint8_t len);
void UART_reset(void);

#endif /* UART_H__ */
