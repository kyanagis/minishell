/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_context.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:04:01 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:32:30 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	init_work_context(t_work_context *ctx, const t_lexout *tokens)
{
	if (!ctx)
		return ;
	ft_memset(ctx, 0, sizeof(*ctx));
	ctx->tokens = tokens;
	if (tokens && tokens->count > 0)
		ctx->heredoc_quote_cache
			= ft_calloc(tokens->count, sizeof(unsigned char));
}

void	init_work_state(t_work_state *state)
{
	if (!state)
		return ;
	ft_memset(state, 0, sizeof(*state));
	state->ok = true;
}
