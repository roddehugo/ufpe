.data
  A: .word 5, 6, 2, 3, 1
.text
.globl main
.ent main
main:
      la $a0, A
      addi $a1, $zero, 0x5
      xor $t0, $t0, $t0
CW1:  beq $t0, $a1, EW1
      addi $t1, $t0,1
CW2:  beq $t1, $a1, EW2
      add $t2, $t0, $t0
      add $t2, $t2, $t2
      add $t2, $a0, $t2
      add $t3, $t1, $t1
      add $t3, $t3, $t3
      add $t3, $a0, $t3
      lw $t4, 0($t2)
      nop
      lw $t5, 0($t3)
      nop
      slt $t6, $t4, $t5
      bne $t6, $zero, IGN
      beq $t4, $t5, IGN
      sw $t5, 0($t2)
      sw $t4, 0($t3)
IGN:  addi $t1, $t1, 1
      beq $0, $0, CW2
EW2:  addi $t0, $t0, 1
      beq $0, $0, CW1
EW1:  beq $0, $0, FIM
FIM:  nop
      nop
.end main
