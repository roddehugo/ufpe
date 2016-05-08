#include <REG51F.H>
#define COUNT_1S 1000000
/*//
sfr P1Hi = P1&0xf0;
sfr P1Lo = P1&0x0f;

sfr P0Hi = P0&0xf0;
sfr P0Lo = P0&0x0f;
//*/

sbit P2Hi = P2^1;		// CH
sbit P2Lo = P2^0;		// CL

void copyHigh(){
	static char state = 2;
	static unsigned long count;
	switch(state){
		case 0: 
			if(P2Hi == 1){
				count = COUNT_1S;
				P1 = (P1&0x0f) | (P0&0xf0);
				state = 1;
			}
			break;
		case 1:
			if(--count == 0){
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
				count = COUNT_1S;
				P1 = (P1&0xf0) | (P0&0x0f);
				state = 1;
			}
			break;
		case 1:
			if(--count == 0){
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
	while(1){
		copyHigh();
		copyLow();
	}
}
