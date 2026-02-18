/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 06:05:55 by skatsuya          #+#    #+#             */
/*   Updated: 2026/02/18 17:42:18 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include <stdbool.h>

static void	print_one_line(t_env *node);
static int	print_export_env(t_shell *shell);
static bool	is_valid_identifier(char *str);
static void	print_export_error(char *arg);

int	ft_export(t_shell *shell, char **argv)
{
	size_t	i;
	int		status;

	status = NO_ERROR;
	if (!argv[1])
		return (print_export_env(shell));
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			print_export_error(argv[i]);
			status = ERROR;
		}
		else
		{
			ft_export_one(shell, argv[i]);
		}
		i++;
	}
	return (status);
}

static int	print_export_env(t_shell *shell)
{
	t_env	*head;
	t_env	*current;

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

static void	print_one_line(t_env *node)
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

static bool	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str[i] || (!ft_isalpha(str[i]) && str[i] != '_'))
		return (false);
	i++;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}
