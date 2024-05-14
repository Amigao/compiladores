#ifndef ANALISADOR_LEXICO_H
#define ANALISADOR_LEXICO_H

#include "hashing.h"

#define INITIAL_STATE 0 
#define END_BUFFER 100
#define ERRO_LEXICO 20

void constroi_tabela_reservada(Tabela *tabela);
lexico analisador_lexico(char character, char* buffer, Tabela *TabelaReservada, int current_state);
    
#endif
