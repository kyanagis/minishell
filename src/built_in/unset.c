/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skatsuya < skatsuya@student.42tokyo.jp>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:34:09 by skatsuya          #+#    #+#             */
/*   Updated: 2026/01/13 19:48:39 by skatsuya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_in.h"
#include "minishell.h"

static void	ft_unset_one(t_shell *shell, char *key);

int	ft_unset(t_shell *shell, char **argv)
{
	size_t	i;

	if (!argv[1])
		return (NO_ERROR);
	i = 1;
	while (argv[i])
	{
		ft_unset_one(shell, argv[i]);
		i++;
	}
	return (NO_ERROR);
}

static void	ft_unset_one(t_shell *shell, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = shell->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev == NULL)
				shell->env_list = current->next;
			else
				prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
