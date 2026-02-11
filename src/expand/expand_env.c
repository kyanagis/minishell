/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:04:19 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/29 13:21:25 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "libft.h"
#include <stdlib.h>

static bool	append_last_status(t_expand_work_buf *buf, t_shell *sh)
{
	char	*value;

	value = ft_itoa(sh->last_status);
	if (!value)
		return (false);
	if (!buf_append_str(buf, value))
	{
		free(value);
		return (false);
	}
	free(value);
	return (true);
}

static size_t	read_var_len(const char *src)
{
	size_t	len;

	len = 0;
	while (src[len])
	{
		if (!ft_isalnum((unsigned char)src[len]) && src[len] != '_')
			break ;
		++len;
	}
	return (len);
}

static bool	append_variable(t_expand_work_buf *buf, t_shell *sh,
		const char *src, size_t len)
{
	const char	*value;

	value = get_envp_value_len(sh, src, len);
	if (!value)
		return (true);
	return (buf_append_str(buf, value));
}

bool	handle_dollar(t_expand_work_buf *buf, t_shell *sh,
		const t_expand_input *input, size_t *idx)
{
	size_t			var_len;
	const char		*src;

	src = input->src;
	if (*idx + 1 >= input->len)
		return (buf_append_char(buf, src[(*idx)++]));
	if (src[*idx + 1] == '?')
	{
		*idx += 2;
		return (append_last_status(buf, sh));
	}
	var_len = read_var_len(src + *idx + 1);
	if (var_len == 0)
		return (buf_append_char(buf, src[(*idx)++]));
	if (!append_variable(buf, sh, src + *idx + 1, var_len))
		return (false);
	*idx += var_len + 1;
	return (true);
}
