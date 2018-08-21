/*
 * bme680 source
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.20
 */

#include "bme680_api.h"
#include "esp_err.h"
#include "esp_log.h"

#include "i2c_api.h"

struct bme680_dev gas_sensor;

esp_err_t bme680_api_init() {
	int8_t rslt = BME680_OK;

    gas_sensor.dev_id = BME680_I2C_ADDR_PRIMARY;
    gas_sensor.intf = BME680_I2C_INTF;
    gas_sensor.read = bme680_api_user_i2c_read;
    gas_sensor.write = bme680_api_user_i2c_write;
    gas_sensor.delay_ms = bme680_api_user_delay_ms;
    gas_sensor.amb_temp = 25;

    rslt = bme680_init(&gas_sensor);
    if (rslt != BME680_OK) {
    	ESP_LOGD(BME680_TAG, "Error Code : %d\n\tat bme680_init\n\tat bme680_api_init", rslt);
    	return ESP_FAIL;
    }

    uint8_t set_required_settings;

    /* Set the temperature, pressure and humidity settings */
    gas_sensor.tph_sett.os_hum = BME680_OS_2X;
    gas_sensor.tph_sett.os_pres = BME680_OS_4X;
    gas_sensor.tph_sett.os_temp = BME680_OS_8X;
    gas_sensor.tph_sett.filter = BME680_FILTER_SIZE_3;

    /* Set the remaining gas sensor settings and link the heating profile */
    gas_sensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
    /* Create a ramp heat waveform in 3 steps */
    gas_sensor.gas_sett.heatr_temp = 320; /* degree Celsius */
    gas_sensor.gas_sett.heatr_dur = 150; /* milliseconds */

    /* Select the power mode */
    /* Must be set before writing the sensor configuration */
    gas_sensor.power_mode = BME680_FORCED_MODE; 

    /* Set the required sensor settings needed */
    set_required_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

    /* Set the desired sensor configuration */
    rslt = bme680_set_sensor_settings(set_required_settings, &gas_sensor);
    if (rslt != BME680_OK) {
    	ESP_LOGD(BME680_TAG, "Error Code : %d\n\tat bme680_set_sensor_settings\n\tat bme680_api_init", rslt);
    	return ESP_FAIL;
    }

    /* Set the power mode */
    rslt = bme680_set_sensor_mode(&gas_sensor);
    if (rslt != BME680_OK) {
    	ESP_LOGD(BME680_TAG, "Error Code : %d\n\tat bme680_set_sensor_mode\n\tat bme680_api_init", rslt);
    	return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t bme680_api_measurement(struct bme680_field_data *data) {
    uint16_t meas_period;
    bme680_get_profile_dur(&meas_period, &gas_sensor);

    bme680_api_user_delay_ms(meas_period);

    int8_t rslt = BME680_OK;
    rslt = bme680_get_sensor_data(data, &gas_sensor);
    if(rslt != BME680_OK) {
    	ESP_LOGD(BME680_TAG, "Error Code : %d\n\tat bme680_set_sensor_data\n\tat bme680_api_measurement", rslt);
		return ESP_FAIL;
    }

    if (gas_sensor.power_mode == BME680_FORCED_MODE) {
        rslt = bme680_set_sensor_mode(&gas_sensor);
        if(rslt != BME680_OK) {
        	ESP_LOGD(BME680_TAG, "Error Code : %d\n\tat bme680_set_sensor_mode\n\tat bme680_api_measurement", rslt);
    		return ESP_FAIL;
        }
    }

    return ESP_OK;
}

void bme680_api_user_delay_ms(uint32_t period) {
	vTaskDelay(period / portTICK_PERIOD_MS);
}

int8_t bme680_api_user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) {
	/*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Stop       | -                   |
     * | Start      | -                   |
     * | Read       | (reg_data[0])       |
     * | Read       | (....)              |
     * | Read       | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */

	esp_err_t err;

	err = i2c_api_master_write_bytes(dev_id, &reg_addr, 1);
	if (err != ESP_OK) {
    	ESP_LOGD(BME680_TAG, "Error Code : %d\n\tat i2c_api_master_write_bytes\n\tat bme680_api_user_i2c_read", err);
		return 1;
	}

	bme680_api_user_delay_ms(20);

	i2c_api_master_read_bytes(dev_id, reg_data, len);
	if (err != ESP_OK) {
    	ESP_LOGD(BME680_TAG, "Error Code : %d\n\tat i2c_api_master_read_bytes\n\tat bme680_api_user_i2c_read", err);
		return 1;
	}

	return 0;
}

int8_t bme680_api_user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) {
	/*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Write      | (reg_data[0])       |
     * | Write      | (....)              |
     * | Write      | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */

	esp_err_t err;
	uint8_t *cmd = (uint8_t*) malloc(len + 1);

	cmd[0] = reg_addr;
	for (int i = 0; i < len; i++) {
		cmd[i + 1] = *(reg_data + i);
	}

	err = i2c_api_master_write_bytes(dev_id, cmd, len + 1);
	if (err != ESP_OK) {
    	ESP_LOGD(BME680_TAG, "Error Code : %d\n\tat i2c_api_master_read_bytes\n\tat bme680_api_user_i2c_read", err);
    	free(cmd);
		return 1;
	}

	free(cmd);
	return 0;
}