#ifndef LEXICAL_ANALYZER_H 
#define LEXICAL_ANALYZER_H

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

void build_reserved_table(Table *table);

TokenInfo lexical_analyzer(char character, char* buffer, Table *reservedTable, int current_state);
    
#endif
