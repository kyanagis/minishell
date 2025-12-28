/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:59:51 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:31:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static void token_start_if_needed(t_lexer *lx)
{
	if (lx->token_start < 0)
		lx->token_start = (ssize_t)lx->write_pos;
}

static void write_with_mask(t_lexer *lx, char c, unsigned char qm)
{
	lx->work[lx->write_pos] = c;
	lx->mask[lx->write_pos] = qm;
	lx->write_pos++;
}

void handle_squote(t_lexer *lx, const char *input, size_t *index, char c)
{
	(void)input;
	(void)index;
	if (c == '\'')
	{
		lx->state = LEX_GEN;
		return;
	}
	token_start_if_needed(lx);
	write_with_mask(lx, c, QM_SQ);
}

void handle_dquote(t_lexer *lx, const char *input, size_t *index, char c)
{
	if (c == '"')
	{
		lx->state = LEX_GEN;
		return;
	}
	token_start_if_needed(lx);
	if (c == '\\' && input[*index] && is_dq_escapable(input[*index]))
	{
		write_with_mask(lx, input[*index], QM_DQ);
		(*index)++;
		return;
	}
	write_with_mask(lx, c, QM_DQ);
}
