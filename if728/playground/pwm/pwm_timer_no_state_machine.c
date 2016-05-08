#include <REG517A.H>

#define _BV(x) (1 << x)

unsigned char pwm_width;
bit pwm_flag = 0;

/*
Setup PWM using timer 0
*/
void pwm_setup() {
    EAL = 0;
    TR0 = 0;
    TMOD = 0;
    pwm_width = 0x80;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
    EAL = 1;
}

/*
Timer 0 Interrupt service routine
*/
void timer0() interrupt 1 {
    if (!pwm_flag) {            /* Start of High level */
        pwm_flag = 1;           /* Set flag */
        P4 |= _BV(0);           /* Set PWM o/p pin */
        TH0 = pwm_width;        /* Load timer */
        TF0 = 0;                /* Clear interrupt flag */
    } else {                    /* Start of Low level */
        pwm_flag = 0;           /* Clear flag */
        P4 &= ~(_BV(0));        /* Clear PWM o/p pin */
        TH0 = 0xFF - pwm_width; /* Load timer */
        TF0 = 0;                /* Clear Interrupt flag */
    }
}

void main() {
    pwm_setup();
    while(1);
}