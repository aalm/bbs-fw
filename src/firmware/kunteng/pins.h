/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */
#ifndef _KUNTENG_PINS_H_
#define _KUNTENG_PINS_H_

#include "kunteng/cpu.h"
#include "kunteng/stm8s/stm8s.h"
#include "kunteng/stm8s/stm8s_gpio.h"

#if 0
#define PIN_HALL_SENSOR_A			GPIOE, GPIO_PIN_5
#define PIN_HALL_SENSOR_B			GPIOD, GPIO_PIN_2
#define PIN_HALL_SENSOR_C			GPIOC, GPIO_PIN_5

#define PIN_PWM_PHASE_A_LOW			GPIOB, GPIO_PIN_2
#define PIN_PWM_PHASE_A_HIGH		GPIOC, GPIO_PIN_3

#define PIN_PWM_PHASE_B_LOW			GPIOB, GPIO_PIN_1
#define PIN_PWM_PHASE_B_HIGH		GPIOC, GPIO_PIN_2

#define PIN_PWM_PHASE_C_LOW			GPIOB, GPIO_PIN_0
#define PIN_PWM_PHASE_C_HIGH		GPIOC, GPIO_PIN_1

#define PIN_BATTERY_CURRENT			GPIOB, GPIO_PIN_5
#define PIN_BATTERY_VOLTAGE			GPIOB, GPIO_PIN_6

#define PIN_PAS1					GPIOD, GPIO_PIN_7
#define PIN_PAS2					GPIOE, GPIO_PIN_0
#define PIN_SPEED_SENSOR			GPIOA, GPIO_PIN_1
#define PIN_BRAKE					GPIOC, GPIO_PIN_6
#define PIN_THROTTLE				GPIOB, GPIO_PIN_7
#define PIN_LIGHTS					GPIOD, GPIO_PIN_4
#endif

/* XXX using just "tuples" here would make more sense; _IOT==IO Tuple */

#define	ADC_B_IOT	GPIOB, (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6)
#define	ADC_E_IOT	GPIOE, (GPIO_PIN_6 | GPIO_PIN_7)

/* PORTA */
#define	X6_IOT		GPIOA, GPIO_PIN_1
#define	X7_IOT		GPIOA, GPIO_PIN_2
#define	BRAKE_IOT	GPIOA, GPIO_PIN_4

/* PORTB */
#define	PWM_C_LO_IOT	GPIOB, GPIO_PIN_0	/* unused */
#define	PWM_B_LO_IOT	GPIOB, GPIO_PIN_1	/* unused */
#define	PWM_A_LO_IOT	GPIOB, GPIO_PIN_2	/* unused */
#define	PWM_ABC_LO_IOT	GPIOB, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2)	/* unused */
#define	THROTTLE_IOT	GPIOB, GPIO_PIN_4

/* PORTC */
#define	PWM_C_HI_IOT	GPIOC, GPIO_PIN_1	/* unused */
#define	PWM_B_HI_IOT	GPIOC, GPIO_PIN_2	/* unused */
#define	PWM_A_HI_IOT	GPIOC, GPIO_PIN_3	/* unused */
#define	PWM_ABC_HI_IOT	GPIOC, (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)	/* unused */
#define	AUX_B_IOT	GPIOC, GPIO_PIN_4	/* blue 2k2 */
#define	LIGHT_IOT	AUX_B_IOT
#define	SPEED_IOT	GPIOC, GPIO_PIN_5
#define	AUX_Y_IOT	GPIOC, GPIO_PIN_7	/* yellow no resistor/direct to the pin */

/* PORTD */
#define	PAS_IOT		GPIOD, GPIO_PIN_0
#define	AUX_G_IOT	GPIOD, GPIO_PIN_2	/* green 2k2 */
#define	T23_IOT		GPIOD, GPIO_PIN_3
#define	T24_IOT		GPIOD, GPIO_PIN_4
#define	UART2_TX_IOT	GPIOD, GPIO_PIN_5
#define	UART2_RX_IOT	GPIOD, GPIO_PIN_6
#define	UART2_IOT	GPIOD, (GPIO_PIN_5 | GPIO_PIN_6)
#define	OC_IOT		GPIOD, GPIO_PIN_7

/* PORTE */
#define	HALL_A_IOT	GPIOE, GPIO_PIN_0
#define	HALL_B_IOT	GPIOE, GPIO_PIN_1
#define	HALL_C_IOT	GPIOE, GPIO_PIN_2
#define	HALL_IOT	GPIOE, (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2)
#define	J2_IOT		GPIOE, GPIO_PIN_3	/* was J2 pin1(0-1) w/pin0==GND on some old pcb */
#define	X8_IOT		GPIOE, GPIO_PIN_5

						/* XXX */
#define	DBG_P0_IOT	AUX_G_IOT	/* isr_timer1_cmp() - pwm */
#define	DBG_P1_IOT	AUX_B_IOT	/* isr_timer1_cmp() - speed limiting */
#define	DBG_P2_IOT	AUX_Y_IOT	/* isr_timer4_ovf() - speed */
#define	DBG_P3_IOT	T23_IOT		/* isr_porta_exti() - brake */
#define	DBG_P4_IOT	T24_IOT		/* trigger high to adc_eoc low */
#define	DBG_P5_IOT	PAS_IOT		/* == TIM1->BKR & TIM1_BKR_MOE(pwm=on|off) */
#define	DBG_P6_IOT	X6_IOT		/* isr_timer1_cmp() - ramp current limiting */
#define	DBG_P7_IOT	X7_IOT		/* isr_timer1_cmp() - phase limiting */

#endif