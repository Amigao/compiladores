#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
#include "errors_management.h"
#include "hashing.h"

#define MAX_BUF_SIZE 100

void sintatic_analyzer(FILE *input_file, FILE *output_file){
    // Constroi tabela reservada
    Table reservedTable;
    build_reserved_table(&reservedTable);
   
    // Constroi lista de erros
    ErrorInfo *error_list = NULL;

    // Estado inicial do automato
    int current_state = INITIAL_STATE;

    // Variaveis para percorrer o arquivo e guardar token/classe 
    char c;
    char buffer[MAX_BUF_SIZE];
    int i = 0;
    buffer[0] = '\0';
    int number_of_lines = 1; 

    //struct para guardar token/classe
    TokenInfo tok;

    // Enquanto nao acabar o arquivo
    while ((c = fgetc(input_file)) != EOF) {
        // contador de linhas
        if (c == '\n') {
            number_of_lines++;
        }

        // chegou no espaço ou \n indica, que acabou a token
        if (isspace(c) || c == '\n') {
            if(tok.state == 10 && isspace(c)){
                buffer[i] = c;
                buffer[i + 1] = '\0';
                current_state = tok.state;
                i++;
                continue;
            }
            
            if(tok.final){
                if (tok.state == -1){
                    insert_error(&error_list, tok.token, number_of_lines, ERRO_LEXICO);
                }
                // imprime no arquivo de saida o par token/identificador
                fprintf(output_file, "%s, %s\n", tok.token, tok.identifier);
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

    // imprime os erros encontrados
    printErrors(error_list);
    
    // libera as tabelas e listas utilizadas
    free_error_list(error_list);
    free_table(&reservedTable);

}

