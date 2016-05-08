.data
    read_M: .asciiz "Enter a value for M: "
    read_N: .asciiz "Enter a value for N: "
    result: .asciiz "The number of divisions was: "
    MltN: .asciiz "Incorrect input: M < N. Please, try again."
    endl: .asciiz "\n"
.text
.globl main
main:
    move $t0,$zero
    la $a0,read_M
    jal print_str
    jal read_int
    move $t1,$v0
    la $a0,read_N
    jal print_str
    jal read_int
    move $t2,$v0
    blt $t1,$t2,error
while:
    blt $t1,$t2,endwhile
    div $t1,$t2
    mflo $t1
    addiu $t0,$t0,1
    j while
endwhile:
    la $a0,result
    jal print_str
    move $a0,$t0
    jal print_int
    jal print_endl
    j exit
print_int:
    li  $v0,1
    syscall
    jr  $ra
print_str:
    li  $v0,4
    syscall
    jr  $ra
print_endl:
    la  $a0,endl
    li  $v0,4
    syscall
    jr  $ra
read_int:
    li $v0,5
    syscall
    jr $ra
error:
    la $a0,MltN
    jal print_str
    jal print_endl
    j main
exit:
    li $v0,10
    syscall