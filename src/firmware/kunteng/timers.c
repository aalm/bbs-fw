/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */

#include "timers.h"
#include "cpu.h"
#include "kunteng/stm8s/stm8s.h"
#include "kunteng/stm8s/stm8s_clk.h"
#include "kunteng/stm8s/stm8s_tim1.h"
#include "kunteng/stm8s/stm8s_tim2.h"
#include "kunteng/stm8s/stm8s_tim3.h"
#include "kunteng/stm8s/stm8s_tim4.h"

#define TIM1_AUTO_RELOAD_PERIOD			511
#define TIM3_AUTO_RELOAD_PERIOD			15999	/* 1ms */
#define TIM4_AUTO_RELOAD_PERIOD			99	/* 100us */


void
timers_init(void)
{
	/* nothing to do here */
}

void
timer1_init_motor_pwm(void)
{

	CLK->PCKENR1 |= CLK_PCKENR1_TIM1;

	/* prescaler */
	TIM1->PSCRH = 0;
	TIM1->PSCRL = 0;

	/*
	 * auto reload
	 * clock = 16MHz, counter period = 1024, PWM freq = 16MHz / 1024 = 15.625MHz
	 * (BUT PWM center aligned mode needs double frequency)
	 */
	TIM1->ARRH = (u8)(TIM1_AUTO_RELOAD_PERIOD >> 8);
	TIM1->ARRL = (u8)TIM1_AUTO_RELOAD_PERIOD;

#if 0
	TIM1->CR1 |= TIM1_COUNTERMODE_CENTERALIGNED1;
#else
	TIM1->CR1 = (u8)((TIM1->CR1 & (u8)~(TIM1_CR1_CMS | TIM1_CR1_DIR)) |
	    (u8)TIM1_COUNTERMODE_CENTERALIGNED1);
#endif
	TIM1->RCR = 1;

	/* OC1 */
	TIM1->CCER1 &= (u8)~(/* XXX just in case */
	    TIM1_CCER1_CC1E | TIM1_CCER1_CC1NE |
	    TIM1_CCER1_CC1P | TIM1_CCER1_CC1NP);
	TIM1->CCER1 |= (u8)(
	    (u8)(TIM1_OUTPUTSTATE_DISABLE & TIM1_CCER1_CC1E) |
	    (u8)(TIM1_OUTPUTNSTATE_DISABLE & TIM1_CCER1_CC1NE) |
	    (u8)(TIM1_OCPOLARITY_HIGH & TIM1_CCER1_CC1P) |
	    (u8)(TIM1_OCNPOLARITY_LOW & TIM1_CCER1_CC1NP));

#if 0
	TIM1->CCMR1 |= TIM1_OCMODE_PWM1;
#else
	TIM1->CCMR1 = (u8)(
	    (TIM1->CCMR1 & (u8)~TIM1_CCMR_OCM) | (u8)TIM1_OCMODE_PWM1);
#endif

	TIM1->OISR &= (u8)~(/* XXX just in case */
	    TIM1_OISR_OIS1 | TIM1_OISR_OIS1N);
	TIM1->OISR |= (u8)(
	    (u8)(TIM1_OCIDLESTATE_RESET & TIM1_OISR_OIS1) |
	    (u8)(TIM1_OCNIDLESTATE_SET & TIM1_OISR_OIS1N));

	TIM1->CCR1H = 0;
	TIM1->CCR1L = 255;

	/* OC2 */
	TIM1->CCER1 &= (u8)~(/* XXX just in case */
	    TIM1_CCER1_CC2E | TIM1_CCER1_CC2NE |
	    TIM1_CCER1_CC2P | TIM1_CCER1_CC2NP);
	TIM1->CCER1 |= (u8)(
	    (u8)(TIM1_OUTPUTSTATE_DISABLE & TIM1_CCER1_CC2E) |
	    (u8)(TIM1_OUTPUTNSTATE_DISABLE & TIM1_CCER1_CC2NE) |
	    (u8)(TIM1_OCPOLARITY_HIGH & TIM1_CCER1_CC2P) |
	    (u8)(TIM1_OCNPOLARITY_LOW & TIM1_CCER1_CC2NP));

#if 0
	TIM1->CCMR2 |= TIM1_OCMODE_PWM1;
#else
	TIM1->CCMR2 = (u8)(
	    (TIM1->CCMR2 & (u8)~TIM1_CCMR_OCM) | (u8)TIM1_OCMODE_PWM1);
#endif

	TIM1->OISR &= (u8)~(/* XXX just in case */
	    TIM1_OISR_OIS2 | TIM1_OISR_OIS2N);
	TIM1->OISR |= (u8)(
	    (u8)(TIM1_OCIDLESTATE_RESET & TIM1_OISR_OIS2) |
	    (u8)(TIM1_OCNIDLESTATE_SET & TIM1_OISR_OIS2N));

	TIM1->CCR2H = 0;
	TIM1->CCR2L = 255;

	/* OC3 */
	TIM1->CCER2 &= (u8)~(/* XXX just in case */
	    TIM1_CCER2_CC3E | TIM1_CCER2_CC3NE |
	    TIM1_CCER2_CC3P | TIM1_CCER2_CC3NP);
	TIM1->CCER2 |= (u8)(
	    (u8)(TIM1_OUTPUTSTATE_DISABLE & TIM1_CCER2_CC3E) |
	    (u8)(TIM1_OUTPUTNSTATE_DISABLE & TIM1_CCER2_CC3NE) |
	    (u8)(TIM1_OCPOLARITY_HIGH & TIM1_CCER2_CC3P) |
	    (u8)(TIM1_OCNPOLARITY_LOW & TIM1_CCER2_CC3NP));

#if 0
	TIM1->CCMR3 |= TIM1_OCMODE_PWM1;
#else
	TIM1->CCMR3 = (u8)(
	    (TIM1->CCMR3 & (u8)~TIM1_CCMR_OCM) | (u8)TIM1_OCMODE_PWM1);
#endif

	TIM1->OISR &= (u8)~(/* XXX just in case */
	    TIM1_OISR_OIS3 | TIM1_OISR_OIS3N);
	TIM1->OISR |= (u8)(
	    (u8)(TIM1_OCIDLESTATE_RESET & TIM1_OISR_OIS3) |
	    (u8)(TIM1_OCNIDLESTATE_SET & TIM1_OISR_OIS3N));

	TIM1->CCR3H = 0;
	TIM1->CCR3L = 255;

#if /* XXX ??? is set anyway? do check */1
	TIM1->CCMR1 |= TIM1_CCMR_OCxPE;		/*TIM1_OC1PreloadConfig(ENABLE);*/
	TIM1->CCMR2 |= TIM1_CCMR_OCxPE;		/*TIM1_OC2PreloadConfig(ENABLE);*/
	TIM1->CCMR3 |= TIM1_CCMR_OCxPE;		/*TIM1_OC3PreloadConfig(ENABLE);*/
#endif

	/* OC4 */
	/*
	 * Used for to fire interrupt at a specific time (middle of DC link current pulses)
	 * and is always syncronized with PWM
	 */
	TIM1->CCER2 |= (u8)(
		(u8)(TIM1_OUTPUTSTATE_DISABLE & TIM1_CCER2_CC4E) |
		(u8)(TIM1_OCPOLARITY_HIGH & TIM1_CCER2_CC4P)
	);

	TIM1->OISR &= (u8)(~TIM1_OISR_OIS4);

	/* timing for interrupt firing (hand adjusted) */
	const uint16_t timing = 285;
	TIM1->CCR4H = (u8)(timing >> 8);
	TIM1->CCR4L = (u8)timing;

	/*
	 * hardware needs a dead time of 1us
	 * 16 // DTG = 0; dead time in 62.5 ns steps; 1us/62.5ns = 16
	 */
	TIM1->DTR = (u8)16;

#if 0
	TIM1->BKR = (uint8_t)(
		TIM1_OSSISTATE_ENABLE |
		TIM1_LOCKLEVEL_OFF |
		TIM1_BREAK_DISABLE |
		TIM1_BREAKPOLARITY_LOW |
		TIM1_AUTOMATICOUTPUT_DISABLE
	);
#else
	/* ^irrelevant defaults(==4 | 0 | 0 | 0 | 0)*/
	TIM1->BKR = (u8)TIM1_OSSISTATE_ENABLE;
#endif

	/* enable cc4 interrupt */
	TIM1->IER |= TIM1_IT_CC4;

	/* enable timer */
	TIM1->CR1 |= TIM1_CR1_CEN;

	TIM1->BKR |= TIM1_BKR_MOE;
}

void
timer3_init_system(void)
{

	/* enable timer3 clock source */
	CLK->PCKENR1 |= CLK_PCKENR1_TIM3;

	/* set period */
	TIM3->PSCR = TIM3_PRESCALER_1;
	TIM3->ARRH = (u8)(TIM3_AUTO_RELOAD_PERIOD >> 8);
	TIM3->ARRL = (u8)(TIM3_AUTO_RELOAD_PERIOD);

	/* clear counter */
	TIM3->CNTRH = 0;
	TIM3->CNTRL = 0;

	/* enable TIM3 interrupt */
	TIM3->IER |= TIM3_IT_UPDATE;

	/* clear interrupt pending bit */
	TIM3->SR1 &= ~TIM3_IT_UPDATE;

	/* TIM3 enable */
	TIM3->CR1 |= TIM3_CR1_CEN;
}

void
timer4_init_sensors(void)
{

	/* enable timer4 clock source */
	CLK->PCKENR1 |= CLK_PCKENR1_TIM4;

	/* set period */
	TIM4->PSCR = TIM4_PRESCALER_16;
	TIM4->ARR = TIM4_AUTO_RELOAD_PERIOD;

	/* clear counter */
	TIM4->CNTR = 0;

	/* enable TIM4 interrupt */
	TIM4->IER |= TIM4_IT_UPDATE;

	/* clear interrupt pending bit */
	TIM4->SR1 &= ~TIM4_IT_UPDATE;

	/* TIM4 enable */
	TIM4->CR1 |= TIM4_CR1_CEN;
}