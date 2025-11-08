#include "minishell.h"
#include "parser.h"

#define PARSE_HANDLER_COUNT 6 //増えたら増やす <-

static bool	word_syntax_scan(t_lexout *lx, size_t i)
{
	(void)lx;
	(void)i;
	return (true);
}
static bool	pipe_syntax_scan(t_lexout *tokens, size_t i)
{
	if (i == 0)
		return (parse_syntax_error(token_str(tokens->kind[i])));
	else if (i + 1 == tokens->count)
		return (parse_syntax_error("newline"));
	else if (tokens->kind[i + 1] == TOK_PIPE)
		return (parse_syntax_error(token_str(tokens->kind[i])));
	return (true);
}

//対象が増えたらここにハンドラ追加
static void	init_parse_handler(t_parse_handler *parse_handler)
{
	parse_handler[TOK_WORD] = word_syntax_scan;
	parse_handler[TOK_PIPE] = pipe_syntax_scan; // test
	parse_handler[TOK_LT] = lt_syntax_scan;
	parse_handler[TOK_GT] = gt_syntax_scan;
	parse_handler[TOK_DLT] = dlt_syntax_scan;
	parse_handler[TOK_DGT] = dgt_syntax_scan;
}

static bool	syntax_scan(t_lexout *tokens)
{
	int				i;
	t_parse_handler	parse_handler[PARSE_HANDLER_COUNT];
	t_tok_kind		k;

	i = 0;
	if (!tokens || tokens->count == 0)
		return (true);
	init_parse_handler(parse_handler);
	while (i < tokens->count)
	{
		k = tokens->kind[i];
		if (k < 0 || k >= PARSE_HANDLER_COUNT)
			return (parse_syntax_error("unknown token"), false);
		if (!parse_handler[k](tokens, i))
			return (false);
		++i;
	}
	return (true);
}

// //パースを作ります
bool	parse_tokens(t_shell *sh, t_lexout *tokens)
{
	if (!syntax_scan(tokens))
	{
		sh->last_status = 258;
		return (false);
	}
	//シンタックスエラーを出してからパース構造体にぶちこみたい
}