/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 20:00:01 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:32:11 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

// static const t_state_handler	g_handlers[3] = {handle_gen, handle_squote,
// 		handle_dquote};

#define HANDLER_COUNT 3 // 解析対象が増えたら++する .

// 解析対象が増えたらここに関数ポインタをセットする .
static void init_handlers(t_state_handler *handlers)
{
	handlers[0] = (void *)handle_gen;
	handlers[1] = (void *)handle_squote;
	handlers[2] = (void *)handle_dquote;
}

static int handle_double_operator(t_lexer *lx, const char *input,
								  size_t *index, char c)
{
	if (c == '<' && input[*index] == '<')
	{
		push_operator_token(lx, "<<", 2, TOK_DLT);
		(*index)++;
		return (1);
	}
	if (c == '>' && input[*index] == '>')
	{
		push_operator_token(lx, ">>", 2, TOK_DGT);
		(*index)++;
		return (1);
	}
	return (0);
}

void scan_line(t_lexer *lx, const char *input)
{
	size_t index;
	char c;
	t_state_handler g_handlers[HANDLER_COUNT];

	init_handlers(g_handlers);
	index = 0;
	while (input[index])
	{
		c = input[index];
		index++;
		if (lx->state == LEX_GEN && handle_double_operator(lx, input, &index,
														   c))
			continue;
		g_handlers[lx->state](lx, input, &index, c);
	}
	if (lx->token_start != -1)
		push_current_token(lx);
}
