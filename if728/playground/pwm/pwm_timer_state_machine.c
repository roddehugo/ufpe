#include <REG517A.H>

#define FREQ_CLOCK 12000000
#define FREQ_T0_HZ 100
#define TH0_VALUE ((65536 - (FREQ_CLOCK / (12 * FREQ_T0_HZ))) >>8)
#define TL0_VALUE ((65536 - (FREQ_CLOCK / (12 * FREQ_T0_HZ))) & 0xff)
#define CYCLE 10

static unsigned long counter = 0;
static unsigned int duty = 3;

void timer0_int (void) interrupt 1 using 2 {
  TR0 = 0;
  TH0 += TH0_VALUE;
  TL0 += TL0_VALUE;
  TR0 = 1;

  counter++;
}

void setTimer(){
	TR0 = 0;
	TMOD = (TMOD & 0xF0) | 0x01;  /* Set Mode (16-bit timer) */
	TH0 = TH0_VALUE;              /* TH0 */
	TL0 = TL0_VALUE;							/* TLO */
	ET0 = 1;                      /* Enable Timer 0 Interrupts */
	TR0 = 1;                      /* Start Timer 0 Running */
}

void pwm(){
	static unsigned char P4_LED;
	static unsigned char state = 0;
	
	switch(state){
		case 0:
			if(counter >= duty){
				P4_LED = 0x00;
				state = 1;
			}
			break;
		case 1:
			if(counter >= CYCLE){
				P4_LED = 0x01;
				state = 0;
				counter = 0;
			}
			break;
	}
	P4 = P4_LED;
}

void main(){
	EAL = 0;
	setTimer();
	EAL = 1;
	while(1){
		pwm();
	}
}
