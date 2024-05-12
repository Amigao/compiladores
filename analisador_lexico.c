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

bool isoperator(char c){
    return(c == '+' || c == '-' || c == '*');
}

bool isponctuation(char c){
    return(c == ',' || c == ';' || c == '.');
}

bool isrelation(char c){
    return(c == '<' || c == '>' || c == '!');
}

int transicao(int state, char c, char a, Tabela *tabela) {
    switch(state) {
        case 0:
            if(uppercase(c))state = 1;
            else if(lowercase(c)) state = 2;
            else if(isnum(c)) state = 3;
            //else if(c == '=') state = 4;
            else if(c == '!') state = 5;
            else if(c == '<') state = 6;
            else if(c == '>') state = 7;
            else if(c == ':') state = 8;
            else if(isponctuation(c)) state = 12;
            //else if(c == ';') state = 9;
            //else if(c == ',') state = 10;
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

char *verifica_tabela_reservados(Tabela *tabela1, char *string){
    if (busca_tabela(tabela1, string)) return string;
  //  if(busca_tabela(tabela2,string)) return string;
    else return "ident";
}

bool olha_tabela(Tabela *tabela, char *string){
    return(busca_tabela(tabela,string));
}

void verifica_estado(int curr, char *sub, Tabela *tabela){
            switch (curr){
                case 1:
                    if(verifica_tabela_reservados(tabela, sub)){
                        printf("Token %s RESERVADO\n", sub);
                    }else{
                        printf("Token %s identificador\n", sub);
                    }
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
                case 10:
                    printf("Token %s virgula\n", sub);
                    break;
                case 11:
                    printf("Token %s operador\n", sub);
                    break;
                case -1:
                    printf("ERRO LEXIXO,  %s\n",sub);
                    break;
                case 12: 
                    printf("Token %s pontuacao\n", sub);
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
        state = transicao(state,c,a, TabelaReservada);
        if(state == 0){
            i--;
            strncpy(sub,string + j,i-j);
            sub[i-j] = '\0';
            verifica_estado(curr, sub, TabelaReservada);
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
    verifica_estado(curr, sub, TabelaReservada);
    
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

