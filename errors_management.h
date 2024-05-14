#ifndef ERRORS_MANAGEMENT_H
#define ERRORS_MANAGEMENT_H

// Definição da estrutura para um nó da lista ligada
typedef struct errors {
    char *word;
    char *token;
    int line;
    struct errors *next;
} errors;

void free_error_list(errors *head);
void insert_error(errors **head, char *word, char *token, int line);
errors *create_node(char *word, char *token, int line);

#endif 
