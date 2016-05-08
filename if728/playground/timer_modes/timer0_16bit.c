#include <REG51F.H>

#define FrClk 12000000
#define FreqTimer0_Hz 100
#define VALOR_TH0 ((65536 - (FrClk / (12 * FreqTimer0_Hz))) >>8)
#define VALOR_TL0 ((65536 - (FrClk / (12 * FreqTimer0_Hz))) & 0xFF)

sbit CH = P2^1;
sbit CL = P2^0;

static unsigned char counterA;
static unsigned char counterB;

void timer0_init() {
  TR0 = 0;                      // turn off timer 0
  TMOD = (TMOD & 0xF0) | 0x01;  // timer 0 as 16 bits
  TH0 = VALOR_TH0;              // program counter timer 0
  TL0 = VALOR_TL0;              //
  ET0 = 1;                      // timer interruption
  TR0 = 1;                      // timer counter
}

void timer0_int (void) interrupt 1 using 2 {
  TR0 = 0;                      // turn off timer 0
  TH0 += VALOR_TH0;             // program counter timer 0
  TL0  += VALOR_TL0;
  TR0 = 1;                      // enable counter timer 0

  counterA++;
  counterB++;
}

void functA() {
    static char state = 0;
	switch (state) {
	    case 0: {
	         if (!CH) {
				state = 1;
		     }
			 break;
		}
		case 1: {
			 if (CH) {
		       P1 = (P1 & 0x0F) | (P0 & 0xF0);
			   counterA = 0;
			   state = 2;
			 }
			 break;
		}
		case 2: {
			if (counterA==100) {
				P1 = (P1 & 0x0F);
				state = 0;
			}
			break;
		}
	   default: break;
	}
}

void functB() {
    static char state = 0;
	switch (state) {
	    case 0: {
	         if (!CL) {
				state = 1;
		     }
			 break;
		}
		case 1: {
			 if (CL) {
		       P1 = (P1 & 0xF0) | (P0 & 0x0F);
			   counterB = 0;
			   state = 2;
			 }
			 break;
		}
		case 2: {
			if (counterB==100) {
				P1 = (P1 & 0xF0);
				state = 0;
			}
			break;
		}
	   default: break;
	}

}

int main() {
	EA=1;
	timer0_init();

	counterA=0;
	counterB=0;

	while(1) {
		functA();
		functB();
	}
	return 0;
}