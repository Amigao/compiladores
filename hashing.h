#ifndef HASHING_H
#define HASHING_H

#define TAMANHO_TABELA 100


// Definição dos nós da tabela
typedef struct {
    char *word;
    struct Node *next;
} Node;

// Definição das tabelas (de simbolos e palavras reservadas e de simbolos e palavras)
typedef struct {
    Node *table[TAMANHO_TABELA]
} Tabela;


int hash_function(char *word);

void inicializa_tabela(Tabela *tabela, char *word);

int busca_tabela(Tabela *tabela, char *word);

void liberar_tabela(Tabela *tabela);

#endif