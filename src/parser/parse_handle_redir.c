#include "lexer.h"
#include "parser.h"

static t_redir_kind	as_redir_kind(t_tok_kind kind)
{
	if (kind == TOK_LT)
		return (R_IN);
	if (kind == TOK_GT)
		return (R_OUT);
	if (kind == TOK_DLT)
		return (R_HEREDOC);
	return (R_APPEND);
}
bool	parse_handle_redir_op(t_ctx *ctx, t_work *work)
{
	if (ctx->need_redir_arg)
	{
		work->ok = false;
		return (true);
	}
	else
	{
		ctx->pend_kind = as_redir_kind(work->kind);
		ctx->need_redir_arg = true;
		return (true);
	}
}
