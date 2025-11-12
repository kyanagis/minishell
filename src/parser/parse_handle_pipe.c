#include "lexer.h"
#include "parser.h"

void	parse_handle_pipe(t_ctx *ctx, t_work *work)
{
	t_cmd *c;

	if (ctx->need_redir_arg || cmd_b_is_completely_empty(ctx->cur))
	{
		work->ok = false;
		return ;
	}
	{
		c = cmd_b_finalize(ctx->cur);
		if (!c)
		{
			work->ok = false;
			return ;
		}
		pl_b_push(&ctx->plb, c);
		free(ctx->cur); // 本体だけ解放（中身は c に委譲済み）
		ctx->cur = NULL;
	}
}