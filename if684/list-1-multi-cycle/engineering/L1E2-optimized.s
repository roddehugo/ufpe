## Lista 1 - Exercicio 2.b
## int x = 4;
## int y = 21;
## int z = 0;
## if(x<y){
##   if(x<6){
##     z=5;
##   }else{
##     z=10;
##   }
## }else{
##     z=15;
## }
.data
    x: .word 4
    y: .word 21
    z: .word 0
.text
.globl main
main:
    lw $t0,x
    lw $t1,y
    lw $t2,z
    blt $t0,$t1,if1
    addiu $t2,$zero,15
    j exit
if1:
    blt $t0,6,if2
    addiu $t2,$zero,10
    j exit
if2:
    addiu $t2,$zero,5
    j exit
exit:
    sw $t2,z
    li $v0,10
    syscall