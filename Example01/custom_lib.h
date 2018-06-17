#ifndef CUSTOM_LIB_H_
#define CUSTOM_LIB_H_

#define UART_DEV LPC_UART3

#include "lpc17xx_ssp.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"

#include "light.h"
#include "oled.h"
//#include "tcpip.h"
#include "rgb.h"
#include "acc.h"
#include "joystick.h"

#include "string.h"

struct accelerometer {
		int8_t x;
		int8_t y;
		int8_t z;
};

struct config {
	void (*init)(void);
	void (*exit) (void);
	uint32_t minL;
	uint32_t maxL;
	uint32_t current;
};

void init_all(void);
void print_disp(uint8_t op, uint32_t minL, uint32_t maxL, struct accelerometer axis, uint8_t acc);

#endif
