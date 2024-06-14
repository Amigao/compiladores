#ifndef LEXICAL_ANALYZER_H 
#define LEXICAL_ANALYZER_H

#include "hashing.h"
#include "tokens_enum.h"
#include "errors_management.h"

#define RETURN_STATE 7
#define INITIAL_STATE 0

#define MAX_BUF_SIZE 100

typedef struct TokenInfo {
    char *token;
    int token_enum;
    char *identifier;  
    int state;
    bool final;
} TokenInfo;


void build_reserved_table(Table *table);
TokenInfo getNextToken(FILE* input_file, FILE *output_file, ErrorInfo *error_list, Table reservedTable);
TokenInfo lexical_analyzer(char character, char* buffer, Table *reservedTable, int current_state);
    
#endif
