/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */

#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "intellisense.h"
#include <stdint.h>
#include <stdbool.h>

void lights_init(void);

void lights_enable(void);
void lights_disable(void);

void lights_set(bool on);

#endif