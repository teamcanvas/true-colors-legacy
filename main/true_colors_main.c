/*
 * colors hardware firmware main
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.14
 */

#include <stdio.h>

#include "esp_err.h"
#include "esp_log.h"

#include "i2c_api.h"
#include "cm1106.h"

void app_main()
{
	i2c_api_master_init();
	esp_err_t err;
	while(1) {
		uint16_t value = 0;
		err = cm1106_measure_result(&value);
		if (err == ESP_OK) {
			printf("CM1106 Value :%d\n", value);
		} else {
			printf("CM1106 measurement failed");
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
