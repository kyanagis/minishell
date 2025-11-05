/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:59:58 by kyanagis          #+#    #+#             */
/*   Updated: 2025/11/04 19:59:59 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static void	write_with_mask(t_lexer *lx, char c, unsigned char qm)
{
	lx->work[lx->write_pos] = c;
	lx->mask[lx->write_pos] = qm;
	lx->write_pos++;
}

static void	store_token_refs(t_lexer *lx, size_t len, t_tok_kind kind)
{
	grow_meta(lx, lx->args_len + 1);
	lx->work[lx->write_pos++] = '\0';
	lx->args[lx->args_len] = &lx->work[lx->token_start];
	lx->masks[lx->args_len] = &lx->mask[lx->token_start];
	lx->lens[lx->args_len] = len;
	if (lx->kinds)
		lx->kinds[lx->args_len] = kind;
	lx->args_len++;
	lx->args[lx->args_len] = NULL;
	lx->token_start = -1;
}

void	push_current_token(t_lexer *lx)
{
	size_t	len;

	if (lx->token_start < 0)
		return ;
	len = lx->write_pos - (size_t)lx->token_start;
	store_token_refs(lx, len, TOK_WORD);
}

void	push_operator_token(t_lexer *lx, const char *op, size_t n,
		t_tok_kind kind)
{
	size_t	i;

	if (lx->token_start != -1)
		push_current_token(lx);
	lx->token_start = (ssize_t)lx->write_pos;
	i = 0;
	while (i < n)
	{
		write_with_mask(lx, op[i], QM_NONE);
		i++;
	}
	store_token_refs(lx, n, kind);
}
