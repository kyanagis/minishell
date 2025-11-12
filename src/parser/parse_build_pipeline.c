#include "lexer.h"
#include "parser.h"

//修正するべきもの
//関数名
//ファイル名
// norm

// work->okの判定漏れの確認
static void	init_ctx(t_ctx *ctx, const t_lexout *lx)
{
	ft_memset(ctx, 0, sizeof(*ctx));
	ctx->lx = lx;
}

static void	cmd_b_free(t_cmd_b *b)
{
	t_redir	*p;
	t_redir	*nxt;
	size_t	i;

	if (!b)
		return ;
	p = b->r_head;
	while (p)
	{
		nxt = p->next;
		free(p->arg);
		free(p);
		p = nxt;
	}
	if (b->argv)
	{
		i = 0;
		while (i < b->argc)
		{
			free(b->argv[i]);
			++i;
		}
		free(b->argv);
	}
	free(b);
}

static void	pl_b_dispose(t_pl_b *pb)
{
	size_t	i;
	t_cmd	*c;
	t_redir	*r;
	t_redir	*nx;
	size_t	j;

	if (!pb->arr)
		return ;
	i = 0;
	while (i < pb->len)
	{
		if (pb->arr[i])
		{
			c = pb->arr[i];
			r = c->redirs;
			while (r)
			{
				nx = r->next;
				free(r->arg);
				free(r);
				r = nx;
			}
			if (c->argv)
			{
				j = 0;
				while (c->argv[j])
				{
					free(c->argv[j]);
					++j;
				}
				free(c->argv);
			}
			free(c);
		}
		++i;
	}
	free(pb->arr);
	pb->arr = NULL;
	pb->len = 0;
	pb->cap = 0;
}

void	finalize_pipeline(t_ctx *ctx, t_work *work)
{
	t_cmd	*c;

	c = cmd_b_finalize(ctx->cur);
	if (!c)
	{
		work->ok = false;
		return ;
	}
	pl_b_push(&ctx->plb, c);
	free(ctx->cur);
	ctx->cur = NULL;
}
static void	cleanup_cmds(t_ctx *ctx)
{
	if (ctx->cur)
		cmd_b_free(ctx->cur);
	pl_b_dispose(&ctx->plb);
}

bool	parse_build_pipeline(const t_lexout *lx, t_pipeline **out)
{
	t_ctx	ctx;
	t_work	work;

	if (!out)
		return (false);
	*out = NULL;
	if (!lx || lx->count == 0)
		return (true);
	init_ctx(&ctx, lx);
	work.ok = true;
	work.idx = 0;
	work.quoted = false;
	while (work.ok && work.idx < lx->count)
		handle_operator(&work, &ctx, lx);
	//ここで分けたいと思います
	if (work.ok)
	{
		if (ctx.need_redir_arg || cmd_b_is_completely_empty(ctx.cur))
			work.ok = false;
	}
	if (work.ok)
	{
		finalize_pipeline(&ctx, &work);
	}
	if (work.ok)
	{
		*out = pl_b_finalize(&ctx.plb);
		if (*out == NULL)
			work.ok = false;
	}
	if (!work.ok)
	{
		cleanup_cmds(&ctx);
		return (false);
	}
	return (true);
}

// // 最終コマンドをパイプラインに積む
// if (work.ok)
// 	finalize_pipeline(last, &ctx);
// // パイプライン確定
// if (work.ok)
// 	definition_pipeline(out, &ctx, &work);
// // 失敗時の一括後始末
// if (!work.ok)
// 	cleanup_cmds(&ctx);
