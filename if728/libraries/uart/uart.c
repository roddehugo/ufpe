/***********************************************************
*		File Name:        uart.c		         		   *
*		Author:	        Ajay Bhargav		         	   *
***********************************************************/

#include "uart.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	UART Initialization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void uart_init()
{
	TMOD = 0x20;	// timer 1 8-bit auto-reload
	SCON = 0x50;	// Tx and Rx enable
	TL1 = 0xFD;		// Baud Rate 9600 8-n-1
	TH1 = 0xFD;		// Reload count
	TR1 = 1;		// Start timer
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Trasmitting 8 bit data
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void uart_send(bit_8 value)
{
	SBUF = value;
	while(!TI);
	TI = 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Receiving 8-bit data
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bit_8 uart_receive()
{
	while(!RI);
	RI = 0;
	return(SBUF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Xmitting String
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void uart_string(bit_8 *str)
{
	bit_8 i=0;
	while(str[i])
		uart_send(str[i++]);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Echoing received data
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void uart_echo()
{
	uart_send(uart_receive());
}