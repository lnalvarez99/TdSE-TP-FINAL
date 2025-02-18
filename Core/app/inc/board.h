/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar>.
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
 * @file   : board.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef BOARD_INC_BOARD_H_
#define BOARD_INC_BOARD_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/
#define NUCLEO_F103RC		(0)
#define NUCLEO_F401RE		(1)
#define NUCLEO_F446RE		(2)
#define NUCLEO_F429ZI		(3)
#define NUCLEO_F439ZI		(4)
#define NUCLEO_F413ZH		(5)
#define STM32F429I_DISCO1	(6)

#define BOARD (NUCLEO_F103RC)

/* STM32 Nucleo Boards - 64 Pins */
#if ((BOARD == NUCLEO_F103RC) || (BOARD == NUCLEO_F401RE) || (BOARD == NUCLEO_F446RE))

/****************************PLACA PARA TRABAJO FINAL********************************/

/***********************************SENSORES*****************************************/

/*Todos los sensores estan en pull up, para accionarlos el boton corta el paso de la
 corriente por lo tanto el pin presionado tiene la logica de reset*/

#define BTN_INGRESO_PIN GPIO_PIN_0 // PA0
#define BTN_INGRESO_PIN GPIO_PIN_0 // PA0
#define BTN_INGRESO_PORT GPIOA
#define BTN_INGRESO_PRESSED GPIO_PIN_RESET
#define BTN_INGRESO_RELEASED GPIO_PIN_SET

#define BTN_EGRESO_PIN GPIO_PIN_1 // PB1
#define BTN_EGRESO_PORT GPIOB
#define BTN_EGRESO_PRESSED GPIO_PIN_RESET
#define BTN_EGRESO_RELEASED GPIO_PIN_SET

#define BTN_ENTER_PIN GPIO_PIN_10 // PB10
#define BTN_ENTER_PORT GPIOB
#define BTN_ENTER_PRESSED GPIO_PIN_RESET
#define BTN_ENTER_RELEASED GPIO_PIN_SET

#define BTN_NEXT_PIN GPIO_PIN_11 // PB11
#define BTN_NEXT_PORT GPIOB
#define BTN_NEXT_PRESSED GPIO_PIN_RESET
#define BTN_NEXT_RELEASED GPIO_PIN_SET

#define BTN_ON_PIN GPIO_PIN_12 // PB12
#define BTN_ON_PORT GPIOB
#define BTN_ON_PRESSED GPIO_PIN_RESET
#define BTN_ON_RELEASED GPIO_PIN_SET

#define SWITCH_BARINF_PIN GPIO_PIN_13 // PB13
#define SWITCH_BARINF_PORT GPIOB
#define SWITCH_BARINF_PRESSED GPIO_PIN_RESET
#define SWITCH_BARINF_RELEASED GPIO_PIN_SET

#define SWITCH_OFF_PIN GPIO_PIN_14 // PB14
#define SWITCH_OFF_PORT GPIOB
#define SWITCH_OFF_PRESSED GPIO_PIN_RESET
#define SWITCH_OFF_RELEASED GPIO_PIN_SET

/***********************************ACTUADORES***************************************/

#define LED_MAX_VEL_PIN GPIO_PIN_4 // PA4
#define LED_MAX_VEL_PORT GPIOA
#define LED_MAX_VEL_ON GPIO_PIN_SET
#define LED_MAX_VEL_OFF GPIO_PIN_RESET

#define LED_MIN_VEL_PIN GPIO_PIN_12 // PA12
#define LED_MIN_VEL_PORT GPIOA
#define LED_MIN_VEL_ON GPIO_PIN_SET
#define LED_MIN_VEL_OFF GPIO_PIN_RESET

#define LED_SYSCTRL_ACT_PIN GPIO_PIN_11 // PA11
#define LED_SYSCTRL_ACT_PORT GPIOA
#define LED_SYSCTRL_ACT_ON GPIO_PIN_SET
#define LED_SYSCTRL_ACT_OFF GPIO_PIN_RESET

#define LED_SYSCTRL_DIS_PIN GPIO_PIN_10 //PA10
#define LED_SYSCTRL_DIS_PORT GPIOA
#define LED_SYSCTRL_DIS_ON GPIO_PIN_SET
#define LED_SYSCTRL_DIS_OFF GPIO_PIN_RESET

#define BUZZER_PIN GPIO_PIN_1 //PA1
#define BUZZER_PORT GPIOA
#define BUZZER_ON GPIO_PIN_SET
#define BUZZER_OFF GPIO_PIN_RESET


#endif/* STM32 Nucleo Boards - 144 Pins */

#if ((BOARD == NUCLEO_F429ZI) || (BOARD == NUCLEO_F439ZI) || (BOARD == NUCLEO_F413ZH))

#define BTN_A_PIN		USER_Btn_Pin
#define BTN_A_PORT		USER_Btn_GPIO_Port
#define BTN_A_PRESSED	GPIO_PIN_SET
#define BTN_A_HOVER		GPIO_PIN_RESET

#define LED_A_PIN		LD1_Pin
#define LED_A_PORT		LD1_GPIO_Port
#define LED_A_ON		GPIO_PIN_SET
#define LED_A_OFF		GPIO_PIN_RESET

#endif

/* STM32 Discovery Kits */
#if (BOARD == STM32F429I_DISCO1)

#define BTN_A_PIN		B1_Pin
#define BTN_A_PORT		B1_GPIO_Port
#define BTN_A_PRESSED	GPIO_PIN_SET
#define BTN_A_HOVER		GPIO_PIN_RESET

#define LED_A_PIN		LD3_Pin
#define LED_A_PORT		LD3_GPIO_Port
#define LED_A_ON		GPIO_PIN_SET
#define LED_A_OFF		GPIO_PIN_RESET

#endif

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* BOARD_INC_BOARD_H_ */

/********************** end of file ******************************************/


