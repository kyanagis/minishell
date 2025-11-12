#ifndef DEBUG_H
#define DEBUG_H

#include "lexer.h"
#include "minishell.h"
#include "parser.h"


void				debug_print_pipeline(const t_pipeline *pl);
void				lexer_debug_print(const t_lexout *o);



#endif