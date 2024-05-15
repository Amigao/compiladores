
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
#include "hashing.h"

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
int is_first_double_operator(char c){
    if (c == ':' || c == '!' || c == '<' || c == '>'){
        return 1;
    }
    return 0;
}

int is_second_double_operator(char c){
    if (c == '='){
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

int buffer_is_symbol(int state){
    if (state == 4 || state == 5 || state == 6 || state == 7){
        return 1;
    }
    return 0;
}

int transition(int state, char c) {
    switch (state) {
        case 0:
            if (isupper(c) || islower(c)) return 1;  // letras maiúsculas e minúsculas vão para o estado 1
            if (isdigit(c)) return 2;  // números
            if (isdelimiter(c)) return 3;  // delimitadores
            if (is_first_double_operator(c)) return 4;  // primeiro caractere de um operador duplo
            if (is_single_operator(c)) return 5;  // operador com um caractere
            break;
        case 1:
            if (isupper(c) || islower(c)) return 1;  // letras maiúsculas e minúsculas continuam no estado 1
            break;
    }
    return -1;  // qualquer outra transição leva a um estado de erro
}


int is_final_state(int state){
    if (state == 0 || state == 5){
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
