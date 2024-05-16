#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
#include "errors_management.h"
#include "hashing.h"

#define MAX_BUF_SIZE 100

int main(int argc, char *argv[]) {
    // Recebe o arquivo de entrada
    if (argc != 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    // Abre arquivo de input para leitura e arquivo de output para escrita
    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen("output.txt", "w");
    // Check files
    if (output_file == NULL || input_file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    
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

        // chegou no espaço ou \n indica que acabou a token
        if (isspace(c) || c == '\n') {
            if(tok.state == 10 && isspace(c)){
                buffer[i] = c;
                buffer[i + 1] = '\0';
                current_state = tok.state;
                i++;
                continue;
            }
           if(tok.final){
                // imprime no arquivo de saida o par token/identificador
                fprintf(output_file, "%s, %s\n", tok.token, tok.identifier);
                //volta para o estado incial e reseta as Variaveis
                current_state = INITIAL_STATE;
                i = 0;
                buffer[i] = '\0';
                tok.final = false;
            }
        }else{
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
            
            // Se chegou ao final do buffer
            else if (tok.state == END_BUFFER) {
                //adiciona ao arquivo de saida
                fprintf(output_file, "%s, %s\n", tok.token, tok.identifier);
                ungetc(c, input_file);
                // reseta as variaveis
                current_state = INITIAL_STATE;
                i = 0;
                buffer[i] = '\0';

            } else if (tok.state == ERROR) { // Caso tenha entrado no estado de erro
                fprintf(output_file, "%s, %s\n", tok.token, tok.identifier);
                // insere na lista de erro e reseta as variaveis
                insert_error(&error_list, tok.token, number_of_lines, ERRO_LEXICO);
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

    // fecha os arquivos
    fclose(input_file);
    fclose(output_file);

    return 0;
}
