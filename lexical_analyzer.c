#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
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

int transition(int state, char c){
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

char *check_reserved_table(Table *table, char *string){
    char *resultado = search_table(table, string);
    if(isdigit(string[0])){
        return "number";
    }else if (resultado) {
        return resultado;
    } else {
        return "ident";
    }
}

void build_reserved_table(Table *table){
    initialize_table(table);
    insert_table(table, "CONST",  "<CONST>");
    insert_table(table, "VAR", "<VAR>");
    insert_table(table, "PROCEDURE", "<PROCEDURE>");
    insert_table(table, "BEGIN", "<BEGIN>");
    insert_table(table, "END", "<END>");
    insert_table(table, "CALL", "<CALL>");
    insert_table(table, "IF", "<IF>");
    insert_table(table, "THEN", "<THEN>");
    insert_table(table, "WHILE", "<WHILE>");
    insert_table(table, "DO", "<DO>");
    insert_table(table, "ODD", "<ODD>");
    insert_table(table, "+", "<SIMBOLO_SOMA>");
    insert_table(table, "-", "<SIMBOLO_SUBTRACAO>");
    insert_table(table, "*", "<SIMBOLO_MULTIPLICACAO>");
    insert_table(table, "/", "<SIMBOLO_DIVISAO>");
    insert_table(table, "=", "<SIMBOLO_IGUAL>");
    insert_table(table, "<>", "<SIMBOLO_DIFERENTE>");
    insert_table(table, "<", "<SIMBOLO_MENOR>");
    insert_table(table, "<=", "<SIMBOLO_MENOR_IGUAL>");
    insert_table(table, ">", "<SIMBOLO_MAIOR>");
    insert_table(table, ">=", "<SIMBOLO_MAIOR_IGUAL>");
    insert_table(table, ":=", "<SIMBOLO_ATRIBUICAO>");
    insert_table(table, "(", "<PARENTESE_ESQUERDA>");
    insert_table(table, ")", "<PARENTESE_DIREITA>");
    insert_table(table, ",", "<VIRGULA>");
    insert_table(table, ";", "<PONTO_E_VIRGULA>");
    insert_table(table, ".", "<PONTO>");
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

TokenInfo lexical_analyzer(char character, char *buffer, Table* reservedTable, int current_state){
    TokenInfo tok;
    tok.final = false;
    int new_state = transition(current_state, character);
    if (new_state == -1){
        tok.token = buffer;
        tok.identifier = "ERRO LEXICO";
        tok.state = new_state;
        int length = strlen(tok.token);
        tok.token[length] = character;
        tok.token[length + 1] = '\0';
        return tok;
    }
    // se mudou de estado e não é um estado final 
    if (is_final_state(current_state) && changed_state(current_state, new_state)){
        // se não é espaço 
        if (!isspace(buffer[0])){
            tok.token = buffer;
            tok.identifier = check_reserved_table(reservedTable,buffer);
            tok.final = true;
        }
        tok.state = END_BUFFER;
        return tok;
    }
    tok.token = buffer;
    tok.identifier = NULL;
    tok.state = new_state;
    
    return tok;
}
