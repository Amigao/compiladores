#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "analisador_lexico.h"
#include "hashing.h"

#define NUM_FINAL_STATE 3

int transicao(int state, char c){
    
    if (state == 0){ // ESTADO q0
        if (isdigit(c)){ 
            state = 3;
        } 
        else if (islower(c)){
            state = 2;
        }
        else if (isupper(c)){
            state = 1;
        }
        else state = -1;
    }
    else if (state == 1){ // ESTADO Q1
        if (isdigit(c)){ 
            state = 2;
        } 
        else if (islower(c)){
            state = 2;
        }
        else if (isupper(c)){
            state = 1;
        }
        else state = -1;

    }
    else if (state == 2){ // ESTADO Q2
        if (isdigit(c)){ 
            state = 2;
        } 
        else if (islower(c)){
            state = 2;
        }
        else if (isupper(c)){
            state = 2;
        }
        else state = -1;

    }
    else if (state == 3){ // ESTADO Q3
        
        if (isdigit(c)){ 
            state = 3;
        } 
        else state = -1;

    }    
    
    return state;
}

char *verifica_tabela_reservados(Tabela *tabela, char *string){
    if (busca_tabela(tabela, string)) return string;
    else return "ident";
}

void constroi_tabela_reservada(Tabela *tabela){
    inicializa_tabela(tabela);
    insere_tabela(tabela, "VAR");
    insere_tabela(tabela, "CONST");
    insere_tabela(tabela, "PROCEDURE");
    insere_tabela(tabela, "BEGIN");
    insere_tabela(tabela, "END");
    insere_tabela(tabela, "CALL");
    insere_tabela(tabela, "IF");
    insere_tabela(tabela, "THEN");
    insere_tabela(tabela, "WHILE");
    insere_tabela(tabela, "DO");
    insere_tabela(tabela, "VAR");
    insere_tabela(tabela, "ODD");
}

char *analisador_lexico(char* string, Tabela* TabelaReservada){

    char c = string[0];
    int state = 0;
    // int final_state[NUM_FINAL_STATE] = {1, 2, 3};
    
    int i = 0;
    while(c != '\0' && state != -1){
        state = transicao (state, c);
        //printf("Estado atual: %d \nCaractere atual: %c\n\n", state, c);
        c = string[i++];
    }

    
    if (state == 1) return verifica_tabela_reservados(&(*TabelaReservada), string);
    // if (state == 1) return "ident";
    if (state == 2) return "ident";
    if (state == 3) return "numero";

    return "ERRO";

}
