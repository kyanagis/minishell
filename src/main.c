/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:35:27 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "expander.h"
#include "executor.h"
#include "free_table.h"
#include "lexer.h"

static void process_line(t_shell *sh, char *line)
{
	t_lexout *tokens;
	t_pipeline *pl;
	bool ok;
	t_free_table table;

	init_free_table(&table);
	tokens = tokenize(line);
	if (!tokens)
	{
		sh->last_status = 1;
		lexer_cleanup_on_error(&table);
		return;
	}
	ft_set_tokens(&table, tokens);
	if (tokens->count == 0)
	{
		lexer_cleanup_on_error(&table);
		return;
	}
	add_history(line);
	pl = NULL;
	ok = parse_tokens(sh, tokens, &pl, &table);
	if (ok)
	{
		ft_set_pipeline(&table, pl);
		ok = expand_pipeline(sh, tokens, pl, &table);
	}
	if (!ok && sh->last_status == 0)
		sh->last_status = 1;
	if (ok)
		execute_commands(sh, pl);
	ft_release(&table);
}

static void run_shell(t_shell *sh)
{
	char *line;
	init_signals();
	while (1)
	{
		update_prompt(sh);
		line = readline(sh->prompt);
		if (!line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break;
		}
		if (g_sig == SIGINT)
		{
			sh->last_status = 130;
			g_sig = 0;
			free(line);
			continue;
		}
		if (*line)
			process_line(sh, line);
		free(line);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_shell sh;

	(void)argc;
	(void)argv;
	shell_init(&sh, envp);
	run_shell(&sh);
	free_env_list(&sh.env_list, free);
	shell_destroy(&sh);
	return (sh.last_status);
}
