#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashing.h"


int hash_function(char *word) {
    int hash = 0;
    int len = strlen(word);

    if(len > 2) {
        // Duas primeiras letras
        hash |= word[0];
        hash |= word[1] << 8;

        // Duas últimas letras
        hash |= word[len - 2] << 16;
        hash |= word[len - 1] << 24;
    } else if(len == 2) {
        // Dois caracteres
        hash |= word[0];
        hash |= word[1] << 8;
    } else if(len == 1) {
        // Apenas um caractere
        hash |= word[0];
    }

    return hash % TABLE_SIZE;
}

// Função para inicializar a tabela hash
void initialize_table(Table *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->table[i] = NULL;
    }
}

// Função para duplicar uma string
char *my_strdup(const char *src) {
    size_t len = strlen(src) + 1; 
    char *dst = malloc(len); 
    strcpy(dst, src); 
    return dst; 
}

// Função para inserir um par palavra-token na tabela hash
void insert_table(Table *table, char *word, char *token, int token_enum){
    int index = hash_function(word);

    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL){
        fprintf(stderr, "Erro ao alocar memória para nó.");
        exit(EXIT_FAILURE);
    }

    new_node->word = my_strdup(word);
    new_node->token = my_strdup(token);
    new_node->token_enum = token_enum;

    new_node->next = NULL;

    if(table->table[index] == NULL){
        table->table[index] = new_node;
    } else { 
        Node *current = table->table[index];
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_node;
    }
}

// Função de busca
char *search_table(Table *table, char *word, int *token_enum){
    int index = hash_function(word); 

    Node *current = table->table[index]; 
    while (current != NULL) { 
        if (strcmp(current->word, word) == 0) { 
            *token_enum = current->token_enum;
            return current->token; 
        }
        current = current->next;
    }
    return NULL; 
}

// Função para liberar a memória alocada para a tabela hash e seus nós
void free_table(Table *table) {
    for (int i = 0; i < TABLE_SIZE; i++){ 
        Node *current = table->table[i];
        while (current != NULL) { 
            Node *temp = current; 
            current = current->next; 
            free(temp->word); 
            free(temp->token); 
            free(temp); 
        }        
    }
}
