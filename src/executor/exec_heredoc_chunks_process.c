/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_chunks_process.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/18 12:48:50 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_heredoc_internal.h"

static bool	handle_null_line(t_shell *sh, t_redir *redir)
{
	if (g_sig == SIGINT)
	{
		sh->last_status = 130;
		return (false);
	}
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file", STDERR_FILENO);
	ft_putstr_fd(" (wanted `", STDERR_FILENO);
	ft_putstr_fd(redir->arg, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
	return (true);
}

static int	process_heredoc_line(t_shell *sh, t_redir *redir,
						t_chunk_state *chunk_state, char *input_line)
{
	if (ft_strcmp(input_line, redir->arg) == 0)
	{
		free(input_line);
		return (0);
	}
	if (!append_expanded_line(sh, redir->delim_quoted,
			chunk_state, input_line))
		return (-1);
	return (1);
}

int	handle_heredoc_line(t_shell *sh, t_redir *redir,
					t_chunk_state *chunk_state, char *input_line)
{
	if (!input_line)
	{
		if (handle_null_line(sh, redir))
			return (0);
		return (-1);
	}
	if (g_sig == SIGINT)
	{
		free(input_line);
		sh->last_status = 130;
		return (-1);
	}
	return (process_heredoc_line(sh, redir, chunk_state, input_line));
}
