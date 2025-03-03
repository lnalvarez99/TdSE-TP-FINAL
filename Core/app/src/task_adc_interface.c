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
 * @file   : task_menu_interface.c
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
#include "task_adc_interface.h"

/********************** macros and definitions *******************************/
#define EVENT_UNDEFINED	(255)
#define MAX_EVENTS 30

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/
struct
{
    uint32_t head;              // Índice de inicio
    uint32_t tail;              // Índice de fin
    uint32_t count;             // Número de elementos en la cola
    uint32_t queue[MAX_EVENTS]; // Buffer de datos
}queue_task_adc;

/****** external functions definition ************************/

void init_queue_value_task_adc(void)
{
	uint32_t i;

	queue_task_adc.head = 0;
	queue_task_adc.tail = 0;
	queue_task_adc.count = 0;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_task_adc.queue[i] = EVENT_UNDEFINED;
}

void put_value_task_adc(uint32_t adc_value)
{
    if (queue_task_adc.count < MAX_EVENTS)
    {
        queue_task_adc.queue[queue_task_adc.tail] = adc_value;
        queue_task_adc.tail = (queue_task_adc.tail + 1) % MAX_EVENTS;
        queue_task_adc.count++;
    }
}

uint32_t get_value_task_adc(void)
{
    uint32_t value = 255;

    if (queue_task_adc.count > 0)
    {
        value = queue_task_adc.queue[queue_task_adc.head];
        queue_task_adc.head = (queue_task_adc.head + 1) % MAX_EVENTS;
        queue_task_adc.count--;
    }

    return value;
}

bool any_value_task_adc(void) {
    return (queue_task_adc.count > 0);
}

/********************** end of file ******************************************/

