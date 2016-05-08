#include <REG517A.H>
#include <string.h>
#include "sio.h"

#define TBUF_SIZE	256
#define RBUF_SIZE	256

static xdata unsigned char tbuf[TBUF_SIZE];
static xdata unsigned char rbuf[RBUF_SIZE];

static xdata unsigned char t_in = 0;
static xdata unsigned char t_out = 0;
static xdata unsigned char t_disabled = 0;

static xdata unsigned char r_in = 0;
static xdata unsigned char r_out = 0;

static void com_isr() interrupt 4 using 2 {
	/* Received data interrupt */
	if (RI0 != 0) {
		RI0 = 0;
		if ((r_in + 1) != r_out) {
			rbuf[r_in++] = S0BUF;
		}
	}

	/* Transmitted data interrupt*/
	if (TI0 != 0) {
		TI0 = 0;
		if (t_in != t_out) {
			S0BUF = tbuf[t_out++];
		}else {
			t_disabled = 1;
		}
	}

}

void com_initialize(unsigned baudrate) {
	/* Setup TIMER1 to generate the proper baud rate */
	com_baudrate(baudrate);

	/* Clear com buffer indexes */
	EAL = 0; 													/* disable interrupts */
	t_in = 0;
	t_out = 0;
	t_disabled = 1;
	r_in = 0;
	r_out = 0;

	/* Setup serial port registers */
	SM0 = 0;
	SM1 = 1;		  										/* serial port MODE 1 */
	SM20 = 0;
	REN0 = 1;													/* enable serial receiver */
	TI0 = 0;													/* clear transmit interrupt */
	RI0 = 0;													/* clear receiver interrupt */
	ES0 = 1;													/* enable serial interrupts */
	EAL = 1;       										/* enable interrupts */
}

void com_baudrate(unsigned baudrate) {
	EAL = 0;

	/* Clear transmit interrupt and buffer */
	TI0 = 0;				    							/* clear transmit interrupt */
	t_in = 0;			    								/* empty transmit buffer */
	t_out = 0;
	t_disabled = 1;			   						/* disable transmitter */

	/* Set timer 1 up as a baud rate generator */
	TR1 = 0;			    								/* stop timer 1 */
	ET1 = 0;			    								/* disable timer 1 interrupt */
	PCON |= 0x80;			    						/* 0x80=SMOD: set serial baudrate doubler */
	TMOD &= ~0xF0;			    					/* clear timer 1 mode bits */
	TMOD |= 0x20;			    						/* put timer 1 into MODE 2 */
	TH1 = (unsigned char)
				(256 - (XTAL / (16L * 12L * baudrate)));
	TR1 = 1;			    								/* start timer 1 */
	EAL = 1;                          /* enable interrupts */
}

char com_putchar(unsigned char c) {
	/* If the buffer is full, return an error value */
	if ((TBUF_SIZE - com_tbuflen()) <= 2) {
		return -1;
	}

	/* Add the data to the transmit buffer.  If the
	transmit interrupt is disabled, then enable it */
	EAL = 0;                         	/* disable interrupts */
	tbuf[t_in++] = c;

	if (t_disabled) {
		t_disabled = 0;
		TI0 = 1;												/* enable it */
	}

	EAL = 1;                         	/* enable interrupts */
	return 0;
}

char com_getchar() {
	char c;

	if (com_rbuflen() == 0) {
		return (-1);
	}

	EAL = 0;                         	/* disable interrupts */
	c = rbuf[r_out++];
	EAL = 1;                         	/* enable interrupts */

	return c;
}

unsigned char com_rbuflen() {
	return (r_in - r_out);
}

unsigned char com_tbuflen() {
	return (t_in - t_out);
}
