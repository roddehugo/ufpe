## Lista 1 - Exercicio 4
## Escreva um programa em linguagem de montagem do MIPS que receba dois
## numeros inteiros armazenados na memoria e realiza a multiplicacao dos
## dois numeros. Considere numeros positivos e negativos.
## A instrucao mult nao devera ser utilizada na implementacao dessa questao.
## O resultado devera ser armazenado em uma variavel na memoria.

## We'll use the russian algorithm with a little trick if x is negative
## Function mult (int x, int y)
##    r = 0
##    while x <> 0
##       If x is odd then r = r + y endif
##       x = x / 2 # We'll use the >> operator
##       y = y * 2 # We'll use the << operator
##    return r

.data
    x:      .word -1
    y:      .word 1
    res:    .word 0
.text
.globl main
main:
    lw      $s0, x              # load x into $s0
    lw      $s1, y              # load y into $s1
    li      $t0, 0              # initialize $t0 wich represent the result of the operation
    li      $s3, 0              # $s3 will store the positive/negative flag
    bgez    $s0, loop           # if x is positive, compute the multiplication
    li      $s3, 1              # else we set a negative flag
    neg     $s0, $s0            # negate the integer to manipulate a positive one
loop:
    beqz    $s0, check          # if x = 0 then we quit the loop
    and     $t1, $s0, 1         # isolate the last bit to check if number is odd or not
    beqz    $t1, even           # if $t1 = 0, x is even
    add     $t0, $t0, $s1       # else (x is odd), r = r + y
even:
    sra     $s0, $s0, 1         # shift right arithmetic as we manipulate signed words (x = x / 2)
    sll     $s1, $s1, 1         # shift left (y = y * 2)
    j       loop
check:
    beqz    $s3, exit           # if the p/n flag is 0, x was positive
    neg     $t0, $t0            # negate the integer to restore a negative one
exit:
    sw      $t0, res            # store the result value in memory
    li      $v0, 10             # exit gracefully
    syscall