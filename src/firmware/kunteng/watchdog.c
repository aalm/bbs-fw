/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */

#include "watchdog.h"
#include "kunteng/cpu.h"
#include "kunteng/stm8s/stm8s_iwdg.h"

static bool triggered;

void
watchdog_init(void)
{
	/* :TODO: implement if possible, check if reset triggered by watchdog */
	triggered = false;	/* XXX is possible to check, iirc. maybe RST (stm8s_rst.h) */

	IWDG->KR = 0xcc;	/* start */
	IWDG->KR = 0x55;	/* unlock */
	IWDG->PR = 6;		/* divide by 256 */
	IWDG->RLR = 156;	/* reload to 625 milliseconds */

	watchdog_yeild();
}

void
watchdog_yeild(void)
{
	IWDG->KR = 0xaa;
}

bool
watchdog_triggered(void)
{
	return triggered;
}