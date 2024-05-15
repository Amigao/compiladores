#ifndef HASHING_H
#define HASHING_H

#define TABLE_SIZE 100

#include <stdbool.h>

// Definição dos nós da table
typedef struct Node {
    char *word;
    char *token;
    struct Node *next;
} Node;

// Definição das tables (de simbolos e palavras reservadas e de simbolos e palavras)
typedef struct {
    Node *table[TABLE_SIZE];
} Table;

int hash_function(char *word);

void initialize_table(Table *table);

void insert_table(Table *table, char *word, char *token);

char *search_table(Table *table, char *word);

void free_table(Table *table);

char *my_strdup(const char *src);

#endif
