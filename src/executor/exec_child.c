/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:08 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:31:30 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "executor.h"
#include "free_table.h"
#include "sig.h"

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

static void	child_exit(t_shell *sh, int status)
{
	if (sh && sh->table)
		ft_release(sh->table);
	if (sh)
		free_env_list(&sh->env_list, free);
	if (sh)
		shell_destroy(sh);
	exit(status);
}

static void	child_fail(t_shell *sh, t_fd_target *tgt, int status)
{
	close_fd_target(tgt);
	child_exit(sh, status);
}

static void	init_child_ctx(t_child_ctx *ctx, int *prev_read, int **pipefd)
{
	*prev_read = -1;
	*pipefd = NULL;
	if (!ctx)
		return ;
	free(ctx->pids);
	*prev_read = ctx->prev_read;
	*pipefd = ctx->pipefd;
}

void	execute_child(t_shell *sh, t_cmd *cmd, t_child_ctx *ctx)
{
	t_fd_target	tgt;
	int			status;
	int			prev_read;
	int			*pipefd;

	init_child_ctx(ctx, &prev_read, &pipefd);
	set_default_signals();
	init_fd_target(&tgt, STDIN_FILENO, STDOUT_FILENO);
	if (prev_read >= 0)
		tgt.in_fd = prev_read;
	if (pipefd && pipefd[1] >= 0)
		tgt.out_fd = pipefd[1];
	status = prepare_redirections(sh, cmd->redirs, &tgt);
	if (status != 0)
		child_fail(sh, &tgt, status);
	status = apply_fd_target(&tgt);
	if (status != 0)
		child_fail(sh, &tgt, status);
	close_child_fds(prev_read, pipefd, &tgt);
	status = exec_builtin(sh, cmd->argv);
	if (status != NOT_BUILTIN)
		child_exit(sh, status);
	status = execute_external(sh, cmd->argv);
	child_exit(sh, status);
}
