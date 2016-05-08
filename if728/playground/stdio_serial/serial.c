#include <REG517A.H>
#include <stdio.h>
#include "sio.h"

/*
_getkey and putchar, replace the library functions of the same name.
These functions use the interrupt-driven serial I/O routines in sio.c
*/

char _getkey() {
	char k;

	do {
		k = com_getchar();
	} while(k == -1);

	return k;
}

char putchar(char c) {
	volatile unsigned int i;

	while (com_putchar(c) != 0) {
		for (i=0; i<1000; i++);
	}

	return c;
}

void main() {
	com_initialize(38400);				/* initialize interrupt driven serial I/O setup for 38400 baud */
	printf("Interrupt-driver Serial I/O\r\n\r\n");

	while(1) {
		unsigned char c;
		printf("Press a key.\r\n");
		c = getchar();
		printf("\r\n");
		printf("You pressed '%c'.\r\n\r\n", c);
	}
}


// transmission and reception circular buffers of 16 bytes each
// sendChar(char c) which transmit one byte through transmission circular buffer
// sendString(char *s) which uses sendChar to transmit a string
// rxEmptyBuffer() which returns 1 if reception circular buffer is empty
// receiveChar() which returns one byte from reception circular buffer
// receiveString(char *s) which uses receiveChar and copy a string from RxBuf to *s
