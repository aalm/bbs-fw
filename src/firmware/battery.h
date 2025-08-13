/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */

#ifndef _BATTERY_H_
#define _BATTERY_H_

#include <stdint.h>

void battery_init(void);
void battery_process(void);

uint8_t battery_get_percent(void);
uint8_t battery_get_mapped_percent(void);

#endif