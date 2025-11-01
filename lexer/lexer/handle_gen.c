/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_gen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 22:09:04 by kyanagis          #+#    #+#             */
/*   Updated: 2025/10/30 16:27:34 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	push_current_token(t_lexer *lxer)
{
	grow_args(&lxer->args, &lxer->args_cap, lxer->args_len + 1);
	lxer->work[lxer->write_pos++] = '\0';
	lxer->args[lxer->args_len++] = &lxer->work[lxer->token_start];
	lxer->args[lxer->args_len] = NULL;
	lxer->token_start = -1;
}

static void	lexer_set_state_esc(t_lexer *lexer)
{
	if (lexer->token_start < 0)
		lexer->token_start = lexer->write_pos;
	lexer->state = LEX_ESC;
}

static void	lexer_set_state_sq(t_lexer *lexer)
{
	if (lexer->token_start < 0)
		lexer->token_start = lexer->write_pos;
	lexer->state = LEX_SQ;
}

static void	lexer_set_state_dq(t_lexer *lexer)
{
	if (lexer->token_start < 0)
		lexer->token_start = lexer->write_pos;
	lexer->state = LEX_DQ;
}

void	handle_gen(t_lexer *lexer, char c)
{
	if (c == '\\')
		lexer_set_state_esc(lexer);
	else if (c == '\'')
		lexer_set_state_sq(lexer);
	else if (c == '\"')
		lexer_set_state_dq(lexer);
	else if (isspace(c)) // libft
	{
		if (lexer->token_start != -1)
			push_current_token(lexer);
	}
	else
	{
		if (lexer->token_start < 0)
			lexer->token_start = lexer->write_pos;
		lexer->work[lexer->write_pos++] = c;
	}
}
