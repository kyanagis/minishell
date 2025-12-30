/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_pipe_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:03:43 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:32:55 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	command_ready_for_pipe(t_work_context *ctx)
{
	if (ctx->expecting_redir_arg)
		return (false);
	return (!work_command_is_empty(ctx->current_builder));
}

static bool	extract_current_command(t_work_context *ctx, t_work_state *state)
{
	t_cmd	*cmd;

	cmd = work_command_extract(ctx->current_builder);
	if (!cmd)
	{
		state->ok = false;
		return (false);
	}
	work_pipeline_push(&ctx->pipeline_builder, cmd);
	return (true);
}

bool	handle_pipe_token(t_work_context *ctx, t_work_state *state)
{
	if (!command_ready_for_pipe(ctx))
	{
		state->ok = false;
		return (false);
	}
	return (extract_current_command(ctx, state));
}
