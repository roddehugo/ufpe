## Lista 1 - Exercicio 2
## Codifique um programa correspondente a
## int a = ...;
## int b = ...;
## x = 0;
## if ( a >= 0 && b <= 60 )
##     x = 1;
.data
    aa: .word 8
    bb: .word 47
    xx: .byte 0
.text
.globl main
main:
    lw      $t0, aa             # load aa data in $t0 from memory
    lw      $t1, bb             # load bb data in $t1 from memory
    lb      $t2, xx             # load xx data in $t2 from memory (assumed it is a boolean so a byte is enought to store it)
    bltz    $t0, exit           # if aa < 0 then we skip the if instruction by jumping to exit
    bgt     $t1, 60, exit       # same if bb > 60
    addiu   $t2, $zero, 1       # if aa >= 0 and bb <= 60, then we set $t2 to 1
exit:
    sb      $t2, xx             # we save the byte $t2 into xx into memory
    li      $v0, 10             # exit gracefully
    syscall