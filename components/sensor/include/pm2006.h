/*
 * pm2006 header
 *
 * @author atkodev <me@atko.space>
 * @date   2018.08.18
 */

#ifndef __PM2006_H__
#define __PM2006_H__

#include "esp_err.h"

#define PM2006_TAG "PM2006"

esp_err_t pm2006_open_measurement();
esp_err_t pm2006_read_particle_measurement(uint16_t *value);

#endif