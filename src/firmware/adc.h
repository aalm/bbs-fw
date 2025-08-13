/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */

#ifndef _ADC_H_
#define _ADC_H_
#include <stdint.h>

void adc_init(void);
void adc_process(void);

uint8_t adc_get_throttle(void);
uint16_t adc_get_torque(void);

uint16_t adc_get_temperature_contr(void);
uint16_t adc_get_temperature_motor(void);

uint16_t adc_get_battery_voltage(void);

#endif