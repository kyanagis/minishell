#include "parser.h"

static t_redir_kind as_redir_kind(t_tok_kind kind)
{
	if (kind == TOK_LT)
		return (R_IN);
	if (kind == TOK_GT)
		return (R_OUT);
	if (kind == TOK_DLT)
		return (R_HEREDOC);
	return (R_APPEND);
}
bool handle_redirection_operator(t_work_context *ctx, t_work_state *state)
{
	if (ctx->expecting_redir_arg)
	{
		state->ok = false;
		return (false);
	}
	ctx->pending_redir_kind = as_redir_kind(state->kind);
	ctx->expecting_redir_arg = true;
	return (true);
}
