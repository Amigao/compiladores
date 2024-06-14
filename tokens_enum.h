#ifndef TOKENS_ENUM_H 
#define TOKENS_ENUM_H

typedef enum {
    // Não-terminais
    PROGRAMA,
    BLOCO,
    DECLARACAO,
    CONSTANTE,
    MAIS_CONST,
    VARIAVEL,
    MAIS_VAR,
    PROCEDIMENTO,
    COMANDO,
    MAIS_CMD,
    EXPRESSAO,
    OPERADOR_UNARIO,
    TERMO,
    MAIS_TERMOS,
    FATOR,
    MAIS_FATORES,
    CONDICAO,
    RELACIONAL,

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
    TOKEN_COUNT //contador do número de tokens
    // Adicione outros terminais conforme necessário

} TokenType;



#endif