#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "analisador_lexico.h"
#include "hashing.h"

int is_first_double_operator(char c){
    if (c == ':' || c == '!' || c == '<' || c == '>'){
        return 1;
    }
    return 0;
}

int is_second_double_operator(char c){
    if (c == '=' || c == '!' || c == '<' || c == '>'){
        return 1;
    }
    return 0;
}

int is_single_operator(char c){
    if (c == '=' || c == '+' || c == '-' || c == '*' || c == '/'){
        return 1; 
    }
    return 0;
}

int isdelimiter(char c){
    if (c == ',' || c == ';' || c == '.'){
        return 1;
    }
    return 0;
}


int transicao(int state, char c){
    // Estado 3 (Apenas dígitos são aceitos)
    if (state == 3 && isdigit(c)) {
        return state;
    }
    // Outros estados
    if (isdigit(c)) {
        return 3;
    }
    if (islower(c)) {
        return 2;
    }
    if (isupper(c)) {
        return 1;
    }
    if (isspace(c)) {
        return 4;
    }
    if (isdelimiter(c)) {
        return 5;
    }
    if (is_first_double_operator(c)) {
        return 6;
    }
    if (is_second_double_operator(c)) {
        return 7;
    }
    if (is_single_operator(c)) {
        return 8;
    }
    return -1;
}

char *verifica_tabela_reservados(Tabela *tabela, char *string){
    if (busca_tabela(tabela, string)) {
        return string;
    } else if (isdigit(string[0])){
        return "number";
    } else {
        return "ident";
    }
}

char *verifica_tabela_simbolos(char *string){
    if (!strcmp(string, ";")){
       return "simbolo_ponto_virgula"; 
    } else if (!strcmp(string, ",")){
        return "simbolo_virgula";
    } else if (!strcmp(string, ":=")){
        return "simbolo_atribuicao";
    } else if (!strcmp(string, "+")){
        return "simbolo_mais";
    } else if (!strcmp(string, ".")){
        return "simbolo_ponto";
    }
    else {
        return "ERRO_LEXICO";
    } 
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

int is_final_state(int state){
    if (state == 0 || state == 6){
        return 0;
    } 
    return 1;
}

int changed_state(int state_a, int state_b){
    if (state_a != state_b){
        return 1;
    }
    return 0;
}

int buffer_is_symbol(int state){
    if (state == 5 || state == 6 || state == 7 || state == 8){
        return 1;
    }
    return 0;
}

int analisador_lexico(char character, char *buffer, Tabela* TabelaReservada, int current_state){
    int new_state = transicao (current_state, character);
    if (current_state == -1){
        printf("%s, ERRO_LEXICO\n", buffer);
        return CHANGED_STATE;
    }
    // se mudou de estado e não é um estado final 
    if (is_final_state(current_state) && changed_state(current_state, new_state)){
        // se não é espaço 
        if (!isspace(buffer[0])){
            if (buffer_is_symbol(current_state)){
                printf("%s, %s\n", buffer, verifica_tabela_simbolos(buffer));
            } else {
                printf("%s, %s\n", buffer, verifica_tabela_reservados(TabelaReservada, buffer));
            }  
        }
        return CHANGED_STATE;
    }
    return new_state;
}
