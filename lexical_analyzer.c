#include "lexical_analyzer.h"

// Funcao para checar se o token esta na tabela de palavras e simbolos reservados
void check_reserved_table(Table *table, TokenInfo *tok) {
    char *result = search_table(table, tok->token, &tok->token_enum);
    if (result != NULL) {
        strncpy(tok->identifier, result, sizeof(tok->identifier) - 1);
        tok->identifier[sizeof(tok->identifier) - 1] = '\0'; 

    } else {
        strncpy(tok->identifier, "ident", sizeof(tok->identifier) - 1);
        tok->identifier[sizeof(tok->identifier) - 1] = '\0'; 

        tok->token_enum = IDENT;
    }
}

// Adiciona palavras e simbolos reservados na tabela
void build_reserved_table(Table *table){
    initialize_table(table);
    insert_table(table, "CONST", "CONST", CONST);
    insert_table(table, "VAR", "VAR", VAR);
    insert_table(table, "PROCEDURE", "PROCEDURE", PROCEDURE);
    insert_table(table, "BEGIN", "BEGIN", BEGIN);
    insert_table(table, "END", "END", END);
    insert_table(table, "CALL", "CALL", CALL);
    insert_table(table, "IF", "IF", IF);
    insert_table(table, "THEN", "THEN", THEN);
    insert_table(table, "WHILE", "WHILE", WHILE);
    insert_table(table, "DO", "DO", DO);
    insert_table(table, "ODD", "ODD", ODD);
    insert_table(table, "+", "SOMA", SOMA);
    insert_table(table, "-", "SUBTRACAO", SUBTRACAO);
    insert_table(table, "*", "MULTIPLICACAO", MULTIPLICACAO);
    insert_table(table, "/", "DIVISAO", DIVISAO);
    insert_table(table, "=", "IGUAL", IGUAL);
    insert_table(table, "<>", "DIFERENTE", DIFERENTE);
    insert_table(table, "<", "MENOR", MENOR);
    insert_table(table, "<=", "MENOR_IGUAL", MENOR_IGUAL);
    insert_table(table, ">", "MAIOR", MAIOR);
    insert_table(table, ">=", "MAIOR_IGUAL", MAIOR_IGUAL);
    insert_table(table, ":=", "ATRIBUICAO", ATRIBUICAO);
    insert_table(table, "(", "PARENTESE_ESQUERDA", PARENTESE_ESQUERDA);
    insert_table(table, ")", "PARENTESE_DIREITA", PARENTESE_DIREITA);
    insert_table(table, ",", "VIRGULA", VIRGULA);
    insert_table(table, ";", "PONTO_E_VIRGULA", PONTO_E_VIRGULA);
    insert_table(table, ".", "PONTO", PONTO);
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
            else return 10;
            break;


        // Estado de erro
        case -1:
            if(is_valid_symbol(c)) return 7;
            break;
    }
    return -1;  // qualquer outra transição leva a um estado de erro
}

bool is_final_state(int state){
    return !(state == 0 || state == 3 || state == 10);
}

// Função para analisar o componente léxico da entrada
TokenInfo lexical_analyzer(char character, char *buffer, Table* reservedTable, int current_state) {
    TokenInfo token_info;
    token_info.final = false;

    // Transição para o novo estado baseado no caractere de entrada
    int new_state = transition(current_state, character);
    token_info.state = new_state;


    // Copia o conteúdo do buffer para token_info.token de forma segura
    strncpy(token_info.token, buffer, sizeof(token_info.token) - 1);
    token_info.token[sizeof(token_info.token) - 1] = '\0';

    // Se estiver em um possível estado final
    if (is_final_state(new_state)) {
        int length = strlen(token_info.token);
        if (new_state == RETURN_STATE) { // Se o estado for de retorno, não adiciona o caractere ao token
            token_info.token[length] = '\0';
        } else {
            token_info.token[length] = character;
            token_info.token[length + 1] = '\0';
        }

        token_info.final = true;

        // Verifica se é um número, erro ou se está na tabela de palavras reservadas
        if (current_state == 2 || token_info.state == 2) {
            strncpy(token_info.identifier, "numero", sizeof(token_info.identifier) - 1);
            token_info.identifier[sizeof(token_info.identifier) - 1] = '\0';
            token_info.token_enum = NUMERO;
        } else if (token_info.state == -1) {
            strncpy(token_info.identifier, "ERRO LÉXICO", sizeof(token_info.identifier) - 1);
            token_info.identifier[sizeof(token_info.identifier) - 1] = '\0';
        } else {
            check_reserved_table(reservedTable, &token_info);
        }
    } else {
        // Se não estiver em um estado final, acumula no buffer
        int length = strlen(token_info.token);
        token_info.token[length] = character;
        token_info.token[length + 1] = '\0';
    }

    // Retorna o par token/classe
    return token_info;
}

// Função para obter o próximo token da entrada
TokenInfo getNextToken(CompilingInfo *comp_info) {
    TokenInfo token_info;
    memset(&token_info, 0, sizeof(TokenInfo));

    // Estado inicial do autômato
    int current_state = INITIAL_STATE;

    // Variáveis para percorrer o arquivo e armazenar token/classe
    char character;
    char buffer[MAX_BUF_SIZE];
    int buffer_index = 0;
    buffer[0] = '\0';

    // Enquanto não for o fim do arquivo
    while ((character = fgetc(comp_info->input_file)) != EOF) {
        // Contador de linhas
        if (character == '\n') {
            comp_info->current_line++;
        }

        // Se espaço ou nova linha indica fim do token
        if (character == ' ' || character == '\n') {
            if (token_info.state == 10) {
                if (character == ' ') {
                    buffer[buffer_index] = character;
                    buffer[buffer_index + 1] = '\0';
                    current_state = token_info.state;
                    buffer_index++;
                    continue;
                } else {
                    insert_error(comp_info, ERRO_COMENTARIO_NAO_FECHADO, token_info.token);
                    // Reseta o estado e as variáveis
                    current_state = INITIAL_STATE;
                    token_info.state = INITIAL_STATE;
                    buffer_index = 0;
                    buffer[buffer_index] = '\0';
                    token_info.final = false;
                    continue;
                }
            }

            if (token_info.final) {
                if (token_info.state == -1) {
                    insert_error(comp_info, ERRO_LEXICO, token_info.token);
                    token_info.token_enum = IDENT;
                }
                // Reseta o estado e as variáveis
                current_state = INITIAL_STATE;
                buffer_index = 0;
                buffer[buffer_index] = '\0';
                token_info.final = false;
                return token_info;
            }
        } else {
            
            // Chama o analisador léxico para cada caractere
            token_info = lexical_analyzer(character, buffer, &comp_info->reservedTable, current_state);

            // Se entrou no estado de comentário
            if (token_info.state == 11) {
                buffer[buffer_index] = character;
                buffer[buffer_index + 1] = '\0';
                // Reseta o estado e as variáveis
                current_state = INITIAL_STATE;
                buffer_index = 0;
                buffer[buffer_index] = '\0';
                token_info.final = false;
            }
            // Se entrou no estado de retorno
            else if (token_info.state == RETURN_STATE) {
                if (current_state == -1) {
                    insert_error(comp_info, ERRO_LEXICO, token_info.token);
                    token_info.token_enum = IDENT;
                }
                // Devolve o caractere para a cadeia de entrada
                ungetc(character, comp_info->input_file);
                // Reseta o estado e as variáveis
                current_state = INITIAL_STATE;
                buffer_index = 0;
                buffer[buffer_index] = '\0';
                return token_info;
            } else { // Caso contrário, continua lendo e adicionando no buffer
                buffer[buffer_index] = character;
                buffer[buffer_index + 1] = '\0';
                current_state = token_info.state;
                buffer_index++;
            }
        }
    }

    // Se o buffer estiver vazio, retorna token EOF
    if (buffer_index == 0) {
        strncpy(token_info.token, "EOF", sizeof(token_info.token) - 1);
        token_info.token[sizeof(token_info.token) - 1] = '\0';
        token_info.token_enum = ENDOFFILE;
    }

    return token_info;
}