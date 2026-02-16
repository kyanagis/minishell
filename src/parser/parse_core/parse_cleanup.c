/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:04:03 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:22:40 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "free_table.h"

static void	free_command_args(char **argv, size_t argc)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (i < argc)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	dispose_command_builder(t_work_command *builder)
{
	if (!builder)
		return ;
	destroy_redir_list(builder->r_head);
	free_command_args(builder->argv, builder->argc);
	free(builder->tok_idx_argv);
	free(builder);
}

static void	free_pipeline_command(t_cmd *cmd)
{
	destroy_command(cmd);
}

void	dispose_pipeline_builder(t_work_pipeline *builder)
{
	size_t	i;

	if (!builder->arr)
		return ;
	i = 0;
	while (i < builder->len)
	{
		free_pipeline_command(builder->arr[i]);
		i++;
	}
	free(builder->arr);
	builder->arr = NULL;
	builder->len = 0;
	builder->cap = 0;
}

void	parser_cleanup_on_error(t_work_context *ctx, t_free_table *table)
{
	(void)table;
	if (ctx)
	{
		dispose_command_builder(ctx->current_builder);
		ctx->current_builder = NULL;
		dispose_pipeline_builder(&ctx->pipeline_builder);
		free(ctx->heredoc_quote_cache);
		ctx->heredoc_quote_cache = NULL;
	}
}
