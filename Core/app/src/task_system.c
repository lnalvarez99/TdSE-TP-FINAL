/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_system.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "task_system_attribute.h"
#include "task_system_interface.h"
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"
#include "task_adc_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_XX_MIN				0ul
#define DEL_SYS_XX_MED				50ul
#define DEL_SYS_XX_MAX				500ul


/********************** internal data declaration ****************************/
task_system_dta_t task_system_dta =
	{DEL_SYS_XX_MIN,0,0, ST_SYS_XX_OFF, EV_SYS_BTN_ON_IDLE, false};

#define SYSTEM_DTA_QTY	(sizeof(task_system_dta)/sizeof(task_system_dta_t))

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_system 		= "Task System (System Statechart)";
const char *p_task_system_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_system_cnt;
volatile uint32_t g_task_system_tick_cnt;
uint32_t temp_amb_raw=0;
uint32_t temp_uC_raw=0;
uint32_t temp_amb=0;
uint32_t temp_uC=0;

/********************** external functions definition ************************/
void task_system_init(void *parameters)
{
	task_system_dta_t 	*p_task_system_dta;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_system_init), p_task_system);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_system), p_task_system_);

	g_task_system_cnt = G_TASK_SYS_CNT_INI;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_system_cnt), g_task_system_cnt);

	init_queue_event_task_system();

	/* Update Task Actuator Configuration & Data Pointer */

	/*NOTA: Recordar que task_system_dta es una variable global declarada en task_system_attribute.h
	 por eso creo el puntero de tipo task_system_dta_t llamado p_task_system_dta, para poder
	 trabajar con ese tipo de variable, podria usar directamente la variable global pero es buena
	 practiva usar un puntero a su direccion de memoria y tabajar con el puntero*/

	p_task_system_dta = &task_system_dta;

	/* Print out: Task execution FSM */
	state = p_task_system_dta->state;
	LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

	event = p_task_system_dta->event;
	LOGGER_LOG("   %s = %lu", GET_NAME(event), (uint32_t)event);

	b_event = p_task_system_dta->flag;
	LOGGER_LOG("   %s = %s\r\n", GET_NAME(b_event), (b_event ? "true" : "false"));

	g_task_system_tick_cnt = G_TASK_SYS_TICK_CNT_INI;
}

void task_system_update(void *parameters)
{
	task_system_dta_t *p_task_system_dta;
	bool b_time_update_required = false;

	/* Update Task System Counter */
	g_task_system_cnt++;

	/* Protect shared resource (g_task_system_tick) */
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
    {
    	g_task_system_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts*/

    while (b_time_update_required)
    {
		/* Protect shared resource (g_task_system_tick) */
		__asm("CPSID i");	/* disable interrupts*/
		if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
		{
			g_task_system_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

    	/* Update Task System Data Pointer */
		p_task_system_dta = &task_system_dta;


		// Si hay algun evento comienzo a ver cual es y dependiendo que evento haya sucedido actuo segun statechart

		if (true == any_event_task_system())
		{
			p_task_system_dta->flag = true;
			p_task_system_dta->event = get_event_task_system(); //Aca levanto los eventos generados por los sensores
		}

		if( true == any_value_task_adc())
		{
				temp_uC_raw  = get_value_task_adc();
				temp_amb_raw = get_value_task_adc();

				temp_amb = (3.30 * 100 * temp_amb_raw)/(4096);
				LOGGER_LOG("temp_uC_raw:%lu\r\n",temp_uC_raw);
				LOGGER_LOG("temp_amb_raw:%lu\r\n",temp_amb);
		}

		switch (p_task_system_dta->state)
		{

			case ST_SYS_XX_IDLE:
			{

				p_task_system_dta->flag = false;
				switch(p_task_system_dta->event)
				{
					case EV_SYS_SWITCH_OFF_ACTIVE:
							put_event_task_actuator(EV_LED_XX_ON, ID_LED_SYSCTRL_DIS);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_SYSCTRL_ACT);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_BUZZER);
							p_task_system_dta->state = ST_SYS_XX_OFF;
					break;

					case EV_SYS_BTN_INGRESO_ACTIVE:
						if(p_task_system_dta->tick++ > p_task_system_dta->cantidad_personas)
						{
							put_event_task_actuator(EV_LED_XX_ON, ID_LED_MIN_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
						else
						{
							put_event_task_actuator(EV_LED_XX_ON,ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
					break;

					case EV_SYS_BTN_EGRESO_ACTIVE:
						if(p_task_system_dta->tick == 0)
						{
							/*Cuando no hay personas en la escalera pongo a titilar a ambos led indicadores de velocidad*/
							put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_MIN_VEL);
							p_task_system_dta->state= ST_SYS_XX_STOP;
						}
						else if(p_task_system_dta->tick-- > p_task_system_dta->cantidad_personas)
						{
							put_event_task_actuator(EV_LED_XX_ON, ID_LED_MIN_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
						else
						{
							put_event_task_actuator(EV_LED_XX_ON,ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
					break;

					case EV_SYS_SWITCH_BIR_ACTIVE:
							/*Cuando no hay personas en la escalera pongo a titilar a ambos led indicadores de velocidad*/
							put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_MIN_VEL);
							p_task_system_dta->tick=0;
							p_task_system_dta->state= ST_SYS_XX_STOP;
					break;

					default: break;

				} // fin del switch

			} break;

			/* CASO SISTEMA APAGADO*/
			case ST_SYS_XX_OFF:
				p_task_system_dta->flag = false;
				switch(p_task_system_dta->event)
				{

				case EV_SYS_SWITCH_OFF_ACTIVE:
					put_event_task_actuator(EV_LED_XX_ON, ID_LED_SYSCTRL_DIS);
					put_event_task_actuator(EV_LED_XX_OFF, ID_LED_SYSCTRL_ACT);
					put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
					put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
					put_event_task_actuator(EV_LED_XX_OFF, ID_BUZZER);
					p_task_system_dta->state = ST_SYS_XX_OFF;

				case EV_SYS_BTN_ON_ACTIVE:
					put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_SYSCTRL_ACT);
					put_event_task_actuator(EV_LED_XX_OFF, ID_LED_SYSCTRL_DIS);
					put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
					put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
					put_event_task_actuator(EV_LED_XX_OFF, ID_BUZZER);
					p_task_system_dta->state = ST_SYS_XX_IDLE;

				default:
					break;

				}

			break;

			/* CASO SISTEMA MODO NORMAL*/
			case ST_SYS_XX_NORMAL:

				p_task_system_dta->flag = false;
				switch(p_task_system_dta->event)
				{
					case EV_SYS_SWITCH_OFF_ACTIVE:
						put_event_task_actuator(EV_LED_XX_ON, ID_LED_SYSCTRL_DIS);
						put_event_task_actuator(EV_LED_XX_OFF, ID_LED_SYSCTRL_ACT);
						put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
						put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
						put_event_task_actuator(EV_LED_XX_OFF, ID_BUZZER);
						p_task_system_dta->state = ST_SYS_XX_OFF;
						break;

					case EV_SYS_BTN_INGRESO_ACTIVE:
						if(p_task_system_dta->tick++ > p_task_system_dta->cantidad_personas)
						{
							put_event_task_actuator(EV_LED_XX_ON, ID_LED_MIN_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
						else
						{
							put_event_task_actuator(EV_LED_XX_ON,ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
						break;

					case EV_SYS_BTN_EGRESO_ACTIVE:
						if(p_task_system_dta->tick == 0)
						{
							put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_MIN_VEL);
							p_task_system_dta->state= ST_SYS_XX_STOP;
						}
						if(p_task_system_dta->tick-- > p_task_system_dta->cantidad_personas)
						{
							put_event_task_actuator(EV_LED_XX_ON, ID_LED_MIN_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
						else
						{
							put_event_task_actuator(EV_LED_XX_ON,ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
						break;

					case EV_SYS_SWITCH_BIR_ACTIVE:
						put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_MAX_VEL);
						put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_MIN_VEL);
						p_task_system_dta->tick=0;
						p_task_system_dta->state= ST_SYS_XX_STOP;
						break;

					default: break;
				}
			break;

			/* CASO SISTEMA EN STOP*/
			case ST_SYS_XX_STOP:

				p_task_system_dta->flag = false;
				switch(p_task_system_dta->event)
				{
					case EV_SYS_SWITCH_OFF_ACTIVE:
						put_event_task_actuator(EV_LED_XX_ON, ID_LED_SYSCTRL_DIS);
						put_event_task_actuator(EV_LED_XX_OFF, ID_LED_SYSCTRL_ACT);
						put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
						put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
						put_event_task_actuator(EV_LED_XX_OFF, ID_BUZZER);
						p_task_system_dta->state = ST_SYS_XX_OFF;
						break;

					case EV_SYS_BTN_INGRESO_ACTIVE:
						if(p_task_system_dta->timer == 0)
						{
							put_event_task_actuator(EV_LED_XX_OFF,ID_BUZZER);
							put_event_task_actuator(EV_LED_XX_ON, ID_LED_SYSCTRL_ACT);
							put_event_task_actuator(EV_LED_XX_ON, ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
							p_task_system_dta->tick++;
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
						else
						{
							put_event_task_actuator(EV_LED_XX_ON, ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
							p_task_system_dta->tick++;
							p_task_system_dta->state = ST_SYS_XX_NORMAL;
						}
						break;

					default:
						if(p_task_system_dta->timer == 0)
						{
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_SYSCTRL_ACT);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MAX_VEL);
							put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MIN_VEL);
							put_event_task_actuator(EV_LED_XX_ON, ID_BUZZER);
							p_task_system_dta->state = ST_SYS_XX_STOP;
						}
						else
						{
							p_task_system_dta->timer--;
						}
						break;
				}

			break; // Fin case ST_SYS_XX_STOP:

		default:
			if(p_task_system_dta->event == EV_SYS_BTN_ON_IDLE)
			{
				put_event_task_actuator(EV_LED_XX_PULSE, ID_LED_SYSCTRL_ACT);
				p_task_system_dta->state = ST_SYS_XX_IDLE;
				/*Mientras el sistema este en reposo estoy dentro del modo setup y el
				 led de sistema activo emite luz en forma de pulsos*/
			}
		}
	}
}

/********************** end of file ******************************************/



