/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/30 15:23:07 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "executor.h"
#include "expander.h"
#include "free_table.h"
#include "lexer.h"
#include "main_utils.h"
#include "minishell.h"
#include "parser.h"
#include "sig.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>

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
		{
			add_history(line);
			parse_and_execute(sh, &table, tokens);
		}
	}
	ft_release(&table);
	sh->table = NULL;
}

static void	run_shell(t_shell *sh)
{
	char	*line;

	init_signals();
	while (1)
	{
		update_prompt(sh);
		line = readline(sh->prompt);
		if (is_eof(line))
		{
			break ;
		}
		if (!should_skip_line(sh, line))
		{
			process_line(sh, line);
			free(line);
		}
		if (sh->should_exit)
		{
			break ;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;

	(void)argc;
	(void)argv;
	shell_init(&sh, envp);
	if (sh.should_exit)
	{
		shell_destroy(&sh);
		return (sh.last_status);
	}
	run_shell(&sh);
	free_env_list(&sh.env_list, free);
	shell_destroy(&sh);
	return (sh.last_status);
}
