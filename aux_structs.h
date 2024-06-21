#ifndef AUX_STRUCTS_H 
#define AUX_STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BUF_SIZE 100

#include "errors_management.h"
#include "hashing.h"

typedef struct CompilingInfo {
    FILE* input_file;
    ErrorInfo **error_list;
    Table reservedTable;
    int current_line;
} CompilingInfo;

typedef struct TokenInfo {
    char identifier[MAX_BUF_SIZE];  
    char token[MAX_BUF_SIZE];
    int token_enum;
    int state;
    bool final;
} TokenInfo;

typedef enum {
    // Terminais
    CONST,
    IDENT,
    NUMERO,
    VIRGULA,
    PONTO_E_VIRGULA,
    IGUAL,
    ATRIBUICAO,
    VAR,
    PROCEDURE,
    CALL,
    BEGIN,
    END,
    IF,
    THEN,
    WHILE,
    DO,
    SUBTRACAO,
    SOMA,
    MULTIPLICACAO,
    DIVISAO,
    PARENTESE_ESQUERDA,
    PARENTESE_DIREITA,
    ODD,
    DIFERENTE,
    MENOR,
    MENOR_IGUAL,
    MAIOR,
    MAIOR_IGUAL,
    PONTO,
    ENDOFFILE,
    TOKEN_COUNT, //contador do número de tokens
    // Adicione outros terminais conforme necessário
} TokenType;

#endif
