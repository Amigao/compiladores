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

    return hash % TABLE_SIZE;
}

// Função para inicializar a tabela hash
void initialize_table(Table *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->table[i] = NULL; // Inicializa cada entrada da tabela como NULL
    }
}

// Função para duplicar uma string
char *my_strdup(const char *src) {
    size_t len = strlen(src) + 1; // Calcula o tamanho da string de origem
    char *dst = malloc(len); // Aloca memória para a nova string
    if (dst == NULL) {
        fprintf(stderr, "Erro ao alocar memória para duplicar a string."); // Mensagem de erro se a alocação falhar
        exit(EXIT_FAILURE); // Sai do programa com falha
    }
    strcpy(dst, src); // Copia o conteúdo da string de origem para a nova string
    return dst; // Retorna um ponteiro para a nova string duplicada
}

// Função para inserir um par palavra-token na tabela hash
void insert_table(Table *table, char *word, char *token){
    int index = hash_function(word); // Calcula o índice usando a função de hash

    Node *new_node = malloc(sizeof(Node)); // Aloca memória para um novo nó
    if (new_node == NULL){ // Verifica se a alocação falhou
        fprintf(stderr, "Erro ao alocar memória para nó."); // Mensagem de erro se a alocação falhar
        exit(EXIT_FAILURE); // Sai do programa com falha
    }

    new_node->word = my_strdup(word); // Duplica a palavra e atribui ao novo nó
    new_node->token = my_strdup(token); // Duplica o token e atribui ao novo nó

    new_node->next = NULL; // Inicializa o ponteiro next do novo nó como NULL

    // Se a posição na tabela estiver vazia, insere o novo nó nessa posição
    if(table->table[index] == NULL){
        table->table[index] = new_node;
    } else { // Caso contrário, percorre a lista encadeada até o final e insere o novo nó lá
        Node *current = table->table[index];
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_node;
    }
}

// Função de busca
char *search_table(Table *table, char *word){
    int index = hash_function(word); // Calcula o índice usando a função de hash

    Node *current = table->table[index]; // Inicializa o ponteiro current com a cabeça da lista encadeada
    while (current != NULL) { // Percorre a lista encadeada
        if (strcmp(current->word, word) == 0) { // Compara a palavra atual com a palavra buscada
            return current->token; // Se encontrada, retorna o token associado
        }
        current = current->next; // Move para o próximo nó na lista
    }
    return NULL; // Se a palavra não for encontrada, retorna NULL
}

// Função para liberar a memória alocada para a tabela hash e seus nós
void free_table(Table *table) {
    for (int i = 0; i < TABLE_SIZE; i++){ // Percorre toda a tabela hash
        Node *current = table->table[i]; // Inicializa o ponteiro current com a cabeça da lista encadeada
        while (current != NULL) { // Percorre a lista encadeada
            Node *temp = current; // Salva o nó atual em temp
            current = current->next; // Move para o próximo nó na lista
            free(temp->word); // Libera a memória da palavra do nó atual
            free(temp); // Libera a memória do nó atual
        }        
    }
}
