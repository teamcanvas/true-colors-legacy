/*
 * i2c slave communication header
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.14
 */

#ifndef __I2C_CUST_H__
#define __I2C_CUST_H__

#include "esp_err.h"
#include "driver/i2c.h"

#define I2C_MASTER_USER_NUM 		I2C_NUM_1
#define I2C_MASTER_DATA_BUF_LEN 	512

#define I2C_MASTER_SDA_PIN 			GPIO_NUM_33
#define I2C_MASTER_SCL_PIN 			GPIO_NUM_32

#define I2C_MASTER_TX_BUF_DISABLE 	0
#define I2C_MASTER_RX_BUF_DISABLE 	0
#define I2C_MASTER_FREQ_HZ 			400000

#define I2C_ACK_CHECK_ENABLE 		0x1
#define I2C_ACK_CHECK_DISABLE 		0x0
#define I2C_ACK_VAL 				0x0
#define I2C_NACK_VAL 				0x1

esp_err_t i2c_api_master_init();
esp_err_t i2c_api_master_write_bytes(uint8_t addr, uint8_t *data, size_t length);
esp_err_t i2c_api_master_read_bytes(uint8_t addr, uint8_t *data, size_t length);

#endif