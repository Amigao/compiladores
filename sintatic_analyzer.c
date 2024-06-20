#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
#include "sintatic_analyzer.h"

//struct para guardar token/classe
TokenInfo tok;
Table reservedTable;
ErrorInfo *error_list = NULL;


void panic_mode(FILE *input_file, TokenType sync){
    while(tok.token_enum != (int)sync && tok.token_enum != ENDOFFILE){
        tok = getNextToken(input_file, &error_list, reservedTable);
    }
    if(tok.token_enum != ENDOFFILE){
        tok = getNextToken(input_file, &error_list, reservedTable);
    }
    else{
        printf("\n\nO modo de panico consumiu ate o final do arquivo :( \n\n");
        exit(0);
    }
}


void sintatic_analyzer(FILE *input_file, FILE *output_file){
    // Constroi tabela reservada
    build_reserved_table(&reservedTable);

    programa(input_file);

    // imprime os erros encontrados
    printErrors(error_list, output_file);
    
    // libera as tabelas e listas utilizadas
    free_error_list(error_list);
    free_table(&reservedTable);

}

void programa(FILE* input_file){
    tok = getNextToken(input_file, &error_list, reservedTable);
    bloco(input_file);
    if (tok.token_enum != PONTO) {
        // Erro: token inesperado
        printf("Erro: '.' esperado no final do programa.\n");
        insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: '.' esperado no final do programa.");
        panic_mode(input_file, PONTO);
        return;
    }
}

void bloco(FILE* input_file){
    declaracao(input_file);
    comando(input_file);
}

void declaracao(FILE* input_file){
    constante(input_file);
    variavel(input_file);
    procedimento(input_file);
}

void constante(FILE* input_file){
    if (tok.token_enum == CONST) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'CONST'.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: Identificador esperado apos 'CONST'.");
            panic_mode(input_file, PONTO_E_VIRGULA );
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != IGUAL) {
            printf("Erro: '=' esperado apos identificador.\n");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != NUMERO) {
            printf("Erro: Numero esperado apos '='.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO:  Numero esperado apos '='.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
        mais_cont(input_file);
        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos declaracao de constante.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO:  ';' esperado apos declaracao de constante.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
    }
}

void mais_cont(FILE* input_file){
    if(tok.token_enum == VIRGULA){
        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: Identificador esperado apos ','.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != IGUAL) {
            printf("Erro: '=' esperado apos identificador.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: '=' esperado apos identificador.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != NUMERO) {
            printf("Erro: Numero esperado apos '='.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: Numero esperado apos '='.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }


        tok = getNextToken(input_file, &error_list, reservedTable);
        mais_cont(input_file);
    }
}

void variavel(FILE* input_file){
    if (tok.token_enum == VAR) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'VAR'.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: Identificador esperado apos 'VAR'.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
        mais_var(input_file);

        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos declaracao de variavel.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: ';' esperado apos declaracao de variavel.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
    }
}

void mais_var(FILE* input_file){
    if(tok.token_enum == VIRGULA){
        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: Identificador esperado apos ','.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
        mais_var(input_file);
    }
}


void procedimento(FILE* input_file){
    if(tok.token_enum == PROCEDURE){
        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'PROCEDURE'.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: Identificador esperado apos 'PROCEDURE'.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }
        
        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos identificador.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: ';' esperado apos identificador.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
        bloco(input_file);


        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos bloco de procedimento.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: ';' esperado apos bloco de procedimento.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }

        tok = getNextToken(input_file, &error_list, reservedTable);
        procedimento(input_file);
    }
}

void comando(FILE* input_file){
    if (tok.token_enum == IDENT) {
        tok = getNextToken(input_file, &error_list, reservedTable);

        if (tok.token_enum == ATRIBUICAO) {
            tok = getNextToken(input_file, &error_list, reservedTable);
            expressao(input_file);
        } else {
            printf("Erro: ':=' esperado apos identificador.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: ':=' esperado apos identificador.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }
    } else if (tok.token_enum == CALL) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'CALL'.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: identificador esperado apos 'CALL'.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }
        tok = getNextToken(input_file, &error_list, reservedTable);
    } else if (tok.token_enum == BEGIN) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        comando(input_file);
        
        mais_comando(input_file);
        if (tok.token_enum != END) {
            printf("Erro: 'END' esperado.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: 'END' esperado para finalizar 'BEGIN'.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }
        tok = getNextToken(input_file, &error_list, reservedTable);
    } else if (tok.token_enum == IF) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        condicao(input_file);
        if (tok.token_enum != THEN) {
            printf("Erro: 'THEN' esperado.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: 'THEN' esperado.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }
        tok = getNextToken(input_file, &error_list, reservedTable);
        comando(input_file);
    } else if (tok.token_enum == WHILE) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        condicao(input_file);
        if (tok.token_enum != DO) {
            printf("Erro: 'DO' esperado.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: 'DO' esperado.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }
        tok = getNextToken(input_file, &error_list, reservedTable);
        comando(input_file);
    }
}

void mais_comando(FILE* input_file){
    if (tok.token_enum == PONTO_E_VIRGULA){
        tok = getNextToken(input_file, &error_list, reservedTable);
        comando(input_file);
        mais_comando(input_file);
    }
}

void expressao(FILE* input_file){
    operador_unitario(input_file);
    termo(input_file);
    mais_termos(input_file);
}

void operador_unitario(FILE* input_file){
    if (tok.token_enum == SOMA || tok.token_enum == SUBTRACAO) {
        tok = getNextToken(input_file, &error_list, reservedTable);
    }
}

void termo(FILE* input_file){
    fator(input_file);
    mais_fatores(input_file);
}

void mais_termos(FILE* input_file){
    if (tok.token_enum == SOMA || tok.token_enum == SUBTRACAO) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        termo(input_file);
        mais_termos(input_file);
    }
}

void fator(FILE* input_file){
    if (tok.token_enum == IDENT || tok.token_enum == NUMERO) {
        tok = getNextToken(input_file, &error_list, reservedTable);

    } else if (tok.token_enum == PARENTESE_ESQUERDA) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        expressao(input_file);
        if (tok.token_enum != PARENTESE_DIREITA) {
            printf("Erro: ')' esperado.\n");
            insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: ')' esperado.");
            panic_mode(input_file, PONTO_E_VIRGULA);
            return;
        }
        tok = getNextToken(input_file, &error_list, reservedTable);

    } else {
        printf("Erro: Fator invalido.\n");
        insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: fator invalido.");
        panic_mode(input_file, PONTO_E_VIRGULA);
        return;
    }
}

void mais_fatores(FILE* input_file){
    if (tok.token_enum == MULTIPLICACAO || tok.token_enum == DIVISAO) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        fator(input_file);
        mais_fatores(input_file);
    }
}

void condicao(FILE* input_file){
    if (tok.token_enum == ODD) {
        tok = getNextToken(input_file, &error_list, reservedTable);
        expressao(input_file);
    } else {
        expressao(input_file);
        relacional(input_file);
        expressao(input_file);
    }
}

void relacional(FILE* input_file){
    if (tok.token_enum == IGUAL || tok.token_enum == DIFERENTE
                                || tok.token_enum == MENOR || tok.token_enum == MENOR_IGUAL
                                || tok.token_enum == MAIOR || tok.token_enum == MAIOR_IGUAL) {
        tok = getNextToken(input_file, &error_list, reservedTable);
    } else {
        printf("Erro: Operador relacional esperado.\n");
        insert_error(&error_list, tok.token_line, ERRO_SINTATICO, "ERRO: operador relacional esperado.");
        panic_mode(input_file, PONTO_E_VIRGULA);
        return;
    }
}
