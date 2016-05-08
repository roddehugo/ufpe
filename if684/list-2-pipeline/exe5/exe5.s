 #include <reg.h>
 .set noreorder
 .text
 .globl start
 .ent start
start: add t0, t1, t2
 lw t1, 0(t2)
 sub t2, t1, t3
 nop
 nop
 nop
 .end start
