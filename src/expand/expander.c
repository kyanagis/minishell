/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:04:15 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/15 04:04:16 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static bool	token_index_valid(const t_lexout *lexer, size_t idx)
{
	if (!lexer || idx >= lexer->count)
		return (false);
	return (true);
}

static bool	expand_argv(t_shell *sh, const t_lexout *lexer, t_cmd *cmd)
{
	size_t			idx;
	size_t			tok_idx;
	char			*expanded;
	t_expand_input	input;

	if (!cmd->argv || !cmd->tok_idx_argv)
		return (true);
	idx = 0;
	while (idx < cmd->argc)
	{
		tok_idx = cmd->tok_idx_argv[idx];
		if (!token_index_valid(lexer, tok_idx))
			return (false);
		input.src = cmd->argv[idx];
		input.mask = lexer->qmask[tok_idx];
		input.len = lexer->len[tok_idx];
		if (!expand_word(sh, &input, &expanded))
			return (false);
		free(cmd->argv[idx]);
		cmd->argv[idx] = expanded;
		++idx;
	}
	return (true);
}

static bool	expand_redirs(t_shell *sh, const t_lexout *lexer, t_redir *redir)
{
	char			*expanded;
	t_expand_input	input;

	while (redir)
	{
		if (redir->kind != R_HEREDOC)
		{
			if (!token_index_valid(lexer, redir->tok_idx))
				return (false);
			input.src = redir->arg;
			input.mask = lexer->qmask[redir->tok_idx];
			input.len = lexer->len[redir->tok_idx];
			if (!expand_word(sh, &input, &expanded))
				return (false);
			free(redir->arg);
			redir->arg = expanded;
		}
		redir = redir->next;
	}
	return (true);
}

static bool	expand_cmd(t_shell *sh, const t_lexout *lexer, t_cmd *cmd)
{
	if (!cmd)
		return (true);
	if (!expand_argv(sh, lexer, cmd))
		return (false);
	return (expand_redirs(sh, lexer, cmd->redirs));
}

bool	expand_pipeline(t_shell *sh, const t_lexout *lexer,
		t_pipeline *pipeline)
{
	size_t	idx;

	if (!sh || !lexer || !pipeline)
		return (false);
	idx = 0;
	while (idx < pipeline->ncmds)
	{
		if (!expand_cmd(sh, lexer, pipeline->cmds[idx]))
			return (false);
		++idx;
	}
	return (true);
}
