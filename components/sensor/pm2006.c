/*
 * pm2006 source
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.18
 */

#include "pm2006.h"

#include "esp_err.h"
#include "esp_log.h"
#include "uart_zero_api.h"

esp_err_t pm2006_open_measurement() {
	uint8_t pm2006_cmd_open_measurement[6] = {
		0x11, 0x03, 0x0C, 0x02, 0x1E, 0xC0
	};

	esp_err_t err;

	err = uart_zero_api_write_bytes(pm2006_cmd_open_measurement, 6);
	if (err != ESP_OK) {
		ESP_LOGD(
				PM2006_TAG,
				"Error Code : %d\n\tat uart_zero_api_write_bytes\n\tat pm2006_open_measurement",
				err
		);

		return err;
	}

	return ESP_OK;
}

esp_err_t pm2006_read_particle_measurement(uint16_t *value) {
	uint8_t pm2006_cmd_read_particle_measurement[5] = {
		0x11, 0x02, 0x0B, 0x01, 0xE1
	};

	esp_err_t err;

	err = uart_zero_api_write_bytes(pm2006_cmd_read_particle_measurement, 5);
	if (err != ESP_OK) {
		ESP_LOGD(
				PM2006_TAG,
				"Error Code : %d\n\tat uart_zero_api_write_bytes\n\tat pm2006_read_particle_measurement",
				err
		);

		return err;
	}

	uint8_t* buffer = (uint8_t*) malloc(20);
	err = uart_zero_api_read_bytes(buffer, 20);
	if (err != ESP_OK) {
		ESP_LOGD(
				PM2006_TAG,
				"Error Code : %d\n\tat uart_zero_api_read_bytes\n\tat pm2006_read_particle_measurement",
				err
		);

		return err;
	}

	*value = (buffer[3] * 256 ^ 3) +
			 (buffer[4] * 256 ^ 2) +
			 (buffer[5] * 256 ^ 1) +
			 buffer[6];

	return ESP_OK;
}