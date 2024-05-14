#ifndef ANALISADOR_LEXICO_H
#define ANALISADOR_LEXICO_H

#include "hashing.h"

void constroi_tabela_reservada(Tabela *tabela);
char *analisador_lexico(char *buffer, Tabela *TabelaReservada, FILE *output_file);
    
#endif


