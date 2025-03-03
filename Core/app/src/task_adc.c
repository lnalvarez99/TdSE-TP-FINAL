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
 * @file   : task_adc.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"
#include "stdbool.h"
/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "task_adc_interface.h"
#include "task_adc_attribute.h"
// Ajusta según tu microcontrolador


/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

uint32_t averaged_temp_amb = 0;
uint32_t averaged_temp_uC = 0;
volatile uint32_t sample_cnt = 0;
uint32_t     first_sample = 1;
uint32_t ADC_VAL[2] = {0, 0};


/********************** internal functions declaration ***********************/
/********************** internal data definition *****************************/
const char *p_task_adc 		= "Task ADC";
/********************** external data declaration *****************************/
extern ADC_HandleTypeDef hadc1;
volatile bool sample_to_take ;

/********************** internal functions declaration ***********************/
/********************** external functions definition ************************/
void task_adc_init(void *parameters){
	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_adc_init), p_task_adc);

    /* Inicializar la cola de valores del ADC */
    init_queue_value_task_adc();
}

void task_adc_update(void *parameters){

	if(0 < first_sample)
    {
	    first_sample = 0;
		HAL_ADC_Start_DMA(&hadc1, ADC_VAL, 2);
    }

    if(sample_cnt< AVERAGER_SIZE ){

        if(sample_to_take==true)
        {
			averaged_temp_amb += ADC_VAL[0];  // Temperatura ambiente (LM35)
            averaged_temp_uC += ADC_VAL[1];   // Temperatura del microcontrolador

			HAL_ADC_Start_DMA(&hadc1, ADC_VAL, 2);
		    sample_cnt++;
		    sample_to_take=false;
        }
    }
	else
    {

		averaged_temp_amb /= AVERAGER_SIZE;
        averaged_temp_uC /= AVERAGER_SIZE;

        // Convertir los valores a grados Celsius
        temp_amb = (3.30 * 100 * averaged_temp_amb) / 4096;
        temp_uC = ((1700 - averaged_temp_uC) / 4.3) + 25;

        // Almacenar los valores en la cola
        put_value_task_adc(temp_amb);
        put_value_task_adc(temp_uC);

        // Reiniciar variables
        averaged_temp_amb = 0;
        averaged_temp_uC = 0;
        sample_cnt = 0;
        first_sample = 1;
	}

    return;
}

//	Callback de interrupción del ADC (se ejecuta cuando la conversión se completa)
	void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

	// Check which version of the adc triggered this callback
	if (hadc == &hadc1){
		sample_to_take=true;}
	}

/********************** end of file ******************************************/


