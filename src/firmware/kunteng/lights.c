/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */

#include "fwconfig.h"
#include "lights.h"
#include "stm8.h"
#include "pins.h"

static bool lights_enabled;
static bool lights_on;

void
lights_init(void)
{
#if HAS_LIGHTS
	SET_PIN_OUTPUT(LIGHT_IOT);
#endif

	lights_enabled = false;
	lights_set(false);
}

void
lights_enable(void)
{
	lights_enabled = true;
	lights_set(lights_on);
}

void
lights_disable(void)
{
	lights_enabled = false;
	lights_set(lights_on);
}

void
lights_set(bool on)
{
	lights_on = on;
#if HAS_LIGHTS
	if (lights_on && lights_enabled) {
		SET_PIN_HIGH(LIGHT_IOT);
	} else {
		SET_PIN_LOW(LIGHT_IOT);
	}
#endif
}
