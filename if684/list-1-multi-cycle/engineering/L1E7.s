## Implemente (recursivamente) na linguagem de montagem do MIPS,
## uma funcao que receba dois numeros a e b ( b >= a) e retorne
## a soma de todos os numeros contidos no intervalo [a,b].
## Se a > b, o valor 1 devera ser armazenado no registrador v1 e
## o programa encerrado.

.data
    aa: .word 0
    bb: .word 10
.text
.globl main
main:
    lw      $t1, aa
    lw      $t2, bb
    bgt     $t1, $t2, error
    li      $t0, 0
    jal     loop
    j       exit
loop:
    bgt     $t1, $t2, return
    add     $t3, $t3, $t1
    addiu   $t1, $t1, 1
    j       loop
return:
    jr      $ra
error:
    li      $v1, 1
    j       exit
exit:
    li      $v0,10
    syscall