#include <stdio.h>
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

int transicao(int state, char c, char a){
    switch(state){
        case 0:
            if(uppercase(c))state = 1;
            else if(lowercase(c)) state = 2;
            else if(isnum(c)) state = 3;
            else if(c == '=') state = 4;
            else if(c == '!') state = 5;
            else if(c == '<') state = 6;
            else if(c == '>') state = 7;
            else if(c ==':' && a =='='){
                state = 8;
                return state;
            }
            else if(c == ';') state = 9;
            else return -1;
            break;
        case 1:
            if(uppercase(c)) state = 1;
            else if(lowercase(c) || isnum(c)) state = 2;
            else return -1;
            break;
        case 2:
            if(uppercase(c) || lowercase(c) || isnum(c)) state = 2;
            else if(c == ':' || c == ',' || c ==';') return 0;
         //   else if(a == ':' || a == ',' || a ==';') return 0;
            else return -1;
            break;
        case 3:
            if(isnum(c)) state = 3;
            else if(c == ';') return 0;
            break;
        case 4:
            break;
        case 5:
            if(c == '=') state = 5;
            break;
        case 6:
            if(c == '=') state = 6;
            break;
        case 7:
            if(c == '=') state = 6;
            break;
    }
    return state;
}

char *verifica_tabela_reservados(Tabela *tabela1, char *string){
    if (busca_tabela(tabela1, string)) return string;
  //  if(busca_tabela(tabela2,string)) return string;
    else return "ident";
}

void verifica_estado(int curr, char *sub){
            switch (curr){
                case 1:
                    printf("Token %s RESERVADO\n", sub);
                    break; 
                case 2:
                    printf("Token %s identificador\n", sub);
                    break;
                case 3:
                    printf("Token %s Numero\n", sub);
                    break;
                case 4:
                    printf("Token %s igual\n", sub);
                    break;
                case 5:
                    printf("Token %s diferente\n", sub);
                    break;
                case 6:
                    printf("Token %s menor\n", sub);
                    break;
                case 7:
                    printf("Token %s maior\n", sub);
                    break;
                case 8:
                    printf("Token %s atribuicao\n", sub);
                    break;
                case 9:
                    printf("Token %s ponto_virgula\n", sub);
                    break;
            }
}


void analisador_lexico(char* string, Tabela* TabelaReservada ){

    int i =0;
    int j =0;
    char c = string[i++];
    int state =0;
    int curr =0;
    char a;
    char sub[256];


    while(c != '\0'){
        a = string[i];
        state = transicao(state,c,a);
        if(state == 0){
            i--;
            strncpy(sub,string + j,i-j);
            sub[i-j] = '\0';
            verifica_estado(curr, sub);
            j = i;
            state =0;
            curr = 0;
        }
        else{
            curr = state;
            c = string[i++];
        }
    }
    strncpy(sub, string +j, i - j);
    sub[i-j] = '\0';
    verifica_estado(curr, sub);
    
    //char *saida = verifica_estado(curr, string, TabelaReservada);
}

void constroi_tabela_reservada(Tabela *tabela){
    inicializa_tabela(tabela);
    insere_tabela(tabela, "CONST");
    insere_tabela(tabela, "VAR");
    insere_tabela(tabela, "PROCEDURE");
    insere_tabela(tabela, "BEGIN");
    insere_tabela(tabela, "END");
    insere_tabela(tabela, "CALL");
    insere_tabela(tabela, "IF");
    insere_tabela(tabela, "THEN");
    insere_tabela(tabela, "WHILE");
    insere_tabela(tabela, "DO");
    insere_tabela(tabela, "ODD");
}

void constroi_tabela_simbolos(Tabela *tabela){
    insere_tabela(tabela, "+");
    insere_tabela(tabela, "-");
    insere_tabela(tabela, "*");
    insere_tabela(tabela, "/");
    insere_tabela(tabela, "=");
    insere_tabela(tabela, "<>");
    insere_tabela(tabela, "<");
    insere_tabela(tabela, "<=");
    insere_tabela(tabela, ">");
    insere_tabela(tabela, ">=");
    insere_tabela(tabela, ":=");
    insere_tabela(tabela, "(");
    insere_tabela(tabela, ")");
    insere_tabela(tabela, ",");
    insere_tabela(tabela, ";");
    insere_tabela(tabela, ".");
}

