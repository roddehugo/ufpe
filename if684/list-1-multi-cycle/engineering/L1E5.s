## Desenvolva um programa que recebe como entrada um array de
## inteiros a e escreve em um outroArray b todos os numeros primos
## contidos em a.
## Ex.: a = [2, 4, 7, 17, 25] → b=[2,7,17]
## Obs.: assuma que so serao testados numeros no intervalo [1, 100].
## 2   3   5   7   11
## 13  17  19  23  29
## 31  37  41  43  47
## 53  59  61  67  71
## 73  79  83  89  97
.data
    array_a: .word 2,4,7,17,25
    array_b: .space 250
.text
.globl main
main:
    lw      $s2, array_a
    li      $t0, 0
    li      $t1, 0
loop:
    addu    $a0, $zero, $zero
    jal     test_prime                  # call test_prime function
    bne     $v0, $zero, add_to_array_b  # was it false
    addiu   $t0, $t0, 32
    j       loop
add_to_array_b:
    sw      $t0($s2), array_b($t1)
    j       loop
    j       exit
exit:
    li $v0,10
    syscall

#-------------------------------------------------------------
# Procedure: test_prime
# Input: integer value n
# Return: 1 if n is prime and 0 if n is not prime
#   of the callee saved registers the subroutine test_prime uses
#   s2, s3 so we will save those to the stack
#   as well as the old fp, and return pointers in
#   the beginng and restore them in the end.
#   we will then set the new $fp to keep with convention
#   although we dont ever use it
#-------------------------------------------------------------
.text
test_prime:
    addi    $sp, $sp, -4                # dec sp
    sw      $ra, ($sp)                  # save return address on stack
    addi    $sp, $sp, -4                # dec sp
    sw      $fp, ($sp)                  # save fp on stack
    addi    $sp, $sp, -4                # dec sp
    sw      $s2, ($sp)                  # save s2 on stack
    addi    $sp, $sp, -4                # dec sp
    sw      $s3, ($sp)                  # save s3 on stack
    addi    $sp, $sp, -4                # dec sp
    addi    $fp, $sp, 32                # set $fp
    addi    $s2, $zero, 1               # put 1 in s2
    bne     $s2, $a0, not_one           # is the arg 1
    add     $v0, $zero, $zero           # if so not a prime
    beq     $zero, $zero, exit_prime    # exit subroutine
not_one:
    addi    $s2, $zero, 2               # put immediate value 2 in  s2
    beq     $a0, $s2, three_done        # two is a prime
    div     $a0, $s2                    # do divide
    mfhi    $t0                         # get remainder
    bgtz    $t0, three                  # is it a mult of 2
    add     $v0, $zero, $zero           # if so not a prime
    beq     $zero, $zero, exit_prime    # exit subroutine
three:
    addi    $s2, $s2, 1                 # put 3 in  s2
three_loop:
    add     $t0, $zero, $zero           # clear reg
    mult    $s2, $s2                    # get pow_2($s2)
    mflo    $s3                         # get answer. for our purposes it will always be in the lo
    sub     $t0, $a0, $s3               # is s2 greater than the squareroot of a0
    bltz    $t0, three_done             # if so were done
    div     $a0, $s2                    # do divide
    mfhi    $t0                         # get remainder
    bgtz    $t0, three_cont             # is $a0 a mult of $s2
    add     $v0, $zero, $zero           # if so not a prime
    beq     $zero, $zero, exit_prime    # exit subroutine
three_cont:
    addi    $s2, $s2, 2                 # continue skipping mults of three and evens
    beq     $zero, $zero, three_loop    # go again
three_done:                             # through loop so number is a prime
    addi    $v0, $zero, 1               # is prime so true
exit_prime:
    addi    $sp, $sp, 4                 # inc sp
    lw      $s3, ($sp)                  # get old s3 off stack
    addi    $sp, $sp, 4                 # inc sp
    lw      $s2, ($sp)                  # get old s2 off stack
    addi    $sp, $sp, 4                 # inc sp
    lw      $fp, ($sp)                  # get old fp off stack
    addi    $sp, $sp, 4                 # inc sp
    lw      $ra, ($sp)                  # get return address off stack
    addi    $sp, $sp, 4                 # inc sp back to where we expect it to start
    jr      $ra                         # go back to caller+4