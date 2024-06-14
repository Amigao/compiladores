#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"

const char* TokenTypeStrings[] = {
    // Não-terminais
    "PROGRAMA",
    "BLOCO",
    "DECLARACAO",
    "CONSTANTE",
    "MAIS_CONST",
    "VARIAVEL",
    "MAIS_VAR",
    "PROCEDIMENTO",
    "COMANDO",
    "MAIS_CMD",
    "EXPRESSAO",
    "OPERADOR_UNARIO",
    "TERMO",
    "MAIS_TERMOS",
    "FATOR",
    "MAIS_FATORES",
    "CONDICAO",
    "RELACIONAL",

    // Terminais
    "CONST",
    "IDENT",
    "NUMERO",
    "VIRGULA",
    "PONTO_E_VIRGULA",
    "IGUAL",
    "ATRIBUICAO",
    "VAR",
    "PROCEDURE",
    "CALL",
    "BEGIN",
    "END",
    "IF",
    "THEN",
    "WHILE",
    "DO",
    "SUBTRACAO",
    "SOMA",
    "MULTIPLICACAO",
    "DIVISAO",
    "PARENTESE_ESQUERDA",
    "PARENTESE_DIREITA",
    "ODD",
    "DIFERENTE",
    "MENOR",
    "MENOR_IGUAL",
    "MAIOR",
    "MAIOR_IGUAL",
    "PONTO"
};

// Funcao para checar se o token esta na tabela de palavras e simbolos reservados
void check_reserved_table(Table *table, TokenInfo *tok) {
    char *result = search_table(table, tok->token);
    if (result != NULL) {
        tok->identifier = result;
        // Atribuir o enum correspondente ao token encontrado
        for (int i = 0; i < TOKEN_COUNT; i++) {
            if (strcmp(result, TokenTypeStrings[i]) == 0) {
                tok->token_enum = (TokenType)i;
                break;
            }
        }
    } else {
        tok->identifier = "IDENT";
        tok->token_enum = IDENT;
    }
}

// Adiciona palavras e simbolos reservados na tabela
void build_reserved_table(Table *table){
    initialize_table(table);
    insert_table(table, "CONST",  "CONST");
    insert_table(table, "VAR", "VAR");
    insert_table(table, "PROCEDURE", "PROCEDURE");
    insert_table(table, "BEGIN", "BEGIN");
    insert_table(table, "END", "END");
    insert_table(table, "CALL", "CALL");
    insert_table(table, "IF", "IF");
    insert_table(table, "THEN", "THEN");
    insert_table(table, "WHILE", "WHILE");
    insert_table(table, "DO", "DO");
    insert_table(table, "ODD", "ODD");
    insert_table(table, "+", "SOMA");
    insert_table(table, "-", "SUBTRACAO");
    insert_table(table, "*", "MULTIPLICACAO");
    insert_table(table, "/", "DIVISAO");
    insert_table(table, "=", "IGUAL");
    insert_table(table, "<>", "DIFERENTE");
    insert_table(table, "<", "MENOR");
    insert_table(table, "<=", "MENOR_IGUAL");
    insert_table(table, ">", "MAIOR");
    insert_table(table, ">=", "MAIOR_IGUAL");
    insert_table(table, ":=", "ATRIBUICAO");
    insert_table(table, "(", "PARENTESE_ESQUERDA");
    insert_table(table, ")", "PARENTESE_DIREITA");
    insert_table(table, ",", "VIRGULA");
    insert_table(table, ";", "PONTO_E_VIRGULA");
    insert_table(table, ".", "PONTO");
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
        if (current_state == 2) {
            tok.identifier = my_strdup("NUMERO");
            tok.token_enum = NUMERO; 
        }
        else if (current_state == -1) tok.identifier = my_strdup("ERRO LEXICO");
        else check_reserved_table(reservedTable, &tok);
        
    }

    // retorna o par token/classe
    return tok;
}

TokenInfo getNextToken(FILE* input_file, FILE *output_file, ErrorInfo *error_list, Table reservedTable){
    TokenInfo tok;

    // Estado inicial do automato
    int current_state = INITIAL_STATE;

    // Variaveis para percorrer o arquivo e guardar token/classe 
    char c;
    char buffer[MAX_BUF_SIZE];
    int i = 0;
    buffer[0] = '\0';
    int number_of_lines = 1; 

    // Enquanto nao acabar o arquivo
    while ((c = fgetc(input_file)) != EOF) {

        // contador de linhas
        if (c == '\n') {
            number_of_lines++;
        }

        // chegou no espaço ou \n indica, que acabou a token
        if (c == ' ' || c == '\n') {

            if(tok.state == 10){
                if(c == ' ') {
                    buffer[i] = c;
                    buffer[i + 1] = '\0';
                    current_state = tok.state;
                    i++;
                    continue;
                } else {
                    insert_error(&error_list, tok.token, number_of_lines, ERRO_COMENTARIO_NAO_FECHADO);
                    //volta para o estado incial e reseta as Variaveis
                    current_state = INITIAL_STATE;
                    i = 0;
                    buffer[i] = '\0';
                    tok.final = false;
                    continue;
                }
            }
            
            if(tok.final){
                if (tok.state == -1){
                    insert_error(&error_list, tok.token, number_of_lines, ERRO_LEXICO);
                }
                // imprime no arquivo de saida o par token/identificador
                fprintf(output_file, "%s, %s\n", tok.token, tok.identifier);
                return tok;
                //volta para o estado incial e reseta as Variaveis
                current_state = INITIAL_STATE;
                i = 0;
                buffer[i] = '\0';
                tok.final = false;
            }

        } else {

            // chama o lexico para cada caracter
            tok = lexical_analyzer(c, buffer, &reservedTable, current_state);
            
            // se entrar no estado de comentario
            if(tok.state == 11){
                buffer[i] = c;
                buffer[i+1] = '\0';
                // imprime o comentario que foi resetado
                printf("\nCOMENTARIO IGNORADO: %s\n", buffer);
                // reseta as variaveis
                current_state = INITIAL_STATE;
                i = 0;
                buffer[i] = '\0';
                tok.final = false;
            }
            
            // Se entrou no estado de retroceder
            else if (tok.state == RETURN_STATE) {
                if (current_state == -1){
                    insert_error(&error_list, tok.token, number_of_lines, ERRO_LEXICO);
                }
                //adiciona ao arquivo de saida
                fprintf(output_file, "%s, %s\n", tok.token, tok.identifier);
                return tok;
                
                // devolve o caractere pra cadeia de entrada
                ungetc(c, input_file);
                // reseta as variaveis
                current_state = INITIAL_STATE;
                i = 0;
                buffer[i] = '\0';

            } else { // se nao, continua lendo e adicionando no buffer
                buffer[i] = c;
                buffer[i + 1] = '\0';
                current_state = tok.state;
                i++;
            }
        }
    }
}