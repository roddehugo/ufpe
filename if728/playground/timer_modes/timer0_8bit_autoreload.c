#include <REG51F.H>
/*//
sfr P1Hi = P1&0xf0;
sfr P1Lo = P1&0x0f;

sfr P0Hi = P0&0xf0;
sfr P0Lo = P0&0x0f;
//*/

sbit P2Hi = P2^1;		// CH
sbit P2Lo = P2^0;		// CL

static unsigned long overflow_count = 0;

void timer1_ISR (void) interrupt 3{
	overflow_count++;   //Increment the overflow count
}

/*//
Set Timer1 for 8-bit timer with reload (mode 2). The timer counts to 255,
overflows, is reloaded with 156, and generates an interrupt.
Each interrupt will occur every 100 clocks.
Since the oscillator runs at 12 MHz, the interrupt will happen every 10 KHz.
//*/
void setTimer(){
	TMOD = (TMOD & 0x0F) | 0x20;  /* Set Mode (8-bit timer with reload) */
	TH1 = 256 - 100;              /* Reload TL1 to count 100 clocks */
	TL1 = TH1;
	ET1 = 1;                      /* Enable Timer 1 Interrupts */
	TR1 = 1;                      /* Start Timer 1 Running */
	EA = 1;                       /* Global Interrupt Enable */
}

void copyHigh(){
	static char state = 2;
	static unsigned long count;
	switch(state){
		case 0:
			if(P2Hi == 1){
				count = overflow_count;
				P1 = (P1&0x0f) | (P0&0xf0);
				state = 1;
			}
			break;
		case 1:
			if(overflow_count >= count + 10000){
				P1 = (P1&0x0f);
				state = 2;
			}
			break;
		case 2:
			if(P2Hi == 0){
				state = 0;
			}
	}
}

void copyLow(){
	static char state = 2;
	static unsigned long count;
	switch(state){
		case 0:
			if(P2Lo == 1){
				count = overflow_count;
				P1 = (P1&0xf0) | (P0&0x0f);
				state = 1;
			}
			break;
		case 1:
			if(overflow_count >= count + 10000){
				P1 = (P1&0xf0);
				state = 2;
			}
			break;
		case 2:
			if(P2Lo == 0){
				state = 0;
			}
	}
}

void main(){
	setTimer();
	while(1){
		copyHigh();
		copyLow();
	}
}
