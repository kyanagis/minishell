/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_runner.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/01/03 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "executor.h"

static bool	open_pipe_if_needed(int pipefd[2], bool needed)
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

static void	close_parent_fds(int *prev_read, int pipefd[2])
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

static pid_t	spawn_child(t_shell *sh, t_cmd *cmd, int prev_read,
			int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
		execute_child(sh, cmd, prev_read, pipefd);
	return (pid);
}

static void	cleanup_failed_pipeline(pid_t *pids, size_t launched,
			int *prev_read, int pipefd[2])
{
	if (prev_read && *prev_read >= 0)
	{
		close(*prev_read);
		*prev_read = -1;
	}
	if (pipefd[0] >= 0)
		close(pipefd[0]);
	if (pipefd[1] >= 0)
		close(pipefd[1]);
	if (launched > 0)
		wait_children(pids, launched);
}

ssize_t	run_pipeline(t_shell *sh, t_pipeline *pl, pid_t *pids)
{
	int		pipefd[2];
	int		prev_read;
	size_t	i;

	prev_read = -1;
	i = 0;
	while (i < pl->ncmds)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
		if (!open_pipe_if_needed(pipefd, i + 1 < pl->ncmds))
		{
			cleanup_failed_pipeline(pids, i, &prev_read, pipefd);
			return (-1);
		}
		pids[i] = spawn_child(sh, pl->cmds[i], prev_read, pipefd);
		if (pids[i] < 0)
		{
			cleanup_failed_pipeline(pids, i, &prev_read, pipefd);
			return (-1);
		}
		close_parent_fds(&prev_read, pipefd);
		++i;
	}
	return (i);
}
