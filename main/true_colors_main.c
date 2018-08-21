/*
 * colors hardware firmware main
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.14
 */

#include <stdio.h>

#include "esp_err.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "i2c_api.h"
#include "uart_zero_api.h"

#include "cm1106.h"
#include "pm2006.h"
#include "bme680_api.h"

#define TRUE_COLORS_TAG "TRUE COLORS"

void i2c_task(void *parameter) {
	uint16_t value = 0;
	esp_err_t err;
	struct bme680_field_data data;

	bme680_api_init();

	while(1) {
		err = cm1106_measure_result(&value);
		if (err == ESP_OK) {
			ESP_LOGI(TRUE_COLORS_TAG, "CM1106 Value :%d\n", value);
		} else {
			ESP_LOGI(TRUE_COLORS_TAG, "CM1106 measurement failed");
		}

		err = bme680_api_measurement(&data);
		if (err == ESP_OK) {
			ESP_LOGI(TRUE_COLORS_TAG, "bme680->temperature : %.2f", data.temperature / 100.0f);
			ESP_LOGI(TRUE_COLORS_TAG, "bme680->humidity : %.2f", data.humidity / 1000.0f);
			ESP_LOGI(TRUE_COLORS_TAG, "bme680->pressure : %.2f", data.pressure / 100.0f);

			if(data.status & BME680_GASM_VALID_MSK)
				ESP_LOGI(TRUE_COLORS_TAG, "bme680->gas : %d", data.gas_resistance);
			
		} else {
			ESP_LOGI(TRUE_COLORS_TAG, "BME680 measurement failed");
		}


		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void uart_task(void *parameter) {
	uint16_t value = 0;
	esp_err_t err;

	pm2006_open_measurement();

	while(1) {
		err = pm2006_read_particle_measurement(&value);
		if (err == ESP_OK) {
			ESP_LOGI(TRUE_COLORS_TAG, "PM2006 Value :%d\n", value);
		} else {
			ESP_LOGI(TRUE_COLORS_TAG, "PM2006 measurement failed");
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void app_main()
{
	i2c_api_master_init();
	uart_zero_api_init();

    xTaskCreate(&i2c_task, "i2c_task", 2048, NULL, 1, NULL);
    xTaskCreate(&uart_task, "uart_task", 2048, NULL, 1, NULL);
}
