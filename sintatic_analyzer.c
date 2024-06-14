#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
#include "sintatic_analyzer.h"

//struct para guardar token/classe
TokenInfo tok;
Table reservedTable;
ErrorInfo *error_list;


void sintatic_analyzer(FILE *input_file, FILE *output_file){
    // Constroi tabela reservada
    build_reserved_table(&reservedTable);

    // Constroi lista de erros
    error_list = NULL;

    programa(input_file, output_file);

    // imprime os erros encontrados
    printErrors(error_list);
    
    // libera as tabelas e listas utilizadas
    free_error_list(error_list);
    free_table(&reservedTable);

}

void programa(FILE* input_file, FILE *output_file){
    tok = getNextToken(input_file, output_file, error_list, reservedTable);
    bloco(input_file, output_file);
    if (tok.token_enum != PONTO) {
        // Erro: token inesperado
        printf("Erro: '.' esperado no final do programa.\n");
        exit(1);
    }
}

void bloco(FILE* input_file, FILE *output_file){
    declaracao(input_file, output_file);
    comando(input_file, output_file);
}

void declaracao(FILE* input_file, FILE *output_file){
    constante(input_file, output_file);
    variavel(input_file, output_file);
    procedimento(input_file, output_file);
}

void constante(FILE* input_file, FILE *output_file){
    if (tok.token_enum == CONST) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'CONST'.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != IGUAL) {
            printf("Erro: '=' esperado apos identificador.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != NUMERO) {
            printf("Erro: Numero esperado apos '='.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        mais_cont(input_file, output_file);
        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos declaracao de constante.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
    }
}

void mais_cont(FILE* input_file, FILE *output_file){
    if(tok.token_enum == VIRGULA){
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != IGUAL) {
            printf("Erro: '=' esperado apos identificador.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != NUMERO) {
            printf("Erro: Numero esperado apos '='.\n");
            exit(1);
        }


        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        mais_cont(input_file, output_file);
    }
}

void variavel(FILE* input_file, FILE *output_file){
    if (tok.token_enum == VAR) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'VAR'.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        mais_var(input_file, output_file);

        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos declaracao de variavel.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
    }
}

void mais_var(FILE* input_file, FILE *output_file){
    if(tok.token_enum == VIRGULA){
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        mais_var(input_file, output_file);
    }
}


void procedimento(FILE* input_file, FILE *output_file){
    if(tok.token_enum == PROCEDURE){
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'PROCEDURE'.\n");
            exit(1);
        }
        
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos identificador.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        bloco(input_file, output_file);

        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos bloco de procedimento.\n");
            exit(1);
        }

        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        procedimento(input_file, output_file);
    }
}

void comando(FILE* input_file, FILE *output_file){
    if (tok.token_enum == IDENT) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum == ATRIBUICAO) {
            tok = getNextToken(input_file, output_file, error_list, reservedTable);
            expressao(input_file, output_file);
        } else {
            printf("Erro: ':=' esperado apos identificador.\n");
            exit(1);
        }
    } else if (tok.token_enum == CALL) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'CALL'.\n");
            exit(1);
        }
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
    } else if (tok.token_enum == BEGIN) {
        do {
            tok = getNextToken(input_file, output_file, error_list, reservedTable);
            comando(input_file, output_file);
        } while (tok.token_enum == PONTO_E_VIRGULA);
        if (tok.token_enum != END) {
            printf("Erro: 'END' esperado.\n");
            exit(1);
        }
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
    } else if (tok.token_enum == IF) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        condicao(input_file, output_file);
        if (tok.token_enum != THEN) {
            printf("Erro: 'THEN' esperado.\n");
            exit(1);
        }
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        comando(input_file, output_file);
    } else if (tok.token_enum == WHILE) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        condicao(input_file, output_file);
        if (tok.token_enum != DO) {
            printf("Erro: 'DO' esperado.\n");
            exit(1);
        }
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        comando(input_file, output_file);
    }
}


void expressao(FILE* input_file, FILE *output_file){
    operador_unitario(input_file, output_file);
    termo(input_file, output_file);
    mais_termos(input_file, output_file);
}

void operador_unitario(FILE* input_file, FILE *output_file){
    if (tok.token_enum == SOMA || tok.token_enum == SUBTRACAO) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
    }
}

void termo(FILE* input_file, FILE* output_file){
    fator(input_file, output_file);
    mais_fatores(input_file, output_file);
}

void mais_termos(FILE* input_file, FILE *output_file){
    if (tok.token_enum == SOMA || tok.token_enum == SUBTRACAO) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        termo(input_file, output_file);
        mais_termos(input_file, output_file);
    }
}

void fator(FILE* input_file, FILE* output_file){
    if (tok.token_enum == IDENT || tok.token_enum == NUMERO) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);

    } else if (tok.token_enum == PARENTESE_ESQUERDA) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        expressao(input_file, output_file);
        if (tok.token_enum != PARENTESE_DIREITA) {
            printf("Erro: ')' esperado.\n");
            exit(1);
        }
        tok = getNextToken(input_file, output_file, error_list, reservedTable);

    } else {
        printf("Erro: Fator inválido.\n");
        exit(1);
    }
}

void mais_fatores(FILE* input_file, FILE* output_file){
    if (tok.token_enum == MULTIPLICACAO || tok.token_enum == DIVISAO) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        fator(input_file, output_file);
        mais_fatores(input_file, output_file);
    }
}

void condicao(FILE* input_file, FILE* output_file){
    if (tok.token_enum == ODD) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
        expressao(input_file, output_file);
    } else {
        expressao(input_file, output_file);
        relacional(input_file, output_file);
        expressao(input_file, output_file);
    }
}

void relacional(FILE* input_file, FILE* output_file){
    if (tok.token_enum == IGUAL || tok.token_enum == DIFERENTE
                                || tok.token_enum == MENOR || tok.token_enum == MENOR_IGUAL
                                || tok.token_enum == MAIOR || tok.token_enum == MAIOR_IGUAL) {
        tok = getNextToken(input_file, output_file, error_list, reservedTable);
    } else {
        printf("Erro: Operador relacional esperado.\n");
        exit(1);
    }
}
