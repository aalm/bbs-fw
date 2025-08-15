/*
 * bbs-fw
 *
 * Copyright (C) Daniel Nilsson, 2022.
 *
 * Released under the GPL License, Version 3
 */

#include "system.h"
#include "watchdog.h"
#include "cpu.h"
#include "kunteng/interrupt.h"
#include "kunteng/timers.h"

#include "kunteng/stm8s/stm8s.h"
#include "kunteng/stm8s/stm8s_clk.h"
#include "kunteng/stm8s/stm8s_tim3.h"

static void	setup_ispr(void);

static volatile uint32_t	_ms;

void
system_init(void)
{

	CLK->CKDIVR = 0x00;	/* set 16MHz */
	while ((CLK->ICKR & CLK_ICKR_HSIRDY) == 0)
		continue;	/* wait for stable clock */

	/* software priorities for interrupts */
	setup_ispr();

	_ms = 0;
	/* Setup timer3 as a ms counter */
	timer3_init_system();

	enableInterrupts();
}

static void
setup_ispr(void)
{

	/* tli sensitivity is correct by default - priority is fixed to highest(top level) */
#if /* 0 to use ITC_SetSoftwarePriority()+DBG_ISPR to dump the regs for use like below*/1
	ITC->ISPR1 = 0x7f;
	ITC->ISPR2 = 0xdf;
	ITC->ISPR4 = 0x7c;
	ITC->ISPR6 = 0x75;
#else
#ifdef DBG_ISPR
	static volatile uint8_t ispr[2][8] = {
		{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
		{ 0x7f, 0xdf, 0xff, 0x7c, 0xff, 0x75, 0xff }
	};
	static uint8_t dump_ispr = 0;

	if (dump_ispr++) {	/* call again to dump(the values first call did collect) */
		for (u8 i = 0; i < 7; i++) {
			eventlog_write_data(EVT_DATA_KT_DBG_UI8, ispr[0][i]);
			system_delay_ms(50);
			eventlog_write_data(EVT_DATA_KT_DBG_UI8, ispr[1][i]);
			system_delay_ms(50);
		}
		return;
	}
	ispr[0][0] = ITC->ISPR1;
	ispr[0][1] = ITC->ISPR2;
	ispr[0][2] = ITC->ISPR3;
	ispr[0][3] = ITC->ISPR4;
	ispr[0][4] = ITC->ISPR5;
	ispr[0][5] = ITC->ISPR6;
	ispr[0][6] = ITC->ISPR7;
#endif
	/* in hw priority order - code from main() is always lowest level 0 when irqs enabled */
	ITC_SetSoftwarePriority(ITC_IRQ_PORTA, ITC_PRIORITYLEVEL_1);			/* low */
	ITC_SetSoftwarePriority(ITC_IRQ_PORTD, ITC_PRIORITYLEVEL_1);			/* low */
	ITC_SetSoftwarePriority(ITC_IRQ_TIM1_CAPCOM, ITC_PRIORITYLEVEL_2);	/* mid */
	ITC_SetSoftwarePriority(ITC_IRQ_TIM3_OVF, ITC_PRIORITYLEVEL_1);			/* low */
	ITC_SetSoftwarePriority(ITC_IRQ_UART2_TX, ITC_PRIORITYLEVEL_1);			/* low */
	ITC_SetSoftwarePriority(ITC_IRQ_UART2_RX, ITC_PRIORITYLEVEL_1);			/* low */
	ITC_SetSoftwarePriority(ITC_IRQ_ADC1, ITC_PRIORITYLEVEL_3);	/* high */
	ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_1);			/* low */
#ifdef DBG_ISPR
	ispr[1][0] = ITC->ISPR1;
	ispr[1][1] = ITC->ISPR2;
	ispr[1][2] = ITC->ISPR3;
	ispr[1][3] = ITC->ISPR4;
	ispr[1][4] = ITC->ISPR5;
	ispr[1][5] = ITC->ISPR6;
	ispr[1][6] = ITC->ISPR7;
#endif
#endif
}

uint32_t
system_ms(void)
{
	uint32_t val;
	uint8_t ier = TIM3->IER;

	TIM3->IER &= ~(TIM3_IT_UPDATE);	/* disable timer3 interrupt */
	val = _ms;
	TIM3->IER = ier;

	return val;
}

void
system_delay_ms(uint16_t ms)
{

	if (!ms)
		return;

	uint32_t end = system_ms() + ms;
	while (system_ms() != end)
		watchdog_yeild();
}

void
isr_timer3_ovf(void) __interrupt(ITC_IRQ_TIM3_OVF)
{

	_ms++;

	/* Clear interrupt pending bit */
	TIM3->SR1 &= (uint8_t)(~TIM3_IT_UPDATE);
}