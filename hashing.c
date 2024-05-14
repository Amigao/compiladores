#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashing.h"


// Função de hash -> valores ASCII das primeiras duas letras da palavra e duas ultimas letras da palavra
int hash_function(char *word) {
    int hash = 0;
    if(strlen(word) > 2){
        //Duas primeiras letras
        hash |= word[0];
        hash |= word[1] << 8;

        //Duas últimas letras
        int len = strlen(word);
        hash |= word[len - 2] << 16;
        hash |= word[len - 1] << 24;
    }    // Caso de strings de tamanho unitário ou de dois caracteres
    if(strlen(word) == 1) {
        // Apenas um caractere
        hash |= word[0];
    } else {
        // Dois caracteres
        hash |= word[0];
        hash |= word[1] << 8;
    }

    return hash % TAMANHO_TABELA;
}

// Função para inicializar a tabela de hash
void inicializa_tabela(Tabela *tabela) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabela->table[i] = NULL;
    }
}

char *my_strdup(const char *src) {
    size_t len = strlen(src) + 1; // +1 para o caractere nulo
    char *dst = malloc(len);
    if (dst == NULL) {
        fprintf(stderr, "Erro ao alocar memória para duplicar a string.");
        exit(EXIT_FAILURE);
    }
    strcpy(dst, src);
    return dst;
}

void insere_tabela(Tabela *tabela, char *word, char *token){
    int index = hash_function(word);

    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL){
        fprintf(stderr, "Erro ao alocar memória para nó.");
        exit(EXIT_FAILURE);
    }

    new_node->word = my_strdup(word);
    new_node->token = my_strdup(token);

    new_node->next = NULL;

    if(tabela->table[index] == NULL){
        tabela->table[index] = new_node;
    } else {
        Node *current = tabela->table[index];
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_node;
    }
}

// Função de busca
char *busca_tabela(Tabela *tabela, char *word){
    int index = hash_function(word);

    Node *current = tabela->table[index];
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return current->token; //Encontrado
        }
        current = current->next;
    }
    return NULL; //Não encontrado
}

void liberar_tabela(Tabela *tabela) {
    for (int i = 0; i < TAMANHO_TABELA; i++){
        Node *current = tabela->table[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp->word);
            free(temp);
        }        
    }
}
