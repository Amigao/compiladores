#ifndef SINTATIC_ANALYZER_H 
#define SINTATIC_ANALYZER_H

#include "aux_structs.h"
#include "lexical_analyzer.h"

void sintatic_analyzer(FILE *input_file, FILE *output_file);

// Declaração das funções usadas no analisador sintático
void programa(CompilingInfo *aux);
void bloco(CompilingInfo *aux);
void declaracao(CompilingInfo *aux);
void constante(CompilingInfo *aux);
void mais_cont(CompilingInfo *aux);
void variavel(CompilingInfo *aux);
void mais_var(CompilingInfo *aux);
void procedimento(CompilingInfo *aux);
void comando(CompilingInfo *aux);
void mais_comando(CompilingInfo *aux);
void expressao(CompilingInfo *aux);
void operador_unitario(CompilingInfo *aux);
void termo(CompilingInfo *aux);
void mais_termos(CompilingInfo *aux);
void fator(CompilingInfo *aux);
void mais_fator(CompilingInfo *aux);
void condicao(CompilingInfo *aux);
void relacional(CompilingInfo *aux);

#endif