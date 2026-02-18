/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:26:51 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free_table.h"
#include "lexer.h"
#include "parser.h"
#include "sig.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

static bool	is_interactive_mode(void)
{
	if (!isatty(STDIN_FILENO))
		return (false);
	if (!isatty(STDOUT_FILENO))
		return (false);
	return (true);
}

static char	*read_shell_line(t_shell *sh, bool interactive)
{
	char	*line;

	if (interactive)
		return (readline(sh->prompt));
	if (write(STDOUT_FILENO, sh->prompt, ft_strlen(sh->prompt)) < 0)
		return (NULL);
	line = read_non_tty_line_fd(STDIN_FILENO);
	if (!line)
		return (NULL);
	write(STDOUT_FILENO, line, ft_strlen(line));
	write(STDOUT_FILENO, "\n", 1);
	return (line);
}

static void	process_line(t_shell *sh, char *line)
{
	t_lexout		*tokens;
	t_free_table	table;

	init_free_table(&table);
	sh->table = &table;
	tokens = tokenize(line);
	if (!tokens)
		sh->last_status = 1;
	if (tokens)
	{
		ft_set_tokens(&table, tokens);
		if (tokens->count > 0)
			parse_and_execute(sh, &table, tokens);
	}
	ft_release(&table);
	sh->table = NULL;
}

static void	run_shell(t_shell *sh)
{
	char	*line;
	bool	interactive;

	init_signals();
	interactive = is_interactive_mode();
	while (1)
	{
		update_prompt(sh);
		line = read_shell_line(sh, interactive);
		if (is_eof(line))
			break ;
		if (!should_skip_line(sh, line))
		{
			if (interactive)
				add_history(line);
			sh->current_line = line;
			process_line(sh, line);
			free(sh->current_line);
		}
		if (sh->should_exit)
			break ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;
	bool	interactive;

	(void)argc;
	(void)argv;
	interactive = is_interactive_mode();
	shell_init(&sh, envp);
	if (sh.should_exit)
	{
		shell_destroy(&sh);
		return (sh.last_status);
	}
	run_shell(&sh);
	free_env_list(&sh.env_list, free);
	if (interactive)
		rl_clear_history();
	shell_destroy(&sh);
	return (sh.last_status);
}
