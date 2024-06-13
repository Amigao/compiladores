#ifndef ERRORS_MANAGEMENT_H
#define ERRORS_MANAGEMENT_H

#define ERRO_LEXICO 0
#define ERRO_COMENTARIO 1

// Definição da estrutura para um nó da lista ligada
typedef struct ErrorInfo{
    char *word;
    int line;
    int type;
    struct ErrorInfo *next;
} ErrorInfo;

void free_error_list(ErrorInfo *head);

void printErrors(ErrorInfo *head);

void insert_error(ErrorInfo **head, char *word, int line, int type);

#endif 
