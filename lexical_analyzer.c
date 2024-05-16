
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
#include "hashing.h"

// Funcao para checar se o token esta na tabela de palavras e simbolos reservados
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

// Funcoes auxiliares para conferir o estado do automato 
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

        // Se entrar no estado de comentario, continua ate achar o simbolo de encerrar comentario
        case 10:
            if(c == '}') return 11;
            return 10;
            break;
        case 11:
            break;
        
        // Estado de erro
        case -1:
            if(is_valid_symbol(c)) return 7;
            break;
    }
    return -1;  // qualquer outra transição leva a um estado de erro
}

bool is_final_state(int state){
    return !(state == 0 || state == 3 || state == 11);
}

// Funcao que sera chamada pelo sintatico
TokenInfo lexical_analyzer(char character, char *buffer, Table* reservedTable, int current_state){
    TokenInfo tok;
    tok.final = false;

    // Faz a transicao no automato baseado no caracter de entrada
    int new_state = transition(current_state, character);
    tok.state = new_state;
    tok.token = buffer;

    //se esta em um possivel estado final
    if (is_final_state(new_state)){

        int length = strlen(tok.token);
        if(new_state == RETURN_STATE){ // se for estado de retroceder, não adiciona o caracter da cadeia no token lido
            tok.token[length] = '\0';
        } else {
            tok.token[length] = character;
            tok.token[length + 1] = '\0';
        }

        tok.final = true;
        
        // Confere se eh um numero, erro ou se esta na tabela de palavras e simbolos reservados
        if (current_state == 2) tok.identifier = my_strdup("number");
        else if (current_state == -1) tok.identifier = my_strdup("ERRO LEXICO");
        else tok.identifier = check_reserved_table(reservedTable,tok.token);
        
    }

    // retorna o par token/classe
    return tok;
}
