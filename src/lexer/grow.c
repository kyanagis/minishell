/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grow.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 02:43:37 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:15:15 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static size_t next_capacity(size_t current, size_t need)
{
	size_t cap;

	if (current == 0)
		cap = 8;
	else
		cap = current * 2;
	while (cap < need)
		cap *= 2;
	return (cap);
}

static void meta_alloc(struct s_meta *m, size_t cap)
{
	m->args = (char **)ft_xcalloc(cap + 1, sizeof(char *));
	m->masks = (unsigned char **)ft_xcalloc(cap, sizeof(unsigned char *));
	m->lens = (size_t *)ft_xcalloc(cap, sizeof(size_t));
	m->kinds = (t_tok_kind *)ft_xcalloc(cap, sizeof(t_tok_kind));
}

static void meta_copy(const t_lexer *lx, struct s_meta *m)
{
	if (!lx->args || !lx->args_len)
		return;
	ft_memcpy(m->args, lx->args, sizeof(char *) * lx->args_len);
	ft_memcpy(m->masks, lx->masks, sizeof(unsigned char *) * lx->args_len);
	ft_memcpy(m->lens, lx->lens, sizeof(size_t) * lx->args_len);
	ft_memcpy(m->kinds, lx->kinds, sizeof(t_tok_kind) * lx->args_len);
	m->args[lx->args_len] = NULL;
}

void grow_meta(t_lexer *lx, size_t need)
{
	size_t cap;
	struct s_meta m;
	size_t i;

	if (need <= lx->args_cap)
		return;
	cap = next_capacity(lx->args_cap, need);
	meta_alloc(&m, cap);
	meta_copy(lx, &m);
	i = lx->args_len;
	while (i < cap)
	{
		m.kinds[i] = TOK_WORD;
		i++;
	}
	free(lx->args);
	free(lx->masks);
	free(lx->lens);
	free(lx->kinds);
	lx->args = m.args;
	lx->masks = m.masks;
	lx->lens = m.lens;
	lx->kinds = m.kinds;
	lx->args_cap = cap;
}
