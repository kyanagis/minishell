/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya <skatsuya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 04:43:14 by skatsuya          #+#    #+#             */
/*   Updated: 2025/12/12 18:08:58 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "minishell.h"

static t_env	*env_new_node(char *str);
void		env_add_back(t_env **head, t_env *new_node);
// static t_env	*init_env_list(char **envp);

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

static t_env	*env_new_node(char *str)
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

void	env_add_back(t_env **head, t_env *new_node)
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

// static t_env	*init_env_list(char **envp)
// {
// 	t_env	*head;
// 	t_env	*new_node;
// 	size_t	i;

// 	head = NULL;
// 	i = 0;
// 	while (envp[i])
// 	{
// 		new_node = env_new_node(envp[i]);
// 		if (!new_node)
// 			return (NULL);
// 		env_add_back(&head, new_node);
// 		i++;
// 	}
// 	return (head);
// }
