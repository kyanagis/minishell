/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_gen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 19:59:48 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:30:58 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static void	token_start_if_needed(t_lexer *lx)
{
	if (lx->token_start < 0)
		lx->token_start = (ssize_t)lx->write_pos;
}

static void	write_with_mask(t_lexer *lx, char c, unsigned char qm)
{
	lx->work[lx->write_pos] = c;
	lx->mask[lx->write_pos] = qm;
	lx->write_pos++;
}

static bool	switch_quote_state(t_lexer *lx, char c)
{
	if (c == '\'')
	{
		token_start_if_needed(lx);
		lx->state = LEX_SQ;
		return (true);
	}
	if (c == '"')
	{
		token_start_if_needed(lx);
		lx->state = LEX_DQ;
		return (true);
	}
	return (false);
}

static bool	try_push_operator(t_lexer *lx, char c)
{
	if (c == '|')
	{
		push_operator_token(lx, "|", 1, TOK_PIPE);
		return (true);
	}
	if (c == '<')
	{
		push_operator_token(lx, "<", 1, TOK_LT);
		return (true);
	}
	if (c == '>')
	{
		push_operator_token(lx, ">", 1, TOK_GT);
		return (true);
	}
	return (false);
}

void	handle_gen(t_lexer *lx, const char *input, size_t *index, char c)
{
	(void)input;
	(void)index;
	if (switch_quote_state(lx, c))
		return ;
	if (lx_isblank((unsigned char)c))
	{
		if (lx->token_start != -1)
			push_current_token(lx);
		return ;
	}
	if (try_push_operator(lx, c))
		return ;
	token_start_if_needed(lx);
	write_with_mask(lx, c, QM_NONE);
}
