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

static char	*join_line_with_newline(char *line, size_t *out_len)
{
	size_t	line_len;
	char	*res;

	line_len = ft_strlen(line);
	res = (char *)ft_calloc(line_len + 2, sizeof(char));
	if (!res)
	{
		free(line);
		return (NULL);
	}
	ft_memcpy(res, line, line_len);
	res[line_len] = '\n';
	res[line_len + 1] = '\0';
	free(line);
	*out_len = line_len + 1;
	return (res);
}

static char *expand_heredoc_line(t_shell *sh, char *line, bool quoted)
{
	t_expand_input input;
	unsigned char *quote_mask;
	char *expanded;
	size_t line_len;

	if (quoted)
		return (line);
	line_len = ft_strlen(line);
	quote_mask = (unsigned char *)ft_calloc(line_len, sizeof(unsigned char));
	if (!quote_mask)
	{
		free(line);
		return (NULL);
	}
	input.src = line;
	input.mask = quote_mask;
	input.len = line_len;
	if (!expand_word(sh, &input, &expanded))
	{
		free(quote_mask);
		free(line);
		return (NULL);
	}
	free(quote_mask);
	free(line);
	return (expanded);
}

static bool	append_expanded_line(t_shell *sh, bool quoted,
			t_chunk_state *chunk_state, char *line)
{
	char		*line_with_newline;
	size_t		line_len;
	t_hd_chunk	*chunk;

	line_with_newline = expand_heredoc_line(sh, line, quoted);
	if (!line_with_newline)
		return (false);
	line_with_newline = join_line_with_newline(line_with_newline, &line_len);
	if (!line_with_newline)
		return (false);
	chunk = (t_hd_chunk *)ft_calloc(1, sizeof(t_hd_chunk));
	if (!chunk)
		return (free(line_with_newline), false);
	chunk->data = line_with_newline;
	chunk->len = line_len;
	if (*chunk_state->tail)
		(*chunk_state->tail)->next = chunk;
	else
		*chunk_state->head = chunk;
	*chunk_state->tail = chunk;
	*chunk_state->total_len += line_len;
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
	char			*input_line;
	t_hd_chunk		*chunk_tail;
	t_chunk_state	chunk_state;

	*total_len = 0;
	chunk_tail = NULL;
	chunk_state.head = head;
	chunk_state.tail = &chunk_tail;
	chunk_state.total_len = total_len;
	input_line = readline(HEREDOC_PROMPT);
	while (input_line)
	{
		if (ft_strcmp(input_line, redir->arg) == 0)
		{
			free(input_line);
			return (true);
		}
		if (!append_expanded_line(sh, redir->delim_quoted,
				&chunk_state, input_line))
			return (false);
		input_line = readline(HEREDOC_PROMPT);
	}
	return (handle_null_line(sh, redir));
}
