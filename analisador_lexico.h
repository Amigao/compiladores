#ifndef ANALISADOR_LEXICO_H
#define ANALISADOR_LEXICO_H

#include "hashing.h"

#define INITIAL_STATE 0 
#define END_BUFFER 100

typedef struct TokenInfo {
    char *token;
    char *identifier;  
    int state;
    int line;
    bool final;
} TokenInfo;

void constroi_tabela_reservada(Tabela *tabela);

TokenInfo analisador_lexico(char character, char* buffer, Tabela *TabelaReservada, int current_state);
    
#endif
