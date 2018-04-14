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
#include "biblioteca.h"

// CodeRed - removed header for original ethernet controller
//#include "cs8900.c"                              // ethernet packet driver

//CodeRed - added for LPC ethernet controller
#include "ethmac.h"

// CodeRed - include .h rather than .c file
// #include "tcpip.c"                               // easyWEB TCP/IP stack
#include "tcpip.h" // easyWEB TCP/IP stack

// CodeRed - added NXP LPC register definitions header
#include "LPC17xx.h"

// CodeRed - include renamed .h rather than .c file
// #include "webside.c"                             // webside for our HTTP server (HTML)
#include "webside.h" // webside for our HTTP server (HTML)

#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_ssp.h"
#include "oled.h"

// CodeRed - added for use in dynamic side of web page
unsigned int aaPagecounter = 0;
unsigned int adcValue = 0;

#define switchMaxMin(x) ({x = lr; aux = 1; })

int main(void)
{

  struct config light = {
	  .init = init_all,
	  .minL = 1000,
	  .maxL = 0,
  };

  light.init();

  print_disp(1, light.minL, light.maxL);

  HTTPStatus = 0; // clear HTTP-server's flag register

  uint32_t lr = 0,
           read = 0;

  uint8_t data = 0,
          btn1 = 0,
          op = 1,
          aux = 0,
          ctrl = 0;

  while (1)
  {

    lr = light_read();

    (lr < light.minL) ? (switchMaxMin(light.minL)) : ((lr > light.maxL) ? (switchMaxMin(light.maxL)) : (aux = 0));

    btn1 = ((GPIO_ReadValue(0) >> 4) & 0x01);

    if (!btn1)
    {
      op = !op;
      print_disp(op, light.minL, light.maxL);
    }

    if (!op && aux)
    {
      print_disp(op, light.minL, light.maxL);
    }

    read = UART_Receive(UART_DEV, &data, 1, NONE_BLOCKING);
    if ((read > 0))
    {
      if (data == '1' && !ctrl)
        ctrl = 1, print_uart(1);
      if (data == '2')
        break;
    }

    if (!(SocketStatus & SOCK_ACTIVE))
      TCPPassiveOpen(); // listen for incoming TCP-connection
    DoNetworkStuff();   // handle network and easyWEB-stack
                        // events
    HTTPServer();
  }

  light.exit();
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
  if (SocketStatus & SOCK_CONNECTED) // check if somebody has connected to our TCP
  {
    if (SocketStatus & SOCK_DATA_AVAILABLE) // check if remote TCP sent data
      TCPReleaseRxBuffer();                 // and throw it away

    if (SocketStatus & SOCK_TX_BUF_RELEASED) // check if buffer is free for TX
    {
      if (!(HTTPStatus & HTTP_SEND_PAGE))      // init byte-counter and pointer to webside
      {                                        // if called the 1st time
        HTTPBytesToSend = sizeof(WebSide) - 1; // get HTML length, ignore trailing zero
        PWebSide = (unsigned char *)WebSide;   // pointer to HTML-code
      }

      if (HTTPBytesToSend > MAX_TCP_TX_DATA_SIZE) // transmit a segment of MAX_SIZE
      {
        if (!(HTTPStatus & HTTP_SEND_PAGE)) // 1st time, include HTTP-header
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

        TCPTxDataCount = MAX_TCP_TX_DATA_SIZE; // bytes to xfer
        InsertDynamicValues();                 // exchange some strings...
        TCPTransmitTxBuffer();                 // xfer buffer
      }
      else if (HTTPBytesToSend) // transmit leftover bytes
      {
        memcpy(TCP_TX_BUF, PWebSide, HTTPBytesToSend);
        TCPTxDataCount = HTTPBytesToSend; // bytes to xfer
        InsertDynamicValues();            // exchange some strings...
        TCPTransmitTxBuffer();            // send last segment
        TCPClose();                       // and close connection
        HTTPBytesToSend = 0;              // all data sent
      }

      HTTPStatus |= HTTP_SEND_PAGE; // ok, 1st loop executed
    }
  }
  else
    HTTPStatus &= ~HTTP_SEND_PAGE; // reset help-flag if not connected
}

// Code Red - new version of InsertDynamicValues()
void InsertDynamicValues(void)
{
  unsigned char *Key;
  char NewKey[6];
  unsigned int i;

  if (TCPTxDataCount < 4)
    return; // there can't be any special string

  Key = TCP_TX_BUF;

  for (i = 0; i < (TCPTxDataCount - 3); i++)
  {
    if (*Key == 'A')
      if (*(Key + 1) == 'D')
        if (*(Key + 3) == '%')
          switch (*(Key + 2))
          {
          case '8': // "AD8%"?
          {
            sprintf(NewKey, "%04d", light_read()); // insert pseudo-ADconverter value
            memcpy(Key, NewKey, 4);
            break;
          }
          case '7': // "AD7%"?
          {
            sprintf(NewKey, "%3u", adcValue); // copy saved value from previous read
            memcpy(Key, NewKey, 3);
            break;
          }
          case '1': // "AD1%"?
          {
            sprintf(NewKey, "%4u", ++aaPagecounter); // increment and insert page counter
            memcpy(Key, NewKey, 4);
            //			 *(Key + 3) = ' ';
            break;
          }
          }
    Key++;
  }
}

// Code Red - commented out original InsertDynamicValues()
/*
void InsertDynamicValues(void)
{
  unsigned char *Key;
  unsigned char NewKey[5];
  unsigned int i;
  
  if (TCPTxDataCount < 4) return;                     // there can't be any special string
  
  Key = TCP_TX_BUF;
  
  for (i = 0; i < (TCPTxDataCount - 3); i++)
  {
    if (*Key == 'A')
     if (*(Key + 1) == 'D')
       if (*(Key + 3) == '%')
         switch (*(Key + 2))
         {
           case '7' :                                 // "AD7%"?
           {
             sprintf(NewKey, "%3u", GetAD7Val());     // insert AD converter value
             memcpy(Key, NewKey, 3);                  // channel 7 (P6.7)
             break;
           }
           case 'A' :                                 // "ADA%"?
           {
             sprintf(NewKey, "%3u", GetTempVal());    // insert AD converter value
             memcpy(Key, NewKey, 3);                  // channel 10 (temp.-diode)
             break;
           }
         }
    Key++;
  }
}

// Code Red - End of original InsertDynamicValues ()
*/
