## Lista 1 - Exercicio 5

## Faca um programa em linguagem de montagem Mips que receba como entrada
## dois numeros, n e s, e que tenha como saida o resultado da combinacao
## de n tomados s a s. Os numeros n e s devem ser carregados da memoria e
## o resultado da combinacao deve ser colocado na mesma em uma variavel.
## Caso s seja maior que n, deve ser armazenado o valor 1 no registrador v1.
## Caso n e/ou s seja menor que zero, o valor 2 deve ser armazenado no registrador v1.
## Quando n for igual a s o valor 3 deve ser armazenado no registrador V1.
## Quando n e/ou s for igual a zero o valor 4 deve ser armazenado no registrador v1.
## Segue abaixo a formula da combinacao

## We'll implement a factorial recursive function
## Function factorial (int n)
##     if n < 2:
##         return 1
##     else:
##         return (n * factorial(n-1))


.data
    n:      .word 4
    s:      .word 2
    res:    .word 0
.text
.globl main
main:
    lw      $s0, n              # load n into $s0
    lw      $s1, s              # load s into $s1
    bgt     $s1, $s0, sGTn      # if s > n
    bltz    $s0, LTzero         # if n < 0
    bltz    $s1, LTzero         # if s < 0
    beq     $s0, $s1, nEQs      # if n = s
    beqz    $s0, nsEQzero       # if n = 0
    beqz    $s1, nsEQzero       # if s = 0
    move    $a0, $s0            # compute n!
    jal     factorial
    move    $t0, $v0            # save restult in $t0
    move    $a0, $s1            # compute s!
    jal     factorial
    move    $t1, $v0            # save restult in $t1
    subu    $t2, $s0, $s1       # compute n - s
    move    $a0, $t2            # compute (n - s)!
    jal     factorial
    move    $t2, $v0            # save restult in $t2
    mul     $t1, $t1, $t2       # compute s! * (n - s)!
    div     $t0, $t0, $t1       # compute combination of s among n
    sw      $t0, res            # store result in memory
    j       exit                # exit gracefully
sGTn:
    li      $v1, 1              # store 1 in $v1
    j       exit
LTzero:
    li      $v1, 2              # store 2 in $v1
    j       exit
nEQs:
    li      $v1, 3              # store 3 in $v1
    j       exit
nsEQzero:
    li      $v1, 4              # store 4 in $v1
    j       exit
exit:
    li      $v0, 10
    syscall

##############################################################
# int function factorial (int)
# argumens:
#       $a0: argument int
# returns:
#       $v0: int value output
# uses register s0
##############################################################
factorial:
    bgtz    $a0, doit           # recursive case
    li      $v0, 1              # base case, 0 != 1
    jr      $ra                 # return address
doit:
    sub     $sp, 8              # stack frame
    sw      $s0, 0($sp)         # will use for argument n
    sw      $ra, 4($sp)         # return address
    move    $s0, $a0            # save argument
    sub     $a0, 1              # n-1
    jal     factorial           # v0 = (n - 1)!
    mul     $v0, $s0, $v0       # v0 = n * (n - 1)!
    lw      $s0, 0($sp)         # restore registers from stack
    lw      $ra, 4($sp)         # restore return address
    add     $sp, 8              # stack frame
    jr      $ra