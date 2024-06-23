#include "sintatic_analyzer.h"

//struct para guardar token/classe
TokenInfo token_info;
FILE *output_file_panic_errors;     // variavel global usada somente quando o modo panico consome todo o arquivo 

void panic_mode(CompilingInfo* comp_info, TokenType sync[], int sync_count) {
    int i;
    while (1) {
        //printf("Panic mode: consumindo token %d\n", token_info.token_enum); // Monitorando tokens

        // Verifica se o token atual é um token de sincronizaçao
        for (i = 0; i < sync_count; i++) {
            if (token_info.token_enum == (int)sync[i] || token_info.token_enum == ENDOFFILE) {
                if (token_info.token_enum == ENDOFFILE) {
                    printf("\n\nO modo de panico consumiu ate o final do arquivo :( !!!!\n\n");
                    printErrors(*comp_info->error_list, output_file_panic_errors);
                    exit(0);
                }
                return;  // Encontrou um símbolo de sincronizaçao, sai da funçao
            }
        }

        token_info = getNextToken(comp_info);  // Continua consumindo tokens até encontrar um símbolo de sincronizaçao

    }
}

void sintatic_analyzer(FILE *input_file, FILE *output_file) {
    output_file_panic_errors = output_file;
    
    ErrorInfo *error_list = NULL;
    CompilingInfo comp_info;
    comp_info.input_file = input_file;
    comp_info.error_list = &error_list;
    comp_info.current_line = 1;

    // Constroi tabela reservada
    build_reserved_table(&comp_info.reservedTable);

    // NÓ RAÍZ
    programa(&comp_info);

    // Imprime os erros encontrados
    printErrors(*comp_info.error_list, output_file);
    
    // Libera as tabelas e listas utilizadas
    free_error_list(*comp_info.error_list);
    free_table(&comp_info.reservedTable);
}

void programa(CompilingInfo *comp_info) {
    token_info = getNextToken(comp_info);
    bloco(comp_info);
    if (token_info.token_enum != PONTO) {
        printf("Erro: '.' esperado no final do programa.\n");
        insert_error(comp_info, ERRO_SINTATICO, "'.' esperado no final do programa.");
        TokenType sync[] = {PONTO, BEGIN, END};
        panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
        return;
    }
}

void bloco(CompilingInfo *comp_info) {
    declaracao(comp_info);
    comando(comp_info);
}

void declaracao(CompilingInfo *comp_info) {
    constante(comp_info);
    variavel(comp_info);
    procedimento(comp_info);
}

void constante(CompilingInfo *comp_info) {
    if (token_info.token_enum == CONST) {
        token_info = getNextToken(comp_info);
        if (token_info.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'CONST'.\n");
            insert_error(comp_info, ERRO_SINTATICO, "Identificador esperado apos 'CONST'.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        if (token_info.token_enum != IGUAL) {
            printf("Erro: '=' esperado apos identificador.\n");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        if (token_info.token_enum != NUMERO) {
            printf("Erro: Numero esperado apos '='.\n");
            insert_error(comp_info, ERRO_SINTATICO, "Numero esperado apos '='.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        mais_const(comp_info);
        if (token_info.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos declaracao de constante.\n");
            insert_error(comp_info, ERRO_SINTATICO, "';' esperado apos declaracao de constante.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
    }
}

void mais_const(CompilingInfo *comp_info) {
    if (token_info.token_enum == VIRGULA) {
        token_info = getNextToken(comp_info);
        if (token_info.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            insert_error(comp_info, ERRO_SINTATICO, "Identificador esperado apos ','.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        if (token_info.token_enum != IGUAL) {
            printf("Erro: '=' esperado apos identificador.\n");
            insert_error(comp_info, ERRO_SINTATICO, "'=' esperado apos identificador.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        if (token_info.token_enum != NUMERO) {
            printf("Erro: Numero esperado apos '='.\n");
            insert_error(comp_info, ERRO_SINTATICO, "Numero esperado apos '='.");
            TokenType sync[] = {PONTO_E_VIRGULA, VAR, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        mais_const(comp_info);
    }
}

void variavel(CompilingInfo *comp_info) {
    if (token_info.token_enum == VAR) {
        token_info = getNextToken(comp_info);
        if (token_info.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'VAR'.\n");
            insert_error(comp_info, ERRO_SINTATICO, "Identificador esperado apos 'VAR'.");
            TokenType sync[] = {PONTO_E_VIRGULA, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        mais_var(comp_info);

        if (token_info.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos declaracao de variavel.\n");
            insert_error(comp_info, ERRO_SINTATICO, "';' esperado apos declaracao de variavel.");
            TokenType sync[] = {PONTO_E_VIRGULA, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
    }
}

void mais_var(CompilingInfo *comp_info) {
    if (token_info.token_enum == VIRGULA) {
        token_info = getNextToken(comp_info);
        if (token_info.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos ','.\n");
            insert_error(comp_info, ERRO_SINTATICO, "Identificador esperado apos ','.");
            TokenType sync[] = {PONTO_E_VIRGULA, PROCEDURE, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        mais_var(comp_info);
    }
}

void procedimento(CompilingInfo *comp_info) {
    if (token_info.token_enum == PROCEDURE) {
        token_info = getNextToken(comp_info);
        if (token_info.token_enum != IDENT) {
            printf("Erro: Identificador esperado apos 'PROCEDURE'.\n");
            insert_error(comp_info, ERRO_SINTATICO, "Identificador esperado apos 'PROCEDURE'.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }
        
        token_info = getNextToken(comp_info);
        if (token_info.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos identificador.\n");
            insert_error(comp_info, ERRO_SINTATICO, "';' esperado apos identificador.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        bloco(comp_info);

        if (token_info.token_enum != PONTO_E_VIRGULA) {
            printf("Erro: ';' esperado apos bloco de procedimento.\n");
            insert_error(comp_info, ERRO_SINTATICO, "';' esperado apos bloco de procedimento.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
        }

        token_info = getNextToken(comp_info);
        procedimento(comp_info);
    }
}

void comando(CompilingInfo *comp_info) {
    switch (token_info.token_enum) {
        case IDENT:
            token_info = getNextToken(comp_info);

            if (token_info.token_enum == ATRIBUICAO) {
                token_info = getNextToken(comp_info);
                expressao(comp_info);
            } else {
                printf("Erro: ':=' esperado apos identificador.\n");
                insert_error(comp_info, ERRO_SINTATICO, "':=' esperado apos identificador.");
                TokenType sync[] = {PONTO_E_VIRGULA, CALL, BEGIN, IF, WHILE, END};
                panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }
            break;

        case CALL:
            token_info = getNextToken(comp_info);
            if (token_info.token_enum != IDENT) {
                printf("Erro: Identificador esperado apos 'CALL'.\n");
                insert_error(comp_info, ERRO_SINTATICO, "Identificador esperado apos 'CALL'.");
                TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END};
                panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }
            token_info = getNextToken(comp_info);
            break;

        case BEGIN:
            token_info = getNextToken(comp_info);
            comando(comp_info);
            mais_comando(comp_info);

            if (token_info.token_enum != END) {
                
                printf("Erro: 'END' esperado apos bloco de comandos.\n");
                insert_error(comp_info, ERRO_SINTATICO, "'END' esperado apos bloco de comandos.");
                TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END};
                panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }

            token_info = getNextToken(comp_info);
            break;

        case IF:
            token_info = getNextToken(comp_info);
            condicao(comp_info);
        
            if (token_info.token_enum != THEN) {
                printf("Erro: 'THEN' esperado.\n");
                insert_error(comp_info, ERRO_SINTATICO, "'THEN' esperado.");
                TokenType sync[] = {PONTO_E_VIRGULA, CALL, BEGIN, IF, WHILE, END};
                panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }

            token_info = getNextToken(comp_info);
            comando(comp_info);
            break;

        case WHILE:
            token_info = getNextToken(comp_info);
            condicao(comp_info);

            if (token_info.token_enum != DO) {
                printf("Erro: 'DO' esperado apos condicao.\n");
                insert_error(comp_info, ERRO_SINTATICO, "'DO' esperado apos condicao.");
                TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END};
                panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }

            token_info = getNextToken(comp_info);
            comando(comp_info);
            break;

        default:
            break;
    }
}

void mais_comando(CompilingInfo *comp_info) {
    switch (token_info.token_enum)
    {
    case IDENT:
    case CALL:
    case BEGIN:
    case WHILE:
    case IF:
        printf("Erro: ';' esperado apos comando.\n");
        insert_error(comp_info, ERRO_SINTATICO, "';' esperado apos comando.");
        comando(comp_info);
        mais_comando(comp_info);
        break;
    case PONTO_E_VIRGULA:
        token_info = getNextToken(comp_info);
        comando(comp_info);
        mais_comando(comp_info);
        break;
    }
}

void condicao(CompilingInfo *comp_info) {
    if (token_info.token_enum == ODD) {
        token_info = getNextToken(comp_info);
        expressao(comp_info);
    } else {
        expressao(comp_info);
        relacional(comp_info);
        expressao(comp_info);
    }
}

void relacional(CompilingInfo *comp_info) {
    switch (token_info.token_enum) {
        case IGUAL:
        case DIFERENTE:
        case MENOR:
        case MENOR_IGUAL:
        case MAIOR:
        case MAIOR_IGUAL:
            token_info = getNextToken(comp_info);
            break;
        default:
            printf("Erro: Operador relacional esperado.\n");
            insert_error(comp_info, ERRO_SINTATICO, "Operador relacional esperado.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
    }
}

void expressao(CompilingInfo *comp_info) {
    operador_unitario(comp_info);
    termo(comp_info);
    mais_termos(comp_info);
}

void operador_unitario(CompilingInfo *comp_info){
    if (token_info.token_enum == SOMA || token_info.token_enum == SUBTRACAO){
        token_info = getNextToken(comp_info);
    }
}

void mais_termos(CompilingInfo *comp_info) {
    if (token_info.token_enum == SOMA || token_info.token_enum == SUBTRACAO) {
        token_info = getNextToken(comp_info);
        termo(comp_info);
        mais_termos(comp_info);
    }
}

void termo(CompilingInfo *comp_info) {
    fator(comp_info);
    mais_fator(comp_info);
}

void mais_fator(CompilingInfo *comp_info) {
    if (token_info.token_enum == MULTIPLICACAO || token_info.token_enum == DIVISAO) {
        token_info = getNextToken(comp_info);
        fator(comp_info);
        mais_fator(comp_info);
    }
}

void fator(CompilingInfo *comp_info) {
    switch (token_info.token_enum) {
        case IDENT:
        case NUMERO:
            token_info = getNextToken(comp_info);
            break;
        case PARENTESE_ESQUERDA:
            token_info = getNextToken(comp_info);
            expressao(comp_info);
            if (token_info.token_enum != PARENTESE_DIREITA) {
                printf("Erro: ')' esperado apos expressao.\n");
                insert_error(comp_info, ERRO_SINTATICO, "')' esperado apos expressao.");
                TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END};
                panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
                return;
            }
            token_info = getNextToken(comp_info);
            break;
        default:
            printf("Erro: Fator invalido.\n");
            insert_error(comp_info, ERRO_SINTATICO, "Fator invalido.");
            TokenType sync[] = {PONTO_E_VIRGULA, IDENT, CALL, BEGIN, IF, WHILE, END};
            panic_mode(comp_info, sync, sizeof(sync)/sizeof(sync[0]));
            return;
    }
}
