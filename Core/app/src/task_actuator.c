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
 * @file   : task_actuator.c
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
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_ACT_CNT_INIT			0ul
#define G_TASK_ACT_TICK_CNT_INI		0ul

#define DEL_LED_XX_PUL				250ul
#define DEL_LED_XX_BLI				10000ul
#define DEL_LED_XX_MIN				0ul

/********************** internal data declaration ****************************/

/* Definiciones de las macros estan en board.h*/

const task_actuator_cfg_t task_actuator_cfg_list[] = {

					{ID_LED_SYSCTRL_DIS,
					LED_SYSCTRL_DIS_PORT,
					LED_SYSCTRL_DIS_PIN,
					LED_SYSCTRL_DIS_ON,//PIN_SET
					LED_SYSCTRL_DIS_OFF,
					DEL_LED_XX_BLI,
					DEL_LED_XX_PUL},

					{ID_LED_SYSCTRL_ACT,
					LED_SYSCTRL_ACT_PORT,
					LED_SYSCTRL_ACT_PIN,
					LED_SYSCTRL_ACT_ON,//PIN_SET
					LED_SYSCTRL_ACT_OFF,
					DEL_LED_XX_BLI,
					DEL_LED_XX_PUL},

					{ID_LED_MAX_VEL,
					LED_MAX_VEL_PORT,
					LED_MAX_VEL_PIN,
					LED_MAX_VEL_ON,//PIN_SET
					LED_MAX_VEL_OFF,
					DEL_LED_XX_BLI,
					DEL_LED_XX_PUL},

					{ID_LED_MIN_VEL,
					LED_MIN_VEL_PORT,
					LED_MIN_VEL_PIN,
					LED_MIN_VEL_ON,//PIN_SET
					LED_MIN_VEL_OFF,
					DEL_LED_XX_BLI,
					DEL_LED_XX_PUL},

					{ID_BUZZER,
					BUZZER_PORT,
					BUZZER_PIN,
					BUZZER_ON, //PIN_SET
					BUZZER_OFF,
					DEL_LED_XX_BLI,
					DEL_LED_XX_PUL},
};

#define ACTUATOR_CFG_QTY	(sizeof(task_actuator_cfg_list)/sizeof(task_actuator_cfg_t))

/* task_actuator_cfg_list[] contiene las condiciones iniciales de los actuadores*/

task_actuator_dta_t task_actuator_dta_list[] = {

		{DEL_LED_XX_MIN,
	ST_LED_XX_OFF,
	EV_LED_XX_OFF,
	false},

	{DEL_LED_XX_MIN,
	ST_LED_XX_OFF,
	EV_LED_XX_OFF,
	false},

	{DEL_LED_XX_MIN,
	ST_LED_XX_OFF,
	EV_LED_XX_OFF,
	false},

	{DEL_LED_XX_MIN,
	ST_LED_XX_OFF,
	EV_LED_XX_OFF,
	false},

	{DEL_LED_XX_MIN,
	ST_LED_XX_OFF,
	EV_LED_XX_OFF,
	false},

	{DEL_LED_XX_MIN,
	ST_LED_XX_OFF,
	EV_LED_XX_OFF,
	false},
};

#define ACTUATOR_DTA_QTY	(sizeof(task_actuator_dta_list)/sizeof(task_actuator_dta_t))

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_actuator 		= "Task Actuator (Actuator Statechart)";
const char *p_task_actuator_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_actuator_cnt;
volatile uint32_t g_task_actuator_tick_cnt;

/********************** external functions definition ************************/

/*La funcion task_actuator_init inicializa los parametros de los actuadores segun las listas
 tasc_actuator_dta_list y task_actuator_cfg_list, que como se menciono mas arriba, contienen
 las condiciones iniciales de mis actuadores */

void task_actuator_init(void *parameters)
{
	uint32_t index;
	const task_actuator_cfg_t *p_task_actuator_cfg;
	task_actuator_dta_t *p_task_actuator_dta;
	task_actuator_st_t state;
	task_actuator_ev_t event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_actuator_init), p_task_actuator);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_actuator), p_task_actuator_);

	g_task_actuator_cnt = G_TASK_ACT_CNT_INIT;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_actuator_cnt), g_task_actuator_cnt);

	for (index = 0; ACTUATOR_DTA_QTY > index; index++)
	{
		/* Update Task Actuator Configuration & Data Pointer */
		p_task_actuator_cfg = &task_actuator_cfg_list[index];
		p_task_actuator_dta = &task_actuator_dta_list[index];

		/* Print out: Index & Task execution FSM */
		LOGGER_LOG("   %s = %lu", GET_NAME(index), index);

		state = p_task_actuator_dta->state;
		LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

		event = p_task_actuator_dta->event;
		LOGGER_LOG("   %s = %lu", GET_NAME(event), (uint32_t)event);

		b_event = p_task_actuator_dta->flag;
		LOGGER_LOG("   %s = %s\r\n", GET_NAME(b_event), (b_event ? "true" : "false"));

		HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
	}

	g_task_actuator_tick_cnt = G_TASK_ACT_TICK_CNT_INI;
}

void task_actuator_update(void *parameters)
{
	uint32_t index;
	const task_actuator_cfg_t *p_task_actuator_cfg;
	task_actuator_dta_t *p_task_actuator_dta;
	bool b_time_update_required = false;

	/* Update Task Actuator Counter */
	g_task_actuator_cnt++;

	/* Protect shared resource (g_task_actuator_tick_cnt) */
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_ACT_TICK_CNT_INI < g_task_actuator_tick_cnt)
    {
    	g_task_actuator_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts*/

    while (b_time_update_required)
    {
		/* Protect shared resource (g_task_actuator_tick_cnt) */
		__asm("CPSID i");	/* disable interrupts*/
		if (G_TASK_ACT_TICK_CNT_INI < g_task_actuator_tick_cnt)
		{
			g_task_actuator_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

		/*Recorro el arreglo de actuadores y reviso uno a uno si hay un cambio de estado debido a un evento entrante*/

    	for (index = 0; ACTUATOR_DTA_QTY > index; index++)
		{
    		/* Update Task Actuator Configuration & Data Pointer */
			p_task_actuator_cfg = &task_actuator_cfg_list[index];
			p_task_actuator_dta = &task_actuator_dta_list[index];

			switch (p_task_actuator_dta->state)
			{

			/*Para cada estado del actuador se analiza que evento hizo que cambie el estado, configurando asi
			 los parametros pertinentes, relacionados con el statechart */

				case ST_LED_XX_OFF:

					/*Cuando el actuador entre en accion, es decir, se interacute con el sistema de forma tal que se ejecute alguna accion
					 el flag contenido en task_actuator_dta se seteara en true, indicando que el acutador fue accionado*/

					if(true == p_task_actuator_dta->flag){

						p_task_actuator_dta->flag = false;

						switch(p_task_actuator_dta->event){

							case EV_LED_XX_ON:
								HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
								p_task_actuator_dta->state = ST_LED_XX_ON;
								break;

							case EV_LED_XX_OFF:
								break;

							case EV_LED_XX_BLINK:
								HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
								p_task_actuator_dta->tick = p_task_actuator_cfg->tick_blink;
								p_task_actuator_dta->state = ST_LED_XX_BLINK;
								break;

							case EV_LED_XX_PULSE:
								HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
								p_task_actuator_dta->tick = p_task_actuator_cfg->tick_pulse;
								p_task_actuator_dta->state = ST_LED_XX_PULSE;
								break;

							default:
								break;
							}
						}
				break;

				case ST_LED_XX_ON:

					switch(p_task_actuator_dta->event){

						case EV_LED_XX_ON:
							break;

						case EV_LED_XX_OFF:
							HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
							p_task_actuator_dta->state = ST_LED_XX_OFF;
							break;

						case EV_LED_XX_BLINK:
							p_task_actuator_dta->tick = p_task_actuator_cfg->tick_blink;
							p_task_actuator_dta->state = ST_LED_XX_BLINK;
							break;

						case EV_LED_XX_PULSE:
							p_task_actuator_dta->tick = p_task_actuator_cfg->tick_pulse;
							p_task_actuator_dta->state = ST_LED_XX_PULSE;
							break;

						default:
							break;
						}
				break;

				case ST_LED_XX_BLINK:

					switch(p_task_actuator_dta->event){

						case EV_LED_XX_ON:
							HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
							p_task_actuator_dta->state = ST_LED_XX_ON;
							break;

						case EV_LED_XX_OFF:
							HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
							p_task_actuator_dta->state = ST_LED_XX_OFF;
							break;

						case EV_LED_XX_BLINK:
							break;

						case EV_LED_XX_PULSE:
							p_task_actuator_dta->tick = p_task_actuator_cfg->tick_pulse;
							p_task_actuator_dta->state = ST_LED_XX_PULSE;
							break;

						default:
							break;
					}

				break;

				case ST_LED_XX_PULSE:

					switch(p_task_actuator_dta->event){
						case EV_LED_XX_ON:
							HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
							p_task_actuator_dta->state = ST_LED_XX_ON;
							break;

						case EV_LED_XX_OFF:
							HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
							p_task_actuator_dta->state = ST_LED_XX_OFF;
							break;

						case EV_LED_XX_BLINK:
							p_task_actuator_dta->tick = p_task_actuator_cfg->tick_blink;
							p_task_actuator_dta->state = ST_LED_XX_BLINK;
							break;

					case EV_LED_XX_PULSE:
						break;

					default:
						break;
					}

					break;

			default:
					break;
			}
		}
    }
}

/********************** end of file ******************************************/
