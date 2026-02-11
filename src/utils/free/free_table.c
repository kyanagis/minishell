/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 11:20:15 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:30:19 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free_table.h"
#include "lexer.h"

void	init_free_table(t_free_table *table)
{
	if (!table)
		return ;
	table->pipeline = NULL;
	table->tokens = NULL;
	table->has_pipeline = false;
	table->has_tokens = false;
}

void	ft_set_pipeline(t_free_table *table, t_pipeline *pipeline)
{
	if (!table)
		return ;
	table->pipeline = pipeline;
	table->has_pipeline = true;
}

void	ft_set_tokens(t_free_table *table, t_lexout *tokens)
{
	if (!table)
		return ;
	table->tokens = tokens;
	table->has_tokens = true;
}

void	ft_release(t_free_table *table)
{
	if (!table)
		return ;
	if (table->has_pipeline)
		destroy_pipeline(table->pipeline);
	if (table->has_tokens)
		free_lexout(table->tokens);
	init_free_table(table);
}
