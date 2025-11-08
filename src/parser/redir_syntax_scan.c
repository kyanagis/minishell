#include "parser.h"

static bool	redir_expect_word(t_lexout *tokens, size_t i)
{
	t_tok_kind	next;

	if (i + 1 == tokens->count)
		return (parse_syntax_error("newline"));
	next = tokens->kind[i + 1];
	if (next != TOK_WORD)
		return (parse_syntax_error(token_str(next)));
	return (true);
}

bool	lt_syntax_scan(t_lexout *tokens, size_t i)
{
	return (redir_expect_word(tokens, i));
}

bool	gt_syntax_scan(t_lexout *tokens, size_t i)
{
	return (redir_expect_word(tokens, i));
}

bool	dlt_syntax_scan(t_lexout *tokens, size_t i)
{
	return (redir_expect_word(tokens, i));
}

bool	dgt_syntax_scan(t_lexout *tokens, size_t i)
{
	return (redir_expect_word(tokens, i));
}
