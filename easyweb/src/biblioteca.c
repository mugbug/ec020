#include "biblioteca.h"

static void init_ssp(void) {
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

static void init_uart(void) {
	PINSEL_CFG_Type PinCfg;
	UART_CFG_Type uartCfg;

	/* Initialize UART3 pin connect */
	PinCfg.Funcnum = 2;
	PinCfg.Pinnum = 0;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);

	uartCfg.Baud_rate = 115200;
	uartCfg.Databits = UART_DATABIT_8;
	uartCfg.Parity = UART_PARITY_NONE;
	uartCfg.Stopbits = UART_STOPBIT_1;

	UART_Init(UART_DEV, &uartCfg);

	UART_TxCmd(UART_DEV, ENABLE);

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

void print_disp(uint8_t op, uint32_t minL, uint32_t maxL) {

	uint8_t buf[50];

	oled_clearScreen(OLED_COLOR_BLACK);

	if (op) {
		oled_putString(1, 1, (uint8_t*) "EC-020", OLED_COLOR_WHITE,
				OLED_COLOR_BLACK);
		oled_putString(1, 17, (uint8_t*) "1137, 1020, 982, 1045", OLED_COLOR_WHITE,
				OLED_COLOR_BLACK);

		oled_putString(1, 34, (uint8_t*) "IP Address:", OLED_COLOR_WHITE,
				OLED_COLOR_BLACK);

		sprintf((char*) buf, " %d.%d.%d.%d", MYIP_1, MYIP_2, MYIP_3, MYIP_4);
		oled_putString(1, 50, (uint8_t*) buf, OLED_COLOR_WHITE,
				OLED_COLOR_BLACK);
	} else {

		sprintf((char*) buf, "MAX VALUE:%d", maxL);
		oled_putString(1, 1, (uint8_t*) buf, OLED_COLOR_WHITE,
				OLED_COLOR_BLACK);

		sprintf((char*) buf, "MIN VALUE:%d", minL);
		oled_putString(1, 17, (uint8_t*) buf, OLED_COLOR_WHITE,
				OLED_COLOR_BLACK);
	}
	Timer0_Wait('.');
}

void print_uart(uint8_t op) {
	if (!op) {
		UART_SendString(UART_DEV, (uint8_t*) "1 - Info\r\n");
		UART_SendString(UART_DEV, (uint8_t*) "2 - Encerrar a aplicacao\r\n");
	} else if (op) {
		UART_SendString(UART_DEV, (uint8_t*) "\n\n\rProjeto de EC - 020\r\n");
		UART_SendString(UART_DEV,
				(uint8_t*) "Aplicacao desenvolvida em super-loop\r\n");
		UART_SendString(UART_DEV,
				(uint8_t*) "Leitura do sensor de luminosidade\r\n");
	}

}

void init_all(void) {
	init_ssp();
	init_i2c();
	init_uart();
	light_enable();
	oled_init();
	TCPLowLevelInit();
	TCPLocalPort = TCP_PORT_HTTP;
}

void exit_(void) {
	oled_clearScreen(OLED_COLOR_BLACK);
	oled_putString(1, 1, (uint8_t*) "Aplicacao", OLED_COLOR_WHITE,
			OLED_COLOR_BLACK);
	oled_putString(1, 15, (uint8_t*) "encerrada", OLED_COLOR_WHITE,
			OLED_COLOR_BLACK);
	for (;;);
}
