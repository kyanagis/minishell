/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:03:51 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/29 07:48:46 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include "free_table.h"

static bool	parse_tokens_loop(const t_lexout *tokens, t_work_context *ctx,
			t_work_state *state)
{
	while (state->ok && state->index < tokens->count)
		dispatch_parse_token(state, ctx, tokens);
	return (state->ok);
}

// : トークン列を検証しながらパイプライン構築処理をまとめて呼び出す。

bool	build_pipeline_from_tokens(const t_lexout *tokens, t_pipeline **out,
			t_free_table *table)
{
	t_work_context	ctx;
	t_work_state	state;

	if (!out)
		return (false);
	*out = NULL;
	if (!tokens || tokens->count == 0)
		return (true);
	init_work_context(&ctx, tokens);
	init_work_state(&state);
	if (!parse_tokens_loop(tokens, &ctx, &state)
		|| !finalize_pipeline_success(&ctx, &state, out))
	{
		parser_cleanup_on_error(&ctx, table);
		return (false);
	}
	dispose_command_builder(ctx.current_builder);
	ctx.current_builder = NULL;
	free(ctx.heredoc_quote_cache);
	ctx.heredoc_quote_cache = NULL;
	return (true);
}
