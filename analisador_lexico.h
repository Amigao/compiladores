#ifndef ANALISADOR_LEXICO_H
#define ANALISADOR_LEXICO_H

#include "hashing.h"

void constroi_tabela_reservada(Tabela *tabela);
void analisador_lexico(char *buffer, Tabela *TabelaReservada);
void constroi_tabela_simbolos(Tabela *tabela);
    
#endif


