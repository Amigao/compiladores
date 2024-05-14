#ifndef HASHING_H
#define HASHING_H

#define TAMANHO_TABELA 100

#include <stdbool.h>

// Definição dos nós da tabela
typedef struct Node {
    char *word;
    char *token;
    struct Node *next;
} Node;

// Definição das tabelas (de simbolos e palavras reservadas e de simbolos e palavras)
typedef struct {
    Node *table[TAMANHO_TABELA];
} Tabela;

typedef struct lexico{
    char *token;
    char *identficador;
    int state;
    int line;
    bool final;
}lexico;

int hash_function(char *word);

void inicializa_tabela(Tabela *tabela);

void insere_tabela(Tabela *tabela, char *word, char *token);

char *busca_tabela(Tabela *tabela, char *word);

void liberar_tabela(Tabela *tabela);

#endif
