## Lista 1 - Exercicio 1
## Codifique um programa correspondente ao pseudo-código abaixo para Assembly MIPS.
## int a = 3;
## int b = 4;
## int m = 8;
## m = a;
## if ( b < m )
##     m = b;

.data
    aa: .word 2
    bb: .word 10
    mm: .word 0
.text
.globl main
main:
    lw      $t0, aa             # load aa data in $t0 from memory
    lw      $t1, bb             # load bb data in $t1 from memory
    move    $t2, $t0            # set m = a, m is stored in $t2
    bge     $t1, $t2, exit      # if b < m, then continue, else jump to exit
    move    $t2, $t1            # set m = b
exit:
    sw      $t2, mm             # store mm data in memory
    li      $v0, 10
    syscall