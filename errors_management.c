#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors_management.h"

char *my_strdup(const char *src) {
    size_t len = strlen(src) + 1; // +1 para o caractere nulo
    char *dst = malloc(len);
    if (dst == NULL) {
        fprintf(stderr, "Erro ao alocar memória para duplicar a string.");
        exit(EXIT_FAILURE);
    }
    strcpy(dst, src);
    return dst;
}

// Função para criar um novo nó da lista ligada
errors *create_node(char *word, char *token, int line) {
    errors *new_node = malloc(sizeof(errors));
    if (new_node == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o novo nó.\n");
        exit(EXIT_FAILURE);
    }
    new_node->word = my_strdup(word);
    new_node->token = my_strdup(token);
    new_node->line = line;
    new_node->next = NULL;
    return new_node;
}

// Função para inserir uma palavra no final da lista ligada
void insert_error(errors **head, char *word, char *token, int line) {
    errors *new_node = create_node(word, token, line);
    if (*head == NULL) {
        *head = new_node;
    } else {
        errors *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Função para liberar a memória alocada para a lista ligada
void free_error_list(errors *head) {
    errors *current = head;
    while (current != NULL) {
        errors *temp = current;
        current = current->next;
        free(temp->word);
        free(temp);
    }
}
