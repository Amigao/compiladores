#ifndef ERRORS_MANAGEMENT_H
#define ERRORS_MANAGEMENT_H

#define ERRO_LEXICO 0
#define ERRO_COMENTARIO_NAO_FECHADO 1
#define ERRO_SINTATICO 2

// Definição da estrutura para um nó da lista ligada
typedef struct ErrorInfo{
    char buffer[256];
    int line;
    int type;
    struct ErrorInfo *next;
} ErrorInfo;

void free_error_list(ErrorInfo *head);

void printErrors(ErrorInfo *head, FILE *output_file);

void insert_error(ErrorInfo **head, int line, int type, const char *buffer);

#endif 
