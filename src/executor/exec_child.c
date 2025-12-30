/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:08 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:17:14 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "executor.h"

static void	close_child_fds(int prev_read, int pipefd[2], t_fd_target *tgt)
{
	if (prev_read >= 0 && prev_read != tgt->in_fd)
		close(prev_read);
	if (pipefd && pipefd[0] >= 0)
		close(pipefd[0]);
	if (pipefd && pipefd[1] >= 0 && pipefd[1] != tgt->out_fd)
		close(pipefd[1]);
	if (tgt->in_fd != STDIN_FILENO)
		close(tgt->in_fd);
	if (tgt->out_fd != STDOUT_FILENO)
		close(tgt->out_fd);
}

void	execute_child(t_shell *sh, t_cmd *cmd, int prev_read, int pipefd[2])
{
	t_fd_target	tgt;
	int			status;

	set_default_signals();
	init_fd_target(&tgt, STDIN_FILENO, STDOUT_FILENO);
	if (prev_read >= 0)
		tgt.in_fd = prev_read;
	if (pipefd && pipefd[1] >= 0)
		tgt.out_fd = pipefd[1];
	status = prepare_redirections(sh, cmd->redirs, &tgt);
	if (status != 0)
		exit(status);
	status = apply_fd_target(&tgt);
	if (status != 0)
		exit(status);
	close_child_fds(prev_read, pipefd, &tgt);
	status = exec_builtin(sh, cmd->argv);
	if (status != NOT_BUILTIN)
		exit(status);
	status = execute_external(sh, cmd->argv);
	exit(status);
}
