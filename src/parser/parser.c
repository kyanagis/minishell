#include "minishell.h"

//パースを作ります
void	parse_tokens(t_lexout *tokens)
{
	int i = 0;

	while (i < tokens->count)
	{
		if (tokens->kind[i] == TOK_WORD)
			(void)tokens;
		else if (tokens->kind[i] == TOK_PIPE)
			(void)tokens;
		else if (tokens->kind[i] == TOK_LT)
			(void)tokens;
		else if (tokens->kind[i] == TOK_GT)
			(void)tokens;
		else if (tokens->kind[i] == TOK_DLT)
			(void)tokens;
		else if (tokens->kind[i] == TOK_DGT)

			++i;
	}
}