/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:05:55 by skatsuya          #+#    #+#             */
/*   Updated: 2026/02/16 23:18:10 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"

static void print_one_line(t_env *node);
static int print_export_env(t_shell *shell);

int ft_export(t_shell *shell, char **argv)
{
	size_t i;

	if (!argv[1])
		return (print_export_env(shell));
	i = 1;
	while (argv[i])
	{
		ft_export_one(shell, argv[i]);
		i++;
	}
	return (NO_ERROR);
}

static int print_export_env(t_shell *shell)
{
	t_env *head;
	t_env *current;

	head = copy_env_list(shell->env_list);
	if (!head)
		return (ERROR);
	sort_env_list(head);
	current = head;
	while (current)
	{
		print_one_line(current);
		current = current->next;
	}
	free_env_list(&head, free);
	return (NO_ERROR);
}

static void print_one_line(t_env *node)
{
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(node->key, STDOUT_FILENO);
	if (node->value)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(node->value, STDOUT_FILENO);
		ft_putstr_fd("\"", STDOUT_FILENO);
	}
	ft_putstr_fd("\n", STDOUT_FILENO);
}
