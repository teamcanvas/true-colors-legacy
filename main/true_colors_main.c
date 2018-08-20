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

#define TRUE_COLORS_TAG "TRUE COLORS"

void cm1106_task(void *parameter) {
	uint16_t value = 0;
	esp_err_t err;

	while(1) {
		err = cm1106_measure_result(&value);
		if (err == ESP_OK) {
			ESP_LOGI(TRUE_COLORS_TAG, "CM1106 Value :%d\n", value);
		} else {
			ESP_LOGI(TRUE_COLORS_TAG, "CM1106 measurement failed");
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void pm2006_task(void *parameter) {
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

    xTaskCreate(&cm1106_task, "cm1106_task", 2048, NULL, 1, NULL);
    xTaskCreate(&pm2006_task, "pm2006_task", 2048, NULL, 1, NULL);
}
