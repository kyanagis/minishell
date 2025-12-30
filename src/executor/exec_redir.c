/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:33 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:16 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "executor.h"
#include "error_messages.h"

static int	open_infile(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (print_error(ERR_PREFIX, path, STATUS_GENERAL_ERR));
	return (fd);
}

static int	open_outfile(const char *path, bool append)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(path, flags, 0644);
	if (fd < 0)
		return (print_error(ERR_PREFIX, path, STATUS_GENERAL_ERR));
	return (fd);
}

static int	open_redir_fd(t_shell *sh, t_redir *redir)
{
	if (redir->kind == R_IN)
		return (open_infile(redir->arg));
	if (redir->kind == R_OUT)
		return (open_outfile(redir->arg, false));
	if (redir->kind == R_APPEND)
		return (open_outfile(redir->arg, true));
	return (open_heredoc(sh, redir));
}

static void	store_redir_target(t_fd_target *tgt, t_redir *redir, int fd)
{
	if (redir->kind == R_IN || redir->kind == R_HEREDOC)
	{
		if (tgt->in_fd != STDIN_FILENO && tgt->in_fd >= 0)
			close(tgt->in_fd);
		tgt->in_fd = fd;
		return ;
	}
	if (tgt->out_fd != STDOUT_FILENO && tgt->out_fd >= 0)
		close(tgt->out_fd);
	tgt->out_fd = fd;
}

int	prepare_redirections(t_shell *sh, t_redir *redir, t_fd_target *tgt)
{
	int	fd;

	while (redir)
	{
		fd = open_redir_fd(sh, redir);
		if (fd < 0)
		{
			close_fd_target(tgt);
			if (sh && redir->kind == R_HEREDOC && sh->last_status == 130)
				return (130);
			return (STATUS_GENERAL_ERR);
		}
		store_redir_target(tgt, redir, fd);
		redir = redir->next;
	}
	return (0);
}
