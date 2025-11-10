#include "lexer.h"
#include "parser.h"

typedef struct s_cmd_b
{
	char			**argv;
	size_t			argc;
	size_t			cap;
	t_redir			*r_head;
	t_redir			*r_tail;
}					t_cmd_b;

typedef struct s_pl_b
{
	t_cmd			**arr;
	size_t			len;
	size_t			cap;
}					t_pl_b;

typedef struct s_ctx
{
	const t_lexout	*lx;
	t_pl_b			plb;
	t_cmd_b *cur;           // 現在構築中のコマンド（NULLならまだ無い）
	bool need_redir_arg;    // リダイレクト直後（次は word 必須）
	t_redir_kind pend_kind; // そのリダイレクト種別
}					t_ctx;

typedef struct s_work
{
	size_t			idx;
	t_tok_kind		kind;
	bool			ok;
	bool			quoted;
}					t_work;

static void	init_ctx(t_ctx *ctx, const t_lexout *lx)
{
	ft_memset(&ctx, 0, sizeof(*ctx));
	ctx->lx = lx;
	// ft_memset(&ctx->plb, 0, sizeof(&ctx->plb));
}

static bool	parse_handle_word(t_ctx *ctx, t_work *work, const t_lexout *lx)
{
	const char	*w = lx->argv[work->idx];

	if (ctx->need_redir_arg)
	{
		if (ctx->pend_kind == R_HEREDOC)
			work->quoted = token_has_any_quote(lx->qmask[work->idx],
					lx->len[work->idx]);
		if (!ctx->cur)
			ctx->cur = cmd_b_new();
		if (!ctx->cur)
			return (false);
		cmd_b_push_redir(ctx->cur, ctx->pend_kind, w, work->quoted);
		ctx->need_redir_arg = false;
	}
	else
	{
		if (!ctx->cur)
			ctx->cur = cmd_b_new();
		cmd_b_push_argv(ctx->cur, w);
	}
}

static void	parse_handle_redir_op(t_ctx *ctx, t_work *work)
{
	if (ctx->need_redir_arg)
		work->ok = false;
	else
	{
		ctx->pend_kind = as_redir_kind(work->kind);
		ctx->need_redir_arg = true;
	}
}

static void	parse_handle_pipe(t_ctx *ctx, t_work *work, t_cmd *final)
{
	if (ctx->need_redir_arg || !ctx->cur || ctx->cur->argc == 0)
		work->ok = false;
	else
	{
		final = cmd_b_finalize(ctx->cur);
		if (!final)
			work->ok = false;
		else
		{
			pl_b_push(&ctx->plb, final);
			free(ctx->cur);
			ctx->cur = NULL;
		}
	}
}

bool	parse_build_pipeline(const t_lexout *lx, t_pipeline **out)
{
	t_ctx	ctx;
	t_cmd	*final;
	t_cmd	*last;
	t_work	work;

	if (!lx || lx->count == 0 || !out) //たぶんいらない
		return (false);
	init_ctx(&ctx, lx);
	work.ok = true;
	work.idx = 0;
	work.quoted = false;
	while (work.ok && work.idx < lx->count)
	{
		work.kind = lx->kind[work.idx];
		if (work.kind == TOK_WORD)
			parse_handle_word(&ctx, &work, lx);
		else if (work.kind == TOK_LT || work.kind == TOK_GT
			|| work.kind == TOK_DLT || work.kind == TOK_DGT)
			parse_handle_redir_op(&ctx, &work);
		else if (work.kind == TOK_PIPE)
			parse_handle_pipe(&ctx, &work, &final);
		else
			work.ok = false;
		++work.idx;
	}
	// ループ終了後の整合性チェック
	if (!work.ok || ctx.need_redir_arg)
		return (false);
	// 最後のコマンドを確定
	if (ctx.cur)
	{
		if (ctx.cur->argc == 0 && !ctx.cur->r_head)
			return (false);
		final = cmd_b_finalize(ctx.cur);
		if (!final)
			return (false);
		pl_b_push(&ctx.plb, final);
		free(ctx.cur);
		ctx.cur = NULL;
	}
	// plb -> t_pipeline へ組み立て
	*out = pl_b_build_pipeline(&ctx.plb);
	if (!*out)
		return (false);
	return (true);
}
