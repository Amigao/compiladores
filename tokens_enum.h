#ifndef TOKENS_ENUM_H 
#define TOKENS_ENUM_H

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
