/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:35 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:19 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "executor.h"

static void	restore_stdio(int saved_in, int saved_out)
{
	if (saved_in >= 0)
	{
		dup2(saved_in, STDIN_FILENO);
		close(saved_in);
	}
	if (saved_out >= 0)
	{
		dup2(saved_out, STDOUT_FILENO);
		close(saved_out);
	}
}

static int	run_parent_builtin(t_shell *sh, t_cmd *cmd)
{
	t_fd_target	tgt;
	int			saved_in;
	int			saved_out;
	int			status;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in < 0 || saved_out < 0)
	{
		if (saved_in >= 0)
			close(saved_in);
		if (saved_out >= 0)
			close(saved_out);
		return (STATUS_GENERAL_ERR);
	}
	init_fd_target(&tgt, STDIN_FILENO, STDOUT_FILENO);
	status = prepare_redirections(sh, cmd->redirs, &tgt);
	if (status == 0)
		status = apply_fd_target(&tgt);
	if (status == 0)
		status = exec_builtin(sh, cmd->argv);
	restore_stdio(saved_in, saved_out);
	close_fd_target(&tgt);
	return (status);
}

static int	fork_external_single(t_shell *sh, t_cmd *cmd)
{
	pid_t		pid;
	int			status;
	pid_t		list[1];
	int			pipefd[2];
	t_child_ctx	ctx;

	pipefd[0] = -1;
	pipefd[1] = -1;
	ctx.prev_read = -1;
	ctx.pipefd = pipefd;
	ctx.pids = NULL;
	pid = fork();
	if (pid < 0)
		return (STATUS_GENERAL_ERR);
	if (pid == 0)
		execute_child(sh, cmd, &ctx);
	list[0] = pid;
	status = wait_children(list, 1);
	return (status);
}

int	execute_single(t_shell *sh, t_cmd *cmd)
{
	t_fd_target	tgt;
	int			status;
	bool		builtin_flag;

	if (!cmd)
		return (0);
	if (!cmd->argv || !cmd->argv[0])
	{
		init_fd_target(&tgt, STDIN_FILENO, STDOUT_FILENO);
		status = prepare_redirections(sh, cmd->redirs, &tgt);
		close_fd_target(&tgt);
		sh->last_status = status;
		return (status);
	}
	builtin_flag = is_builtin(cmd->argv[0]);
	if (builtin_flag)
		status = run_parent_builtin(sh, cmd);
	else
		status = fork_external_single(sh, cmd);
	if (builtin_flag)
		sync_envp(sh);
	sh->last_status = status;
	return (status);
}
