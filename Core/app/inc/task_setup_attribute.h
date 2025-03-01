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
 * @file   : task_menu_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_MENU_ATTRIBUTE_H_
#define TASK_INC_TASK_MENU_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/




/********************** typedef **********************************************/
/* Menu Statechart - State Transition Table */
/* 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 * 	| Current               | Event                 |                       | Next                  |                       |
 * 	| State                 | (Parameters)          | [Guard]               | State                 | Actions               |
 * 	|=======================+=======================+=======================+=======================+=======================|
 * 	| ST_MEN_XX_IDLE        | EV_MEN_MAN_IDLE     |                       | ST_MEN_XX_ACTIVE      |                       |
 * 	|                       |                       |                       |                       |                       |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_MEN_XX_ACTIVE      | EV_MEN_MAN_IDLE       |                       | ST_MEN_XX_IDLE        |                       |
 * 	|                       |                       |                       |                       |                       |
 * 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 */

/* Events to excite Task Menu */
typedef enum task_menu_ev {
						   EV_SETUP_BTN_ENTER_IDLE,
						   EV_SETUP_BTN_ENTER_ACTIVE,
						   EV_SETUP_BTN_NEXT_IDLE,
						   EV_SETUP_BTN_NEXT_ACTIVE,
						} task_setup_ev_t;

/* State of Task Menu */
typedef enum task_menu_st {ST_SETUP_IDLE,
						   ST_SETUP_ACTIVE, // En este estado podria pasar por display las configuraciones del setup
						   ST_SETUP_CFG_XX,
						   ST_SETUP_CFG_01, // Pasar por display que se esta en configuracion 1
						   ST_SETUP_CFG_02, // Pasar por display que se esta en configuracion 2
							} task_setup_st_t;


typedef struct
{
	uint32_t		tick;
	task_setup_st_t	state;
	task_setup_ev_t	event;
	bool 			flag;
	bool			flag_cfg_02;

} task_setup_dta_t;


typedef struct
{	uint32_t       cantidad_personas;
	uint32_t       tiempo_espera;
} task_configuration_dta_t;


/********************** external data declaration ****************************/
extern task_setup_dta_t        task_setup_dta;
extern task_configuration_dta_t  task_configuracion_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif  // TASK_INC_TASK_MENU_ATTRIBUTE_H

/********************** end of file ******************************************/
