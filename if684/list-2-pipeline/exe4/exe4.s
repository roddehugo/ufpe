 #include <reg.h>
 .set noreorder
 .text
 .globl start
 .ent start
start: nop
 nop
 beq t0, t1, start
 addi t0, t0, 1
 nop
 nop
 nop
 .end start
