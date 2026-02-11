/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_table.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 11:00:00 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 13:00:19 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FREE_TABLE_H
# define FREE_TABLE_H

# include <stdbool.h>
# include "minishell.h"

typedef struct s_work_context	t_work_context;

typedef struct s_free_table
{
	t_pipeline	*pipeline;
	t_lexout	*tokens;
	bool		has_pipeline;
	bool		has_tokens;
}	t_free_table;

void	init_free_table(t_free_table *table);
void	ft_set_pipeline(t_free_table *table, t_pipeline *pipeline);
void	ft_set_tokens(t_free_table *table, t_lexout *tokens);
void	ft_release(t_free_table *table);

void	destroy_redir_list(t_redir *head);
void	destroy_command(t_cmd *cmd);
void	destroy_pipeline(t_pipeline *pl);

void	parser_cleanup_on_error(t_work_context *ctx, t_free_table *table);
void	expander_cleanup_on_error(t_free_table *table);

#endif
