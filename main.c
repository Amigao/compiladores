#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analisador_lexico.h"

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
    while((c = fgetc(file)) != EOF){
        if ((c != '\n') & (c != ' ') & (c != ',')) {
            buffer[i++] = c;
        } else {
            buffer[i++] = '\0';
            i = 0;
            // printf("Palavra lida: %s\n", buffer);
            printf("PALAVRA: %s; TOKEN: %s\n", buffer, analisador_lexico(buffer));
        }
    }
    buffer[i++] = '\0';
    // printf("Palavra lida: %s\n", buffer);
    printf("PALAVRA: %s; TOKEN: %s\n", buffer, analisador_lexico(buffer));
    
    fclose(file);
    return 0;
}