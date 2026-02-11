/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 10:19:41 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 10:35:21 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "executor.h"

int	status_from_wait(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		else if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (128 + sig);
	}
	return (STATUS_GENERAL_ERR);
}

int	wait_children(pid_t *pids, size_t count)
{
	size_t	waited;
	pid_t	last_pid;
	int		status;
	int		last_status;
	pid_t	pid;

	if (!pids || count == 0)
		return (0);
	last_pid = pids[count - 1];
	last_status = 0;
	set_ignore_signals();
	waited = 0;
	while (waited < count)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (pid == last_pid)
			last_status = status_from_wait(status);
		++waited;
	}
	init_signals();
	return (last_status);
}
