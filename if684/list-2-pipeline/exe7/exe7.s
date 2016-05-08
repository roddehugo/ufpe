#include <reg.h>
.data
string: .ASCIIZ “coloque os primeiros nomes dos integrantes da equipe”
//Define uma string de caracteres
caractere: .ASCIIZ “a” //Define o caractere de entrada
.set noreorder //Obrigatório para essa questão
.text
.globl start
.ent start
start:
// local onde os caracteres serão carregados e comparados
.end start 