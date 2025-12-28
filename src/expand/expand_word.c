/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:04:17 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:30:49 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static unsigned char read_mask(const unsigned char *mask, size_t idx)
{
	if (!mask)
		return (QM_NONE);
	return (mask[idx]);
}

static bool is_pure_single_quote(const t_expand_input *input)
{
	size_t idx;

	if (!input || !input->mask)
		return (false);
	idx = 0;
	while (idx < input->len)
	{
		if (input->mask[idx] != QM_SQ)
			return (false);
		++idx;
	}
	return (true);
}

static bool duplicate_literal(const char *src, char **out)
{
	*out = ft_strdup(src);
	if (!*out)
		return (false);
	return (true);
}

static bool expand_word_loop(t_expand_work_buf *buf, t_shell *sh,
							 const t_expand_input *input)
{
	size_t idx;
	const char *src;

	src = input->src;
	idx = 0;
	while (idx < input->len)
	{
		if (src[idx] == '$' && read_mask(input->mask, idx) != QM_SQ)
		{
			if (!handle_dollar(buf, sh, input, &idx))
				return (false);
			continue;
		}
		if (!buf_append_char(buf, src[idx]))
			return (false);
		++idx;
	}
	return (true);
}

bool expand_word(t_shell *sh, const t_expand_input *input, char **out)
{
	t_expand_work_buf buf;

	if (!input || !input->src || !out)
		return (false);
	if (is_pure_single_quote(input))
		return (duplicate_literal(input->src, out));
	init_work_buf(&buf);
	if (!expand_word_loop(&buf, sh, input))
	{
		destroy_work_buf(&buf);
		return (false);
	}
	if (!buf.data)
		return (duplicate_literal("", out));
	*out = buf.data;
	return (true);
}
