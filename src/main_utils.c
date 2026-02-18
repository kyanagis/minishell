/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:20:35 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "expander.h"
#include "executor.h"
#include "free_table.h"
#include "sig.h"

static bool	grow_non_tty_line(char **line, size_t *cap, size_t len)
{
	char	*new_line;

	if (len + 1 < *cap)
		return (true);
	new_line = (char *)malloc(*cap * 2);
	if (!new_line)
	{
		free(*line);
		*line = NULL;
		return (false);
	}
	ft_memcpy(new_line, *line, len);
	free(*line);
	*line = new_line;
	*cap *= 2;
	return (true);
}

char	*read_non_tty_line_fd(int fd)
{
	char	*line;
	size_t	len;
	size_t	cap;
	ssize_t	rd;

	cap = 64;
	len = 0;
	line = (char *)malloc(cap);
	if (!line)
		return (NULL);
	while (1)
	{
		if (!grow_non_tty_line(&line, &cap, len + 2))
			return (NULL);
		rd = read(fd, line + len, 1);
		if (rd <= 0 || line[len] == '\n')
			break ;
		len++;
	}
	if (rd <= 0 && len == 0)
		return (free(line), NULL);
	line[len] = '\0';
	return (line);
}

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
		if (!*line)
		{
			free(line);
			return (true);
		}
	}
	if (!*line)
	{
		free(line);
		return (true);
	}
	return (false);
}
