/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:04:08 by kyanagis          #+#    #+#             */
/*   Updated: 2026/02/16 23:22:26 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	work_command_is_empty(t_work_command *builder)
{
	if (!builder)
		return (true);
	return (builder->argc == 0 && builder->r_head == NULL);
}

//  ビルダー内部のデータ所有権を確定済みコマンド構造体へ移す。

t_cmd	*work_command_extract(t_work_command *builder)
{
	t_cmd	*cmd;

	if (!builder)
		return (NULL);
	cmd = ft_xcalloc(1, sizeof (t_cmd));
	cmd->argv = builder->argv;
	cmd->argc = builder->argc;
	cmd->tok_idx_argv = builder->tok_idx_argv;
	cmd->redirs = builder->r_head;
	builder->argv = NULL;
	builder->tok_idx_argv = NULL;
	builder->r_head = NULL;
	builder->r_tail = NULL;
	builder->argc = 0;
	builder->cap = 0;
	return (cmd);
}
