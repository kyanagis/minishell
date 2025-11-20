#include "expander.h"

static bool token_index_valid(const t_lexout *lexer, size_t idx)
{
	if (!lexer || idx >= lexer->count)
		return (false);
	return (true);
}

static bool expand_argv(t_shell *sh, const t_lexout *lexer, t_cmd *cmd)
{
	size_t idx;
	size_t tok_idx;
	char *expanded;

	if (!cmd->argv || !cmd->tok_idx_argv)
		return (true);
	idx = 0;
	while (idx < cmd->argc)
	{
		tok_idx = cmd->tok_idx_argv[idx];
		if (!token_index_valid(lexer, tok_idx))
			return (false);
		if (!expand_word(sh, cmd->argv[idx], lexer->qmask[tok_idx],
						 lexer->len[tok_idx], &expanded))
			return (false);
		free(cmd->argv[idx]);
		cmd->argv[idx] = expanded;
		++idx;
	}
	return (true);
}

static bool expand_redirs(t_shell *sh, const t_lexout *lexer, t_redir *redir)
{
	char *expanded;

	while (redir)
	{
		if (redir->kind != R_HEREDOC)
		{
			if (!token_index_valid(lexer, redir->tok_idx))
				return (false);
			if (!expand_word(sh, redir->arg, lexer->qmask[redir->tok_idx],
							 lexer->len[redir->tok_idx], &expanded))
				return (false);
			free(redir->arg);
			redir->arg = expanded;
		}
		redir = redir->next;
	}
	return (true);
}

static bool expand_cmd(t_shell *sh, const t_lexout *lexer, t_cmd *cmd)
{
	if (!cmd)
		return (true);
	if (!expand_argv(sh, lexer, cmd))
		return (false);
	return (expand_redirs(sh, lexer, cmd->redirs));
}

bool expand_pipeline(t_shell *sh, const t_lexout *lexer,
					 t_pipeline *pipeline)
{
	size_t idx;

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
