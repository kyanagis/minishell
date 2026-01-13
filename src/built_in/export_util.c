/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 04:43:14 by skatsuya          #+#    #+#             */
/*   Updated: 2026/01/13 19:05:28 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "minishell.h"

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
				new_node->value = NULL;
			}
			ft_envlst_delone(new_node, free);
			return ;
		}
		current = current->next;
	}
	env_add_back(&shell->env_list, new_node);
}

bool	append_env_copy(t_env **copy_head, t_env *env_node)
{
	t_env	*copy;

	copy = malloc(sizeof(t_env));
	if (!copy)
		return (false);
	copy->key = NULL;
	copy->value = NULL;
	copy->next = NULL;
	copy->key = ft_strdup(env_node->key);
	if (!copy->key)
		return (ft_envlst_delone(copy, free), false);
	if (env_node->value)
	{
		copy->value = ft_strdup(env_node->value);
		if (!copy->value)
			return (ft_envlst_delone(copy, free), false);
	}
	env_add_back(copy_head, copy);
	return (true);
}

t_env	*copy_env_list(t_env *env_list)
{
	t_env	*copy_head;

	copy_head = NULL;
	while (env_list)
	{
		if (!append_env_copy(&copy_head, env_list))
		{
			free_env_list(&copy_head, free);
			return (NULL);
		}
		env_list = env_list->next;
	}
	return (copy_head);
}

void	sort_env_list(t_env *head)
{
	t_env	*current;
	int		swapped;

	if (!head)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = head;
		while (current->next)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				swap_node(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
	}
}

void	swap_node(t_env *node_a, t_env *node_b)
{
	char	*temp;

	temp = node_a->key;
	node_a->key = node_b->key;
	node_b->key = temp;
	temp = node_a->value;
	node_a->value = node_b->value;
	node_b->value = temp;
}
