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
 * @file   : task_menu.c
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
#include "display.h"
//#include "task_adc_interface.h"
#include "task_setup_interface.h"
#include "task_system_attribute.h"
#include "task_setup_attribute.h"


/********************** macros and definitions *******************************/
#define G_TASK_MEN_CNT_INI			0ul
#define G_TASK_MEN_TICK_CNT_INI		0ul

#define DEL_MEN_XX_MIN				0ul
#define DEL_MEN_XX_MED				50ul
#define DEL_MEN_XX_MAX				500ul

#define CANTIDAD_PERSONAS_CFG_01   15ul
#define TIMER_CFG_01			   30ul
#define CANTIDAD_PERSONAS_CFG_02   20ul
#define TIMER_CFG_02			   45ul
/********************** internal data declaration ****************************/
task_setup_dta_t task_setup_dta = {DEL_MEN_XX_MIN, ST_SETUP_IDLE, EV_SETUP_BTN_ENTER_IDLE, false, false};

task_configuration_dta_t task_configuracion_dta = {0,0};

#define SETUP_DTA_QTY	(sizeof(task_setup_dta)/sizeof(task_setup_dta_t))

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_menu 		= "Task Menu (Interactive Menu)";
const char *p_task_menu_ 		= "Non-Blocking & Update By Time Code";

/*
uint32_t temp_amb_raw=0;
uint32_t temp_uC_raw=0;
uint32_t temp_amb=0;
uint32_t temp_uC=0;
*/

/********************** external data declaration ****************************/
uint32_t g_task_menu_cnt;
volatile uint32_t g_task_menu_tick_cnt;



/********************** external functions definition ************************/
void task_setup_init(void *parameters)
{
	task_setup_dta_t   *p_task_setup_dta;
	task_setup_st_t	state;
	task_setup_ev_t	event;
	bool b_event;


	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_menu_init), p_task_menu);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_menu), p_task_menu_);

	g_task_menu_cnt = G_TASK_MEN_CNT_INI;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_menu_cnt), g_task_menu_cnt);

	init_queue_event_task_setup();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_setup_dta = & task_setup_dta;

	/* Print out: Task execution FSM */
	state = p_task_setup_dta->state;
	LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

	event = p_task_setup_dta->event;
	LOGGER_LOG("   %s = %lu", GET_NAME(event), (uint32_t)event);

	b_event = p_task_setup_dta->flag;
	LOGGER_LOG("   %s = %s\r\n", GET_NAME(b_event), (b_event ? "true" : "false"));

    displayInit( DISPLAY_CONNECTION_GPIO_4BITS );

	g_task_menu_tick_cnt = G_TASK_MEN_TICK_CNT_INI;
}

void task_setup_update(void *parameters)
{
	task_setup_dta_t *p_task_setup_dta;
	task_system_dta_t *p_task_system_dta;

	bool b_time_update_required = false;
    char menu_str[128] = {0};

	g_task_menu_cnt++;

	/* Protect shared resource (g_task_menu_tick) */
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
    {
    	g_task_menu_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts*/

    while (b_time_update_required)
    {
		/* Protect shared resource (g_task_menu_tick) */
		__asm("CPSID i");	/* disable interrupts*/
		if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
		{
			g_task_menu_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

    	/* Update Task Set Up Data Pointer */
		p_task_setup_dta = &task_setup_dta;
		p_task_system_dta = &task_system_dta;

    	if (DEL_MEN_XX_MIN < p_task_setup_dta->tick)
		{
			p_task_setup_dta->tick--;
		}
		else
		{

			p_task_setup_dta->tick = DEL_MEN_XX_MAX;

			if (true == any_event_task_setup())
			{
				p_task_setup_dta->flag = true;
				p_task_setup_dta->event = get_event_task_setup();
			}

			switch (p_task_setup_dta->state)
			{
				case ST_SETUP_CFG_XX:

					displayCharPositionWrite(0, 0);
					snprintf(menu_str, sizeof(menu_str), "PRESS ENTER TO");
					displayStringWrite(menu_str);
					displayCharPositionWrite(0, 1);
					snprintf(menu_str, sizeof(menu_str), "SELECT MODE");
					displayStringWrite(menu_str);
					p_task_setup_dta->flag = false;


	            	if ((true == p_task_setup_dta->flag) && (EV_SETUP_BTN_ENTER_ACTIVE == p_task_setup_dta->event)){
	            	{
	            		displayCharPositionWrite(0, 0);
	            		displayStringWrite("   Enter/Next    ");
	            		displayCharPositionWrite(0, 1);
	            		displayStringWrite(menu_str);
	            		snprintf(menu_str, sizeof(menu_str), "MODE 1");
	            		p_task_setup_dta->flag = false;
	            	  	p_task_setup_dta->state = ST_SETUP_ACTIVE;
	            	 }

	            break;//Fin case ST_SETUP_CFG_XX

				case ST_SETUP_ACTIVE:

					switch(p_task_setup_dta->event)
					{
						case EV_SETUP_BTN_ENTER_ACTIVE:

							displayCharPositionWrite(0, 0);
						 	displayStringWrite(menu_str);
						 	snprintf(menu_str, sizeof(menu_str), "MAX PERSONS:%ul",CANTIDAD_PERSONAS_CFG_01);
						 	displayCharPositionWrite(0, 1);
						 	displayStringWrite(menu_str);
						 	snprintf(menu_str, sizeof(menu_str), "WAIT TIME:%ul",TIMER_CFG_01);
						 	p_task_setup_dta->state = ST_SETUP_CFG_01;

						break;

						case EV_SETUP_BTN_NEXT_ACTIVE:

							displayCharPositionWrite(0, 0);
							displayStringWrite("   Enter/Next    ");
							displayCharPositionWrite(0, 1);
							displayStringWrite(menu_str);
							snprintf(menu_str, sizeof(menu_str), "MODE 2");
							p_task_setup_dta->flag = false;
							p_task_setup_dta->state = ST_SETUP_CFG_02;

						break;

						default:break;
					}


					/*if((true == p_task_setup_dta->flag) && (EV_SETUP_BTN_ENTER_ACTIVE == p_task_setup_dta->event))
					{
						displayCharPositionWrite(0, 0);
					 	displayStringWrite(menu_str);
					 	snprintf(menu_str, sizeof(menu_str), "MAX PERSONS:%ul",CANTIDAD_PERSONAS_CFG_01);
					 	displayCharPositionWrite(0, 1);
					 	displayStringWrite(menu_str);
					 	snprintf(menu_str, sizeof(menu_str), "WAIT TIME:%ul",TIMER_CFG_01);
					 	p_task_setup_dta->state = ST_SETUP_CFG_01;
					}
					else if((true == p_task_setup_dta->flag) && (EV_SETUP_BTN_NEXT_ACTIVE == p_task_setup_dta->event))
					{
						displayCharPositionWrite(0, 0);
						displayStringWrite("   Enter/Next    ");
						displayCharPositionWrite(0, 1);
						displayStringWrite(menu_str);
						snprintf(menu_str, sizeof(menu_str), "MODE 2");
						p_task_setup_dta->flag = false;
						p_task_setup_dta->state = ST_SETUP_CFG_02;
					}*/

				break; //Fin case ST_SETUP_ACTIVE

				case ST_SETUP_CFG_01:

					switch(p_task_setup_dta->event)
					{
						case EV_SETUP_BTN_ENTER_ACTIVE:

							p_task_system_dta->cantidad_personas = CANTIDAD_PERSONAS_CFG_01;
							p_task_system_dta->timer = TIMER_CFG_01;
							p_task_setup_dta->flag = false;
							p_task_setup_dta->state = ST_SETUP_IDLE;

						break;

						case EV_SETUP_BTN_NEXT_ACTIVE:

							displayCharPositionWrite(0, 0);
							displayStringWrite("   Enter/Next    ");
							displayCharPositionWrite(0, 1);
							displayStringWrite(menu_str);
							snprintf(menu_str, sizeof(menu_str), "MODE 2");
							p_task_setup_dta->flag = false;
							p_task_setup_dta->state = ST_SETUP_CFG_02;

						break;

						default:break;
					}

					/*if((true == p_task_setup_dta->flag) && (EV_SETUP_BTN_ENTER_ACTIVE == p_task_setup_dta->event))
					{
						p_task_system_dta->cantidad_personas = CANTIDAD_PERSONAS_CFG_01;
						p_task_system_dta->timer = TIMER_CFG_01;
						p_task_setup_dta->flag = false;
						p_task_setup_dta->state = ST_SETUP_IDLE;
					}
					else if((true == p_task_setup_dta->flag) && (EV_SETUP_BTN_NEXT_ACTIVE == p_task_setup_dta->event))
					{
						displayCharPositionWrite(0, 0);
						displayStringWrite("   Enter/Next    ");
						displayCharPositionWrite(0, 1);
						displayStringWrite(menu_str);
						snprintf(menu_str, sizeof(menu_str), "MODE 2");
						p_task_setup_dta->flag = false;
						p_task_setup_dta->state = ST_SETUP_CFG_02;
					}*/

				break;//Fin case ST_SETUP_CFG_XX

				case ST_SETUP_CFG_02:

					if(false == p_task_setup_dta->flag_cfg_02)
					{
						switch(p_task_setup_dta->event)
						{
							case EV_SETUP_BTN_ENTER_ACTIVE:

								displayCharPositionWrite(0, 0);
								snprintf(menu_str, sizeof(menu_str), "MAX PERSONS: %lu,CANTIDAD_PERSONAS_CFG_02");
								displayStringWrite(menu_str);
								displayCharPositionWrite(0, 1);
								snprintf(menu_str, sizeof(menu_str), "WAIT TIME: %lu",TIMER_CFG_02);
								displayStringWrite(menu_str);
								p_task_setup_dta->state = ST_SETUP_CFG_02;
								p_task_setup_dta->flag = false;
								p_task_setup_dta->flag_cfg_02 = true;

							break;

							case EV_SETUP_BTN_NEXT_ACTIVE:

								p_task_setup_dta->flag = false;
								p_task_setup_dta->state = ST_SETUP_IDLE;

							break;

							default:break;
						}

						/*if((true == p_task_setup_dta->flag) && (EV_SETUP_BTN_ENTER_ACTIVE == p_task_setup_dta->event))
						{
							displayCharPositionWrite(0, 0);
							snprintf(menu_str, sizeof(menu_str), "MAX PERSONS: %ul",CANTIDAD_PERSONAS_CFG_02);
							displayStringWrite(menu_str);
							displayCharPositionWrite(0, 1);
							snprintf(menu_str, sizeof(menu_str), "WAIT TIME: %ul",TIMER_CFG_02);
							displayStringWrite(menu_str);
							p_task_setup_dta->state = ST_SETUP_CFG_02;
							p_task_setup_dta->flag = false;
							p_task_setup_dta->flag_cfg_02 = true;
						}
						else if((true == p_task_setup_dta->flag) && (EV_SETUP_BTN_NEXT_ACTIVE == p_task_setup_dta->event))
						{
							p_task_setup_dta->flag = false;
							p_task_setup_dta->state = ST_SETUP_IDLE;
						}*/
					}
					else
					{
						if(EV_SETUP_BTN_ENTER_ACTIVE == p_task_setup_dta->event)
						{
							p_task_system_dta->cantidad_personas = CANTIDAD_PERSONAS_CFG_02;
							p_task_system_dta->timer = TIMER_CFG_02;
							p_task_setup_dta->flag = false;
							p_task_setup_dta->flag_cfg_02 = false;
							p_task_setup_dta->state = ST_SETUP_IDLE;
						}
					}

				break; //Fin case ST_SETUP_CFG_02

				case ST_SETUP_IDLE:

					displayCharPositionWrite(0, 0);
					//snprintf(menu_str, sizeof(menu_str), "PRESS ENTER TO"); Aca van a ir los valores de la temperatura
					displayStringWrite(menu_str);
					displayCharPositionWrite(0, 1);
					snprintf(menu_str, sizeof(menu_str), "MP = %lu WT = %lu",p_task_system_dta->cantidad_personas, p_task_system_dta->timer);
					displayStringWrite(menu_str);
					if(EV_SETUP_BTN_NEXT_ACTIVE == p_task_setup_dta->event)
					{
						p_task_setup_dta->flag = false;
						p_task_setup_dta->state = ST_SETUP_CFG_XX;
					}

				break; //Fin case ST_SETUP_IDLE

	}//EN SWITCH
   }//EN SWITCH
  }//END ELSE
 }//END WHILE
}// END FUNCTION

/********************** end of file ******************************************/

