/*
 * MainApp.c
 *
 *  Created on: 15 de abr de 2018
 *      Author: samuelpereira
 */

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "basic_io.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include <Systick.h>

#include "Button.h"
#include "LightSensor.h"
#include "Accelerometer.h"
#include "RGB_Leds.h"
#include "OLED_display.h"
#include "JoystickTask.h"

#include "common.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xfffff )

void acc_callback(message_t msg);
void but_callback(message_t msg);
void light_callback(message_t msg);
void RGB_callback(message_t msg);
void joystick_callback(message_t msg);

int main( void )
{
	/* Init the semi-hosting. */
	printf( "\n" );

	Accelerometer_init();
	Accelerometer_setCallback( acc_callback );

	Button_init();
	Button_setCallback( but_callback );

	Joystick_init();
	Joystick_setCallback( joystick_callback );

	LightSensor_init();
	LightSensor_setCallback( light_callback );

	RGB_Leds_init();
	RGB_Leds_setCallback( RGB_callback );

	OLED_display_init();

	/* Start the scheduler so our tasks start executing. */
	vTaskStartScheduler();

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
	return 0;
}

void acc_callback(message_t msg) {

	vPrintString("acc_callback");
	if(msg.source == ACC) {
		/* Send message to OLED display */
		xQueueSendToBack( OLED_display_queue, &msg, 15);
	}
}

void but_callback(message_t msg) {

	vPrintString("but_callback");
	if(msg.source == BUT) {
		/* Send message to RGB Leds */
		xQueueSendToBack( RGB_Leds_queue, &msg, 15);
	}
}

void joystick_callback(message_t msg) {
	if(msg.source == JOY) {
		/* Send message to RGB Leds */
		xQueueSendToBack( OLED_display_queue, &msg, 15);
	}
}

void light_callback(message_t msg) {
	vPrintString("light_callback");
	if(msg.source == LIGHT)
	{
		/* Send message to OLED display */
		xQueueSendToBack( OLED_display_queue, &msg, 15);
	}
}

void RGB_callback(message_t msg) {
	vPrintString("RGB_callback");
	if(msg.source == RGB)
	{
		/* Send message to OLED display */
		xQueueSendToBack( OLED_display_queue, &msg, 15);
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

