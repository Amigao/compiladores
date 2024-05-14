#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "analisador_lexico.h"
#include "hashing.h"

#define NUM_FINAL_STATE 3

bool uppercase(char c){
    return(c >= 'A' && c <= 'Z');
}

bool lowercase(char c){
    return(c >= 'a' && c <= 'z');
}

bool isnum(char c){
    return(c>='0' && c<= '9');
}

bool isoperator(char c){
    return(c == '+' || c == '-' || c == '*');
}

bool isponctuation(char c){
    return(c == ',' || c == ';' || c == '.');
}

bool isrelation(char c){
    return(c == '<' || c == '>' || c == '!');
}

void constroi_tabela_reservada(Tabela *tabela){
    inicializa_tabela(tabela);
    insere_tabela(tabela, "CONST",  "<CONST>");
    insere_tabela(tabela, "VAR", "<VAR>");
    insere_tabela(tabela, "PROCEDURE", "<PROCEDURE>");
    insere_tabela(tabela, "BEGIN", "<BEGIN>");
    insere_tabela(tabela, "END", "<END>");
    insere_tabela(tabela, "CALL", "<CALL>");
    insere_tabela(tabela, "IF", "<IF>");
    insere_tabela(tabela, "THEN", "<THEN>");
    insere_tabela(tabela, "WHILE", "<WHILE>");
    insere_tabela(tabela, "DO", "<DO>");
    insere_tabela(tabela, "ODD", "<ODD>");
    insere_tabela(tabela, "+", "<SIMBOLO_SOMA>");
    insere_tabela(tabela, "-", "<SIMBOLO_SUBTRACAO>");
    insere_tabela(tabela, "*", "<SIMBOLO_MULTIPLICACAO>");
    insere_tabela(tabela, "/", "<SIMBOLO_DIVISAO>");
    insere_tabela(tabela, "=", "<SIMBOLO_IGUAL>");
    insere_tabela(tabela, "<>", "<SIMBOLO_DIFERENTE>");
    insere_tabela(tabela, "<", "<SIMBOLO_MENOR>");
    insere_tabela(tabela, "<=", "<SIMBOLO_MENOR_IGUAL>");
    insere_tabela(tabela, ">", "<SIMBOLO_MAIOR>");
    insere_tabela(tabela, ">=", "<SIMBOLO_MAIOR_IGUAL>");
    insere_tabela(tabela, ":=", "<SIMBOLO_ATRIBUICAO>");
    insere_tabela(tabela, "(", "<PARENTESE_ESQUERDA>");
    insere_tabela(tabela, ")", "<PARENTESE_DIREITA>");
    insere_tabela(tabela, ",", "<VIRGULA>");
    insere_tabela(tabela, ";", "<PONTO_E_VIRGULA>");
    insere_tabela(tabela, ".", "<PONTO>");
}

char *verifica_tabela_reservados(Tabela *tabela, char *string){
    char *resultado = busca_tabela(tabela, string);
    if (resultado != NULL) return resultado;
    else return "ident";
}

int transicao(int state, char c) {
    switch(state) {
        case 0:
            if(uppercase(c))state = 1;
            else if(lowercase(c)) state = 2;
            else if(isnum(c)) state = 3;
            else if(c == '!') state = 5;
            else if(c == '<') state = 6;
            else if(c == '>') state = 7;
            else if(c == ':') state = 8;
            else if(isponctuation(c)) state = 12;
            else if(isoperator(c)) state = 11;
            else return -1;
            break;
        case 1:
            if(uppercase(c)) state = 1;
            else if(lowercase(c) || isnum(c)) state = 2;
            else if(isponctuation(c) || isrelation(c)) state = 0;
            else return -1;
            break;
        case 2:
            if(uppercase(c) || lowercase(c) || isnum(c)) state = 2;
            else if(c == ':' || c == ',' || c ==';' || isoperator(c) || isrelation(c)) state = 0;
            else return -1;
            break;
        case 3:
            if(isnum(c)) state = 3;
            else if(c == ';') state = 0;
            else return -1;
            break;
       case 5:
            if(c == '=' || c == '!') state = 5;
            else if(uppercase(c) || lowercase(c) || isnum(c)) return 0;
            else return -1;
            break;
        case 6:
            if(c == '=' || c == '<') state = 6;
            else if(uppercase(c) || lowercase(c) || isnum(c)) return 0;
            else return -1;
            break;
        case 7:
            if(c == '=' || c == '>') state = 7;
            else if(uppercase(c) || lowercase(c) || isnum(c)) return 0;
            else return -1;
            break;
        case 8:
            if(c == '=' || c == ':') state = 8;
            else if(uppercase(c) || lowercase(c) || isnum(c)) return 0;
            else return -1;
            break;
        case 12:
            if(isponctuation(c)) return state;
            else if(uppercase(c) || lowercase(c) || isnum(c)) state = 0;
            else return -1;
            break;
        case 11:
            if(isoperator(c)) return 11;
            else if(uppercase(c) || lowercase(c) || isnum(c)) state = 0;
            else return -1;
            break;
    }
    return state;
}

char *verifica_estado(int curr, char *sub, Tabela *tabela, FILE *output_file) {
    char* saida;
    switch (curr) {
        case 1:
            saida = verifica_tabela_reservados(tabela, sub);
            fprintf(output_file, "%s, %s\n", sub, saida);
            break;
        case 2:
            fprintf(output_file, "%s, identificador\n", sub);
            saida = "identificador";
            break;
        case 3:
            fprintf(output_file, "%s, numero\n", sub);
            saida = "numero";
            break;
        case 4:
            fprintf(output_file, "Token %s igual\n", sub);
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            saida = verifica_tabela_reservados(tabela, sub);
            fprintf(output_file, "%s, %s\n", sub, saida);
            break;
        case -1:
            saida = "ERRO_LEXICO";
            fprintf(output_file, "%s, ERRO LEXICO \n", sub);
            break;
    }
    return saida;
}
char *analisador_lexico(char* string, Tabela* tabela, FILE *output_file){
    int i =0;
    int j =0;
    char c = string[i++];
    int state =0;
    int curr =0;
    char sub[256];

    while(c != '\0'){
        state = transicao(state,c);
        if(state == 0){
            i--;
            strncpy(sub,string + j,i-j);
            sub[i-j] = '\0';
            verifica_estado(curr, sub, tabela, output_file);
            j = i;
            state = 0;
            curr = 0;
        }
        else{
            curr = state;
            c = string[i++];
        }
    }
    strncpy(sub, string + j, i - j);
    sub[i-j] = '\0';
    return verifica_estado(curr, sub, tabela, output_file);
}
