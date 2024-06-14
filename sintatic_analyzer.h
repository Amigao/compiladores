#ifndef SINTATIC_ANALYZER_H 
#define SINTATIC_ANALYZER_H

#include<stdlib.h>

void sintatic_analyzer(FILE *input_file, FILE *output_file);

// Declaração das funções usadas no analisador sintático
void programa(FILE* input_file, FILE *output_file);
void bloco(FILE* input_file, FILE *output_file);
void declaracao(FILE* input_file, FILE *output_file);
void constante(FILE* input_file, FILE *output_file);
void mais_cont(FILE* input_file, FILE *output_file);
void variavel(FILE* input_file, FILE *output_file);
void mais_var(FILE* input_file, FILE *output_file);
void procedimento(FILE* input_file, FILE *output_file);
void comando(FILE* input_file, FILE *output_file);
void expressao(FILE* input_file, FILE *output_file);
void operador_unitario(FILE* input_file, FILE *output_file);
void termo(FILE* input_file, FILE *output_file);
void mais_termos(FILE* input_file, FILE *output_file);
void fator(FILE* input_file, FILE *output_file);
void mais_fatores(FILE* input_file, FILE *output_file);
void condicao(FILE* input_file, FILE *output_file);
void relacional(FILE* input_file, FILE* output_file);

#endif