#include <stdio.h>
#include <stdlib.h>


#include "sintatic_analyzer.h"

int main() {

    // Recebe um aqruivi de entrada do usuario
    char user_input[256];
    printf("\nDigite o nome do arquivo de entrada\n");
    scanf("%s", user_input);
    // Abre arquivo de input para leitura e arquivo de output para escrita
    FILE *input_file = fopen(user_input, "r");
    FILE *output_file = fopen("output.txt", "w");
    // Check files
    if (output_file == NULL) {
        printf("Erro ao abrir o arquivo de saida.\n");
        return 1;
    }
    if (input_file == NULL) {
        printf("Erro ao abrir o arquivo de entrada. Certifique-se de adiciona-lo ao comando.\n");
        return 1;
    }

    // chama o analisador sintatico
    sintatic_analyzer(input_file, output_file);

    // fecha os arquivos
    fclose(input_file);
    fclose(output_file);

    return 0;
}
