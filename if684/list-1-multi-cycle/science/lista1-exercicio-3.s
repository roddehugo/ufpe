## Lista 1 - Exercicio 3
## Faca um programa em linguagem de montagem Mips que receba como
## entrada uma string com n caracteres e gere como saida uma nova
## string com a inversao da ordem dos caracteres. Essa nova string
## tambem tera a troca das letras maiusculas por minusculas e
## vice-versa. Por exemplo: se a entrada for HArdwArE a saida devera
## ser eRaWDRah. A entrada deve ser lida da memoria e a saida deve ser
## escrita na memoria. Caso a string possua algum caractere que nao
## seja letra o valor 1 devera ser armazenado no registrador v1 e o
## programa devera ser encerrado. Lembramos que o fim da string e dado
## pelo caractere nulo e que para manipular string nessa questao
## recomendamos que sejam usadas variaveis do tipo ASCIIZ.
## Atencao: os caracteres deverao ser armazenados em sequencia na memoria.

## Exemplo:
## Entrada - "HArdwArE"
## Saida   - "eRaWDRah"

## Uppercase ASCII : [A(65):Z(90)]
## Lowercase ASCII : [a(97):z(122)]

## We'll implement such an algorithm
## Function reverse (char in, char output)
##    i = 0
##    j = 0
##    While not end of input:
##        read input[i]
##        check if input[i] is an alpha character
##        increment i
##    Now i points to the end of input string
##    While i > 0:
##        read input[i]
##        switch input[i] case
##        store input[i] swithed in output[j]
##        increment j
##        decrement i
## End function

.data
    in: .asciiz "HArdwArE"
    out: .asciiz ""
.text
.globl main
main:
    li      $s0, 0              # pointer for input string
    li      $s1, 0              # pointer for output string
goToEnd:                        # loop to go to the end of the entered string
    lbu     $a0, in($s0)        # load the next character into a0
    beqz    $a0, inverse        # check for the null character
    jal     isAlpha             # jump to the isAlpha procedure
    blez    $v0, notAlpha       # check if is an alpha char ($v0!=0)
    addiu   $s0, $s0, 1         # increment the string pointer
    j       goToEnd             # return to the top of the goToEnd loop
inverse:
    sub     $s0, $s0, 1         # $s0 was pointing to the null char, we decrement it
    lbu     $a0, in($s0)        # load the char into $a0 by indexed addressing
    xor     $a0, $a0, 0x20      # by XORing the 5th bit we switch from upper to lower and vice versa
    sb      $a0, out($s1)       # store the switched char into out data address
    addiu   $s1, $s1, 1         # increment out string pointer
    beqz    $s0, exit           # if $s0=0 we have finished to read the in string backward
    j       inverse             # if not, we loop to the next char
notAlpha:
    li      $v1, 1              # store 1 in return value
    j       exit                # exit gracefully
exit:
    li      $v0, 10             # load exit code to syscall
    syscall                     # syscall exit the program

#############################################################
# boolean function isAlpha (char)
# arguments:
#       $a0: argument char
# returns:
#       $v0: boolean value output, true (1) if char is letter
# uses registers t5, t6, t7, t8
#############################################################
isAlpha:
    sgt     $t5, $a0, 0x40      # char > 'A'-1
    slt     $t6, $a0, 0x5b      # char < 'Z'+1
    and     $t7, $t5, $t6       # (char >= 'A' and char <= 'Z')
    sgt     $t5, $a0, 0x60      # char < 'a'-1
    slt     $t6, $a0, 0x7b      # char > 'z'+1
    and     $t8, $t5, $t6       # (char >= 'a' and ch <= 'z')
    or      $v0, $t8, $t7       # set the final condition on return register
    jr      $ra