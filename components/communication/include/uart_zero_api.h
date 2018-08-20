/*
 * uart api header
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.18
 */

#ifndef __UART_API_H__
#define __UART_API_H__

#include "esp_err.h"
#include "driver/uart.h"

#define UART_API_TAG 		"UART API"
#define UART_API_USER_NUM	UART_NUM_1

#define UART_API_BAUD_RATE  9600
#define UART_API_DATA_BITS	UART_DATA_8_BITS
#define UART_API_PARITY		UART_PARITY_DISABLE
#define UART_API_STOP_BITS	UART_STOP_BITS_1
#define UART_API_FLOW_CTRL	UART_HW_FLOWCTRL_DISABLE

#define UART_API_TXD 		GPIO_NUM_19
#define UART_API_RXD 		GPIO_NUM_22
#define UART_API_RTS 		UART_PIN_NO_CHANGE
#define UART_API_CTS 		UART_PIN_NO_CHANGE

#define UART_API_BUF_SIZE 	1024

esp_err_t uart_zero_api_init();
esp_err_t uart_zero_api_read_bytes(uint8_t *data, size_t length);
esp_err_t uart_zero_api_write_bytes(uint8_t *data, size_t length);


#endif