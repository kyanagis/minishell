/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 05:55:32 by kyanagis          #+#    #+#             */
/*   Updated: 2025/08/28 22:24:01 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	handle_esc(t_lexer *lexer, char c)
{
	if (lexer->token_start < 0)
		lexer->token_start = lexer->write_pos;
	lexer->work[lexer->write_pos++] = c;
	lexer->state = LEX_GEN;
}

void	handle_squote(t_lexer *lexer, char c)
{
	if (c == '\'')
		lexer->state = LEX_GEN;
	else
	{
		if (lexer->token_start < 0)
			lexer->token_start = lexer->write_pos;
		lexer->work[lexer->write_pos++] = c;
	}
}

void	handle_dquote(t_lexer *lexer, char c)
{
	if (c == '\"')
		lexer->state = LEX_GEN;
	else if (c == '\\')
		lexer->state = LEX_DQ_ESC;
	else
	{
		if (lexer->token_start < 0)
			lexer->token_start = lexer->write_pos;
		lexer->work[lexer->write_pos++] = c;
	}
}

void	handle_dq_esc(t_lexer *lexer, char c)
{
	if (lexer->token_start < 0)
		lexer->token_start = lexer->write_pos;
	if (is_dq_escapable(c))
		lexer->work[lexer->write_pos++] = c;
	else
	{
		lexer->work[lexer->write_pos++] = '\\';
		lexer->work[lexer->write_pos++] = c;
	}
	lexer->state = LEX_DQ;
}
