/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_chunks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/30 15:22:57 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "exec_heredoc_internal.h"
#include "expander.h"

void free_chunks(t_hd_chunk *head)
{
	t_hd_chunk *next;

	while (head)
	{
		next = head->next;
		free(head->data);
		free(head);
		head = next;
	}
}

static char *join_line_with_newline(char *line, size_t *out_len)
{
	size_t len;
	char *res;

	len = ft_strlen(line);
	res = (char *)ft_calloc(len + 2, sizeof(char));
	if (!res)
	{
		free(line);
		return (NULL);
	}
	ft_memcpy(res, line, len);
	res[len] = '\n';
	res[len + 1] = '\0';
	free(line);
	*out_len = len + 1;
	return (res);
}

static char *expand_heredoc_line(t_shell *sh, char *line, bool quoted)
{
	t_expand_input input;
	unsigned char *mask;
	char *expanded;

	if (quoted)
		return (line);
	mask = (unsigned char *)ft_calloc(ft_strlen(line), sizeof(unsigned char));
	if (!mask)
	{
		free(line);
		return (NULL);
	}
	input.src = line;
	input.mask = mask;
	input.len = ft_strlen(line);
	if (!expand_word(sh, &input, &expanded))
	{
		free(mask);
		free(line);
		return (NULL);
	}
	free(mask);
	free(line);
	return (expanded);
}

static bool append_expanded_line(t_shell *sh, bool quoted,
								 t_chunk_state *state, char *line)
{
	char *with_nl;
	size_t chunk_len;
	t_hd_chunk *node;

	with_nl = expand_heredoc_line(sh, line, quoted);
	if (!with_nl)
		return (false);
	with_nl = join_line_with_newline(with_nl, &chunk_len);
	if (!with_nl)
		return (false);
	node = (t_hd_chunk *)ft_calloc(1, sizeof(t_hd_chunk));
	if (!node)
		return (free(with_nl), false);
	node->data = with_nl;
	node->len = chunk_len;
	if (*state->tail)
		(*state->tail)->next = node;
	else
		*state->head = node;
	*state->tail = node;
	*state->total_len += chunk_len;
	return (true);
}

static bool handle_null_line(t_shell *sh, t_redir *redir)
{
	if (g_sig == SIGINT)
	{
		sh->last_status = 130;
		return (false);
	}
	ft_putstr_fd("minishell: warning: here-document delimited by end-of-file",
				 STDERR_FILENO);
	ft_putstr_fd(" (wanted `", STDERR_FILENO);
	ft_putstr_fd(redir->arg, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
	return (true);
}

bool collect_chunks(t_shell *sh, t_redir *redir,
					t_hd_chunk **head, size_t *total_len)
{
	char *line;
	t_hd_chunk *tail;
	t_chunk_state state;

	*total_len = 0;
	tail = NULL;
	state.head = head;
	state.tail = &tail;
	state.total_len = total_len;
	line = readline(HEREDOC_PROMPT);
	while (line)
	{
		if (ft_strcmp(line, redir->arg) == 0)
		{
			free(line);
			return (true);
		}
		if (!append_expanded_line(sh, redir->delim_quoted, &state, line))
			return (false);
		line = readline(HEREDOC_PROMPT);
	}
	return (handle_null_line(sh, redir));
}
