#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analisador_lexico.h"
#include "hashing.h"

#define TAMANHO_MAXIMO_LINHA 100

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

    char c;
    char buffer[TAMANHO_MAXIMO_LINHA];
    int i = 0;
    
    // Constroi tabela reservada
    Tabela TabelaReservada;
    constroi_tabela_reservada(&TabelaReservada);

    while((c = fgetc(file)) != EOF){
        if ((c != '\n') & (c != ' ') & (c != ',')) {
            buffer[i++] = c;
        } else {
            buffer[i++] = '\0';
            i = 0;
            // printf("Palavra lida: %s\n", buffer);
            printf("PALAVRA: %s; TOKEN: %s\n", buffer, analisador_lexico(buffer, &TabelaReservada));
        }
    }
    buffer[i++] = '\0';
    // printf("Palavra lida: %s\n", buffer);
    printf("PALAVRA: %s; TOKEN: %s\n", buffer, analisador_lexico(buffer, &TabelaReservada));
    
    fclose(file);
    return 0;
}
