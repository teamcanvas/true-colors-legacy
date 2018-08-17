/*
 * i2c slave communication source
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.14
 */

#include "i2c_api.h"

#include "esp_err.h"
#include "esp_log.h"

#include "driver/i2c.h"

static const char* I2C_MASTER_TAG = "I2C_MASTER";

esp_err_t i2c_api_master_init() {
	i2c_config_t conf;

	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = I2C_MASTER_SDA_PIN;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = I2C_MASTER_SCL_PIN;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

	esp_err_t err;

	err = i2c_param_config(I2C_MASTER_USER_NUM, &conf);
	if(err != ESP_OK) {
		ESP_LOGD(I2C_MASTER_TAG, "Error Code : %d\n\tat i2c_param_config\n\tat i2c_master_init", err);
		return err;
	}

	err = i2c_driver_install(I2C_MASTER_USER_NUM, conf.mode, 0, 0, 0);
	vTaskDelay(200 / portTICK_PERIOD_MS);

	if(err != ESP_OK) {
		ESP_LOGD(I2C_MASTER_TAG, "Error code %d\n\tat i2c_driver_install\n\tat i2c_master_init", err);
		return err;
	}

	return ESP_OK;
}

esp_err_t i2c_api_master_write_bytes(uint8_t addr, uint8_t *data, size_t length) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	esp_err_t err;

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, I2C_ACK_CHECK_ENABLE);
	i2c_master_write(cmd, data, length, I2C_ACK_CHECK_ENABLE);
	i2c_master_stop(cmd);

	err = i2c_master_cmd_begin(I2C_MASTER_USER_NUM, cmd, 1000 / portTICK_RATE_MS);
	if(err != ESP_OK) {
		ESP_LOGD(I2C_MASTER_TAG, "Error Code : %d\n\tat i2c_master_cmd_begin\n\tat i2c_api_master_write_bytes", err);
		i2c_cmd_link_delete(cmd);
		return err;
	}

	i2c_cmd_link_delete(cmd);

	return ESP_OK;
}

esp_err_t i2c_api_master_read_bytes(uint8_t addr, uint8_t *data, size_t length) {
	if (length == 0) {
        return ESP_OK;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, I2C_ACK_CHECK_ENABLE);
    if (length > 1) {
        i2c_master_read(cmd, data, length - 1, I2C_ACK_VAL);
    }
    i2c_master_read_byte(cmd, data + length - 1, I2C_NACK_VAL);
    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_USER_NUM, cmd, 1000 / portTICK_RATE_MS);
    if(err != ESP_OK) {
		ESP_LOGD(I2C_MASTER_TAG, "Error Code : %d\n\tat i2c_master_cmd_begin\n\tat i2c_api_master_read_bytes", err);
		i2c_cmd_link_delete(cmd);
		return err;
	}

    i2c_cmd_link_delete(cmd);

    return ESP_OK;
}