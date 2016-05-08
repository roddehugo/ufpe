## Implemente um programa na linguagem Mips que recebe como entrada
## uma string e passe todos os seus caracteres para maiusculo.
## Se houver algum caractere que nao seja letra, o valor 1 devera
## ser armazenado no registrador v1($3) e o programa encerrado.
## Exemplo:
## Entrada - "EntradA"
## Saida   - "ENTRADA"

## Uppercase ASCII : [A(65):Z(90)]
## Lowercase ASCII : [a(97):z(122)]
## Enter ASCII     : LF(10)

.data
    welcome:    .asciiz "Enter a word to uppercase [0:20]: "
    result:     .asciiz "Result: "
    str:        .space  160
.text
.globl main
main:
    la      $a0, welcome
    jal     print_str
    jal     read_str
    li      $t0, 0
loop:
    lbu     $a0, str($t0)
    beq     $a0, 10, return
    jal     isAlpha
    blez    $v0, not_letter
    and     $a0, $a0, 0xDF
    sb      $a0, str($t0)
    addiu   $t0, $t0, 1
    j       loop
return:
    la      $a0, result
    jal     print_str
    la      $a0, str
    jal     print_str
    j       exit
not_letter:
    li      $v1, 1
    j       exit
print_str:
    li      $v0, 4
    syscall
    jr      $ra
read_str:
    li      $v0, 8
    li      $a1, 20
    la      $a0, str
    syscall
    jr      $ra
exit:
    li      $v0, 10
    syscall

#############################################################
# boolean function isAlpha (char)
#       $a0: argument char, unchanged
#       $v0: boolean value output, TRUE (1) if char is letter
#       Uses registers t5, t6, t7, t8
#############################################################
isAlpha:
        sgt     $t5,$a0,0x40    # char > 'A'-1
        slt     $t6,$a0,'Z'+1   # char < 'Z'+1
        and     $t7,$t5,$t6     # (char >= 'A' and char <= 'Z')
        sgt     $t5,$a0,0x60    # char < 'a'-1
        slt     $t6,$a0,'z'+1   # char > 'z'+1
        and     $t8,$t5,$t6     # (char >= 'a' and ch <= 'z')
        or      $v0, $t8,$t7    # $v0 = isAlpha truth value returned
        jr      $ra