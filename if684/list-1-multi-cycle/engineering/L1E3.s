.data
    M: .word 11
    N: .word 2
    endl: .asciiz "\n"
.text
.globl main
main:
    move $t0,$zero
    lw $t1,M
    lw $t2,N
while:
    blt $t1,$t2,endwhile
    div $t1,$t2
    mflo $t1
    addiu $t0,$t0,1
    j while
endwhile:
    move $a0,$t0
    jal print_int
    jal endline
    j exit
print_int:
    li  $v0,1
    syscall
    jr  $ra
endline:
    la  $a0,endl
    li  $v0,4
    syscall
    jr  $ra
read_int:
    li $v0,5
    syscall
    jr $ra
exit:
    li $v0,10
    syscall