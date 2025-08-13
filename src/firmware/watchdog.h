/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */

#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include "intellisense.h"
#include <stdint.h>
#include <stdbool.h>

void watchdog_init(void);
void watchdog_yeild(void);

bool watchdog_triggered(void);

#endif