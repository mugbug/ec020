/******************************************************************
 *****                                                        *****
 *****  Name: easyweb.c                                       *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 07/05/2001                                      *****
 *****  Auth: Andreas Dannenberg                              *****
 *****        HTWK Leipzig                                    *****
 *****        university of applied sciences                  *****
 *****        Germany                                         *****
 *****        adannenb@et.htwk-leipzig.de                     *****
 *****  Func: implements a dynamic HTTP-server by using       *****
 *****        the easyWEB-API                                 *****
 *****  Rem.: In IAR-C, use  linker option                    *****
 *****        "-e_medium_write=_formatted_write"              *****
 *****                                                        *****
 ******************************************************************/

// Modifications by Code Red Technologies for NXP LPC1768

// CodeRed - removed header for MSP430 microcontroller
//#include "msp430x14x.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// CodeRed - added #define extern on next line (else variables
// not defined). This has been done due to include the .h files 
// rather than the .c files as in the original version of easyweb.
#define extern 

#include "easyweb.h"

// CodeRed - removed header for original ethernet controller
//#include "cs8900.c"                              // ethernet packet driver

//CodeRed - added for LPC ethernet controller
#include "ethmac.h"

// CodeRed - include .h rather than .c file
// #include "tcpip.c"                               // easyWEB TCP/IP stack
#include "tcpip.h"                               // easyWEB TCP/IP stack

// CodeRed - added NXP LPC register definitions header
#include "LPC17xx.h"


// CodeRed - include renamed .h rather than .c file
// #include "webside.c"                             // webside for our HTTP server (HTML)
#include "webside.h"                             // webside for our HTTP server (HTML)

#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_ssp.h"
#include "oled.h"

// custom lib include
#include "custom_lib.h"

// CodeRed - added for use in dynamic side of web page
unsigned int aaPagecounter=0;
unsigned int adcValue = 0;

#define switch_oled_value(x) ({x = lr; aux = 1;})

// Custom functions

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

int main (void) {

	light.init();

	print_disp(1, light.minL, light.maxL, axis, 0);

	HTTPStatus = 0;                         // clear HTTP-server's flag register

	uint32_t lr   = 0;

	uint8_t  btn1 = 0,
			 op   = 1,
			 aux  = 0,
			 update_acc = 0;


	while (1){

		// delay to update display with new accelerometer values
		(update_acc <= 101) ? (update_acc++) : (update_acc = 0);

		acc_read(&axis.x, &axis.y, &axis.z);

		lr = light_read();
		light.current = lr;

		(lr < light.minL) ? (switch_oled_value(light.minL)) : ((lr > light.maxL) ? (switch_oled_value(light.maxL)) : (aux = 0));

		// change RGB color according to light sensor value
		// (lr < 300) ? rgb_setLeds(RGB_GREEN) : ((lr < 500) ? rgb_setLeds(RGB_BLUE): rgb_setLeds(RGB_RED));

		// change RGB according to joystick
		switch (joystick_read()) {
		case JOYSTICK_LEFT: rgb_setLeds(RGB_RED); break;
		case JOYSTICK_RIGHT: rgb_setLeds(RGB_GREEN); break;
		}

		btn1 = ((GPIO_ReadValue(0) >> 4) & 0x01);

		// check if button was pressed
		if (!btn1) {
			op = !op;
			print_disp(op, light.minL, light.maxL, axis, update_acc);
		}

		if (!op && (aux || update_acc == 100)) {
			print_disp(op, light.minL, light.maxL, axis, update_acc);
		}

		if (!(SocketStatus & SOCK_ACTIVE))
			TCPPassiveOpen();   // listen for incoming TCP-connection
		DoNetworkStuff();   // handle network and easyWEB-stack
							// events
		HTTPServer();
	}
}

// searches the TX-buffer for special strings and replaces them
// with dynamic values (AD-converter results)
void InsertDynamicValues(void) {
  unsigned char *Key;
           char NewKey[6];
  unsigned int i;
  
  if (TCPTxDataCount < 4) return;                     // there can't be any special string
  
  Key = TCP_TX_BUF;
  
  for (i = 0; i < (TCPTxDataCount - 3); i++)
  {
    if (*Key == 'L')
     if (*(Key + 1) == 'S')
       if (*(Key + 3) == '%')
         switch (*(Key + 2))
         {
           case '1' :                                 // "AD8%"?
           {
             sprintf(NewKey, "%04d", light.current);
             memcpy(Key, NewKey, 4);
             break;
           }
           case '2' :                                 // "AD7%"?
		  {
			sprintf(NewKey, "%04d", light.maxL);
			memcpy(Key, NewKey, 4);
			break;
		  }case '3' :                                 // "AD1%"?
		  {
			sprintf(NewKey, "%04d", light.minL);
			memcpy(Key, NewKey, 4);
			break;
		  }
         }
    Key++;
  }
}

// This function implements a very simple dynamic HTTP-server.
// It waits until connected, then sends a HTTP-header and the
// HTML-code stored in memory. Before sending, it replaces
// some special strings with dynamic values.
// NOTE: For strings crossing page boundaries, replacing will
// not work. In this case, simply add some extra lines
// (e.g. CR and LFs) to the HTML-code.
void HTTPServer(void)
{
  if (SocketStatus & SOCK_CONNECTED)             // check if somebody has connected to our TCP
  {
    if (SocketStatus & SOCK_DATA_AVAILABLE)      // check if remote TCP sent data
      TCPReleaseRxBuffer();                      // and throw it away

    if (SocketStatus & SOCK_TX_BUF_RELEASED)     // check if buffer is free for TX
    {
      if (!(HTTPStatus & HTTP_SEND_PAGE))        // init byte-counter and pointer to webside
      {                                          // if called the 1st time
        HTTPBytesToSend = sizeof(WebSide) - 1;   // get HTML length, ignore trailing zero
        PWebSide = (unsigned char *)WebSide;     // pointer to HTML-code
      }

      if (HTTPBytesToSend > MAX_TCP_TX_DATA_SIZE)     // transmit a segment of MAX_SIZE
      {
        if (!(HTTPStatus & HTTP_SEND_PAGE))           // 1st time, include HTTP-header
        {
          memcpy(TCP_TX_BUF, GetResponse, sizeof(GetResponse) - 1);
          memcpy(TCP_TX_BUF + sizeof(GetResponse) - 1, PWebSide, MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1);
          HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
          PWebSide += MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
        }
        else
        {
          memcpy(TCP_TX_BUF, PWebSide, MAX_TCP_TX_DATA_SIZE);
          HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE;
          PWebSide += MAX_TCP_TX_DATA_SIZE;
        }

        TCPTxDataCount = MAX_TCP_TX_DATA_SIZE;   // bytes to xfer
        InsertDynamicValues();                   // exchange some strings...
        TCPTransmitTxBuffer();                   // xfer buffer
      }
      else if (HTTPBytesToSend)                  // transmit leftover bytes
      {
        memcpy(TCP_TX_BUF, PWebSide, HTTPBytesToSend);
        TCPTxDataCount = HTTPBytesToSend;        // bytes to xfer
        InsertDynamicValues();                   // exchange some strings...
        TCPTransmitTxBuffer();                   // send last segment
        TCPClose();                              // and close connection
        HTTPBytesToSend = 0;                     // all data sent
      }

      HTTPStatus |= HTTP_SEND_PAGE;              // ok, 1st loop executed
    }
  }
  else
    HTTPStatus &= ~HTTP_SEND_PAGE;               // reset help-flag if not connected
}
