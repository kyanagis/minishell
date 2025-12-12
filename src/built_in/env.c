/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya <skatsuya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 22:14:50 by skatsuya          #+#    #+#             */
/*   Updated: 2025/12/12 09:08:07 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "minishell.h"

t_env			*env_new_node(char *str);
static void		env_add_back(t_env **head, t_env *new_node);
static t_env	*init_env_list(char **envp);

int	ft_env(t_shell *shell, char **argv)
{
	(void)argv;
	t_env	*current;

	current = shell->env_list;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		current = current->next;
	}
	return (NO_ERROR);
}

t_env	*env_new_node(char *str)
{
	t_env	*new;
	char	*eq_pos;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	eq_pos = ft_strchr(str, '=');
	if (eq_pos)
	{
		new->key = ft_substr(str, 0, eq_pos - str);
		new->value = ft_strdup(eq_pos + 1);
	}
	else
	{
		new->key = ft_strdup(str);
		new->value = NULL;
	}
	new->next = NULL;
	return (new);
}

static void	env_add_back(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

static t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	*new_node;
	size_t	i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = env_new_node(envp[i]);
		if (!new_node)
			return (NULL);
		env_add_back(&head, new_node);
		i++;
	}
	return (head);
}
