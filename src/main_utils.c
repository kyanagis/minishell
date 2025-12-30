/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/30 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_utils.h"
#include "minishell.h"
#include "parser.h"
#include "expander.h"
#include "executor.h"
#include "free_table.h"

void	parse_and_execute(t_shell *sh, t_free_table *table, t_lexout *tokens)
{
	t_pipeline	*pl;
	bool		ok;

	pl = NULL;
	ok = parse_tokens(sh, tokens, &pl, table);
	if (ok)
	{
		ft_set_pipeline(table, pl);
		ok = expand_pipeline(sh, tokens, pl, table);
	}
	if (!ok && sh->last_status == 0)
		sh->last_status = 1;
	if (ok)
		execute_commands(sh, pl);
}

bool	is_eof(char *line)
{
	if (line)
		return (false);
	ft_putendl_fd("exit", STDOUT_FILENO);
	return (true);
}

bool	should_skip_line(t_shell *sh, char *line)
{
	if (g_sig == SIGINT)
	{
		sh->last_status = 130;
		g_sig = 0;
		free(line);
		return (true);
	}
	if (!*line)
	{
		free(line);
		return (true);
	}
	return (false);
}
