#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors_management.h"
#include "hashing.h"

// Função para criar um novo nó da lista ligada
ErrorInfo *create_error_node(char *word, int line, int type, char *message) {
    ErrorInfo *new_node = malloc(sizeof(ErrorInfo));
    if (new_node == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o novo nó.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(word, new_node->word);
    sprintf(message, new_node->message);
    new_node->line = line;
    new_node->type = type;
    new_node->next = NULL;
    return new_node;
}

// Função para inserir uma palavra no final da lista ligada
void insert_error(ErrorInfo **head, char *word, int line, int type, char *message) {
    ErrorInfo *new_node = create_error_node(word, line, type, message);
    if (*head == NULL) {
        *head = new_node;
    } else {
        ErrorInfo *current = *head;
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
        free(temp->word);
        free(temp);
    }
}

// Função para liberar a memória alocada para a lista ligada
void printErrors(ErrorInfo *head, FILE *output_file) {
    printf("\n\n");
    ErrorInfo *current = head;
    while (current != NULL) {
        if (current->type == ERRO_LEXICO) fprintf(output_file, "ERRO: erro LEXICO encontrado na linha %d. Termo \"%s\" mal formado.\n", current->line, current->word);
        if (current->type == ERRO_COMENTARIO_NAO_FECHADO) fprintf(output_file, "ERRO: comentario nao fechado encontrado na linha %d. Termo \"%s\" mal formado.\n", current->line, current->word);
        if (current->type == ERRO_SINTATICO) fprintf(output_file, "%s. Linha %d.\n", current->message);
        current = current->next;        
    }
    printf("\n\n");
}
