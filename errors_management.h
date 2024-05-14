#ifndef ERRORS_MANAGEMENT_H
#define ERRORS_MANAGEMENT_H

// Definição da estrutura para um nó da lista ligada
typedef struct Errors{
    char *word;
    char *token;
    int line;
    struct Errors *next;
} Errors;

void free_error_list(Errors *head);
void insert_error(Errors **head, char *word, char *token, int line);
Errors *create_node(char *word, char *token, int line);

#endif 
