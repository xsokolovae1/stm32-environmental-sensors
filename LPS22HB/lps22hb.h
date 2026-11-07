/*
 * lps22hb.h
 *
 *  Created on: 7. 11. 2026
 *      Author: Ekaterina
 */

#ifndef LPS22HB_H_
#define LPS22HB_H_

#include <stdint.h>

#define LPS22HB_DEVICE_ADDRESS   0xBA

#define LPS22HB_WHO_AM_I         0x0F
#define LPS22HB_CTRL_REG1        0x10
#define LPS22HB_PRESS_OUT_XL     0x28

#define LPS22HB_WHO_AM_I_VALUE   0xB1
#define LPS22HB_CTRL_REG1_CONFIG 0x2A

typedef uint8_t (*lps22hb_read_t)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);
typedef uint8_t (*lps22hb_write_t)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);

uint8_t lps22hb_init(lps22hb_read_t read_cb, lps22hb_write_t write_cb);
float   lps22hb_get_pressure(void);
float   lps22hb_get_altitude(float pressure, float pressure_ref);

#endif
