/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finalize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:59:43 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/05 02:45:41 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>

static unsigned char	*mem_dup_uc(const unsigned char *s, size_t n)
{
	unsigned char	*p;
	size_t			i;

	p = (unsigned char *)ft_xmalloc(n);
	i = 0;
	while (i < n)
	{
		p[i] = s[i];
		i++;
	}
	return (p);
}

static t_lexout	*alloc_output(t_lexer *lx)
{
	t_lexout	*out;

	out = (t_lexout *)ft_xmalloc(sizeof(*out));
	out->count = lx->args_len;
	out->argv = (char **)ft_xmalloc(sizeof(char *) * (out->count + 1));
	out->qmask = (unsigned char **)ft_xmalloc(sizeof(unsigned char *)
			* out->count);
	out->len = (size_t *)ft_xmalloc(sizeof(size_t) * out->count);
	out->kind = (t_tok_kind *)ft_xmalloc(sizeof(t_tok_kind) * out->count);
	return (out);
}

static void	fill_output(t_lexout *out, t_lexer *lx)
{
	size_t	i;
	size_t	n;

	i = 0;
	while (i < out->count)
	{
		n = lx->lens[i];
		out->argv[i] = ft_strndup(lx->args[i], n);
		out->qmask[i] = mem_dup_uc(lx->masks[i], n);
		out->len[i] = n;
		out->kind[i] = lx->kinds[i];
		i++;
	}
	out->argv[out->count] = NULL;
}

t_lexout	*finalize_lexer(t_lexer *lx)
{
	t_lexout	*out;

	if (lx->state != LEX_GEN)
	{
		ft_putstr_fd("minishell: lexer: unclosed quotes\n", STDERR_FILENO);
		cleanup_lexer(lx);
		return (NULL);
	}
	out = alloc_output(lx);
	fill_output(out, lx);
	cleanup_lexer(lx);
	return (out);
}
