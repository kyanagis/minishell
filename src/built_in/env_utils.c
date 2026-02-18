/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyanagis <kyanagis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 07:44:48 by skatsuya          #+#    #+#             */
/*   Updated: 2026/02/16 23:17:28 by kyanagis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_env **lst, void (*del)(void *))
{
	t_env	*temp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_envlst_delone(*lst, del);
		*lst = temp;
	}
	*lst = NULL;
}

void	ft_envlst_delone(t_env *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->key);
	del(lst->value);
	free(lst);
}

t_env	*env_new_node(char *str)
{
	t_env	*new;
	char	*eq_pos;

	new = malloc(sizeof (t_env));
	if (!new)
		return (NULL);
	eq_pos = ft_strchr(str, '=');
	new->key = ft_strdup(str);
	new->value = NULL;
	if (eq_pos)
	{
		free(new->key);
		new->key = ft_substr(str, 0, eq_pos - str);
		if (new->key)
			new->value = ft_strdup(eq_pos + 1);
	}
	new->next = NULL;
	if (new->key && (!eq_pos || new->value))
		return (new);
	free(new->key);
	free(new->value);
	free(new);
	return (NULL);
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

t_env	*init_env_list(char **envp)
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
		{
			free_env_list(&head, free);
			return (NULL);
		}
		env_add_back(&head, new_node);
		i++;
	}
	return (head);
}
