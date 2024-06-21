#ifndef ERRORS_MANAGEMENT_H
#define ERRORS_MANAGEMENT_H

#include "aux_structs.h"

#define ERRO_LEXICO 0
#define ERRO_COMENTARIO_NAO_FECHADO 1
#define ERRO_SINTATICO 2

// Definição da estrutura para um nó da lista ligada
typedef struct ErrorInfo{
    char buffer[MAX_BUF_SIZE];
    int line;
    int type;
    struct ErrorInfo *next;
} ErrorInfo;

// Forward declaration para evitar dependência circular
struct CompilingInfo;
typedef struct CompilingInfo CompilingInfo;

void free_error_list(ErrorInfo *head);

void printErrors(ErrorInfo *head, FILE *output_file);

void insert_error(CompilingInfo *aux, int type, const char *buffer);

#endif 
