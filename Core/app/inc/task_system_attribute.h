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
 * @file   : task_system_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_
#define TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/
/* System Statechart - State Transition Table */
/* 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 * 	| Current               | Event                 |                       | Next                  |                       |
 * 	| State                 | (Parameters)          | [Guard]               | State                 | Actions               |
 * 	|=======================+=======================+=======================+=======================+=======================|
 * 	| ST_SYS_XX_IDLE        | EV_SYS_XX_ACTIVE      |                       | ST_SYS_XX_ACTIVE      | put_event_t.._actuator|
 * 	|                       |                       |                       |                       | (event, identifier)   |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_SYS_XX_ACTIVE      | EV_SYS_XX_IDLE        |                       | ST_SYS_XX_IDLE        | put_event_t.._actuator|
 * 	|                       |                       |                       |                       | (event, identifier)   |
 * 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 */

/* Events to excite Task System */
typedef enum task_system_ev {EV_BTN_SYSCTRL_UP,
							 EV_BTN_SYSCTRL_DOWN,
							 EV_SWITCH_SYSCTRL_UP,
							 EV_SWITCH_SYSCTRL_DOWN,
							 EV_BTN_IN_UP,
							 EV_BTN_IN_DOWN,
							 EV_BTN_OUT_UP,
							 EV_BTN_OUT_DOWN,
							 EV_SWITCH_BARINF_UP,
							 EV_SWITCH_BARINF_DOWN,
							 EV_BTN_NEXT_UP, // INICIO DE BOTONES PARA CAMBIAR ENTRE MODO NORMAL Y SETUP
							 EV_BTN_NEXT_DOWN,
							 EV_BTN_ENTR_UP,
							 EV_BTN_ENTR_DOWN
							} task_system_ev_t;

/* State of Task System */
typedef enum task_system_st {ST_SYSCTRL_OFF,
							 ST_SYSCTRL_ON,
							 ST_SYSCTRL_NOR,
							 ST_SYSCTRL_SETUP,
							 ST_SYSCTRL_CPRS, //CANTIDAD DE PERSONAS
							 ST_SYSCTRL_WTMR, // TIEMPO DE ESPERA
							 ST_SYSCTRL_STOP,// SISTEMA FRENADO POR AUSENCIA DE PERSONAS
							 ST_SYSCTRL_IDLE//SISTEMA EN REPOSO
							 } task_system_st_t;

typedef struct
{
	uint16_t			tick; // contador para la cantidad de personas sobre la escalera
	uint16_t			cntd_personas; // Guarda para la cantidad de personas
	uint16_t			timer; // Guarda para el tiempo de espera
	task_system_st_t	state;
	task_system_ev_t	event;
	bool				flag;
} task_system_dta_t;

/********************** external data declaration ****************************/
extern task_system_dta_t task_system_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_ */

/********************** end of file ******************************************/

