#include "lexer.h"

int	is_dq_escapable(char c)
{
	return (c == '\"' || c == '\\' || c == '$' || c == '`');
}
