/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya <skatsuya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:05:55 by skatsuya          #+#    #+#             */
/*   Updated: 2025/12/12 18:12:28 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static void swap_node(t_env *node_a, t_env *node_b);
static t_env *copy_env_list(t_env *env_list);
static void sort_env_list(t_env *head);
static bool append_env_copy(t_env **copy_head, t_env *env_node);

int ft_export(t_shell *shell, char **argv)
{
	t_env *current;
	t_env *head;

	if (argv[1])
	{
		size_t i;
		i = 1;
		while (argv[i])
		{
			ft_export_one(shell, argv[i]);
			i++;
		}
	}
	else if (!argv[1])
	{
		current = copy_env_list(shell->env_list);
		head = current;
		sort_env_list(current);
		while (current)
		{
			if (current->value)
			{
				ft_putstr_fd("declare -x ", STDOUT_FILENO);
				ft_putstr_fd(current->key, STDOUT_FILENO);
				ft_putstr_fd("=", STDOUT_FILENO);
				ft_putstr_fd("\"", STDOUT_FILENO);
				ft_putstr_fd(current->value, STDOUT_FILENO);
				ft_putstr_fd("\"", STDOUT_FILENO);
				write(STDOUT_FILENO, "\n", 1);
			}
			else
			{
				ft_putstr_fd("declare -x ", STDOUT_FILENO);
				ft_putstr_fd(current->key, STDOUT_FILENO);
				write(STDOUT_FILENO, "\n", 1);
			}
			current = current->next;
		}
		free_env_list(&head, free);
	}
	return (NO_ERROR);
}

static bool append_env_copy(t_env **copy_head, t_env *env_node)
{
	t_env *copy;

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

static t_env *copy_env_list(t_env *env_list)
{
	t_env *copy_head = NULL;

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

static void sort_env_list(t_env *head)
{
	t_env *current;
	int swapped;

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

static void swap_node(t_env *node_a, t_env *node_b)
{
	char *temp;

	temp = node_a->key;
	node_a->key = node_b->key;
	node_b->key = temp;

	temp = node_a->value;
	node_a->value = node_b->value;
	node_b->value = temp;
}
