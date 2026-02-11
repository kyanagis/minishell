/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_builder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 04:04:05 by kyanagis          #+#    #+#             */
/*   Updated: 2025/12/27 11:32:23 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <stdlib.h>

// パイプライン配列がいっぱいなら倍増させる。

static void	ensure_command_capacity(t_work_pipeline *builder)
{
	t_cmd	**next;
	size_t	new_capacity;

	if (builder->len + 1 <= builder->cap)
		return ;
	new_capacity = builder->cap;
	if (new_capacity == 0)
		new_capacity = 2;
	else
		new_capacity *= 2;
	next = ft_xcalloc(new_capacity, sizeof(t_cmd *));
	if (builder->arr && builder->len)
		ft_memcpy(next, builder->arr, sizeof(t_cmd *) * builder->len);
	free(builder->arr);
	builder->arr = next;
	builder->cap = new_capacity;
}

void	work_pipeline_push(t_work_pipeline *builder, t_cmd *cmd)
{
	if (!builder || !cmd)
		return ;
	ensure_command_capacity(builder);
	builder->arr[builder->len] = cmd;
	builder->len++;
}

static void	reset_builder_storage(t_work_pipeline *builder)
{
	free(builder->arr);
	builder->arr = NULL;
	builder->len = 0;
	builder->cap = 0;
}

//  ビルダー内容を確定パイプライン構造体へコピーする。
t_pipeline	*work_pipeline_build(t_work_pipeline *builder)
{
	t_pipeline	*pipeline;
	t_cmd		**commands;

	if (!builder || builder->len == 0)
		return (NULL);
	pipeline = ft_xcalloc(1, sizeof(t_pipeline));
	commands = ft_xcalloc(builder->len, sizeof(t_cmd *));
	if (builder->arr && builder->len)
		ft_memcpy(commands, builder->arr, sizeof(t_cmd *) * builder->len);
	pipeline->ncmds = builder->len;
	pipeline->cmds = commands;
	reset_builder_storage(builder);
	return (pipeline);
}
