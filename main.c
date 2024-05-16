#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexical_analyzer.h"
#include "errors_management.h"
#include "hashing.h"

#define MAX_BUF_SIZE 100

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL || input_file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    
    // Constroi tabela reservada
    Table reservedTable;
    build_reserved_table(&reservedTable);
   
    // Constroi lista de erros
    ErrorInfo *error_list = NULL;

    int current_state = INITIAL_STATE;
    char c;
    char buffer[MAX_BUF_SIZE];
    int i = 0;
    buffer[0] = '\0';
    int number_of_lines = 1; 
    TokenInfo tok;
    tok.line = 1; 

    while ((c = fgetc(input_file)) != EOF) {
        if (c == '\n') {
            number_of_lines++;
            tok.line++;
        }

        if (isspace(c) || c == '\n') {
            if(tok.final){
                fprintf(output_file, "%s, %s\n", tok.token, tok.identifier);
                printf("TOKEN FINAL E IDENTIFICADOR: %s, %s\n", tok.token, tok.identifier);
                current_state = INITIAL_STATE;
                i = 0;
                buffer[i] = '\0';
            }
        } 
        
        else {
            
            tok = lexical_analyzer(c, buffer, &reservedTable, current_state);

            if (tok.state == END_BUFFER) {
                fprintf(output_file, "%s, %s\n", tok.token, tok.identifier);
                ungetc(c, input_file);
                current_state = INITIAL_STATE;
                i = 0;
                buffer[i] = '\0';

            } else if (tok.state == ERROR) {
                current_state = INITIAL_STATE;
                i = 0;
                buffer[i] = '\0';
                fprintf(output_file, "%s, %s\n", tok.token, tok.identifier);

            } else {
                buffer[i] = c;
                buffer[i + 1] = '\0';
                current_state = tok.state;
                i++;
            }
        }
    }

    printErrors(error_list);
    
    free_error_list(error_list);
    free_table(&reservedTable);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
