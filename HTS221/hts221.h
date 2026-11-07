/*
 * hts221.h
 *
 *  Created on: 7. 11. 2026
 *      Author: Ekaterina
 */

#ifndef HTS221_H_
#define HTS221_H_

#include <stdint.h>

#define HTS221_DEVICE_ADDRESS   0xBE

#define HTS221_WHO_AM_I         0x0F
#define HTS221_CTRL_REG1        0x20
#define HTS221_HUMIDITY_OUT_L   0x28
#define HTS221_TEMP_OUT_L       0x2A
#define HTS221_CALIB_START      0x30

#define HTS221_WHO_AM_I_VALUE   0xBC
#define HTS221_AUTO_INCREMENT   0x80
#define HTS221_CTRL_REG1_CONFIG 0x85

typedef uint8_t (*hts221_read_t)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);
typedef uint8_t (*hts221_write_t)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);

uint8_t hts221_init(hts221_read_t read_cb, hts221_write_t write_cb);
float   hts221_get_temperature(void);
float   hts221_get_humidity(void);

#endif
