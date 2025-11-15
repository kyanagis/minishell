#include "parser.h"

bool	parse_syntax_error(const char *unexpected)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd((char *)unexpected, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	return (false);
}

const char	*token_str(t_tok_kind kind)
{
	if (kind == TOK_PIPE)
		return ("|");
	if (kind == TOK_LT)
		return ("<");
	if (kind == TOK_GT)
		return (">");
	if (kind == TOK_DLT)
		return ("<<");
	if (kind == TOK_DGT)
		return (">>");
	return ("?");
}
