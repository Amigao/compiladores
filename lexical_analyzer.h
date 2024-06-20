#ifndef LEXICAL_ANALYZER_H 
#define LEXICAL_ANALYZER_H

#include "hashing.h"
#include "tokens_enum.h"
#include <stdio.h>
#include "errors_management.h"

#define RETURN_STATE 7
#define INITIAL_STATE 0

#define MAX_BUF_SIZE 100

typedef struct TokenInfo {
    char token[MAX_BUF_SIZE];
    int token_enum;
    char identifier[MAX_BUF_SIZE];  
    int state;
    int token_line;
    bool final;
} TokenInfo;


void build_reserved_table(Table *table);
TokenInfo getNextToken(FILE* input_file, ErrorInfo **error_list, Table reservedTable);
TokenInfo lexical_analyzer(char character, char* buffer, Table *reservedTable, int current_state);
    
#endif
