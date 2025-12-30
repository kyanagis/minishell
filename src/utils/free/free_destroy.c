/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 11:20:10 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:30:20 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free_table.h"

void	destroy_redir_list(t_redir *head)
{
	t_redir	*next;

	while (head)
	{
		next = head->next;
		free(head->arg);
		free(head);
		head = next;
	}
}

void	destroy_command(t_cmd *cmd)
{
	size_t	i;

	if (!cmd)
		return ;
	destroy_redir_list(cmd->redirs);
	i = 0;
	while (cmd->argv && i < cmd->argc)
	{
		free(cmd->argv[i]);
		i++;
	}
	free(cmd->argv);
	free(cmd->tok_idx_argv);
	free(cmd);
}

void	destroy_pipeline(t_pipeline *pl)
{
	size_t	i;

	if (!pl)
		return ;
	i = 0;
	while (i < pl->ncmds)
	{
		destroy_command(pl->cmds[i]);
		i++;
	}
	free(pl->cmds);
	free(pl);
}
