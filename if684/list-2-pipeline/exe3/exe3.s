#include <reg.h>
 .set noreorder
 .text
 .globl start
 .ent start
start: add t2, t0, t1
 add t4, t2, t3
 nop
 nop
 nop
 .end start