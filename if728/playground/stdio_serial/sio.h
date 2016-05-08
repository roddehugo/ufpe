#ifndef __SIO_H__
#define __SIO_H__

#ifndef XTAL
	#define XTAL 12000000
#endif

void com_initialize(unsigned baudrate);

void com_baudrate(unsigned baudrate);

char com_putchar(unsigned char c);

char com_getchar();

unsigned char com_rbuflen();

unsigned char com_tbuflen();

#endif
