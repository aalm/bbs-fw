#pragma once

#ifndef _KUNTENG_STM_8_H_
#define _KUNTENG_STM_8_H_

#include <stdint.h>

#include "kunteng/stm8s/stm8s.h"

#define EXPAND(x) x

#define SET_PIN_INPUT_(PORT,PIN)		\
    PORT->DDR &= (u8)(~(PIN));			\
    PORT->CR1 &= (u8)(~(PIN))
#define SET_PIN_INPUT(...)			EXPAND(SET_PIN_INPUT_(__VA_ARGS__))

#define	SET_PIN_INPUT_IT_(PORT,PIN)		\
    PORT->DDR &= (u8)(~(PIN)); 			\
    PORT->CR1 &= (u8)(~(PIN));			\
    PORT->CR2 |= (u8)PIN
#define	SET_PIN_INPUT_IT(...)			EXPAND(SET_PIN_INPUT_IT_(__VA_ARGS__))

#define SET_PIN_INPUT_PULLUP_(PORT,PIN)		\
    PORT->DDR &= (u8)(~(PIN));			\
    PORT->CR1 |= (u8)PIN
#define SET_PIN_INPUT_PULLUP(...)		EXPAND(SET_PIN_INPUT_PULLUP_(__VA_ARGS__))

#define	SET_PIN_INPUT_PULLUP_IT_(PORT,PIN) 	\
    PORT->DDR &= (u8)(~(PIN));			\
    PORT->CR1 |= (u8)PIN;			\
    PORT->CR2 |= (u8)PIN
#define	SET_PIN_INPUT_PULLUP_IT(...)		EXPAND(SET_PIN_INPUT_PULLUP_IT_(__VA_ARGS__))

#define SET_PIN_OUTPUT_(PORT,PIN)		\
    PORT->DDR |= (u8)PIN;			\
    PORT->CR1 |= (u8)PIN;			\
    PORT->CR2 |= (u8)(PIN)
#define SET_PIN_OUTPUT(...)			EXPAND(SET_PIN_OUTPUT_(__VA_ARGS__))

#define SET_PIN_OUTPUT_OPEN_DRAIN_(PORT,PIN)	\
    PORT->DDR |= (u8)PIN;			\
    PORT->CR1 &= (u8)(~(PIN));		\
    PORT->CR2 |= (u8)(PIN)
#define SET_PIN_OUTPUT_OPEN_DRAIN(...)		EXPAND(SET_PIN_OUTPUT_OPEN_DRAIN_(__VA_ARGS__))


#define GET_PIN_INPUT_STATE_(PORT,PIN)	((PORT->IDR & (uint8_t)PIN) != 0)
#define GET_PIN_INPUT_STATE(...)		EXPAND(GET_PIN_INPUT_STATE_(__VA_ARGS__))

#define	GET_PINS_INPUT_STATE_(_PORT,_PINS)	(u8)(_PORT->IDR & (u8)_PINS)
#define	GET_PINS_INPUT_STATE(_IOT)		GET_PINS_INPUT_STATE_(_IOT)

#define SET_PIN_HIGH_(PORT,PIN)		PORT->ODR |= (uint8_t)PIN
#define SET_PIN_HIGH(...)			EXPAND(SET_PIN_HIGH_(__VA_ARGS__))

#define SET_PIN_LOW_(PORT,PIN)		PORT->ODR &= (uint8_t)(~PIN)
#define SET_PIN_LOW(...)			EXPAND(SET_PIN_LOW_(__VA_ARGS__))

#define TOGGLE_PIN_(PORT,PIN)		PORT->ODR ^= (PIN)
#define TOGGLE_PIN(...)				EXPAND(TOGGLE_PIN_(__VA_ARGS__))


#define GET_PIN_(PORT,PIN)		PIN
#define GET_PIN(...)				EXPAND(GET_PIN_(__VA_ARGS__))

#define GET_PORT_(PORT,PIN)		PORT
#define GET_PORT(...)				EXPAND(GET_PORT_(__VA_ARGS__))

/* XXX unfortunately seems like the best place for these - atleast for now */
#define ADC1_CHANNEL_THROTTLE		4/*ADC1_CHANNEL_4*/
#define ADC1_CHANNEL_PHASEB_CURRENT 	5/*ADC1_CHANNEL_5*/
#define ADC1_CHANNEL_ISHUNT_CURRENT	6/*ADC1_CHANNEL_6*/
#define ADC1_CHANNEL_X4			7/*ADC1_CHANNEL_7*/
#define ADC1_CHANNEL_BATTERY_CURRENT	8/*ADC1_CHANNEL_8*/
#define ADC1_CHANNEL_BATTERY_VOLTAGE	9/*ADC1_CHANNEL_9*/

/* XXX */
#if 1
#define	ADC_CONT_CH	ADC1_CHANNEL_BATTERY_CURRENT
/*extern uint16_t adc_battery_current_filtered; from motor.h*/
#define	adc_cont_current_filtered	adc_battery_current_filtered
#define	EVT_DATA_KT_DBG_CURRENT_CONT	EVT_DATA_KT_DBG_CURRENT_BATTERY
#else
#define	ADC_CONT_CH	ADC1_CHANNEL_ISHUNT_CURRENT
/*extern uint16_t adc_ishunt_current_filtered; from motor.h */
#define	adc_cont_current_filtered	adc_ishunt_current_filtered
#define	EVT_DATA_KT_DBG_CURRENT_CONT	EVT_DATA_KT_DBG_CURRENT_ISHUNT
#endif

#define	ADC_CURRENT_SAMPLES		16
#define	ADC_CURRENT_AVGSHIFT		4
#define	ADC_VOLTAGE_SAMPLES		8
#define	ADC_VOLTAGE_AVGSHIFT		3

/*
 * CR1 =
 * Note: If any other bit in this register apart from ADON is changed at the same time,
 * then conversion is not triggered. This is to prevent triggering an erroneous conversion.
 *
 * zeroing the _CR1_CONT bit after value is read, so ADC should be ready when triggering
 * other modes
 */
#define	adc_trigger(_ch)							\
    do {									\
	/*ADC1->CR2 = (u8)ADC1_ALIGN_LEFT;*/					\
	ADC1->CSR = (u8)(_ch);							\
	ADC1->CR1 = ADC1_CR1_ADON;						\
    } while (0)

#define	adc_trigger_cont(_ch)							\
    do {									\
	/*ADC1->CR2 = (u8)ADC1_ALIGN_LEFT;*/					\
	ADC1->CSR = (u8)(_ch);							\
	ADC1->CR1 = (ADC1_CR1_ADON | ADC1_CR1_CONT);				\
	ADC1->CR1 = (ADC1_CR1_ADON | ADC1_CR1_CONT);	/* XXX */		\
    } while (0)

#define	adc_trigger_eoc(_ch)							\
    do {									\
	/*ADC1->CR2 = (u8)ADC1_ALIGN_LEFT;*/					\
	ADC1->CSR = (u8)((_ch) | ADC1_CSR_EOCIE);				\
	ADC1->CR1 = ADC1_CR1_ADON;						\
    } while (0)

#define	adc_trigger_scan(_ch)							\
    do {									\
	ADC1->CR2 = (u8)(ADC1_ALIGN_LEFT | ADC1_CR2_SCAN);			\
	ADC1->CSR = (u8)(_ch);							\
	ADC1->CR1 = ADC1_CR1_ADON;						\
    } while (0)

#define	adc_trigger_scan_eoc(_ch)						\
    do {									\
	ADC1->CR2 = (u8)(ADC1_ALIGN_LEFT | ADC1_CR2_SCAN);			\
	ADC1->CSR = (u8)((_ch) | ADC1_CSR_EOCIE);				\
	ADC1->CR1 = ADC1_CR1_ADON;						\
    } while (0)

extern volatile uint8_t adc_trigger_all;
extern volatile uint8_t adc_throttle;
/* XXX extern volatile uint16_t adc_ishunt_current;*/
extern volatile uint16_t adc_battery_current;
extern volatile uint16_t adc_battery_voltage;

extern uint16_t adc_battery_zero_current;

/* XXX or use averages ? */
/* XXX extern uint16_t adc_ishunt_current_filtered;*/
extern uint16_t adc_battery_current_filtered;

/* XXX extern u16	avg_ishunt_current_buffer[ADC_CURRENT_SAMPLES];
extern u8	avg_ishunt_current_index;*/
extern u16	avg_battery_current_buffer[ADC_CURRENT_SAMPLES];
extern u8	avg_battery_current_index;
extern u16	avg_voltage_buffer[ADC_VOLTAGE_SAMPLES];
extern u8	avg_voltage_index;

#if 0
extern uint16_t		adc_update_avg_current_ishunt(void);
extern uint16_t		adc_update_avg_current_battery(void);
extern uint16_t		adc_update_avg_voltage_battery(void);
#endif
/* XXX extern uint16_t		adc_get_avg_current_ishunt(void);*/
extern uint16_t		adc_get_avg_current_battery(void);
extern uint16_t		adc_get_avg_voltage_battery(void);

#endif