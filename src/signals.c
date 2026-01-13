/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 03:13:00 by sakurako          #+#    #+#             */
/*   Updated: 2026/01/13 19:58:38 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sig.h"
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t	g_sig = 0;

void	handle_sigint(int sig)
{
	g_sig = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	if (sigaction(SIGINT, &sa_int, NULL) == -1 || sigaction(SIGQUIT, &sa_quit,
			NULL) == -1)
	{
		ft_putstr_fd("Error: Failed to set up signal handlers\n", 2);
		exit(1);
	}
}

void	set_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	set_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

int	wait_child_process(pid_t pid)
{
	int	status;
	int	exit_code;
	int	sig;

	set_ignore_signals();
	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		exit_code = 128 + sig;
	}
	else if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else
		exit_code = 1;
	return (exit_code);
}
