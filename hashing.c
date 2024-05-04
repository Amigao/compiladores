#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashing.h"


// Função de hash -> valores ASCII das primeiras duas letras da palavra e duas ultimas letras da palavra
int hash_function(char *word) {
    int hash = 0;

    //Duas primeiras letras
    hash |= word[0];
    hash |= word[1] << 8;

    //Duas últimas letras
    int len = strlen(word);
    hash |= word[len - 2] << 16;
    hash |= word[len - 1] << 24;

    return hash % TAMANHO_TABELA;
}

// Função para inicializar a tabela de hash
void inicializa_tabela(Tabela *tabela) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabela->table[i] = NULL;
    }
}

void insere_tabela(Tabela *tabela, char *word){
    int index = hash_function(word);

    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL){
        fprintf(stderr, "Erro ao alocar memória para nó.");
        exit(EXIT_FAILURE);
    }

    new_node->word = strdup(word);
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
    return 0;
}

// Função de busca
int busca_tabela(Tabela *tabela, char *word){
    int index = hash_function(word);

    Node *current = tabela->table[index];
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return 1; //Encontrado
        }
        current = current->next;
    }
    return 0; //Não encontrado
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
