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

static t_hd_sigctx	*heredoc_sigctx(t_hd_sigctx *ctx, bool set)
{
	static t_hd_sigctx	*saved;

	if (set)
		saved = ctx;
	return (saved);
}

static void	heredoc_sigint(int sig)
{
	t_hd_sigctx	*ctx;
	ssize_t		w;

	(void)sig;
	g_sig = SIGINT;
	ctx = heredoc_sigctx(NULL, false);
	w = write(STDOUT_FILENO, "\n", 1);
	if (w < 0 || !ctx || !ctx->ready)
		return ;
	siglongjmp(ctx->jmp, 1);
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

static void	switch_heredoc_signals(struct sigaction old[2], bool restore)
{
	struct sigaction	sa[2];

	if (restore)
	{
		set_heredoc_echoctl(true);
		rl_catch_signals = 1;
		sigaction(SIGINT, &old[0], NULL);
		sigaction(SIGQUIT, &old[1], NULL);
		return ;
	}
	g_sig = 0;
	sa[0].sa_handler = heredoc_sigint;
	sa[0].sa_flags = 0;
	sigemptyset(&sa[0].sa_mask);
	sigaction(SIGINT, &sa[0], &old[0]);
	sa[1].sa_handler = SIG_IGN;
	sa[1].sa_flags = 0;
	sigemptyset(&sa[1].sa_mask);
	sigaction(SIGQUIT, &sa[1], &old[1]);
	rl_catch_signals = 0;
	set_heredoc_echoctl(false);
}

bool	collect_chunks(t_shell *sh, t_redir *redir,
								t_hd_chunk	**head, size_t *total_len)
{
	t_hd_sigctx			ctx;
	t_chunk_state		chunk_state;
	struct sigaction	old[2];
	int					status;

	*total_len = 0;
	chunk_state.tail = head;
	chunk_state.total_len = total_len;
	switch_heredoc_signals(old, false);
	heredoc_sigctx(&ctx, true);
	ctx.ready = 1;
	if (sigsetjmp(ctx.jmp, 1) != 0)
		status = -1;
	else
	{
		status = 1;
		while (status > 0)
			status = handle_heredoc_line(sh, redir, &chunk_state,
					readline(HEREDOC_PROMPT));
	}
	if (status == -1 && g_sig == SIGINT)
		sh->last_status = 130;
	switch_heredoc_signals(old, true);
	return (status == 0);
}
