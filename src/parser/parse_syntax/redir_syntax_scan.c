/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_syntax_scan.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:03:48 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/15 04:03:49 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//  リダイレクト演算子の直後に単語が並ぶかを確認する。
static bool	validate_redir_operand(t_lexout *tokens, size_t index)
{
	t_tok_kind	next;

	if (index + 1 == tokens->count)
		return (parse_syntax_error("newline"));
	next = tokens->kind[index + 1];
	if (next != TOK_WORD)
		return (parse_syntax_error(token_str(next)));
	return (true);
}

bool	lt_syntax_scan(t_lexout *tokens, size_t i)
{
	return (validate_redir_operand(tokens, i));
}

bool	gt_syntax_scan(t_lexout *tokens, size_t i)
{
	return (validate_redir_operand(tokens, i));
}

bool	dlt_syntax_scan(t_lexout *tokens, size_t i)
{
	return (validate_redir_operand(tokens, i));
}

bool	dgt_syntax_scan(t_lexout *tokens, size_t i)
{
	return (validate_redir_operand(tokens, i));
}
