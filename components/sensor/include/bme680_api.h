/*
 * bme680 api
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.20
 */

#ifndef __BME680_API_H__
#define __BME680_API_H__

#include "bme680_defs.h"
#include "bme680.h"

#include "esp_err.h"

#define BME680_TAG "BME680"

esp_err_t bme680_api_init();
esp_err_t bme680_api_measurement(struct bme680_field_data *bme680_data);

void bme680_api_user_delay_ms(uint32_t period);
int8_t bme680_api_user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t bme680_api_user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

#endif