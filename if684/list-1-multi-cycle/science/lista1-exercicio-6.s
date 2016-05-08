## Lista 1 - Exercicio 6

## Escreva um programa em linguagem de montagem do Mips que converta um
## numero inteiro (representacao complemento a dois) na sua representacao
## em ASCII. Numeros positivos e negativos devem ser considerados.
## O numero binario tera 32 caracteres e sera lido da memoria.
## Os numeros estarao representados como string de caracteres ASCII e
## o fim de string sera denotado com o caractere null. O numero binario devera
## ser armazenado no registrador v0. Caso o numero a ser convertido nao seja valido,
## o registrador v1 retornara o valor 1.

## We'll implement a integer to string function with two sub recursives functions
## Function ItoA (int n, char *str)
##     if n = 0:
##         convert 0 in ASCII
##         store converted 0 in str (null terminated)
##         return length=1
##     else:
##         length = recursion(n, &str)
##         return length
##
## Function recursion (int n, char *str)
##     push old quotient in stack
##     push old remainder in stack
##     quotient = n/10
##     remainder = n%10
##     if quotient = 0:
##         return write(remainder, &str)
##     else:
##         n = quotient
##         recursion(n, &str)
##
## Function write (int n, char *str)
##     increment string length
##     convert n in ASCII
##     store converted n in str (null terminated)
##     pop old quotient from stack
##     pop old remainder from stack
##     if no more stack to pop:
##         return length
##     else:
##         return length = length + write(remainder, &str)

.data
    number:     .word -11543
    string:     .asciiz ""
.text
.globl main
main:
    lw      $a0, number         # load number in $a0
    la      $a1, string         # load string buffer pointer in $a1
    jal     ItoA                # call the recursive function
    move    $t0, $v0            # get string length
    la      $a0, string         # load string to be print
    li      $v0, 4              # print string
    syscall
    li      $v0, 10             # exit gracefully
    syscall

##############################################################
# int function ItoA (int, char*)
# arguments:
#       $a0: integer to convert
#       $a1: character buffer to write to
# return:
#       $v0: number of characters in converted string
# uses registers t0, t1, t2, s0, s1
##############################################################
ItoA:
    bnez    $a0, ItoA_non_zero  # first, handle the special case of a value of zero
    li      $t0, 0x30           # load '0' char in $t0
    sb      $t0, 0($a1)         # store '0' in string buffer
    sb      $zero, 1($a1)       # store null char for end string
    li      $v0, 1              # set the string length to 1
    jr      $ra                 # return to main function
ItoA_non_zero:
    addi    $t0, $zero, 10      # load 10 to divide later
    li      $v0, 0              # set the string length
    bgtz    $a0, ItoA_recurse   # check for a negative value
    li      $t1, 0x2d           # load '-' char in $t1
    sb      $t1, 0($a1)         # store '-' char in string buffer
    addi    $v0, $v0, 1         # increment string length
    neg     $a0, $a0            # negate integer to manipulate a positive one
ItoA_recurse:
    addi    $sp, $sp, -24       # reserve stack space to store values
    sw      $fp, 8($sp)         # store local recursive frame pointer scope
    addi    $fp, $sp, 8         # set the frame pointer to the next free space
    sw      $a0, 4($fp)         # store first param (integer to convert)
    sw      $a1, 8($fp)         # store second param (char buffer pointer)
    sw      $ra, -4($fp)        # store return address in top of frame pointer
    sw      $s0, -8($fp)        # store quotient in top of $ra
    sw      $s1, -12($fp)       # store remainder in top of $s0
    div     $a0, $t0            # $a0/10
    mflo    $s0                 # $s0 = quotient
    mfhi    $s1                 # s1 = remainder
    beqz    $s0, ItoA_write     # if $s0 = 0 we are done we can write the string
    move    $a0, $s0            # if not we continue our recursion
    jal     ItoA_recurse        # with quotient as the number to manipulate
ItoA_write:
    add     $t1, $a1, $v0       # increment string buffer pointer with string length
    addi    $v0, $v0, 1         # increment string length
    addi    $t2, $s1, 0x30      # convert to ASCII by adding '0' char
    sb      $t2, 0($t1)         # store in the string buffer
    sb      $zero, 1($t1)       # store null char at the end of the string
    lw      $a0, 4($fp)         # restore first param (integer to convert)
    lw      $a1, 8($fp)         # restore second param (char buffer pointer)
    lw      $ra, -4($fp)        # restore return address
    lw      $s0, -8($fp)        # restore quotient
    lw      $s1, -12($fp)       # restore remainder
    lw      $fp, 8($sp)         # restore local recursive frame pointer scope
    addi    $sp, $sp, 24        # restore stack space to its original point
    jr      $ra                 # jump to return address