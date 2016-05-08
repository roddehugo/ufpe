## Dado tres inteiros Hora, Min e Sec, faca um programa que converta
## esse horario em segundos, armazene em um dos registradores temporarios
## e na memoria(posicao a sua escolha).
## OBS.: Nao e permitido usar a funcao mult.

.data
    read_HMS: .asciiz "Enter Hour, Min and Sec successively:\n"
    result: .asciiz "Conversion to seconds gave: "
.text
.globl main
main:
    la $a0,read_HMS
    jal print_str
    jal read_int
    jal push_stack
    jal read_int
    jal push_stack
    jal read_int
    jal push_stack
    jal pop_stack
    move $t2,$t1
    li $t0,60
    jal pop_stack
    jal loop
    li $t0,3600
    jal pop_stack
    jal loop
    la $a0,result
    jal print_str
    move $a0,$t2
    jal print_int
    j exit
loop:
    blez $t1,return
    addu $t2,$t2,$t0
    subu $t1,$t1,1
    j loop
return:
    jr $ra
push_stack:
    sub $sp,$sp,4          # PUSH on the stack
    sw $v0, ($sp)       # our read value
    jr $ra
pop_stack:
    lw $t1, ($sp)       #POP our value from the stack
    add $sp,$sp,4          #top of stack back as it was before the PUSH
    jr $ra
read_int:
    li $v0,5
    syscall
    jr $ra
print_str:
    li  $v0,4
    syscall
    jr  $ra
print_int:
    li  $v0,1
    syscall
    jr  $ra
exit:
    li $v0,10
    syscall