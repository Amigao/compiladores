#include "sintatic_analyzer.h"

//struct para guardar token/classe
TokenInfo tok;
Table reservedTable;
ErrorInfo *error_list = NULL;


void panic_mode(CompilingInfo* aux, TokenType sync[], int sync_count) {
    int i;
    while (1) {
        //printf("Panic mode: consumindo token %d\n", tok.token_enum); // Monitorando tokens

        // Verifica se o token atual é um token de sincronização
        for (i = 0; i < sync_count; i++) {
            if (tok.token_enum == (int)sync[i] || tok.token_enum == ENDOFFILE) {
                if (tok.token_enum == ENDOFFILE) {
                    printf("\n\nO modo de pânico consumiu até o final do arquivo :( !!!!\n\n");
                    exit(-1);
                }
                return;  // Encontrou um símbolo de sincronização, sai da função
            }
        }

        tok = getNextToken(aux);  // Continua consumindo tokens até encontrar um símbolo de sincronização
    }
}

void sintatic_analyzer(FILE *input_file, FILE *output_file){
    CompilingInfo aux;
    aux.input_file = input_file;
    aux.error_list = &error_list;
    aux.current_line = 1;

    // Constroi tabela reservada
    build_reserved_table(&aux.reservedTable);

    programa(&aux);

    // imprime os erros encontrados
    printErrors(error_list, output_file);
    
    // libera as tabelas e listas utilizadas
    free_error_list(error_list);
    free_table(&reservedTable);

}

void programa(CompilingInfo *aux){
    tok = getNextToken(aux);
    bloco(aux);
    if (tok.token_enum != PONTO) {
        // Erro: token inesperado
        printf("Erro: '.' esperado no final do programa.\n");
        insert_error(aux, ERRO_SINTATICO, "ERRO: '.' esperado no final do programa.");
        TokenType sync[] = {PONTO, ENDOFFILE, BEGIN, END};
        panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
        return;
    }
}

void bloco(CompilingInfo *aux) {
    declaracao(aux);
    comando(aux);
    if (tok.token_enum != PONTO_E_VIRGULA && tok.token_enum != ENDOFFILE) {
        printf("Erro: ';' esperado apos comando.\n");
        insert_error(aux, ERRO_SINTATICO, "ERRO: ';' esperado apos comando.");
        TokenType sync[] = {PONTO_E_VIRGULA, BEGIN, END, ENDOFFILE};
        panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
        return;
    }
}

void declaracao(CompilingInfo *aux){
    constante(aux);
    variavel(aux);
    procedimento(aux);
}

void constante(CompilingInfo *aux){
    if (tok.token_enum == CONST) {
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'CONST'.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: Identificador esperado apos 'CONST'.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        if (tok.token_enum != IGUAL) {
            printf("Erro: '=' esperado apos identificador.\n");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        if (tok.token_enum != NUMERO) {
            printf("Erro: Numero esperado apos '='.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO:  Numero esperado apos '='.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        mais_cont(aux);
        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos declaracao de constante.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO:  ';' esperado apos declaracao de constante.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
    }
}

void mais_cont(CompilingInfo *aux){
    if(tok.token_enum == VIRGULA){
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: Identificador esperado apos ','.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        if (tok.token_enum != IGUAL) {
            printf("Erro: '=' esperado apos identificador.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: '=' esperado apos identificador.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        if (tok.token_enum != NUMERO) {
            printf("Erro: Numero esperado apos '='.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: Numero esperado apos '='.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }


        tok = getNextToken(aux);
        mais_cont(aux);
    }
}

void variavel(CompilingInfo *aux){
    if (tok.token_enum == VAR) {
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'VAR'.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: Identificador esperado apos 'VAR'.");
            TokenType sync[] = {PONTO_E_VIRGULA, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        mais_var(aux);

        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos declaracao de variavel.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: ';' esperado apos declaracao de variavel.");
            TokenType sync[] = {PONTO_E_VIRGULA, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
    }
}

void mais_var(CompilingInfo *aux){
    if(tok.token_enum == VIRGULA){
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: Identificador esperado apos ','.");
            TokenType sync[] = {PONTO_E_VIRGULA, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        mais_var(aux);
    }
}


void procedimento(CompilingInfo *aux){
    if(tok.token_enum == PROCEDURE){
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'PROCEDURE'.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: Identificador esperado apos 'PROCEDURE'.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }
        
        tok = getNextToken(aux);
        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos identificador.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: ';' esperado apos identificador.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        bloco(aux);


        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos bloco de procedimento.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: ';' esperado apos bloco de procedimento.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        procedimento(aux);
    }
}


void comando(CompilingInfo *aux) {
    if (tok.token_enum == IDENT) {
        tok = getNextToken(aux);

        if (tok.token_enum == ATRIBUICAO) {
            tok = getNextToken(aux);
            expressao(aux);
        } else {
            printf("Erro: ':=' esperado apos identificador.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: ':=' esperado apos identificador.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }
    } else if (tok.token_enum == CALL) {
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'CALL'.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: identificador esperado apos 'CALL'.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }
        tok = getNextToken(aux);
    } else if (tok.token_enum == BEGIN) {
        tok = getNextToken(aux);
        comando(aux);

        mais_comando(aux);
        if (tok.token_enum != END) {
            printf("Erro: 'END' esperado.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: 'END' esperado para finalizar 'BEGIN'.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }
        tok = getNextToken(aux);
    } else if (tok.token_enum == IF) {
        tok = getNextToken(aux);
        condicao(aux);
            printf("Erro: 'THEN' esperado.\n");
        if (tok.token_enum != THEN) {
            printf("Erro: 'THEN' esperado.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: 'THEN' esperado.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }
        tok = getNextToken(aux);
        comando(aux);
    } else if (tok.token_enum == WHILE) {
        tok = getNextToken(aux);
        condicao(aux);
        if (tok.token_enum != DO) {
            printf("Erro: 'DO' esperado.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: 'DO' esperado.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }
        tok = getNextToken(aux);
        comando(aux);
    }
}

void mais_comando(CompilingInfo *aux){
    if (tok.token_enum == PONTO_E_VIRGULA){
        tok = getNextToken(aux);
        comando(aux);
        mais_comando(aux);
    }
}

void expressao(CompilingInfo *aux){
    operador_unitario(aux);
    termo(aux);
    mais_termos(aux);
}

void operador_unitario(CompilingInfo *aux){
    if (tok.token_enum == SOMA || tok.token_enum == SUBTRACAO) {
        tok = getNextToken(aux);
    }
}

void termo(CompilingInfo *aux){
    fator(aux);
    mais_fatores(aux);
}

void mais_termos(CompilingInfo *aux){
    if (tok.token_enum == SOMA || tok.token_enum == SUBTRACAO) {
        tok = getNextToken(aux);
        termo(aux);
        mais_termos(aux);
    }
}

void fator(CompilingInfo *aux){
    if (tok.token_enum == IDENT || tok.token_enum == NUMERO) {
        tok = getNextToken(aux);

    } else if (tok.token_enum == PARENTESE_ESQUERDA) {
        tok = getNextToken(aux);
        expressao(aux);
        if (tok.token_enum != PARENTESE_DIREITA) {
            printf("Erro: ')' esperado.\n");
            insert_error(aux, ERRO_SINTATICO, "ERRO: ')' esperado.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }
        tok = getNextToken(aux);

    } else {
        printf("Erro: Fator invalido.\n");
        insert_error(aux, ERRO_SINTATICO, "ERRO: fator invalido.");
        TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
        panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
        return;
    }
}

void mais_fatores(CompilingInfo *aux){
    if (tok.token_enum == MULTIPLICACAO || tok.token_enum == DIVISAO) {
        tok = getNextToken(aux);
        fator(aux);
        mais_fatores(aux);
    }
}

void condicao(CompilingInfo *aux){
    if (tok.token_enum == ODD) {
        tok = getNextToken(aux);
        expressao(aux);
    } else {
        expressao(aux);
        relacional(aux);
        expressao(aux);
    }
}

void relacional(CompilingInfo *aux){
    if (tok.token_enum == IGUAL || tok.token_enum == DIFERENTE
                                || tok.token_enum == MENOR || tok.token_enum == MENOR_IGUAL
                                || tok.token_enum == MAIOR || tok.token_enum == MAIOR_IGUAL) {
        tok = getNextToken(aux);
    } else {
        printf("Erro: Operador relacional esperado.\n");
        insert_error(aux, ERRO_SINTATICO, "ERRO: operador relacional esperado.");
        TokenType sync[] = {IDENT, NUMERO, PARENTESE_ESQUERDA, ODD, DIFERENTE, MENOR, MENOR_IGUAL, MAIOR, MAIOR_IGUAL, ENDOFFILE};
        panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
        return;
    }
}
