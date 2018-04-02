#ifndef BIBLIOTECA_H_
#define BIBLIOTECA_H_

#define UART_DEV LPC_UART3

#include "lpc17xx_ssp.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"

#include "light.h"
#include "oled.h"
#include "uart2.h"
#include "tcpip.h"

#include "string.h"

void init_all(void);
void print_disp(uint8_t op, uint32_t minL, uint32_t maxL);
void print_uart(uint8_t op);
void exit_(void);

struct config {
	void (*init)(void);
	void (*exit) (void);
	uint32_t minL;
	uint32_t maxL;
};

#endif
