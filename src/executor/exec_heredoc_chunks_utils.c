/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_chunks_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 00:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/11 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "exec_heredoc_internal.h"
#include "expander.h"

void	free_chunks(t_hd_chunk *head)
{
	t_hd_chunk	*next;

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

static char	*cleanup_expand(char *line, unsigned char *mask, char *expanded)
{
	free(mask);
	free(line);
	return (expanded);
}

static char	*expand_heredoc_line(t_shell *sh, char *line, bool quoted)
{
	t_expand_input	input;
	unsigned char	*quote_mask;
	char			*expanded;
	size_t			line_len;

	if (quoted)
		return (line);
	line_len = ft_strlen(line);
	quote_mask = (unsigned char *)ft_calloc(line_len, sizeof(unsigned char));
	if (!quote_mask)
		return (cleanup_expand(line, NULL, NULL));
	input.src = line;
	input.mask = quote_mask;
	input.len = line_len;
	if (!expand_word(sh, &input, &expanded))
		return (cleanup_expand(line, quote_mask, NULL));
	return (cleanup_expand(line, quote_mask, expanded));
}

bool	append_expanded_line(t_shell *sh, bool quoted,
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
	{
		free(line_with_newline);
		return (false);
	}
	chunk->data = line_with_newline;
	chunk->len = line_len;
	*chunk_state->tail = chunk;
	chunk_state->tail = &chunk->next;
	*chunk_state->total_len += line_len;
	return (true);
}
