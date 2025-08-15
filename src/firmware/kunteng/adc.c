/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */
#include <stdint.h>
#include "adc.h"
#include "kunteng/interrupt.h"
#include "kunteng/stm8.h"
#include "kunteng/pins.h"
#include "kunteng/stm8s/stm8s_adc1.h"
#include "kunteng/stm8s/stm8s.h"

/* XXX u16	avg_ishunt_current_buffer[ADC_CURRENT_SAMPLES] = { 0 };
u8	avg_ishunt_current_index = 0;*/
u16	avg_battery_current_buffer[ADC_CURRENT_SAMPLES] = { 0 };
u8	avg_battery_current_index = 0;
u16	avg_voltage_buffer[ADC_VOLTAGE_SAMPLES] = { 0 };
u8	avg_voltage_index = 0;

volatile uint8_t adc_throttle = 0;
volatile uint16_t adc_battery_voltage = 0;
/* XXX volatile uint16_t adc_ishunt_current = 0;*/
volatile uint16_t adc_battery_current = 0;
/* XXX volatile uint16_t adc_phaseb_current = 0;*/

/* cached variables read from voltatile uint16_t vars while ADC1 interrupt disabled */
static uint16_t adc_battery_voltage_cache = 0;
/* XXX static uint16_t adc_ishunt_current_cache = 0;*/
static uint16_t adc_battery_current_cache = 0;
/* XXX static uint16_t adc_phaseb_current_cache = 0;*/


void
adc_init(void)
{

	/*
	 * NOTE:
	 * adc configuration (except ADC1->CR1) is overwritten in motor.c/isr_timer1_cmp
	 * which triggeres the conversion.
	 *
	 * The motor control interrupt routines performs single mode
	 * adc conversion of battery current, reads the result and
	 * then starts buffered scan mode conversion of all adc channels
	 * with end of conversion interrupt enabled which is handled here.
	 */

	ADC1->CR1 = ADC1_PRESSEL_FCPU_D2;
	ADC1->CR2 = ADC1_ALIGN_LEFT;

	/* channel (none) */
	ADC1->CSR = 0x00;

	/* schmittrig disable all */
	ADC1->TDRL |= (u8)0xff;
	ADC1->TDRH |= (u8)0xff;

	/* enable the ADC1 peripheral */
	ADC1->CR1 |= ADC1_CR1_ADON;
}

void
adc_process(void)
{
	/*
	 * Have to disable interrupts globally since ADC1->CSR register
	 * is manipulated from motor control isr. Very short time, should have no effect.
	 */
	disableInterrupts();
	adc_battery_voltage_cache = adc_battery_voltage;
/* XXX	adc_ishunt_current_cache = adc_ishunt_current;*/
	adc_battery_current_cache = adc_battery_current;
/* XXX	adc_phaseb_current_cache = adc_phaseb_current;*/
	enableInterrupts();
}


uint8_t
adc_get_throttle(void)
{
	/* atomic read */
	return adc_throttle;
}

uint16_t
adc_get_torque(void)
{
	return 0;
}

uint16_t
adc_get_temperature_contr(void)
{
	return 0;
}

uint16_t
adc_get_temperature_motor(void)
{
	return 0;
}

uint16_t
adc_get_battery_voltage(void)
{
	/* 10 bit resolution */
	return adc_battery_voltage_cache;
}

#if 0
uint16_t
adc_update_avg_current_ishunt(void)
{
	u16 _sum = 0;

	/* Simple moving average */
	avg_ishunt_current_buffer[avg_ishunt_current_index++ & (ADC_CURRENT_SAMPLES - 1)] =
	    adc_ishunt_current_cache;

	for (u8 i = 0; i < ADC_CURRENT_SAMPLES; i++)
		_sum += avg_ishunt_current_buffer[i];
	return _sum >> ADC_CURRENT_AVGSHIFT;
}


uint16_t
adc_update_avg_current_battery(void)
{
	u16 _sum = 0;

	/* Simple moving average */
	avg_battery_current_buffer[avg_battery_current_index++ & (ADC_CURRENT_SAMPLES - 1)] =
	    adc_battery_current_cache;

	for (u8 i = 0; i < ADC_CURRENT_SAMPLES; i++)
		_sum += avg_battery_current_buffer[i];
	return _sum >> ADC_CURRENT_AVGSHIFT;
}


uint16_t
adc_update_avg_voltage_battery(void)
{
	u16 _sum = 0;

	/* Simple moving average */
	avg_voltage_buffer[avg_voltage_index++ & (ADC_VOLTAGE_SAMPLES - 1)] = adc_battery_voltage_cache;

	for (u8 i = 0; i < ADC_VOLTAGE_SAMPLES; i++)
		_sum += avg_voltage_buffer[i];
	return _sum >> ADC_VOLTAGE_AVGSHIFT;
}
#endif

/* XXX uint16_t
adc_get_avg_current_ishunt(void)
{
	u16 _sum = 0;

	for (u8 i = 0; i < ADC_CURRENT_SAMPLES; i++)
		_sum += avg_ishunt_current_buffer[i];
	return _sum >> ADC_CURRENT_AVGSHIFT;
}*/

uint16_t
adc_get_avg_current_battery(void)
{
	u16 _sum = 0;

	for (u8 i = 0; i < ADC_CURRENT_SAMPLES; i++)
		_sum += avg_battery_current_buffer[i];
	return _sum >> ADC_CURRENT_AVGSHIFT;
}

uint16_t
adc_get_avg_voltage_battery(void)
{
	u16 _sum = 0;

	for (u8 i = 0; i < ADC_VOLTAGE_SAMPLES; i++)
		_sum += avg_voltage_buffer[i];
	return _sum >> ADC_VOLTAGE_AVGSHIFT;
}


/*
 * adc_trigger_all == selective "manual scanning"
 * will change eoc for everything but battery voltage,
 * as it will trigger throttle in any case
 */
volatile uint8_t adc_trigger_all = 0;		/* XXX should fix, but not used atm. */

void
isr_adc1_eoc(void) __interrupt(ITC_IRQ_ADC1)
{

	SET_PIN_HIGH(DBG_P4_IOT);
	if (ADC1->CSR & ADC1_CSR_EOC &&
	    !(ADC1->CR2 & ADC1_CR2_SCAN)) {	/* NOT a scanned conversion */
		switch (ADC1->CSR & ADC1_CSR_CH) {
		case ADC1_CHANNEL_THROTTLE:
			adc_throttle = ADC1->DRH;

			if (adc_trigger_all)
				adc_trigger_eoc(ADC_CONT_CH);
			else	/* or go for _cont conversion on current */
				adc_trigger_cont(ADC_CONT_CH);
			break;
#if 0
		case ADC1_CHANNEL_PHASEB_CURRENT:
			adc_phaseb_current = (u16)(ADC1->DRH << 2) | ADC1->DRL;

			if (adc_trigger_all)
#if (ADC_CONT_CH == ADC1_CHANNEL_BATTERY_CURRENT)	/* to skip ishunt if it's read already */
				adc_trigger_eoc(ADC1_CHANNEL_ISHUNT_CURRENT);
#else
				adc_trigger_eoc(ADC1_CHANNEL_BATTERY_CURRENT);
#endif
			else
				ADC1->CSR = 0x00;
			break;
		case ADC1_CHANNEL_ISHUNT_CURRENT:
			adc_ishunt_current = (u16)(ADC1->DRH << 2) | ADC1->DRL;
#if 0
			avg_ishunt_current_buffer[avg_ishunt_current_index++ & (ADC_CURRENT_SAMPLES - 1)] =
			    adc_ishunt_current;
#endif

			if (adc_trigger_all)
				adc_trigger_eoc(ADC1_CHANNEL_BATTERY_CURRENT);
			else
#if (ADC_CONT_CH == ADC1_CHANNEL_ISHUNT_CURRENT)	/* XXX just for now so both get read */
				adc_trigger_eoc(ADC1_CHANNEL_BATTERY_CURRENT);
#else
				adc_trigger_cont(ADC_CONT_CH);
#endif
			break;
#endif
		case ADC1_CHANNEL_BATTERY_CURRENT:
			adc_battery_current = (u16)(ADC1->DRH << 2) | ADC1->DRL;
#if 0
			avg_battery_current_buffer[avg_battery_current_index++ & (ADC_CURRENT_SAMPLES - 1)] =
			    adc_battery_current;
#endif
			/*if (adc_trigger_all) XXX unnecessary check, likely cheaper w/o */
				adc_trigger_all = 0;	/* manual scan done */
			adc_trigger_cont(ADC_CONT_CH);
			break;
		case ADC1_CHANNEL_BATTERY_VOLTAGE:
			adc_battery_voltage = (u16)(ADC1->DRH << 2) | ADC1->DRL;
#if 0
			avg_voltage_buffer[avg_voltage_index++ & (ADC_VOLTAGE_SAMPLES - 1)] =
			    adc_battery_voltage;
#endif
			/* throttle is read unconditionally after battery voltage */
			adc_trigger_eoc(ADC1_CHANNEL_THROTTLE);
			break;
		default:
			ADC1->CSR = 0x00;	/* clear EOC flag and disable EOC interrupt */
			break;
		}
	} else
	if (ADC1->CSR & ADC1_CSR_EOC &&
	    (ADC1->CR2 & ADC1_CR2_SCAN)) {

		adc_throttle = ADC1->DB4RH;
		/* XXX adc_phaseb_current = (u16)(ADC1->DB5RH << 2) | ADC1->DB5RL;
		adc_ishunt_current = (u16)(ADC1->DB6RH << 2) | ADC1->DB6RL;*/
		adc_battery_current = (u16)(ADC1->DB8RH << 2) | ADC1->DB8RL;
		adc_battery_voltage = (u16)(ADC1->DB9RH << 2) | ADC1->DB9RL;

		ADC1->CR2 = (u8)ADC1_ALIGN_LEFT;	/* disable scanning */
		ADC1->CSR = 0x00;		/* clear EOC flag and disable EOC interrupt */
	}
	SET_PIN_LOW(DBG_P4_IOT);
}