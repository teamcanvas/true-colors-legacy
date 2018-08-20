/*
 * cm1106 sensor
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.15
 *
 */

#include "esp_err.h"
#include "esp_log.h"
#include "i2c_api.h"

#include "cm1106.h"

static const char* CM1106_TAG = "CM1106";

esp_err_t cm1106_measure_result(uint16_t *value) {
	uint8_t cmd[1] = { CM1106_CMD_MEASURE_RESULT };
	uint8_t rcv[5];

	esp_err_t err = i2c_api_master_write_bytes(CM1106_I2C_ADDRESS, cmd, 1);
	if (err != ESP_OK) {
		ESP_LOGD(CM1106_TAG, "Error Code : %d\n\tat i2c_api_master_write_bytes\n\tat cm1106_measure_result", err);
		return err;
	}

	vTaskDelay(80 / portTICK_PERIOD_MS);

	err = i2c_api_master_read_bytes(CM1106_I2C_ADDRESS, rcv, 5);
	if (err != ESP_OK) {
		ESP_LOGD(CM1106_TAG, "Error Code : %d\n\tat i2c_api_master_read_bytes\n\tat cm1106_measure_result", err);
		return err;
	}

	ESP_LOGD(CM1106_TAG, "CM1106 Status Code : %d", rcv[3]);

	*value = rcv[1] * 256 + rcv[2];

	vTaskDelay(80 / portTICK_PERIOD_MS);

	return ESP_OK;
}