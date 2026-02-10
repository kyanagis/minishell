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

static bool	open_pipe_if_needed(int pipe_fd[2], bool needed)
{
	if (!needed)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		return (true);
	}
	if (pipe(pipe_fd) == -1)
		return (false);
	return (true);
}

static void	close_parent_fds(int *prev_read_fd, int pipe_fd[2])
{
	if (*prev_read_fd >= 0)
		close(*prev_read_fd);
	if (pipe_fd[1] >= 0)
		close(pipe_fd[1]);
	if (pipe_fd[0] >= 0)
		*prev_read_fd = pipe_fd[0];
	else
		*prev_read_fd = -1;
}

static pid_t	spawn_child(t_shell *sh, t_cmd *cmd, int prev_read_fd,
			int pipe_fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
		execute_child(sh, cmd, prev_read_fd, pipe_fd);
	return (pid);
}

static void	cleanup_failed_pipeline(pid_t *pids, size_t launched,
			int *prev_read_fd, int pipe_fd[2])
{
	if (prev_read_fd && *prev_read_fd >= 0)
	{
		close(*prev_read_fd);
		*prev_read_fd = -1;
	}
	if (pipe_fd[0] >= 0)
		close(pipe_fd[0]);
	if (pipe_fd[1] >= 0)
		close(pipe_fd[1]);
	if (launched > 0)
		wait_children(pids, launched);
}

ssize_t	run_pipeline(t_shell *sh, t_pipeline *pl, pid_t *pids)
{
	int		pipe_fd[2];
	int		prev_read_fd;
	size_t	index;

	prev_read_fd = -1;
	index = 0;
	while (index < pl->ncmds)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		if (!open_pipe_if_needed(pipe_fd, index + 1 < pl->ncmds))
		{
			cleanup_failed_pipeline(pids, index, &prev_read_fd, pipe_fd);
			return (-1);
		}
		pids[index] = spawn_child(sh, pl->cmds[index], prev_read_fd, pipe_fd);
		if (pids[index] < 0)
		{
			cleanup_failed_pipeline(pids, index, &prev_read_fd, pipe_fd);
			return (-1);
		}
		close_parent_fds(&prev_read_fd, pipe_fd);
		++index;
	}
	return (index);
}
