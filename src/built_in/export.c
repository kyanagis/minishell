/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya <skatsuya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:05:55 by skatsuya          #+#    #+#             */
/*   Updated: 2025/12/12 09:27:57 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static void swap_node(t_env *node_a, t_env *node_b);

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

t_env *copy_env_list(t_env *env_list)
{
	t_env	*copy;
	t_env *copy_head = NULL;

	while (env_list)
	{
		copy = malloc(sizeof(t_env));
		if (!copy)
			return (NULL);
		copy->key = ft_strdup(env_list->key);
		if (!env_list->value)
			copy->value = NULL;
		else
			copy->value = ft_strdup(env_list->value);
		copy->next = NULL;
		env_add_back(&copy_head, copy);
		env_list = env_list->next;
	}
	return (copy_head);
}

void sort_env_list(t_env *head)
{
	t_env *current;
	char *temp;
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
				swap(current, current->next);
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
