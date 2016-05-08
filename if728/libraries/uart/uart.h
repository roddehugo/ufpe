/***********************************************************
*		File Name:        uart.h		        		   *
*		Author:	        Ajay Bhargav		         	   *
***********************************************************/
#ifndef __UART_H__
#define __UART_H__

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Type Definations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef unsigned char bit_8;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Required Registers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL1 = 0x8B;
sfr TH1 = 0x8D;
sfr SCON = 0x98;
sfr SBUF = 0x99;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	TCON Bit Registers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
sbit IT0  = 0x88;
sbit IE0  = 0x89;
sbit IT1  = 0x8A;
sbit IE1  = 0x8B;
sbit TR0  = 0x8C;
sbit TF0  = 0x8D;
sbit TR1  = 0x8E;
sbit TF1  = 0x8F;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	SCON Bit Registers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

sbit RI   = 0x98;
sbit TI   = 0x99;
sbit RB8  = 0x9A;
sbit TB8  = 0x9B;
sbit REN  = 0x9C;
sbit SM2  = 0x9D;
sbit SM1  = 0x9E;
sbit SM0  = 0x9F;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Function Prototypes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void uart_init(void);
void uart_send(bit_8);
bit_8 uart_receive(void);
void uart_string(bit_8*);
void uart_echo(void);

#endif