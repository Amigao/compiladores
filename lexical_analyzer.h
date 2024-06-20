#ifndef LEXICAL_ANALYZER_H 
#define LEXICAL_ANALYZER_H

#include "aux_structs.h"

#define RETURN_STATE 7
#define INITIAL_STATE 0

void build_reserved_table(Table *table);
TokenInfo getNextToken(CompilingInfo *aux);
TokenInfo lexical_analyzer(char character, char* buffer, Table *reservedTable, int current_state);
    
#endif
