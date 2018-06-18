#include "JoystickTask.h"
#include "joystick.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "basic_io.h"

#include <stdlib.h>
#include <stdbool.h>

static callback_t tx_callback;

void Joystick_readerTask( void *pvParameters );

void Joystick_init(void) {
	joystick_init();
	xTaskCreate( Joystick_readerTask, "Joystick", 200, NULL, 1, NULL);
}

void Joystick_setCallback(callback_t c) {
	if (c != NULL)
	{
		tx_callback = c;
	}
}

bool canUpdateJoystickState(uint8_t joystick_state) {
	return joystick_state == JOYSTICK_LEFT || joystick_state == JOYSTICK_RIGHT;
}

void Joystick_readerTask( void *pvParameters ) {
	message_t msg;
	bool joystick_state_changed = true;
	while (1) {
		memset(&msg, 0x00, sizeof(msg));
		msg.source = JOY;
		msg.payload[0] = joystick_read();

		if (tx_callback != NULL && canUpdateJoystickState(msg.payload[0])) {
			msg.payload[1] = joystick_state_changed;
			tx_callback(msg);
		}
		vTaskDelay( 400 / portTICK_RATE_MS );
	}
}

