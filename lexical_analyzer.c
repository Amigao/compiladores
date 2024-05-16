
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
#include "hashing.h"

char *check_reserved_table(Table *table, char *string){
    if (isdigit(string[0])){
        return "number";
    }
    char *result = search_table(table, string);
    if (result != NULL) {
        return result;
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

int is_valid_symbol(char c){
    if (c == '=' || c == '+' || c == '-' || c == '*' || c == '/' || c == ',' || c == ';' || c == '.' || c == ':' || c == '!' || c == '<' || c == '>'){
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
    if (state == 3 || state == 5 || state == 6 || state == 7){
        return 1;
    }
    return 0;
}

int transition(int state, char c) {

    switch (state) {
        case 0:
            if (isalpha(c)) return 1;  // letras maiúsculas e minúsculas vão para o estado 1
            if (isdigit(c)) return 2;  // números
            if (isdelimiter(c)) return 3;  // delimitadores
            if (is_first_double_operator(c)) return 4;  // primeiro caractere de um operador duplo
            if (is_single_operator(c)) return 5;  // operador com um caractere
            break;
        case 1:
            if (isupper(c) || islower(c) || isdigit(c)) return 1;  // letras maiúsculas e minúsculas continuam no estado 1
            if (is_valid_symbol(c)) return 6;
            break;
        case 2:
            if (isalpha(c)) return -1;
            if (isdigit(c)) return 2;
            if (is_valid_symbol(c)) return 6;
            break;
        case 3:
            if(isalpha(c) || isdigit(c)) return 6;
            break;
        case 4:
            if(is_second_double_operator(c)) return 5;
            else return 6;
            break;
        case 5:
            if(isalpha(c) || isdigit(c)) return 6;
            break;
        case 7:
            if(isalpha(c) || isdigit(c)) return 6;
            break;  
    }
    return -1;  // qualquer outra transição leva a um estado de erro
}


int is_final_state(int state){
    return (state == 0 || state == 4) ? 0 : 1;
}

int changed_state(int state_a, int state_b){
    return (state_a != state_b) ? 1 : 0;
}

TokenInfo lexical_analyzer(char character, char *buffer, Table* reservedTable, int current_state){
    TokenInfo tok;
    tok.final = false;
    printf("estado atual: %d\n", current_state);
    int new_state = transition(current_state, character);
    printf("novo atual: %d\n", new_state);
    if (new_state == -1){
        tok.token = buffer;
        tok.identifier = "ERRO LEXICO";
        tok.state = new_state;
        int length = strlen(tok.token);
        tok.token[length] = character;
        tok.token[length + 1] = '\0';
        return tok;
    }

    else if (new_state == 6){
        tok.token = buffer;
        int length = strlen(tok.token);
        tok.token[length] = '\0';
        tok.identifier = check_reserved_table(reservedTable,tok.token);
        tok.final = true;
        tok.state = END_BUFFER;
        return tok;
    }

    else if (is_final_state(new_state)){
        tok.state = new_state;
        tok.token = buffer;
        int length = strlen(tok.token);
        tok.token[length] = character;
        tok.token[length + 1] = '\0';
        printf("token: %s\n", tok.token);
        tok.identifier = check_reserved_table(reservedTable,tok.token);
        tok.final = true;
        return tok;
    }

    // Se não é um estado final, atualiza o buffer e o estado
    tok.token = buffer;
    tok.identifier = NULL;
    tok.state = new_state;

    return tok;
}
