#ifndef LEXICAL_ANALYZER_H 
#define LEXICAL_ANALYZER_H

#include "hashing.h"

#define END_BUFFER 100

typedef struct TokenInfo {
    char *token;
    char *identifier;  
    int state;
    bool final;
} TokenInfo;

enum STATE {
    ERROR = -1,
    INITIAL_STATE = 0, 
    ALPHA,
    DIGIT,
    SPACE,
    DELIMITER,
    FIRST_DOUBLE_OP,
    SECOND_DOUBLE_OP,
    SINGLE_OP
};

void build_reserved_table(Table *table);

TokenInfo lexical_analyzer(char character, char* buffer, Table *reservedTable, int current_state);
    
#endif
