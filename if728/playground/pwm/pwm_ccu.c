#include <REG517A.H>

#define INTERVAL 10000
#define _BV(x) (1 << x)
#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

/*
## 8-bit resolution
This means that only eight bits of the 16-bit wide timer and compare circuitry are used to generate the PWM signal.
Thus the duty cycle of the signal is programmable in 256 steps.
Input clock is set to fOSC/2, the timer is incremented every machine cycle (166.6 ns at 12 MHz)
We want a period cycle of at least, 100 Hz
(1/(256 * 128 * 166.6) = 183,2 Hz)

## Input clock
The input clock is set to fOSC/2/128. This can be done in special function register CTCON.
Prescaler value is set by SFR bits CLK2(1) CLK1(1) CLK0(0) in register CTCON
Compare timer overflow flag is set by SFR bit CTF in register CTCON

## Reload register
The reload register CTRELH (high byte) is set to 0xFF, CTRELL (low byte) must contain 0x00.
Thus the timer counts from 0xFF00 to 0xFFFF (= 8-bit reload ? 256 steps).
Restriction of module range = (1/2n) * 100% = 0.195%
This leads to a variation of the duty cycle from 0.195% to 99.805% when 8 of 16 bits are used.

## Compare mode
Compare register CM0 (consisting of CMH0 and CML0) is used in compare mode 0.
This means bit CMSEL.0 must be set (in register CMSEL) to assign CM0 to the compare timer and switch on compare mode 0.

## Enable port output
For P4.0, the compare is enabled with SFR bit CMEN.0 in register CMEN.

## Enable Interrupt
The compare timer overflow interrupt is enabled by SFR bit ECT in register IEN2.
The general enable flag EAL in register IEN0 must be set, too.

## References
 * http://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM
 * https://electrosome.com/pwm-pic-microcontroller-hi-tech-c/
 * SAB 80C517A/83C517A-5 Manual
*/

unsigned char duty[] = {0x40, 0x80, 0xC0}; // {25%, 50%, 75%}
volatile unsigned long count = 0;

/*
Initialize Compare Timer & Interrupts
*/
void init() {
    EAL = 0;
    IEN2 &= ~(_BV(3));
    CMSEL |= _BV(0);
    CMEN |= _BV(0);
    CTCON |= (_BV(2) | _BV(1));
    IEN2 |= _BV(3);
    EAL = 1;
}

/*
Start Compare Timer by writing into it
*/
void start() {
    CTRELH = 0xFF;
    CTRELL = 0x00;
}

/*
Set duty cycle in Reload Register
*/
void set_duty() {
    CMH0 = 0xFF;
    CML0 = duty[count/INTERVAL];
}

/*
Compare Timer Interrupt service routine
*/
void compare_timer() interrupt 19 using 2 {
    if (++count >= NELEMS(duty) * INTERVAL) {
        count = 0;
    }
    if (count % INTERVAL == 0) {
        set_duty();
    }
    // Clear CTF bit in CTCON rregister
    CTCON &= ~(_BV(3));
}

void main() {
    init();
    set_duty();
    start();
    while(1);
}
