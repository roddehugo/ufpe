#include <REG517A.H>

#define FrClk 12000000
#define FrTimer0_Hz 100 /* 10ms */
#define VAL_TH0 ((65536 - (FrClk / (12 * FrTimer0_Hz))) >>8)
#define VAL_TL0 ((65536 - (FrClk / (12 * FrTimer0_Hz))) & 0xFF)

typedef unsigned char u_char;

/*
P1[0] = Output keypress signal
P1[1,3] = C[1,3] Input columns
P1[4,7] = L[1,4] Outut rows
P2[0,7] = Output character ASCII
*/

sbit KP = P1^0;
sbit C1 = P1^1;
sbit C2 = P1^2;
sbit C3 = P1^3;
sbit R1 = P1^4;
sbit R2 = P1^5;
sbit R3 = P1^6;
sbit R4 = P1^7;

static u_char counter;

void start_timer0() {
    TR0 = 0;
    TMOD = (TMOD & 0xF0) | 0x01;
    TH0 = VAL_TH0;
    TL0 = VAL_TL0;
    ET0 = 1;
    TR0 = 1;
    counter = 0;
}

void int_timer0 (void) interrupt 1 using 2 {
    counter++;
}

void wait(u_char delay_10ms) {
    start_timer0();
    while(counter < delay_10ms);
    TR0 = 0;
}

char read_keypad() {
    C1=1;
    C2=1;
    C3=1;
    R1=0;
    R2=1;
    R3=1;
    R4=1;
    if(C1==0){wait(4); return '1';}
    if(C2==0){wait(4); return '2';}
    if(C3==0){wait(4); return '3';}
    R1=1;
    R2=0;
    R3=1;
    R4=1;
    if(C1==0){wait(4); return '4';}
    if(C2==0){wait(4); return '5';}
    if(C3==0){wait(4); return '6';}
    R1=1;
    R2=1;
    R3=0;
    R4=1;
    if(C1==0){wait(4); return '7';}
    if(C2==0){wait(4); return '8';}
    if(C3==0){wait(4); return '9';}
    R1=1;
    R2=1;
    R3=1;
    R4=0;
    if(C1==0){wait(4); return '*';}
    if(C2==0){wait(4); return '0';}
    if(C3==0){wait(4); return '#';}
    return 0;
}

void main() {
    char c, p;
      EAL = 1;
    while(1) {
        P2 = 0;
        KP = 0;
        while(!(c = read_keypad()));
        p=c;
        while(p==c) {
            wait(10);
            P2 = c;
            KP = 1;
            while(!(c = read_keypad()));
        }
    }
}