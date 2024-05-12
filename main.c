#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analisador_lexico.h"
#include "hashing.h"

#define TAMANHO_MAXIMO_LINHA 256
#define DELIMITADOR " \t\n"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        fclose(output_file);
        return 1;
    }

    char buffer[TAMANHO_MAXIMO_LINHA];
    
    // Constroi tabela reservada
    Tabela TabelaReservada;
    constroi_tabela_reservada(&TabelaReservada);

    char * parameter;

    while(fgets(buffer, sizeof(buffer), input_file)){
        parameter = strtok(buffer, DELIMITADOR);
        while(parameter != NULL){
            analisador_lexico(parameter, &TabelaReservada, output_file);
            parameter = strtok(NULL,DELIMITADOR);
        }
    }
    
    fclose(input_file);
    fclose(output_file);
    liberar_tabela(&TabelaReservada);
    return 0;
}
