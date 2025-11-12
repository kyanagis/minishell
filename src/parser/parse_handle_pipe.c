#include "lexer.h"
#include "parser.h"

void	pl_b_push(t_pl_b *pb, t_cmd *c)
{
	t_cmd	**n;
	size_t	ncap;

	if (pb->len + 1 > pb->cap)
	{
		if (pb->cap == 0)
			ncap = 2;
		else
			ncap = pb->cap * 2;
		n = ft_xcalloc(ncap, sizeof(t_cmd *));
		if (pb->arr && pb->len)
			ft_memcpy(n, pb->arr, sizeof(t_cmd *) * pb->len);
		free(pb->arr);
		pb->arr = n;
		pb->cap = ncap;
	}
	pb->arr[pb->len++] = c;
}

bool	parse_handle_pipe(t_ctx *ctx, t_work *work)
{
	t_cmd	*c;

	if (ctx->need_redir_arg || cmd_b_is_completely_empty(ctx->cur))
	{
		work->ok = false;
		return (false);
	}
	c = cmd_b_finalize(ctx->cur);
	if (!c)
	{
		work->ok = false;
		return (false);
	}
	pl_b_push(&ctx->plb, c);
	free(ctx->cur); // 本体だけ解放（中身は c に委譲済み）
	ctx->cur = NULL;
	return (true);
}
