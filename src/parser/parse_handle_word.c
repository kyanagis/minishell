#include "lexer.h"
#include "parser.h"

static bool	token_has_any_quote(const unsigned char *mask, size_t len)
{
	size_t	i;

	if (!mask || len == 0)
		return (false);
	i = 0;
	while (i < len)
	{
		if (mask[i] != 0)
			return (true);
		++i;
	}
	return (false);
}

static t_cmd_b	*cmd_b_new(void)
{
	t_cmd_b	*b;

	b = ft_xcalloc(1, sizeof(t_cmd_b));
	return (b);
}

static void	cmd_b_push_argv(t_cmd_b *b, const char *s)
{
	char	**n;
	size_t	ncap;

	if (!s)
		return ;
	if (b->argc + 1 >= b->cap)
	{
		if (b->cap == 0)
			ncap = 4;
		else
			ncap = b->cap * 2;
		n = ft_xcalloc(ncap + 1, sizeof(char *));
		if (b->argv && b->argc)
			ft_memcpy(n, b->argv, sizeof(char *) * b->argc);
		free(b->argv);
		b->argv = n;
		b->cap = ncap;
	}
	b->argv[b->argc++] = ft_xstrdup(s);
	b->argv[b->argc] = NULL;
}

static void	cmd_b_push_redir(t_cmd_b *b, t_redir_kind kind, const char *arg_s,
		bool delim_quoted)
{
	t_redir	*r;

	r = ft_xcalloc(1, sizeof(t_redir));
	r->kind = kind;
	r->arg = ft_xstrdup(arg_s);
	r->delim_quoted = delim_quoted;
	r->next = NULL;
	if (!b->r_head)
	{
		b->r_head = r;
		b->r_tail = r;
	}
	else
	{
		b->r_tail->next = r;
		b->r_tail = r;
	}
}

bool	parse_handle_word(t_ctx *ctx, t_work *work, const t_lexout *lx)
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
		{
			work->ok = false;
			return (false);
		}
		cmd_b_push_redir(ctx->cur, ctx->pend_kind, w, work->quoted);
		ctx->need_redir_arg = false;
	}
	else
	{
		if (!ctx->cur)
			ctx->cur = cmd_b_new();
		cmd_b_push_argv(ctx->cur, w);
	}
	return (true);
}
