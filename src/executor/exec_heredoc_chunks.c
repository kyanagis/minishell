/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_chunks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:30:34 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_heredoc_internal.h"
#include "executor.h"
#include <termios.h>

static void	heredoc_sigint(int sig)
{
	ssize_t	w;

	(void)sig;
	g_sig = SIGINT;
	w = write(STDOUT_FILENO, "\n", 1);
	if (w < 0)
		return ;
}

static void	heredoc_sigquit(int sig)
{
	ssize_t	w;

	(void)sig;
	w = write(STDOUT_FILENO, "\b \b", 3);
	if (w < 0)
		return ;
}

static void	setup_heredoc_signals(struct sigaction *old_int,
				struct sigaction *old_quit)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	g_sig = 0;
	sa_int.sa_handler = heredoc_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, old_int);
	sa_quit.sa_handler = heredoc_sigquit;
	sa_quit.sa_flags = SA_RESTART;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, old_quit);
}

static void	set_heredoc_echoctl(bool restore)
{
	static struct termios	saved;
	static bool				saved_valid;
	struct termios			current;

	if (!isatty(STDIN_FILENO))
		return ;
	if (!restore)
	{
		if (tcgetattr(STDIN_FILENO, &saved) == -1)
			return ;
		saved_valid = true;
		current = saved;
		current.c_lflag &= ~(ECHOCTL);
		tcsetattr(STDIN_FILENO, TCSANOW, &current);
		return ;
	}
	if (saved_valid)
		tcsetattr(STDIN_FILENO, TCSANOW, &saved);
	saved_valid = false;
}

bool	collect_chunks(t_shell *sh, t_redir *redir,
						t_hd_chunk	**head, size_t *total_len)
{
	char				*input_line;
	t_chunk_state		chunk_state;
	int					status;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	*total_len = 0;
	chunk_state.tail = head;
	chunk_state.total_len = total_len;
	setup_heredoc_signals(&old_int, &old_quit);
	set_heredoc_echoctl(false);
	while (1)
	{
		ft_putstr_fd(HEREDOC_PROMPT, STDOUT_FILENO);
		input_line = get_next_line(STDIN_FILENO);
		status = handle_heredoc_line(sh, redir, &chunk_state, input_line);
		if (status <= 0)
			break ;
	}
	set_heredoc_echoctl(true);
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	return (status == 0);
}
