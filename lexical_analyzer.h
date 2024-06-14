#ifndef LEXICAL_ANALYZER_H 
#define LEXICAL_ANALYZER_H

#include "hashing.h"

#define RETURN_STATE 7
#define INITIAL_STATE 0

typedef struct TokenInfo {
    char *token;
    int token_enum;
    char *identifier;  
    int state;
    bool final;
} TokenInfo;

void build_reserved_table(Table *table);

TokenInfo lexical_analyzer(char character, char* buffer, Table *reservedTable, int current_state);
    
#endif
