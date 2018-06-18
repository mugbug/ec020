/*
 * RGB_Leds.c
 *
 *  Created on: 11 de abr de 2018
 *      Author: samuelpereira
 */

#include "RGB_Leds.h"

/* FreeRTOS.org includes. */
#include "task.h"

/* Demo includes. */
#include "basic_io.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define INVALID 126

typedef struct tagRGB_Leds
{
	bool on;
	int8_t y;
	uint8_t threshold;
}ttagRGB_Leds;

static ttagRGB_Leds RGB_Leds_Instance;

static callback_t tx_callback;

void RGB_Leds_updaterTask( void *pvParameters );
inline static uint8_t calc_threshold( uint16_t trimpot_value );

void RGB_Leds_init (void)
{
	rgb_init();

	RGB_Leds_setLeds(0);

	RGB_Leds_Instance.on = true;
	RGB_Leds_Instance.y = INVALID;
	RGB_Leds_Instance.threshold = INVALID;

	RGB_Leds_queue = xQueueCreate( 6, sizeof(message_t) );

	xTaskCreate( RGB_Leds_updaterTask, "RGB Task", 128, NULL, 1, NULL );
}

void RGB_Leds_setCallback(callback_t c)
{
	if (c != NULL)
	{
		tx_callback = c;
	}
}

void RGB_Leds_setLeds (uint8_t ledMask)
{
	rgb_setLeds(ledMask);
}

void RGB_Leds_updaterTask( void *pvParameters )
{
	message_t msg;
	message_t msg_out;
	char buffer[20];
	portBASE_TYPE xStatus = pdFALSE;
	portTickType blockTime = 20 / portTICK_RATE_MS;
	bool valid_source = false;

	while(1)
	{
		xStatus = xQueueReceive(RGB_Leds_queue, &msg, blockTime);

		if( xStatus == pdTRUE )
		{
			memset( buffer, 0x00, sizeof(buffer) );

			if( msg.source == BUT ) {
				vPrintString( "Button message received\n" );

				if( msg.payload[0] == 0) {
					RGB_Leds_Instance.on = !RGB_Leds_Instance.on;

					memset( &msg_out, 0x00, sizeof(msg_out) );
					msg_out.source = RGB;
					msg_out.payload[0] = RGB_Leds_Instance.on;
					tx_callback( msg_out );
					RGB_Leds_setLeds(RGB_LEDS_GREEN);
				} else {
					RGB_Leds_setLeds(RGB_LEDS_RED);
				}
			}
		}
	}
}

uint8_t calc_threshold( uint16_t trimpot_value )
{
	return (uint8_t) (10 + (trimpot_value / 1000) * 2);
}
