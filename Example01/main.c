/*
    FreeRTOS V6.1.1 - Copyright (C) 2011 Real Time Engineers Ltd.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "basic_io.h"
#include "custom_lib.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/*-----------------------------------------------------------*/

unsigned long counter = 0;
xQueueHandle oled_queue;

struct config light = {
		.init = init_all,
		.minL = 1000,
		.maxL = 0,
		.current = 0
};

struct accelerometer axis = {
		.x = 0,
		.y = 0,
		.z = 0
};

uint8_t  btn1 = 0,
		 op   = 1,
		 aux  = 0,
		 update_acc = 0;

/* ------------------------------------------*/

#define INPUT 	0
#define OUTPUT	1

#define DEFAULT_PORT	0
#define DEFAULT_PIN		4

void Button_init(void)
{
	GPIO_SetDir( DEFAULT_PORT, DEFAULT_PIN, INPUT);

	xTaskCreate( taskButton, "Button", 192, NULL, 1, NULL );
}

/* ------------------------------------------*/
int main (void) {

	light.init();

	uint32_t lr   = 0;

	/* Init the semi-hosting. */
	printf( "\n" );

	/* Create one of the two tasks. */
	xTaskCreate(	vTask1,		/* Pointer to the function that implements the task. */
					"Task 1",	/* Text name for the task.  This is to facilitate debugging only. */
					240,		/* Stack depth in words. */
					NULL,		/* We are not using the task parameter. */
					2,			/* This task will run at priority 1. */
					&xHandle_1 );		/* We are not using the task handle. */

	/* Create the other task in exactly the same way. */
	xTaskCreate( vTask2, "Task 2", 240, NULL, 1, &xHandle_2 );

	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
	return 0;
}
/*-----------------------------------------------------------*/

void taskButton( void *pvParameters ) {

}

void vTask1( void *pvParameters ) {
	const char *pcTaskName = "\nTask 1; counter =";
	volatile unsigned long ul;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		btn1 = ((GPIO_ReadValue(0) >> 4) & 0x01);

		// check if button was pressed
		if (!btn1) {
			op = !op;
			vPortEnterCritical();
			print_disp(op, counter, light.maxL, axis, update_acc);
			vPortExitCritical();
			vTaskDelay(10);
			counter += 1;
		}
		/* Print out the name of this task. */
		vPrintStringAndNumber( pcTaskName, counter);
		//vTaskDelay( 250 / portTICK_RATE_MS );
	}
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
const char *pcTaskName = "\nTask 2; counter =";
volatile unsigned long ul;

	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		if (counter >= 30) vTaskSuspend(NULL);
		if (counter >= 120) vTaskDelete(NULL);

		counter += 1;

		/* Print out the name of this task. */
		vPrintStringAndNumber( pcTaskName, counter );
		/* Delay for a period. */
		vTaskDelay( 500 / portTICK_RATE_MS );
	}
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* This function will only be called if an API call to create a task, queue
	or semaphore fails because there is too little heap RAM remaining. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* This function will only be called if a task overflows its stack.  Note
	that stack overflow checking does slow down the context switch
	implementation. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* This example does not use the idle hook to perform any processing. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This example does not use the tick hook to perform any processing. */
}


