#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "analisador_lexico.h"
#include "hashing.h"

#define TAMANHO_MAXIMO_LINHA 100

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
    
    int i = 0;
    char c;
    char buffer[TAMANHO_MAXIMO_LINHA];
    // leitura até fim do arquivo PL/0 
    while((c = fgetc(file)) != EOF){
        // leitura de caracteres do alfabeto
        if (isalpha(c)){
            while (isalnum(c)){
                buffer[i++] = c;   
                c = fgetc(file);
            }
            ungetc(c, file);
            buffer[i++] = '\0';
            i = 0; 
            printf("%s, %s\n", buffer, analisador_lexico(buffer, &TabelaReservada));
            continue;
        } 

        if (isdigit(c)){
            while (isdigit(c)){
                buffer[i++] = c;   
                c = fgetc(file);   
            }
            ungetc(c, file);
            buffer[i++] = '\0';
            i = 0; 
            printf("%s, %s\n", buffer, analisador_lexico(buffer, &TabelaReservada));
            continue; 
        } 
   
        if (isSymbol(c)){
            while (isSymbol(c)){
                buffer[i++] = c;
                c = fgetc(file);
            }
            ungetc(c, file); 
            buffer[i++] = '\0';
            i = 0; 
            printf("%s, %s\n", buffer, analisador_lexico(buffer, &TabelaReservada));
            continue; 
        }

        
    }

    fclose(file);
    return 0;
}
