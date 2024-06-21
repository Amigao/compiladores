#include "sintatic_analyzer.h"

//struct para guardar token/classe
TokenInfo tok;
Table reservedTable;
ErrorInfo *error_list = NULL;

void panic_mode(CompilingInfo* aux, TokenType sync[], int sync_count) {
    int i;
    while (1) {
        //printf("Panic mode: consumindo token %d\n", tok.token_enum); // Monitorando tokens

        // Verifica se o token atual é um token de sincronizaçao
        for (i = 0; i < sync_count; i++) {
            if (tok.token_enum == (int)sync[i] || tok.token_enum == ENDOFFILE) {
                if (tok.token_enum == ENDOFFILE) {
                    printf("\n\nO modo de pânico consumiu ate o final do arquivo :( !!!!\n\n");
                }
                return;  // Encontrou um símbolo de sincronizaçao, sai da funçao
            }
        }

        tok = getNextToken(aux);  // Continua consumindo tokens até encontrar um símbolo de sincronizaçao

    }
}

void sintatic_analyzer(FILE *input_file, FILE *output_file) {
    CompilingInfo aux;
    aux.input_file = input_file;
    aux.error_list = &error_list;
    aux.current_line = 1;

    // Constroi tabela reservada
    build_reserved_table(&aux.reservedTable);

    programa(&aux);

    // Imprime os erros encontrados
    printErrors(error_list, output_file);
    
    // Libera as tabelas e listas utilizadas
    free_error_list(error_list);
    free_table(&reservedTable);
}

void programa(CompilingInfo *aux) {
    tok = getNextToken(aux);
    bloco(aux);
    if (tok.token_enum != PONTO) {
        printf("Erro: '.' esperado no final do programa.\n");
        insert_error(aux, ERRO_SINTATICO, "'.' esperado no final do programa.");
        TokenType sync[] = {PONTO, ENDOFFILE, BEGIN, END};
        panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
        return;
    }
}

void bloco(CompilingInfo *aux) {
    declaracao(aux);
    comando(aux);
}

void declaracao(CompilingInfo *aux) {
    constante(aux);
    variavel(aux);
    procedimento(aux);
}

void constante(CompilingInfo *aux) {
    if (tok.token_enum == CONST) {
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'CONST'.\n");
            insert_error(aux, ERRO_SINTATICO, "Identificador esperado apos 'CONST'.");
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
            printf("Erro: Número esperado apos '='.\n");
            insert_error(aux, ERRO_SINTATICO, "Numero esperado apos '='.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        mais_cont(aux);
        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos declaracao de constante.\n");
            insert_error(aux, ERRO_SINTATICO, "';' esperado apos declaracao de constante.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
    }
}

void mais_cont(CompilingInfo *aux) {
    if (tok.token_enum == VIRGULA) {
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            insert_error(aux, ERRO_SINTATICO, "Identificador esperado apos ','.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        if (tok.token_enum != IGUAL) {
            printf("Erro: '=' esperado apos identificador.\n");
            insert_error(aux, ERRO_SINTATICO, "'=' esperado apos identificador.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        if (tok.token_enum != NUMERO) {
            printf("Erro: Número esperado apos '='.\n");
            insert_error(aux, ERRO_SINTATICO, "Número esperado apos '='.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        mais_cont(aux);
    }
}

void variavel(CompilingInfo *aux) {
    if (tok.token_enum == VAR) {
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'VAR'.\n");
            insert_error(aux, ERRO_SINTATICO, "Identificador esperado apos 'VAR'.");
            TokenType sync[] = {PONTO_E_VIRGULA, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        mais_var(aux);

        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("linha atual: %d\n", aux->current_line);
            printf("Erro: ';' esperado apos declaracao de variavel.\n");
            insert_error(aux, ERRO_SINTATICO, "';' esperado apos declaracao de variavel.");
            TokenType sync[] = {PONTO_E_VIRGULA, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
    }
}

void mais_var(CompilingInfo *aux) {
    if (tok.token_enum == VIRGULA) {
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            insert_error(aux, ERRO_SINTATICO, "Identificador esperado apos ','.");
            TokenType sync[] = {PONTO_E_VIRGULA, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        mais_var(aux);
    }
}

void procedimento(CompilingInfo *aux) {
    if (tok.token_enum == PROCEDURE) {
        tok = getNextToken(aux);
        if (tok.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'PROCEDURE'.\n");
            insert_error(aux, ERRO_SINTATICO, "Identificador esperado apos 'PROCEDURE'.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }
        
        tok = getNextToken(aux);
        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos identificador.\n");
            insert_error(aux, ERRO_SINTATICO, "';' esperado apos identificador.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        bloco(aux);

        if (tok.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos bloco de procedimento.\n");
            insert_error(aux, ERRO_SINTATICO, "';' esperado apos bloco de procedimento.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        tok = getNextToken(aux);
        procedimento(aux);
    }
}

void comando(CompilingInfo *aux) {
    switch (tok.token_enum) {
        case IDENT:
            tok = getNextToken(aux);

            if (tok.token_enum == ATRIBUICAO) {
                tok = getNextToken(aux);
                expressao(aux);
            } else {
                printf("Erro: ':=' esperado apos identificador.\n");
                insert_error(aux, ERRO_SINTATICO, "':=' esperado apos identificador.");
                TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
                panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }
            break;

        case CALL:
            tok = getNextToken(aux);
            if (tok.token_enum != IDENT) {
                printf("Erro: Identificador esperado apos 'CALL'.\n");
                insert_error(aux, ERRO_SINTATICO, "Identificador esperado apos 'CALL'.");
                TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
                panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }
            tok = getNextToken(aux);
            break;

        case BEGIN:
            tok = getNextToken(aux);
            comando(aux);
            mais_comando(aux);

            if (tok.token_enum != END) {
                printf("Erro: 'END' esperado apos bloco de comandos.\n");
                insert_error(aux, ERRO_SINTATICO, "'END' esperado apos bloco de comandos.");
                TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
                panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }

            tok = getNextToken(aux);
            break;

        case IF:
            tok = getNextToken(aux);
            condicao(aux);
        
            if (tok.token_enum != THEN) {
                printf("Erro: 'THEN' esperado.\n");
                insert_error(aux, ERRO_SINTATICO, "'THEN' esperado.");
                TokenType sync[] = {PONTO_E_VIRGULA, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
                panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }
        
            tok = getNextToken(aux);
            comando(aux);
            break;

        case WHILE:
            tok = getNextToken(aux);
            condicao(aux);

            if (tok.token_enum != DO) {
                printf("Erro: 'DO' esperado apos condicao.\n");
                insert_error(aux, ERRO_SINTATICO, "'DO' esperado apos condicao.");
                TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
                panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }

            tok = getNextToken(aux);
            comando(aux);
            break;

        default:
            break;
    }
}

void mais_comando(CompilingInfo *aux) {
    if (tok.token_enum == PONTO_E_VIRGULA) {
        tok = getNextToken(aux);
        comando(aux);
        mais_comando(aux);
    }
}

void condicao(CompilingInfo *aux) {
    expressao(aux);
    relacional(aux);
    expressao(aux);
}

void relacional(CompilingInfo *aux) {
    switch (tok.token_enum) {
        case IGUAL:
        case DIFERENTE:
        case MENOR:
        case MENOR_IGUAL:
        case MAIOR:
        case MAIOR_IGUAL:
            tok = getNextToken(aux);
            break;
        default:
            printf("Erro: Operador relacional esperado.\n");
            insert_error(aux, ERRO_SINTATICO, "Operador relacional esperado.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
    }
}

void expressao(CompilingInfo *aux) {
    termo(aux);
    mais_termos(aux);
}

void mais_termos(CompilingInfo *aux) {
    if (tok.token_enum == SOMA || tok.token_enum == SUBTRACAO) {
        tok = getNextToken(aux);
        termo(aux);
        mais_termos(aux);
    }
}

void termo(CompilingInfo *aux) {
    fator(aux);
    mais_fator(aux);
}

void mais_fator(CompilingInfo *aux) {
    if (tok.token_enum == MULTIPLICACAO || tok.token_enum == DIVISAO) {
        tok = getNextToken(aux);
        fator(aux);
        mais_fator(aux);
    }
}

void fator(CompilingInfo *aux) {
    switch (tok.token_enum) {
        case IDENT:
        case NUMERO:
            tok = getNextToken(aux);
            break;
        case PARENTESE_ESQUERDA:
            tok = getNextToken(aux);
            expressao(aux);
            if (tok.token_enum != PARENTESE_DIREITA) {
                printf("Erro: ')' esperado apos expressao.\n");
                insert_error(aux, ERRO_SINTATICO, "')' esperado apos expressao.");
                TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
                panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }
            tok = getNextToken(aux);
            break;
        default:
            printf("Erro: Fator invalido.\n");
            insert_error(aux, ERRO_SINTATICO, "Fator invalido.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END, ENDOFFILE};
            panic_mode(aux, sync, sizeof(sync)/sizeof(sync[0]));
            return;
    }
}
