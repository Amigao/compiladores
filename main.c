#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "analisador_lexico.h"
#include "hashing.h"

#define TAMANHO_MAXIMO_BUFFER 100

int isSymbol(char c){
    char simbolos_especiais[12] = "+-*/()=<>:;.";
    for (size_t j = 0; j < strlen(simbolos_especiais); j++) {
        if (c == simbolos_especiais[j]) {
            return 1; 
        }
    }
    return 0;
}

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

    
    // Constroi tabela reservada
    Tabela TabelaReservada;
    constroi_tabela_reservada(&TabelaReservada);
   
    int current_state = 0;
    int new_state;
    char c;
    char buffer[TAMANHO_MAXIMO_BUFFER];
    // leitura até fim do arquivo PL/0 
    int i = 0; 
    while((c = fgetc(file)) != EOF){
        new_state = analisador_lexico(c, buffer, &TabelaReservada, current_state); 
        if (new_state == CHANGED_STATE) {
            ungetc(c, file);
            current_state = 0; 
            i = 0;
        } else {
            buffer[i] = c;
            buffer[i+1] = '\0';
            current_state = new_state;
            i++;
        }
    }

    fclose(file);
    return 0;
}
