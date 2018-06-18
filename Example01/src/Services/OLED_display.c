/*
 * OLED_display.c
 *
 *  Created on: 15 de abr de 2018
 *      Author: samuelpereira
 */


#include "OLED_display.h"
#include "oled.h"
#include "joystick.h"

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_ssp.h"

#include <stdbool.h>

/* FreeRTOS.org includes. */
#include "task.h"
#include "semphr.h"

/* Demo includes. */
#include "basic_io.h"

#define INVALID -126

typedef struct oled_data
{
	int32_t x;
	int32_t y;
	int32_t z;
	int32_t light;
	int32_t joystick_state;
	bool rgb_on;
	bool should_init_display;
	bool ready;
} oled_data_t;

uint8_t buf[30];
static oled_data_t data;
xSemaphoreHandle data_semaphr;

static void OLED_display_init_ssp(void);

void OLED_display_writerTask( void *pvParameters );
void OLED_display_updaterTask( void *pvParameters );

void OLED_display_init_ssp(void)
{
	SSP_CFG_Type SSP_ConfigStruct;
	PINSEL_CFG_Type PinCfg;

	/*
	 * Initialize SPI pin connect
	 * P0.7 - SCK;
	 * P0.8 - MISO
	 * P0.9 - MOSI
	 * P2.2 - SSEL - used as GPIO
	 */
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 7;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 8;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 9;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Funcnum = 0;
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);

	SSP_ConfigStructInit(&SSP_ConfigStruct);

	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP1, ENABLE);
}

void OLED_display_init (void)
{
	OLED_display_init_ssp();
	oled_init();

	OLED_display_clearScreen();

	data.rgb_on = false;
	data.should_init_display = true;
	data.light = INVALID;
	data.x = INVALID;
	data.y = INVALID;
	data.z = INVALID;
	data.ready = false;

	OLED_display_queue = xQueueCreate( 8, sizeof(message_t) );
	data_semaphr = xSemaphoreCreateMutex();

	xTaskCreate( OLED_display_writerTask, "OLED writer", 240, NULL, 1, NULL );
	xTaskCreate( OLED_display_updaterTask, "OLED updater", 100, NULL, 1, NULL );
}

void OLED_display_fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	oled_fillRect(x0, y0, x1, y1, OLED_COLOR_BLACK);
}

void OLED_display_clearScreen(void)
{
	oled_clearScreen(OLED_COLOR_BLACK);
}

void OLED_display_putString(uint8_t x, uint8_t y, uint8_t *pStr)
{
	oled_putString(x, y, pStr, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void initDisplayAccordingToJoystick() {
	if (data.joystick_state == JOYSTICK_LEFT) {
		vPortEnterCritical();

		OLED_display_clearScreen();
		OLED_display_putString( 1, 1, (uint8_t*) "Projeto RTOS" );
		OLED_display_putString( 1, 13, (uint8_t*) "Light:" );
		OLED_display_putString( 1, 25, (uint8_t*) "Accelerometer:" );
		OLED_display_putString( 15, 33, (uint8_t*) "X -> " );
		OLED_display_putString( 15, 41, (uint8_t*) "Y -> " );
		OLED_display_putString( 15, 49, (uint8_t*) "Z -> " );

		vPortExitCritical();
	} else if (data.joystick_state == JOYSTICK_RIGHT) {
		vPortEnterCritical();

		OLED_display_clearScreen();
		OLED_display_putString( 1, 1, (uint8_t*) "** VEM P10 **" );
		OLED_display_putString( 1, 17, (uint8_t*) "Alexandre Miz." );
		OLED_display_putString( 1, 29, (uint8_t*) "Pedro Zaroni" );
		OLED_display_putString( 1, 41, (uint8_t*) "Tiago Pitaluga" );
		OLED_display_putString( 1, 53, (uint8_t*) "Rodrigo Maciel" );

		vPortExitCritical();
	}
	data.should_init_display = false;
}

void printInfoOnDisplay() {
	if (!data.rgb_on) {
		OLED_display_clearScreen();
		data.should_init_display = true;
	} else if (data.rgb_on) {
		if (data.should_init_display) {
			initDisplayAccordingToJoystick();
			data.should_init_display = false;
		}
		if (data.joystick_state == JOYSTICK_LEFT) {
			vPortEnterCritical();

			/* displaying info in the oled display */
			OLED_display_fillRect( (1 + 9 * 6), 13, 80, 24 );
			intToString( data.light, buf, 10, 10 );

			OLED_display_putString( (1 + 9 * 6), 13, (uint8_t*) buf );

			vPortExitCritical();

			vTaskDelay(10);

			vPortEnterCritical();

			intToString( data.x, buf, 10, 10 );
			OLED_display_fillRect( (1 + 9 * 6), 33, 80, 32 );
			OLED_display_putString( (1 + 9 * 6), 33, buf );

			intToString( data.y, buf, 10, 10 );
			OLED_display_fillRect( (1 + 9 * 6), 41, 80, 40 );
			OLED_display_putString( (1 + 9 * 6), 41, buf );

			intToString( data.z, buf, 10, 10 );
			OLED_display_fillRect( (1 + 9 * 6), 49, 80, 48 );
			OLED_display_putString( (1 + 9 * 6), 49, buf );


			vPortExitCritical();

			vTaskDelay( 1000 / portTICK_RATE_MS);
		}
	}
}

void OLED_display_writerTask( void *pvParameters )
{
	data.joystick_state = JOYSTICK_RIGHT;
	while(1)
	{
		if (!data.ready) {
			if ( data.x != INVALID && data.y != INVALID && data.z != INVALID ) {
				data.ready = true;
			}
		} else {
			printInfoOnDisplay();
		}
	}
}

void OLED_display_updaterTask( void *pvParameters )
{
	message_t msg;
	portBASE_TYPE xStatus = pdFALSE;
	portTickType blockTime = 20 / portTICK_RATE_MS;

	while(1)
	{
		xStatus = xQueueReceive( OLED_display_queue , &msg, blockTime );

		if( xStatus == pdTRUE )
		{
			switch( msg.source )
			{
				case LIGHT:
					xSemaphoreTake(data_semaphr, blockTime);
					data.light = msg.payload[0];
					xSemaphoreGive(data_semaphr);
					break;
				case ACC:
					xSemaphoreTake(data_semaphr, blockTime);
					data.x = msg.payload[0];
					data.y = msg.payload[1];
					data.z = msg.payload[2];
					xSemaphoreGive(data_semaphr);
					break;
				case RGB:
					xSemaphoreTake(data_semaphr, blockTime);
					data.rgb_on = msg.payload[0];
					xSemaphoreGive(data_semaphr);
					break;
				case JOY:
					xSemaphoreTake(data_semaphr, blockTime);
					data.joystick_state = msg.payload[0];
					data.should_init_display = msg.payload[1];
					xSemaphoreGive(data_semaphr);
				default:
					break;
			}
		}
	}
}
