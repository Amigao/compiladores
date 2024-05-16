#include <stdio.h>
#include <stdlib.h>

#include "sintatic_analyzer.h"

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

    // chama o analisador sintatico
    sintatic_analyzer(input_file, output_file);

    // fecha os arquivos
    fclose(input_file);
    fclose(output_file);

    return 0;
}
