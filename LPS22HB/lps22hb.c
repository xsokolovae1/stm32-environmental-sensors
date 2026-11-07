/*
 * lps22hb.c
 *
 *  Created on: 7. 11. 2026
 *      Author: Ekaterina
 */


#include "lps22hb.h"
#include <math.h>

static lps22hb_read_t  lps22hb_read_cb  = 0;
static lps22hb_write_t lps22hb_write_cb = 0;


static uint8_t lps22hb_read(uint8_t reg, uint8_t *data, uint8_t len)
{
	return lps22hb_read_cb(LPS22HB_DEVICE_ADDRESS, reg, data, len);
}

static uint8_t lps22hb_write(uint8_t reg, uint8_t *data, uint8_t len)
{
	return lps22hb_write_cb(LPS22HB_DEVICE_ADDRESS, reg, data, len);
}


uint8_t lps22hb_init(lps22hb_read_t read_cb, lps22hb_write_t write_cb)
{
	if(read_cb == 0 || write_cb == 0) return 0;
	lps22hb_read_cb  = read_cb;
	lps22hb_write_cb = write_cb;

	uint8_t who_am_i = 0;
	if(!lps22hb_read(LPS22HB_WHO_AM_I, &who_am_i, 1) || who_am_i != LPS22HB_WHO_AM_I_VALUE)
		return 0;

	uint8_t ctrl = LPS22HB_CTRL_REG1_CONFIG;
	lps22hb_write(LPS22HB_CTRL_REG1, &ctrl, 1);

	return 1;
}


float lps22hb_get_pressure(void)
{
	uint8_t d[3];
	lps22hb_read(LPS22HB_PRESS_OUT_XL, d, 3);

	int32_t raw = (int32_t)((uint32_t)d[2] << 16 | (uint32_t)d[1] << 8 | d[0]);
	if(raw & 0x00800000) raw |= 0xFF000000;

	return raw / 4096.0f;
}


float lps22hb_get_altitude(float pressure, float pressure_ref)
{
	return 44330.0f * (1.0f - powf(pressure / pressure_ref, 1.0f / 5.255f));
}
