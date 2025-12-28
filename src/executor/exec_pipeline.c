/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:30 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:14 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "executor.h"

static bool open_pipe_if_needed(int pipefd[2], bool needed)
{
	if (!needed)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
		return (true);
	}
	if (pipe(pipefd) == -1)
		return (false);
	return (true);
}

static void close_parent_fds(int *prev_read, int pipefd[2])
{
	if (*prev_read >= 0)
		close(*prev_read);
	if (pipefd[1] >= 0)
		close(pipefd[1]);
	if (pipefd[0] >= 0)
		*prev_read = pipefd[0];
	else
		*prev_read = -1;
}

static pid_t spawn_child(t_shell *sh, t_cmd *cmd, int prev_read, int pipefd[2])
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
		execute_child(sh, cmd, prev_read, pipefd);
	return (pid);
}

static ssize_t run_pipeline(t_shell *sh, t_pipeline *pl, pid_t *pids)
{
	int pipefd[2];
	int prev_read = -1;
	size_t i = 0;
	while (i < pl->ncmds)
	{
		if (!open_pipe_if_needed(pipefd, i + 1 < pl->ncmds))
		{
			if (prev_read >= 0)
				close(prev_read);
			return (-1);
		}
		pids[i] = spawn_child(sh, pl->cmds[i], prev_read, pipefd);
		if (pids[i] < 0)
		{
			close_parent_fds(&prev_read, pipefd);
			return (-1);
		}
		close_parent_fds(&prev_read, pipefd);
		++i;
	}
	return (i);
}

int execute_pipeline(t_shell *sh, t_pipeline *pl)
{
	pid_t *pids;
	ssize_t launched;
	int status;

	if (!pl || pl->ncmds == 0)
		return (0);
	pids = (pid_t *)ft_calloc(pl->ncmds, sizeof(pid_t));
	if (!pids)
		return (STATUS_GENERAL_ERR);
	launched = run_pipeline(sh, pl, pids);
	status = 0;
	if (launched > 0)
		status = wait_children(pids, launched);
	free(pids);
	if (launched < 0)
		return (STATUS_GENERAL_ERR);
	return (status);
}
