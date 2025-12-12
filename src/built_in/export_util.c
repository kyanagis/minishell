/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya <skatsuya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 04:43:14 by skatsuya          #+#    #+#             */
/*   Updated: 2025/12/12 09:36:13 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "minishell.h"

// 引数有りの場合のexport処理
void	ft_export_one(t_shell *shell, char *arg)
{
	t_env	*new_node;
	t_env	*current;

	new_node = env_new_node(arg);
	if (!new_node)
		return ;
	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, new_node->key) == 0)
		{
			if (new_node->value)
			{
				free(current->value);
				current->value = new_node->value;
			}
			free(new_node->key);
			free(new_node);
			return ;
		}
		current = current->next;
	}
	env_add_back(&shell->env_list, new_node);
}
