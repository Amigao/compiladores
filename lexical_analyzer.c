
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
#include "hashing.h"

// Funcao para checar se o buffer esta na tabela de palavras e simbolos reservados
char *check_reserved_table(Table *table, char *string){
    char *result = search_table(table, string);
    if (result != NULL) {
        return result;
    } else {
        return "ident";
    }
}

// Adiciona palavras e simbolos reservados na tabela
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

#include <stdbool.h> // Incluindo a biblioteca para usar o tipo de dado booleano

/* Funcoes auxiliares para conferir o estado do automato */
bool is_first_double_operator(char c) {
    return (c == ':' || c == '!' || c == '<' || c == '>');
}

bool is_second_double_operator(char c) {
    return (c == '=');
}

bool is_single_operator(char c) {
    return (c == '=' || c == '+' || c == '-' || c == '*' || c == '/');
}

bool is_delimiter(char c) {
    return (c == ',' || c == ';' || c == '.');
}

bool is_valid_symbol(char c) {
    return (is_second_double_operator(c) || is_first_double_operator(c) || is_single_operator(c) || is_delimiter(c) || c == '{');
}



// Funcao de transicao de estados do automato
int transition(int state, char c) {

    switch (state) {
        case 0:
            if (isalpha(c)) return 1;  // letras maiúsculas e minúsculas vão para o estado 1
            if (isdigit(c)) return 2;  // números
            if (is_first_double_operator(c)) return 3;  // primeiro caractere de um operador duplo
            if (is_delimiter(c)) return 4;  // delimitadores
            if (is_single_operator(c)) return 6;  // operador com um caractere
            if (c == '{') return 10;  // comentario
            break;
        case 1:
            if (isalpha(c) || isdigit(c)) return 1;  // letras maiúsculas e minúsculas continuam no estado 1
            if (is_valid_symbol(c)) return 7;
            break;
        case 2:
            if (isalpha(c)) return -1;
            if (isdigit(c)) return 2;
            if (is_valid_symbol(c)) return 7;
            break;
        case 3:
            if(is_second_double_operator(c)) return 5;
            else return 7;
            break;
        case 4:
        case 5:
        case 6:
            return 7;
            break;  
        case 10:// Se entrar no estado de comentario, continua ate achar o simbolo de encerrar comentario
            if(c == '}') return 11;
            return 10;
            break;
        case 11:
            break;
        case -1:
            if(is_valid_symbol(c)) return 7;
            break;
    }
    return -1;  // qualquer outra transição leva a um estado de erro
}

bool is_final_state(int state){
    return !(state == 0 || state == 3 || state == 11);
}

bool changed_state(int state_a, int state_b){
    return (state_a != state_b);
}

// Funcao que sera chamada pelo sintatico
TokenInfo lexical_analyzer(char character, char *buffer, Table* reservedTable, int current_state){
    TokenInfo tok;
    tok.final = false;
    // Faz a transicao no automato baseado no caracter de entrada
    int new_state = transition(current_state, character);


    // Representa o estado de retroceder no automato
    if (new_state == RETURN_STATE){
        tok.token = buffer;
        int length = strlen(tok.token);
        tok.token[length] = '\0';

        // Confere se eh um numero, erro ou se esta na tabela de palavras e simbolos reservados
        if (current_state == 2) tok.identifier = my_strdup("number");
        else if (current_state == -1) tok.identifier = my_strdup("ERRO LEXICO");
        else tok.identifier = check_reserved_table(reservedTable,tok.token);

        // retorna que chegou ao final do buffer
        tok.final = true;
        tok.state = RETURN_STATE;
        // retorna o par token/classe
        return tok;
    }

    //se esta em um possivel estado final
    else if (is_final_state(new_state)){
        tok.token = buffer;
        int length = strlen(tok.token);
        tok.token[length] = character;
        tok.token[length + 1] = '\0';

        tok.state = new_state;
        // Confere se eh um numero, erro ou se esta na tabela de palavras e simbolos reservados
        if (current_state == 2) tok.identifier = my_strdup("number");
        else if (current_state == -1) tok.identifier = my_strdup("ERRO LEXICO");
        else tok.identifier = check_reserved_table(reservedTable,tok.token);
        tok.final = true;
        // retorna o par token/classe
        return tok;
    }

    // Se não é um estado final, atualiza o buffer e o estado
    tok.token = buffer;
    tok.identifier = NULL;
    tok.state = new_state;

    return tok;
}
