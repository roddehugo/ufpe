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
    move $t0,$v0
    sub $sp, 4      #PUSH on the stack
    sw $ra, ($sp)  #our return address
    jal read_int
    move $t1,$v0
    jal read_int
    move $t2,$v0
    li $t3,3600
    jal loop
    li $t3,60
    jal loop
    la $a0,result
    jal print_str
    move $a0,$t2
    jal print_int
    j exit
loop:
    blez $t1,return
    addu $t2,$t2,$t3
    subu $t1,$t1,1
    j loop
return:
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