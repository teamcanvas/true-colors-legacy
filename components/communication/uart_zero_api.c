/*
 * uart api
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.18
 */

#include "uart_zero_api.h"

#include "esp_err.h"
#include "esp_log.h"
#include "driver/uart.h"

esp_err_t uart_zero_api_init() {
	esp_err_t err;
	uart_config_t uart_config = {
		.baud_rate = UART_API_BAUD_RATE,
		.data_bits = UART_API_DATA_BITS,
		.parity    = UART_API_PARITY,
		.stop_bits = UART_API_STOP_BITS,
		.flow_ctrl = UART_API_FLOW_CTRL
	};

	uart_param_config(UART_API_USER_NUM, &uart_config);
	uart_set_pin(
			UART_API_USER_NUM,
			UART_API_TXD,
			UART_API_RXD,
			UART_API_RTS,
			UART_API_CTS
	);

	err = uart_driver_install(UART_API_USER_NUM, UART_API_BUF_SIZE * 2, 0, 0, NULL, 0);
	if (err != ESP_OK) {
		ESP_LOGD(
				UART_API_TAG,
				"Error Code : %d\n\tat uart_driver_install\n\tat uart_api_init",
				err
		);
		
		return err;
	}

	return ESP_OK;
}

esp_err_t uart_zero_api_read_bytes(uint8_t *data, size_t length) {
	int len = uart_read_bytes(UART_API_USER_NUM, data, length, 100);
	if (len == -1) {
		ESP_LOGD(
				UART_API_TAG,
				"Error Code : %d\n\tat uart_read_bytes\n\tat uart_zero_api_read_bytes",
				ESP_FAIL
		);

		return ESP_FAIL;
	}

	return ESP_OK;
}

esp_err_t uart_zero_api_write_bytes(uint8_t *data, size_t length) {
	int len = uart_write_bytes(UART_API_USER_NUM, (const char*)data, length);
	if (len == -1) {
		ESP_LOGD(
				UART_API_TAG,
				"Error Code : %d\n\tat uart_write_bytes\n\tat uart_zero_api_write_bytes",
				ESP_FAIL
		);

		return ESP_FAIL;
	}

	return ESP_OK;
}