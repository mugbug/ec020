/*
 * Temperature.c
 *
 *  Created on: 10 de abr de 2018
 *      Author: samuelpereira
 */

#include "LightSensor.h"
#include "light.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "basic_io.h"

static callback_t tx_callback;

void LightSensor_readerTask( void *pvParameters );
static void init_i2c(void);

typedef struct tagLightSensor
{
	int32_t light;
}ttagLightSensor;

static ttagLightSensor Light_Instance;

void LightSensor_init ()
{
	init_i2c();
	light_enable();
	xTaskCreate( LightSensor_readerTask, "Light", 192, NULL, 1, NULL );
}

void LightSensor_setCallback(callback_t c)
{
	if (c != NULL)
	{
		tx_callback = c;
	}
}

int32_t LightSensor_read(void)
{
	Light_Instance.light = light_read();
	return Light_Instance.light;
}

int32_t LightSensor_getLight(void)
{
	return Light_Instance.light;
}

void LightSensor_readerTask( void *pvParameters )
{
	message_t msg;

	while(1)
	{
		memset(&msg, 0x00, sizeof(msg));
		msg.source = LIGHT;
		msg.payload[0] = LightSensor_read();

		if (tx_callback != NULL)
		{
			tx_callback(msg);
		}

		vTaskDelay( 500 / portTICK_RATE_MS );
	}
}

static void init_i2c(void) {
	PINSEL_CFG_Type PinCfg;

	/* Initialize I2C2 pin connect */
	PinCfg.Funcnum = 2;
	PinCfg.Pinnum = 10;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 11;
	PINSEL_ConfigPin(&PinCfg);

	// Initialize I2C peripheral
	I2C_Init(LPC_I2C2, 100000);

	/* Enable I2C1 operation */
	I2C_Cmd(LPC_I2C2, ENABLE);
}
