/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */

#include "sensors.h"
#include "intellisense.h"
#include "fwconfig.h"
#include "kunteng/interrupt.h"
#include "kunteng/timers.h"
#include "kunteng/stm8.h"
#include "kunteng/pins.h"
#include "kunteng/stm8s/stm8s_tim4.h"

/* interrupt runs at 100us interval, see timer4 setup in timers.c */

/* XXX :TODO: this file contains a lot of duplicated code from bbsx version, try to share code */

#define PAS_SENSOR_NUM_SIGNALS			PAS_PULSES_REVOLUTION
#define PAS_SENSOR_MIN_PULSE_MS_X10		50	/* 500rpm limit */

#define SPEED_SENSOR_MIN_PULSE_MS_X10		500
#define SPEED_SENSOR_TIMEOUT_MS_X10		25000

#if /* XXX PAS */0
static volatile uint16_t pas_pulse_counter;
static volatile bool pas_direction_backward;
static volatile uint16_t pas_period_length;	// pulse length counted in interrupt frequency (100us)
static uint16_t pas_period_counter;
static bool pas_prev1;
static bool pas_prev2;
static uint16_t pas_stop_delay_periods;
#endif

static volatile uint16_t speed_ticks_period_length; // pulse length counted in interrupt frequency (100us)
static uint16_t speed_period_counter;
static bool speed_prev_state;
static uint8_t speed_ticks_per_rpm;

volatile uint8_t brake_was_set = 0;
volatile uint8_t brake_counter = 100;

void
sensors_init(void)
{

#if /* XXX PAS */0
	pas_period_counter = 0;
	pas_pulse_counter = 0;
	pas_direction_backward = false;
	pas_period_length = 0;
	pas_stop_delay_periods = 1500;

#endif
	speed_period_counter = 0;
	speed_ticks_period_length = 0;
	speed_prev_state = false;
	speed_ticks_per_rpm = 1;

	/* pins do not have external interrupt, use timer4 to evaluate state frequently */
	timer4_init_sensors();
}

void
sensors_process(void)
{

	/* process brake */
	if (brake_was_set) {
		if (brake_counter) {
			brake_counter--;
		} else {
			brake_counter = 100;
			SET_PIN_LOW(DBG_P3_IOT);
			brake_was_set = 0;
		}
	}
}

bool
torque_sensor_ok(void)
{
	return true;
}

void
pas_set_stop_delay(uint16_t delay_ms)
{
/* XXX	pas_stop_delay_periods = delay_ms * 10;*/
}

uint16_t
pas_get_cadence_rpm_x10(void)
{
	return 0;
}

uint16_t
pas_get_pulse_counter(void)
{
	return 0;
}

bool
pas_is_pedaling_forwards(void)
{
	return false;
}

bool
pas_is_pedaling_backwards(void)
{
	return false;
}

void
speed_sensor_set_signals_per_rpm(uint8_t num_signals)
{
	speed_ticks_per_rpm = num_signals;
}

bool
speed_sensor_is_moving(void)
{
	uint16_t tmp;
	TIM4->IER &= ~TIM4_IT_UPDATE;	/* disable timer4 interrupts */
	tmp = speed_ticks_period_length;
	TIM4->IER |= TIM4_IT_UPDATE;

	return tmp > 0;
}

uint16_t
speed_sensor_get_rpm_x10(void)
{
	uint16_t tmp;

	TIM4->IER &= ~TIM4_IT_UPDATE;	/* disable timer4 interrupts */
	tmp = speed_ticks_period_length;
	TIM4->IER |= TIM4_IT_UPDATE;

	if (tmp > 0)
		return 6000000ul / tmp / speed_ticks_per_rpm;
	return 0;
}

int16_t
temperature_contr_x100(void)
{
	return 0;
}

int16_t
temperature_motor_x100(void)
{
	return 0;
}

bool
brake_is_activated(void)
{
	return !GET_PIN_INPUT_STATE(BRAKE_IOT);
}

bool
shift_sensor_is_activated(void)
{
	return false;
}

void
isr_porta_exti(void) __interrupt(ITC_IRQ_PORTA)
{
	volatile bool brake = GET_PIN_INPUT_STATE(BRAKE_IOT) ? false : true;	/* active low */

	if (brake) {
		SET_PIN_HIGH(DBG_P3_IOT);
		brake_was_set |= 0x01;
	} else {
		/* XXX to be cleared from main()-loop, for small delay/smoothness ? */
		/* this is handled from sensors_process() */
	}
}

void
isr_timer4_ovf(void) __interrupt(ITC_IRQ_TIM4_OVF)
{

	/* XXX PAS was here */

	/* Speed sensor */
	volatile bool spd = GET_PIN_INPUT_STATE(SPEED_IOT);
	if (spd && !speed_prev_state &&
	    speed_period_counter > SPEED_SENSOR_MIN_PULSE_MS_X10) {
		if (speed_period_counter <= SPEED_SENSOR_TIMEOUT_MS_X10)
			speed_ticks_period_length = speed_period_counter;
		else
			speed_ticks_period_length = 0;

		speed_period_counter = 0;
	} else {
		/* Do not allow wraparound or computed speed will wrong after bike has been still. */
		if (speed_period_counter < 65535)
			speed_period_counter++;

		if (speed_period_counter > SPEED_SENSOR_TIMEOUT_MS_X10 &&
		    speed_ticks_period_length > 0)
			speed_ticks_period_length = 0;
	}
	speed_prev_state = spd;

	/* clear interrupt bit */
	TIM4->SR1 &= (uint8_t)(~TIM4_IT_UPDATE);
}