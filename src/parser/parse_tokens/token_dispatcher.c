/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dispatcher.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:03:46 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:32:51 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	is_redirection_token(t_tok_kind kind)
{
	if (kind == TOK_LT || kind == TOK_GT)
		return (true);
	if (kind == TOK_DLT || kind == TOK_DGT)
		return (true);
	return (false);
}

void	dispatch_parse_token(t_work_state *state, t_work_context *ctx,
			const t_lexout *tokens)
{
	state->kind = tokens->kind[state->index];
	if (state->kind == TOK_WORD)
		state->ok = handle_word_token(ctx, state, tokens);
	else if (is_redirection_token(state->kind))
		state->ok = handle_redirection_operator(ctx, state);
	else if (state->kind == TOK_PIPE)
		state->ok = handle_pipe_token(ctx, state);
	else
		state->ok = false;
	if (state->ok)
		state->index++;
}
