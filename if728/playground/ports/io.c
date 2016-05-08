#include <REG51F.H>

/*//
sfr P1Hi = P1&0xf0;
sfr P1Lo = P1&0x0f;

sfr P0Hi = P0&0xf0;
sfr P0Lo = P0&0x0f;
//*/

sbit P2Hi = P2^1;
sbit P2Lo = P2^0;

void main(){	
	while(1){
		if(P2Hi==1){
			P1 = (P1&0x0f) | (P0&0xf0);
		}else{
			P1 = (P1&0x0f);
		}
		if(P2Lo==1){
			P1 = (P1&0xf0) | (P0&0x0f);
		}else{
			P1 = (P1&0xf0);
		}
	}
}