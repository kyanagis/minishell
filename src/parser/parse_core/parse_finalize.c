/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_finalize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:03:58 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/30 00:00:00 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	flush_current_command(t_work_context *ctx, t_work_state *state)
{
	t_cmd	*cmd;

	if (!ctx->current_builder)
	{
		state->ok = false;
		return ;
	}
	cmd = work_command_extract(ctx->current_builder);
	if (!cmd)
	{
		state->ok = false;
		return ;
	}
	work_pipeline_push(&ctx->pipeline_builder, cmd);
}

static bool	validate_final_state(t_work_context *ctx, t_work_state *state)
{
	if (ctx->expecting_redir_arg)
	{
		state->ok = false;
		return (false);
	}
	if (work_command_is_empty(ctx->current_builder))
	{
		state->ok = false;
		return (false);
	}
	return (true);
}

static bool	emit_pipeline(t_work_context *ctx, t_pipeline **out,
		t_work_state *state)
{
	*out = work_pipeline_build(&ctx->pipeline_builder);
	if (*out)
		return (true);
	state->ok = false;
	return (false);
}

bool	finalize_pipeline_success(t_work_context *ctx, t_work_state *state,
		t_pipeline **out)
{
	if (!validate_final_state(ctx, state))
		return (false);
	flush_current_command(ctx, state);
	if (!state->ok)
		return (false);
	return (emit_pipeline(ctx, out, state));
}
