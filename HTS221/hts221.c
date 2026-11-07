/*
 * hts221.c
 *
 *  Created on: 7. 11. 2026
 *      Author: Ekaterina
 */

#include "hts221.h"

static hts221_read_t  hts221_read_cb  = 0;
static hts221_write_t hts221_write_cb = 0;

static float   T0_degC, T1_degC, H0_rH, H1_rH;
static int16_t T0_OUT, T1_OUT, H0_T0_OUT, H1_T0_OUT;


static uint8_t hts221_read(uint8_t reg, uint8_t *data, uint8_t len)
{
	if(len > 1) reg |= HTS221_AUTO_INCREMENT;
	return hts221_read_cb(HTS221_DEVICE_ADDRESS, reg, data, len);
}

static uint8_t hts221_write(uint8_t reg, uint8_t *data, uint8_t len)
{
	if(len > 1) reg |= HTS221_AUTO_INCREMENT;
	return hts221_write_cb(HTS221_DEVICE_ADDRESS, reg, data, len);
}


uint8_t hts221_init(hts221_read_t read_cb, hts221_write_t write_cb)
{
	if(read_cb == 0 || write_cb == 0) return 0;
	hts221_read_cb  = read_cb;
	hts221_write_cb = write_cb;

	uint8_t who_am_i = 0;
	if(!hts221_read(HTS221_WHO_AM_I, &who_am_i, 1) || who_am_i != HTS221_WHO_AM_I_VALUE)
		return 0;

	uint8_t ctrl = HTS221_CTRL_REG1_CONFIG;
	hts221_write(HTS221_CTRL_REG1, &ctrl, 1);

	uint8_t c[16];
	hts221_read(HTS221_CALIB_START, c, 16);

	H0_rH   = c[0] / 2.0f;
	H1_rH   = c[1] / 2.0f;
	T0_degC = (((c[5] & 0x03) << 8) | c[2]) / 8.0f;
	T1_degC = (((c[5] & 0x0C) << 6) | c[3]) / 8.0f;

	H0_T0_OUT = (int16_t)(c[7]  << 8 | c[6]);
	H1_T0_OUT = (int16_t)(c[11] << 8 | c[10]);
	T0_OUT    = (int16_t)(c[13] << 8 | c[12]);
	T1_OUT    = (int16_t)(c[15] << 8 | c[14]);

	return 1;
}


float hts221_get_temperature(void)
{
	uint8_t d[2];
	hts221_read(HTS221_TEMP_OUT_L, d, 2);
	int16_t t_out = (int16_t)(d[1] << 8 | d[0]);

	return T0_degC + (t_out - T0_OUT) * (T1_degC - T0_degC) / (T1_OUT - T0_OUT);
}


float hts221_get_humidity(void)
{
	uint8_t d[2];
	hts221_read(HTS221_HUMIDITY_OUT_L, d, 2);
	int16_t h_out = (int16_t)(d[1] << 8 | d[0]);

	float h = H0_rH + (h_out - H0_T0_OUT) * (H1_rH - H0_rH) / (H1_T0_OUT - H0_T0_OUT);

	if(h < 0.0f)   h = 0.0f;
	if(h > 100.0f) h = 100.0f;
	return h;
}
