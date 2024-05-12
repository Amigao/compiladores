#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analisador_lexico.h"
#include "hashing.h"

#define TAMANHO_MAXIMO_LINHA 100
#define DELIMITADOR " \t\n"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char buffer[512];
    
    // Constroi tabela reservada
    Tabela TabelaReservada;
    constroi_tabela_reservada(&TabelaReservada);

    char * parameter;

    while(fgets(buffer, sizeof(buffer),file)){
        parameter = strtok(buffer, DELIMITADOR);
        while(parameter != NULL){
            analisador_lexico(parameter, &TabelaReservada);
            parameter = strtok(NULL,DELIMITADOR);
        }
    }

    
    fclose(file);
    liberar_tabela(&TabelaReservada);
    //liberar_tabela(&TabelaSimbolos);
    return 0;
}
