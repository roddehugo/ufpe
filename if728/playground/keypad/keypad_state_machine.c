#include <REG517A.H>

#define _BV(x) (1 << x)

#define FrClk 12000000
#define FrTimer0_Hz 100 /* 10ms */
#define VAL_TH0 ((65536 - (FrClk / (12 * FrTimer0_Hz))) >>8)
#define VAL_TL0 ((65536 - (FrClk / (12 * FrTimer0_Hz))) & 0xFF)

/*
P4[0,3] = C[1,4] Input columns
P4[4,7] = L[1,4] Outut rows
P1[0,7] = Output character ASCII
P2[0] = Output character signal
*/

static const char keypad[4][4] = {
	{'1', '2', '3', '\0'},
	{'4', '5', '6', '\0'},
	{'7', '8', '9', '\0'},
	{'*', '0', '#', '\0'}};

static unsigned char counter = 0;

void start_timer0() {
	TR0 = 0;
	TMOD = (TMOD & 0xF0) | 0x01;
	TH0 = VAL_TH0;
	TL0 = VAL_TL0;
	ET0 = 1;
	TR0 = 1;
}

void int_timer0 (void) interrupt 1 using 2 {
	TR0 = 0;
	TH0 = VAL_TH0;
	TL0 = VAL_TL0;
	TR0 = 1;
	/*
	Counter from 1 to 5 because of the fact that whatever
	x is, 0 % x will always be true. Counter is used in the
	state machine.
	L1 -> counter = 1
	...
	L4 -> counter = 4
	As the interrupt occurs each 10ms, each row as a debounce
	time of 40ms. Time needed for the counter to reach the according
	row's indice.
	*/
	if (++counter>=5)
		counter = 1;
}

void set_output(char keycode) {
	if (!keycode || keycode == '\0') {
		P1 = 0x00;
		P2 &= ~(_BV(1));
	}else {
		P1 = keycode;
		P2 |= _BV(1);
	}
}

void sweep_and_set(unsigned char pos) {
	P4 = 0xF0;
	P4 &= ~(_BV(pos+4));
}

unsigned char get_column(unsigned char cleared) {
	unsigned char i, pos;
	for (i=1, pos=0; i<16; i<<=1, pos++)
		if (cleared) {
			if (~P4 & i) { /* Test if C[pos] is cleared */
				return pos;
			}
		}else {
			if (P4 & i) { /* Test if C[pos] is setted */
				return pos;
			}
		}
	return 10;
}

void state_machine(unsigned char row) {
	static unsigned char row_states[] = {0, 0, 0, 0};
	static unsigned char col_states[] = {1, 1 ,1 ,1};
	static char keycode;
	switch (row_states[row]) {
		case 0: {
			unsigned char col = get_column(1);
			if (col!=10 && col<4) {
				col_states[col] = 0;
				row_states[row] = 1;
			}
			break;
		}
		case 1:{
			unsigned char col = get_column(1);
			if (counter%(row+1)==0 && col_states[col]==0) {
				row_states[row] = 2;
				keycode = keypad[row][col];
			}
			break;
		}
		case 2:{
			unsigned char col = get_column(0);
			if (col!=10 && col<4) {
				col_states[col] = 1;
				row_states[row] = 3;
				set_output(keycode);
			}
			break;
		}
		case 3: {
			unsigned char col = get_column(0);
			if (counter%(row+1)==0 && col_states[col]==1) {
				row_states[row] = 0;
				set_output(0);
			}
			break;
		}
		default:
			break;
	}
}

void main() {
	EAL=1;
	start_timer0();
	set_output(0);
	while (1) {
		unsigned char pos;
		for (pos=0; pos<4; pos++) {
			sweep_and_set(pos);
			state_machine(pos);
		}
	}
}