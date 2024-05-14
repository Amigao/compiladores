#ifndef ERRORS_MANAGEMENT_H
#define ERRORS_MANAGEMENT_H

// Definição da estrutura para um nó da lista ligada
typedef struct Node {
    char *word;
    char *token;
    int line;
    struct Node *next;
} Node;

void free_error_list(Node *head);
void insert_error(Node **head, char *word, char *token, int line);
Node *create_node(char *word, char *token, int line);

#endif 
