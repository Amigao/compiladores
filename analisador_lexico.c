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
    if (state == 3 && isalpha(c)){
        return -1;
    }

    // identificadores com números
    if (state == 2 && isdigit(c)){
        return state;
    }
    // palavras reservadas 
    if (isupper(c)) {
        return 1;
    }
    // identificadores
    if (islower(c)) {
        return 2;
    } 
    // números
    if (isdigit(c)) {
        return 3;
    }
    // espaço - descartado na lógica principal
    if (isspace(c)) {
        return 4;
    }
    // delimitador 
    if (isdelimiter(c)) {
        return 5;
    }
    // primeiro caractere de um operador com dois caracteres
    if (is_first_double_operator(c)) {
        return 6;
    }
    // segundo caractere de um operador com dois caracteres
    if (is_second_double_operator(c)) {
        return 7;
    }
    // operador com um caractere
    if (is_single_operator(c)) {
        return 8;
    }
    return -1;
}

char *verifica_tabela_reservados(Tabela *tabela, char *string){
    char *resultado = busca_tabela(tabela, string);
    if(isdigit(string[0])){
        return "number";
    }else if (resultado) {
        return resultado;
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

lexico analisador_lexico(char character, char *buffer, Tabela* TabelaReservada, int current_state){
    lexico tok;
    tok.final = false;
    int new_state = transicao (current_state, character);
    if (new_state == -1){
        tok.token = buffer;
        tok.identficador = "Erro";
        tok.state = new_state;
        int length = strlen(tok.token);
        tok.token[length] = character;
        tok.token[length + 1] = '\0';
        return tok;
    }
    //printf("new_state = %d | current_state = %d | character = %c \n", new_state, current_state, character);
    // se mudou de estado e não é um estado final 
    if (is_final_state(current_state) && changed_state(current_state, new_state)){
        // se não é espaço 
        if (!isspace(buffer[0])){
            tok.token = buffer;
            tok.identficador = verifica_tabela_reservados(TabelaReservada,buffer);
            tok.final = true;
        }
        tok.state = END_BUFFER;
        return tok;
    }
    tok.token = buffer;
    tok.identficador = NULL;
    tok.state = new_state;
    return tok;
}
