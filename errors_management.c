#include "aux_structs.h"

// Função para criar um novo nó da lista ligada
ErrorInfo *create_error_node(int line, int type, const char *buffer) {

    ErrorInfo *new_node = malloc(sizeof(ErrorInfo));
    if (new_node == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o novo nó.\n");
        exit(EXIT_FAILURE);
    }    
    
    strncpy(new_node->buffer, buffer, sizeof(new_node->buffer) - 1);
    new_node->buffer[sizeof(new_node->buffer) - 1] = '\0'; 
    
    new_node->line = line;
    new_node->type = type;
    new_node->next = NULL;
    return new_node;
}

// Função para inserir uma palavra no final da lista ligada
void insert_error(CompilingInfo *aux, int type, const char *buffer) {
    ErrorInfo *new_node = create_error_node(aux->current_line, type, buffer);
    if (*aux->error_list == NULL) {
        *aux->error_list = new_node;
    } else {
        ErrorInfo *current = *aux->error_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Função para liberar a memória alocada para a lista ligada
void free_error_list(ErrorInfo *head) {
    ErrorInfo *current = head;
    while (current != NULL) {
        ErrorInfo *temp = current;
        current = current->next;
        free(temp);
    }
}

// Função para construir o arquivo de saída
// Os decrementos nos contadores de linha para o erro sintático e comentário não fechado são adicionados pois esses erros só são identificados quando o 
// compilador chega na linha seguinte e não encontra o token esperado, então marcaria o erro uma linha abaixo daquela onde o erro de fato está. Isso pode
// gerar imprecisões se variáveis ou constantes forem inicializadas de forma errada com diversas linhas vazias abaixo delas.
void printErrors(ErrorInfo *head, FILE *output_file) {
    ErrorInfo *current = head;
    if (current == NULL) fprintf(output_file, "CÓDIGO COMPILADO COM SUCESSO!");
    while (current != NULL) {
        if (current->type == ERRO_LEXICO) fprintf(output_file, "ERRO LEXICO: Termo \"%s\" mal formado. Linha %d.\n", current->buffer, current->line);
        if (current->type == ERRO_COMENTARIO_NAO_FECHADO) fprintf(output_file, "ERRO: comentario nao fechado encontrado na linha %d: \"%s\"\n", current->line - 1, current->buffer);
        if (current->type == ERRO_SINTATICO) fprintf(output_file, "ERRO SINTATICO: %s Linha %d.\n", current->buffer, current->line - 1);
        current = current->next;        
    }
}
