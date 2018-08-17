/*
 * cm1106 sensor header
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.15
 */

#ifndef __CM1106_H__
#define __CM1106_H__

#include "esp_err.h"

#define CM1106_I2C_ADDRESS 0x31

#define CM1106_CMD_MEASURE_RESULT 0x01
#define CM1106_CMD_ZERO_SETTING 0x02
#define CM1106_CMD_CHECK_SOFTWARE_VERSION 0x03

esp_err_t cm1106_measure_result(uint16_t *value);

#endif