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
 * @file   : task_sensor.c
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
#include "task_sensor_attribute.h"
#include "task_system_attribute.h"
#include "task_system_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SEN_CNT_INIT			0ul
#define G_TASK_SEN_TICK_CNT_INI		0ul

#define DEL_BTN_XX_MIN				0ul
#define DEL_BTN_XX_MED				25ul
#define DEL_BTN_XX_MAX				50ul

/********************** internal data declaration ****************************/
const task_sensor_cfg_t task_sensor_cfg_list[] = {

	/*Todos las definiciones de numero de puerto y pin, junto a la definicion
	 de PRESSED (Si esta precionado cunado el pin se encuentra en estado alto (SET)
	 o en estado bajo (RESET)*/

	/*El prefijo SYS significa que afecta el boton simula ser un sensor real que interactua con el sistema
	 y el prefijo MAN significa que el boton es utilizado para moverse sobre el menu*/

	{ID_BTN_INGRESO, // Sensor (boton) que registra el ingreso de las personas
	BTN_INGRESO_PORT,
	BTN_INGRESO_PIN,
	BTN_INGRESO_PRESSED, // GPIO_PIN_RESET
	DEL_BTN_XX_MAX,
	EV_SYS_BTN_INGRESO_IDLE, // signal up (el boton entra en reposo cuando vuelve a estar en SET)
	EV_SYS_BTN_INGRESO_ACTIVE // signal down (el boton esta activo cuando su estado es RESET)
	},

	{ID_BTN_EGRESO, // Sensor (boton) que registra el egreso de las personas
	BTN_EGRESO_PORT,
	BTN_EGRESO_PIN,
	BTN_EGRESO_PRESSED,// GPIO_PIN_RESET
	DEL_BTN_XX_MAX,
	EV_SYS_BTN_EGRESO_IDLE,
	EV_SYS_BTN_EGRESO_ACTIVE},

	{ID_BTN_ENTER, // Boton para ingresar a los modos "Normal" o "Set Up"
	BTN_ENTER_PORT,
	BTN_ENTER_PIN,
	BTN_ENTER_PRESSED,// GPIO_PIN_RESET
	DEL_BTN_XX_MAX,
	EV_MEN_BTN_ENTER_IDLE,
	EV_MEN_BTN_ENTER_ACTIVE},

	{ID_BTN_NEXT, // Boton para navegar entre las opciones "Normal" o "Set Up"
	BTN_NEXT_PORT,
	BTN_NEXT_PIN,
	BTN_NEXT_PRESSED,// GPIO_PIN_RESET
	DEL_BTN_XX_MAX,
	EV_MEN_BTN_NEXT_IDLE,
	EV_MEN_BTN_NEXT_ACTIVE},

	{ID_BTN_ON, // Boton para activar el sistema de control
	BTN_ON_PORT,
	BTN_ON_PIN,
	BTN_ON_PRESSED,// GPIO_PIN_RESET
	DEL_BTN_XX_MAX,
	EV_SYS_BTN_ON_IDLE,
	EV_SYS_BTN_ON_ACTIVE},

	{ID_SWITCH_OFF, // Switch para desactivar el sistema de control
	SWITCH_SWITCH_OFF_PORT,
	SWITCH_SWITCH_OFF_PIN,
	SWITCH_SWITCH_OFF_PRESSED,// GPIO_PIN_RESET
	DEL_BTN_XX_MAX,
	EV_SYS_SWITCH_OFF_IDLE,
	EV_SYS_SWITCH_OFF_ACTIVE},

	{ID_SWITCH_BIR, // Sensor (switch) que registra si hay personas sobre la escalera
	SWITCH_BIR_PORT,
	SWITCH_BIR_PIN,
	SWITCH_BIR_PRESSED,// GPIO_PIN_RESET
	DEL_BTN_XX_MAX,
	EV_SYS_SWITCH_BIR_IDLE,
	EV_SYS_SWITCH_BIR_ACTIVE},

};

#define SENSOR_CFG_QTY	(sizeof(task_sensor_cfg_list)/sizeof(task_sensor_cfg_t))

task_sensor_dta_t task_sensor_dta_list[] = {
	{DEL_BTN_XX_MIN,
	ST_BTN_XX_UP,
	EV_BTN_XX_UP},

	{DEL_BTN_XX_MIN,
	ST_BTN_XX_UP,
	EV_BTN_XX_UP},

	{DEL_BTN_XX_MIN,
	ST_BTN_XX_UP,
	EV_BTN_XX_UP},

	{DEL_BTN_XX_MIN,
	ST_BTN_XX_UP,
	EV_BTN_XX_UP},

	{DEL_BTN_XX_MIN,
	ST_BTN_XX_UP,
	EV_BTN_XX_UP},

	{DEL_BTN_XX_MIN,
	ST_BTN_XX_UP,
	EV_BTN_XX_UP},

	{DEL_BTN_XX_MIN,
	ST_BTN_XX_UP,
	EV_BTN_XX_UP},
};

#define SENSOR_DTA_QTY	(sizeof(task_sensor_dta_list)/sizeof(task_sensor_dta_t))

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_sensor 		= "Task Sensor (Sensor Statechart)";
const char *p_task_sensor_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_sensor_cnt;
volatile uint32_t g_task_sensor_tick_cnt;

/********************** external functions definition ************************/
void task_sensor_init(void *parameters)
{
	uint32_t index;
	task_sensor_dta_t *p_task_sensor_dta;
	task_sensor_st_t state;
	task_sensor_ev_t event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_sensor_init), p_task_sensor);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_sensor), p_task_sensor_);

	g_task_sensor_cnt = G_TASK_SEN_CNT_INIT;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_sensor_cnt), g_task_sensor_cnt);

	for (index = 0; SENSOR_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Data Pointer */
		p_task_sensor_dta = &task_sensor_dta_list[index];

		/* Print out: Index & Task execution FSM */
		LOGGER_LOG("   %s = %lu", GET_NAME(index), index);

		state = p_task_sensor_dta->state;
		LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

		event = p_task_sensor_dta->event;
		LOGGER_LOG("   %s = %lu\r\n", GET_NAME(event), (uint32_t)event);
	}
	g_task_sensor_tick_cnt = G_TASK_SEN_TICK_CNT_INI;
}

void task_sensor_update(void *parameters)
{
	uint32_t index;
	const task_sensor_cfg_t *p_task_sensor_cfg;
	task_sensor_dta_t *p_task_sensor_dta;
	bool b_time_update_required = false;

	/* Update Task Sensor Counter */
	g_task_sensor_cnt++;

	/* Protect shared resource (g_task_sensor_tick_cnt) */
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_tick_cnt)
    {
    	g_task_sensor_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts*/

    while (b_time_update_required)
    {
		/* Protect shared resource (g_task_sensor_tick_cnt) */
		__asm("CPSID i");	/* disable interrupts*/
		if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_tick_cnt)
		{
			g_task_sensor_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

    	for (index = 0; SENSOR_DTA_QTY > index; index++)
		{
    		/* Update Task Sensor Configuration & Data Pointer */
			p_task_sensor_cfg = &task_sensor_cfg_list[index];
			p_task_sensor_dta = &task_sensor_dta_list[index];

			if (p_task_sensor_cfg->pressed == HAL_GPIO_ReadPin(p_task_sensor_cfg->gpio_port, p_task_sensor_cfg->pin))
			{
				p_task_sensor_dta->event =	EV_BTN_XX_DOWN;
			}
			else
			{
				p_task_sensor_dta->event =	EV_BTN_XX_UP;
			}

			switch (p_task_sensor_dta->state)
			{
				case ST_BTN_XX_UP:

					/* SETEO LA GUARDA PARA EVITAR EL BOUNCING*/

					if (EV_BTN_XX_DOWN == p_task_sensor_dta->event)
					{
						p_task_sensor_dta->state = ST_BTN_XX_FALLING;
						p_task_sensor_dta->tick = p_task_sensor_cfg->tick_max /*DEL_BTN_XX_MAX al incio de la iteracion*/ ;

					}

					break;


				case ST_BTN_XX_FALLING:

					/*MIENTRAS LA GUARDA NO SEA 0, DECREMENTO POR CADA SEÑAL DE CLOCK, SI ES CERO
					 CAMBIO AL SIGUIENTE ESTADO DETERMINADO POR LA TABLA DE ESTADOS */

					if (p_task_sensor_dta->tick > 0 ){

						p_task_sensor_dta->tick --;

					}

					else {

						if (EV_BTN_XX_DOWN == p_task_sensor_dta->event){

							put_event_task_system(p_task_sensor_cfg->signal_down);
							put_event_task_menu(p_task_sensor_cfg->signal_down);
							p_task_sensor_dta->state = ST_BTN_XX_DOWN;

						}

						else{
							p_task_sensor_dta->state = ST_BTN_XX_UP;

						}
					}

					break;


				case ST_BTN_XX_DOWN:

					/*MISMO CASO QUE CUANDO SE APRETA EL BOTON, CUANDO LLEGA EL EVENTO DE SOLTAR
					 EL BOTON, SE ESPERA A QUE TRANSCURRA EL TRANSITORIO PARA LEER CORRECTAMENTE
					 EL ESTADO DEL SENSOR */

					if (EV_BTN_XX_UP == p_task_sensor_dta->event)
					{
						p_task_sensor_dta->tick = p_task_sensor_cfg->tick_max;
						p_task_sensor_dta->state = ST_BTN_XX_RISING;
					}

					break;

				case ST_BTN_XX_RISING:

					/*UNA VEZ TRANSCURRIDO EL TRANSITORIO Y EL EVENTO DEL SENSOR ES UP, ENTONCES
					 SE PROCEDE A PONER DICHO EVENTO EN EL SISTEMA*/

					if (p_task_sensor_dta->tick > 0 )
					{
							p_task_sensor_dta->tick --;
					}

					else {

					   	   if (EV_BTN_XX_UP == p_task_sensor_dta->event)
					   	   {
					   		   put_event_task_menu(p_task_sensor_cfg->signal_up); // Pone un elemento en la cola (envia una señal alto al micro porduccida por el sensor marcado como INDEX
					   		   put_event_task_system(p_task_sensor_cfg->signal_up);

					   		   /* ¿ Porque se le pasa a las funciones put_event_task_menu() y put_event_task_system() una variable del tipo task_sensor_ev_t cuando en realidad
					   		    deberia recibir task_menu_ev_t y task_system_ev_t y  respectivamente ? Se que es un enum pero no hay relacion directa y los eventos a los que
					   		    estan igualados signal_up y signal_down no estan definidos en ningun lugar  */

					   		   p_task_sensor_dta->state = ST_BTN_XX_UP;
							}
							else
							{
								p_task_sensor_dta->state = ST_BTN_XX_DOWN;
							}
					}

					break;

				default:

					break;
			}
		}
    }
}

/********************** end of file ******************************************/


