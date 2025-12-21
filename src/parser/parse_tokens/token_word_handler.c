/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:03:38 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/15 04:12:37 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

// マスク情報をキャッシュし、ヒアドキュメントが引用されたかを取得する。

static bool	get_heredoc_quote_flag(t_work_context *ctx, const t_lexout *tokens,
		size_t index)
{
	unsigned char	*cache;
	unsigned char	*mask;
	size_t			len;
	size_t			i;
	bool			quoted;

	cache = ctx->heredoc_quote_cache;
	if (cache && cache[index] != 0)
		return (cache[index] == 2);
	mask = tokens->qmask[index];
	len = tokens->len[index];
	quoted = false;
	i = 0;
	while (mask && i < len)
	{
		if (mask[i++] != 0)
			quoted = true;
		if (quoted)
			break ;
	}
	if (cache && quoted)
		cache[index] = 2;
	else if (cache)
		cache[index] = 1;
	return (quoted);
}

static void	ensure_argument_capacity(t_work_command *builder)
{
	char	**argv;
	size_t	*idx;
	size_t	new_cap;

	if (builder->cap != 0 && builder->argc + 1 < builder->cap)
		return ;
	new_cap = 4;
	if (builder->cap != 0)
		new_cap = builder->cap * 2;
	argv = ft_xcalloc(new_cap + 1, sizeof(char *));
	idx = ft_xcalloc(new_cap, sizeof(size_t));
	if (builder->argv && builder->argc != 0)
	{
		ft_memcpy(argv, builder->argv, sizeof(char *) * builder->argc);
		ft_memcpy(idx, builder->tok_idx_argv, sizeof(size_t) * builder->argc);
	}
	free(builder->argv);
	free(builder->tok_idx_argv);
	builder->argv = argv;
	builder->tok_idx_argv = idx;
	builder->cap = new_cap;
}

static bool	ensure_builder_ready(t_work_context *ctx, t_work_state *state)
{
	if (ctx->current_builder)
		return (true);
	ctx->current_builder = ft_xcalloc(1, sizeof(t_work_command));
	if (!ctx->current_builder)
		state->ok = false;
	return (ctx->current_builder != NULL);
}

static bool	append_redirection_token(t_work_context *ctx, t_work_state *state,
		const char *word, const t_lexout *tokens)
{
	t_redir			*redir;
	t_work_command	*builder;

	state->heredoc_delim_quoted = false;
	if (ctx->pending_redir_kind == R_HEREDOC)
		state->heredoc_delim_quoted = get_heredoc_quote_flag(ctx, tokens,
				state->index);
	if (!ensure_builder_ready(ctx, state))
		return (false);
	builder = ctx->current_builder;
	redir = ft_xcalloc(1, sizeof(t_redir));
	redir->kind = ctx->pending_redir_kind;
	redir->arg = ft_xstrdup(word);
	redir->tok_idx = state->index;
	redir->delim_quoted = state->heredoc_delim_quoted;
	if (!builder->r_head)
		builder->r_head = redir;
	else
		builder->r_tail->next = redir;
	builder->r_tail = redir;
	ctx->expecting_redir_arg = false;
	return (true);
}

// 受け取った単語トークンを引数/リダイレクトとしてコマンドに積む。

bool	handle_word_token(t_work_context *ctx, t_work_state *state,
		const t_lexout *tokens)
{
	const char		*word;
	t_work_command	*builder;

	word = tokens->argv[state->index];
	if (ctx->expecting_redir_arg)
		return (append_redirection_token(ctx, state, word, tokens));
	if (!ensure_builder_ready(ctx, state))
		return (false);
	builder = ctx->current_builder;
	ensure_argument_capacity(builder);
	builder->argv[builder->argc] = ft_xstrdup(word);
	builder->tok_idx_argv[builder->argc] = state->index;
	builder->argc++;
	builder->argv[builder->argc] = NULL;
	return (true);
}
